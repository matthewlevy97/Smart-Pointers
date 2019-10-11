#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "smart_pointers.h"

void *smart_malloc(size_t size)
{
    char * ptr;
    struct _smart_pointer_s *header;
    
    ptr = malloc(size + sizeof(struct _smart_pointer_s));
    if(!ptr) return NULL;
    
    header = ptr;
    header->signature       = SMART_POINTER_SIGNATURE;
    header->valid           = VALID_POINTER;
    header->reference_count = 0;
    header->size            = size;
    header->new_ptr         = NULL;
    
    return ptr + sizeof(*header);
}

void smart_free(void *ptr)
{
    struct _smart_pointer_s *header;
    if(!ptr) return;
    
    header = ((char*)ptr - sizeof(struct _smart_pointer_s));
    
    // Ensure this is really a smart pointer
    if(header->signature != SMART_POINTER_SIGNATURE)
        goto free_ptr;
    
    // Are there other references?
    if(--header->reference_count)
        goto dont_free;
    
    ptr = header;
    
free_ptr:
    free(ptr);
dont_free:
    return;
}

void *smart_calloc(size_t nmemb, size_t size)
{
    void * ptr;
    int malloc_size;
    
    malloc_size = nmemb * size;
    ptr = smart_malloc(malloc_size);
    if(!ptr) return NULL;
    
    memset(ptr, 0, malloc_size);
    return ptr;
}

void *smart_realloc(void *ptr, size_t size)
{
    void * realloc_ptr;
    struct _smart_pointer_s *header;
    
    header = ((char*)ptr - sizeof(struct _smart_pointer_s));
    
    // Ensure this is really a smart pointer
    if(header->signature != SMART_POINTER_SIGNATURE)
        return realloc(ptr, size);
    
    // Catchup if we can
    ptr = smart_catchpointer(ptr);
    header = ((char*)ptr - sizeof(struct _smart_pointer_s));
    
    // Only realloc is new size is larger than old
    if(header->size < size) {
        // Realloc
        realloc_ptr = smart_malloc(size);
        if(!realloc_ptr) return NULL;
        
        // Copy over content
        memcpy(realloc_ptr, ptr, header->size);
        
        // There are others, so mark this as invalid
        header->valid = INVALID_POINTER;
        header->new_ptr = realloc_ptr;
        
        smart_free(ptr);
        ptr = realloc_ptr;
    }
    
    return ptr;
}

void *smart_reference(void *ptr)
{
    struct _smart_pointer_s *header;
    header = ((char*)ptr - sizeof(struct _smart_pointer_s));
    
    // Ensure this is a smart pointer
    if(header->signature != SMART_POINTER_SIGNATURE) goto return_ptr;
    
    header->reference_count++;
    
return_ptr:
    return ptr;
}

void *smart_catchpointer(void *ptr)
{
    void *tmp;
    struct _smart_pointer_s *header;
    header = ((char*)ptr - sizeof(struct _smart_pointer_s));
    
    // Ensure this is a smart pointer
    if(header->signature != SMART_POINTER_SIGNATURE) goto return_ptr;
    
    while(header->valid == INVALID_POINTER) {
        tmp = header->new_ptr;
        // Ensure next is not NULL
        if(!tmp) goto return_ptr;
        
        // Free old pointer
        smart_free(ptr);
        
        // Get new pointer
        ptr = tmp;
        header = ((char*)ptr - sizeof(struct _smart_pointer_s));
        if(header->signature != SMART_POINTER_SIGNATURE)
            goto return_ptr;
    }
    
    ptr = smart_reference(ptr);

return_ptr:
    return ptr;
}
