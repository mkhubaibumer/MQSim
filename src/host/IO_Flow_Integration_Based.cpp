//
// Created by Khubaib Umer on 16/12/2022.
//

#include "IO_Flow_Integration_Based.h"

namespace Host_Components {
    IO_Flow_Integration_Based::IO_Flow_Integration_Based(
            const sim_object_id_type &name, uint16_t flow_id,
            LHA_type start_lsa_on_device, LHA_type end_lsa_on_device,
            uint16_t io_queue_id, uint16_t nvme_submission_queue_size,
            uint16_t nvme_completion_queue_size,
            IO_Flow_Priority_Class::Priority priority_class,
            double initial_occupancy_ratio,
            Trace_Time_Unit time_unit, unsigned int total_replay_count,
            unsigned int percentage_to_be_simulated,
            HostInterface_Types SSD_device_type, PCIe_Root_Complex *pcie_root_complex,
            SATA_HBA *sata_hba, bool enabled_logging, sim_time_type logging_period,
            std::string logging_file_path)
            : IO_Flow_Base(name, flow_id, start_lsa_on_device, end_lsa_on_device,
                           io_queue_id, nvme_submission_queue_size,
                           nvme_completion_queue_size, priority_class, 0,
                           initial_occupancy_ratio, 0, SSD_device_type,
                           pcie_root_complex, sata_hba, enabled_logging, logging_period,
                           logging_file_path),
              time_unit(time_unit),
              time_offset(0) {
    }

    Host_IO_Request *IO_Flow_Integration_Based::Generate_next_request() {
        if (!user_data_is_set)
            return nullptr;
        auto user_data_req = Get_User_Data_Request();
        auto request = new Host_IO_Request;
        if (user_data_req->type == kREQUEST_TYPE::WRITE) {
            request->Type = Host_IO_Request_Type::WRITE;
            STAT_generated_write_request_count++;
        } else {
            request->Type = Host_IO_Request_Type::READ;
            STAT_generated_read_request_count++;
        }

        request->LBA_count = user_data_req->size_in_sectors;

        request->Start_LBA = user_data_req->start_sector_addr;
        if (request->Start_LBA <= (end_lsa_on_device - start_lsa_on_device)) {
            request->Start_LBA += start_lsa_on_device;
        } else {
            request->Start_LBA = start_lsa_on_device + request->Start_LBA % (end_lsa_on_device - start_lsa_on_device);
        }

        request->Arrival_time = time_offset + Simulator->Time();
        STAT_generated_request_count++;

        return request;
    }

    void IO_Flow_Integration_Based::NVMe_consume_io_request(Completion_Queue_Entry *io_request) {
        IO_Flow_Base::NVMe_consume_io_request(io_request);
        IO_Flow_Base::NVMe_update_and_submit_completion_queue_tail();
    }

    void IO_Flow_Integration_Based::SATA_consume_io_request(Host_IO_Request *io_request) {
        IO_Flow_Base::SATA_consume_io_request(io_request);
    }

    void IO_Flow_Integration_Based::Start_simulation() {
        IO_Flow_Base::Start_simulation();
        // TODO: Not sure if this is required...
        //  will confirm and remove if not required
        sim_time_type prev_time = last_req_arrival_time;
        last_req_arrival_time = curr_request->arrival_time;
        if (last_req_arrival_time < prev_time) {
            PRINT_ERROR("Unexpected request arrival time: " << last_req_arrival_time
                                                            << "\nMQSim expects request arrival times to be monotonically increasing in the input trace!")
        }
    }

    void IO_Flow_Integration_Based::Validate_simulation_config() {
        // There is no simulator config for workloads in INTEGRATION_BASED mode
    }

    void IO_Flow_Integration_Based::Execute_simulator_event(MQSimEngine::Sim_Event *) {
        Host_IO_Request *request = Generate_next_request();
        if (request != NULL) {
            Submit_io_request(request);
        }
        ++total_requests;

        Simulator->Register_sim_event(time_offset + curr_request->arrival_time, this);
    }

    void IO_Flow_Integration_Based::Get_statistics(Utils::Workload_Statistics &stats,
                                                   LPA_type (*Convert_host_logical_address_to_device_address)(LHA_type),
                                                   page_status_type (*Find_NVM_subunit_access_bitmap)(LHA_type)) {

        stats.Type = Utils::Workload_Type::INTEGRATION_BASED;
        //In MQSim, there is a simple relation between stream id and the io_queue_id of NVMe
        stats.Stream_id = io_queue_id - 1;
        stats.Min_LHA = start_lsa_on_device;
        stats.Max_LHA = end_lsa_on_device;
        for (int i = 0; i < MAX_ARRIVAL_TIME_HISTOGRAM + 1; i++) {
            stats.Write_arrival_time.push_back(0);
            stats.Read_arrival_time.push_back(0);
        }
        for (int i = 0; i < MAX_REQSIZE_HISTOGRAM_ITEMS + 1; i++) {
            stats.Write_size_histogram.push_back(0);
            stats.Read_size_histogram.push_back(0);
        }
        stats.Total_generated_requests = 0;
        stats.Total_accessed_lbas = 0;

        // MQSim replays the file to calculate stats after execution is done
        // We can't afford that as the requests are coming in from user-executables
        // from library interface
        // We need to calculate the stats on the fly when requests are coming in as
        // we can't replay the user requests
    }

} // namespace Host_Components