//
// Created by Khubaib Umer on 11/01/2023.
//

#include "integration.h"
#include <stdlib.h>

#define MAG_DATA ((mag_data_t*) MAGNITION->internal)

void set_clock(clock_f clock) { MAG_DATA->master_clock = clock; }

uint64_t get_clock_time() { return MAG_DATA->master_clock(); }

void notify_waiter() { sem_post(&MAG_DATA->semaphore); }

void wait_for_event() { sem_wait(&MAG_DATA->semaphore); }

mag_layer_t* get_instance() {
    static mag_data_t g_data = { .init = false };

    static mag_layer_t g_instance = {
            .set_master_clock = &set_clock,
            .get_logical_time = &get_clock_time,
            .notify = &notify_waiter,
            .wait = &wait_for_event,
            .internal = &g_data
    };

    if (((mag_data_t*)(g_instance.internal))->init == false) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        sem_init(&MAG_DATA->semaphore, 0, 0);
#pragma GCC diagnostic pop
        MAG_DATA->init = true;
    }
    return &g_instance;
}
