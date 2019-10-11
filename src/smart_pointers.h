#pragma once

#define SMART_POINTER_SIGNATURE 0xCAFEBABE

#define VALID_POINTER 1
#define INVALID_POINTER 0

struct _smart_pointer_s {
    int signature;
    int valid;
    size_t reference_count;
    size_t size;
    void * new_ptr;
};

void *smart_malloc(size_t size);
void  smart_free(void *ptr);
void *smart_calloc(size_t nmemb, size_t size);
void *smart_realloc(void *ptr, size_t size);

void *smart_reference(void *ptr);
void *smart_catchpointer(void *ptr);
