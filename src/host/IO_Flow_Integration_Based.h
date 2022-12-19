//
// Created by Khubaib Umer on 16/12/2022.
//

#ifndef MQSIM_IO_FLOW_INTEGRATION_BASED_H
#define MQSIM_IO_FLOW_INTEGRATION_BASED_H

#include "ASCII_Trace_Definition.h"
#include "IO_Flow_Base.h"
#include "../lib/libmqsim.h"
#include <fstream>
#include <iostream>
#include <string>

namespace Host_Components {
    class IO_Flow_Integration_Based : public IO_Flow_Base {
    public:
        IO_Flow_Integration_Based(const sim_object_id_type &name, uint16_t flow_id, LHA_type start_lsa_on_device,
                                  LHA_type end_lsa_on_device, uint16_t io_queue_id,
                                  uint16_t nvme_submission_queue_size, uint16_t nvme_completion_queue_size,
                                  IO_Flow_Priority_Class::Priority priority_class, double initial_occupancy_ratio,
                                  Trace_Time_Unit time_unit,
                                  unsigned int total_replay_count, unsigned int percentage_to_be_simulated,
                                  HostInterface_Types SSD_device_type, PCIe_Root_Complex *pcie_root_complex,
                                  SATA_HBA *sata_hba,
                                  bool enabled_logging, sim_time_type logging_period, std::string logging_file_path);

        ~IO_Flow_Integration_Based() = default;

        Host_IO_Request *Generate_next_request();

        void NVMe_consume_io_request(Completion_Queue_Entry *);

        void SATA_consume_io_request(Host_IO_Request *);

        void Start_simulation();

        void Validate_simulation_config();

        void Execute_simulator_event(MQSimEngine::Sim_Event *);

        void Get_statistics(Utils::Workload_Statistics &stats,
                            LPA_type (*Convert_host_logical_address_to_device_address)(LHA_type lha),
                            page_status_type (*Find_NVM_subunit_access_bitmap)(LHA_type lha));

        void Set_Request(request_type_t *req) {
            curr_request = req;
            user_data_is_set = true;
        }

        request_type_t *Get_User_Data_Request() {
            user_data_is_set = false;
            return curr_request;
        }

    private:
        Trace_Time_Unit time_unit;
        unsigned int total_requests;
        sim_time_type time_offset;
        request_type_t *curr_request;
        bool user_data_is_set = false;
        sim_time_type last_req_arrival_time = 0;

    };
} // namespace Host_Components

#endif // MQSIM_IO_FLOW_INTEGRATION_BASED_H
