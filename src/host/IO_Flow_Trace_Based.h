#ifndef IO_FLOW_TRACE_BASED_H
#define IO_FLOW_TRACE_BASED_H

#include <string>
#include <iostream>
#include <fstream>
#include "IO_Flow_Base.h"
#include "ASCII_Trace_Definition.h"

namespace Host_Components
{
	class IO_Flow_Trace_Based : public IO_Flow_Base
	{
	public:
		IO_Flow_Trace_Based(const sim_object_id_type& name, LSA_type start_lsa_on_device, LSA_type end_lsa_on_device, uint16_t io_queue_id,
			uint16_t nvme_submission_queue_size, uint16_t nvme_completion_queue_size, IO_Flow_Priority_Class priority_class,
			std::string trace_file_path, Trace_Time_Unit time_unit, unsigned int total_replay_count, unsigned int percentage_to_be_simulated,
			HostInterfaceType SSD_device_type, PCIe_Root_Complex* pcie_root_complex);
		~IO_Flow_Trace_Based();
		Host_IO_Reqeust* Generate_next_request();
		void NVMe_consume_io_request(Completion_Queue_Entry*);
		void Start_simulation();
		void Validate_simulation_config();
		void Execute_simulator_event(MQSimEngine::Sim_Event*);
		void Get_statistics(Preconditioning::Workload_Statistics& stats);
	private:
		Trace_Time_Unit time_unit;
		unsigned int percentage_to_be_simulated;
		std::string trace_file_path;
		std::ifstream trace_file;
		unsigned int total_replay_no, replay_counter;
		unsigned int total_requests_in_file;
		std::vector<std::string> current_trace_line;
		sim_time_type time_offset;
	};
}

#endif// !IO_FLOW_TRACE_BASED_H