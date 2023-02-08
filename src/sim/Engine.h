#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include "Sim_Defs.h"
#include "EventTree.h"
#include "Sim_Object.h"
#include "Host_IO_Request.h"

namespace MQSimEngine {
	class Engine
	{
		friend class EventTree;
	public:
		Engine()
		{
			this->_EventList = new EventTree;
			started = false;
		}

		~Engine() {
			delete _EventList;
		}

        uint64_t get_next_request_id()
        {
            return ++req_id;
        }

        static Engine* Instance();
		sim_time_type Time();
		Sim_Event* Register_sim_event(sim_time_type fireTime, Sim_Object* targetObject, void* parameters = NULL, int type = 0);
		void Ignore_sim_event(Sim_Event*);
		void Reset();
		void AddObject(Sim_Object* obj);
		Sim_Object* GetObject(sim_object_id_type object_id);
		void RemoveObject(Sim_Object* obj);
		void Start_simulation();
		void Stop_simulation();
		bool Has_started();
		bool Is_integrated_execution_mode();
        void report_request_complete(completion_info_t *info);
        void print_stats();

    private:
		sim_time_type _sim_time;
		EventTree* _EventList;
        uint64_t req_id = 0;
		std::unordered_map<sim_object_id_type, Sim_Object*> _ObjectList;
		bool stop;
		bool started;
        std::queue<completion_info_t*> done_queue;
		static Engine* _instance;
	};
}

#define Simulator MQSimEngine::Engine::Instance()
#endif // !ENGINE_H
