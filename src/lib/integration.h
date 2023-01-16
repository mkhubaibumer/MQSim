//
// Created by Khubaib Umer on 11/01/2023.
//

#ifndef INTEGRATION_H
#define INTEGRATION_H

#include <semaphore.h>

#ifndef __cplusplus
#define bool uint8_t
#define true 1
#define false 0
#define nullptr NULL
#endif

typedef uint64_t (*clock_f)(void);
typedef void (*reg_clock_f) (clock_f);

typedef struct {
    sem_t semaphore;
    clock_f master_clock;
    bool init;
} mag_data_t;

// mag_layer_t will be a singleton instance
typedef struct {
    reg_clock_f set_master_clock;
    clock_f get_logical_time;
    void (*notify)(void); // post event to blocked thread
    void (*wait)(void); // blocks until an event is posted
    // For internal use
    void *internal;
} mag_layer_t;

mag_layer_t* get_instance();

static mag_layer_t* get_mag_integration() { return get_instance(); }

#define MAGNITION get_mag_integration()

#endif //INTEGRATION_H
