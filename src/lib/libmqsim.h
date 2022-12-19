//#ifdef BUILD_LIB

#pragma once

enum kREQUEST_TYPE {
    READ = 10,
    WRITE
};

typedef struct request_type {
    request_type(kREQUEST_TYPE _type, uint16_t _id, uint64_t _time, uint64_t _size, uint64_t _lba)
    : type(_type), dev_id(_id), arrival_time(_time), size_in_sectors(_size), start_sector_addr(_lba)
    { }
    kREQUEST_TYPE type;
    uint16_t dev_id;
    uint64_t arrival_time;
    uint64_t size_in_sectors;
    uint64_t start_sector_addr;
} request_type_t;

extern "C" inline void* mqsim_alloc(size_t size)
{
    return malloc(size);
}

extern "C" inline void* mqsim_free(void *ptr)
{
    return ((ptr) ? free(ptr), nullptr : nullptr);
}

extern "C" bool initialize(const char *ssd_cfg, const char *workload_cfg);

extern "C" void run_simulation();

extern "C" void destroy();

//#endif /* BUILD_LIB */
