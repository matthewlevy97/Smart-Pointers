#include <stdio.h>
#include <string.h>
#include "malloc_wrapper.h"

int main(int argc, char ** argv)
{
    void *ptrs[4];
    
    ptrs[0] = smart_malloc(10);
    ptrs[1] = smart_reference(ptrs[0]);
    ptrs[2] = smart_reference(ptrs[0]);
    
    strcpy(ptrs[1], "test");
    ptrs[2] = smart_realloc(ptrs[2], 15);
    
    smart_free(ptrs[0]);
    smart_free(ptrs[1]);
    
    ptrs[0] = smart_catchpointer(ptrs[2]);
    ptrs[1] = smart_catchpointer(ptrs[2]);
    ptrs[3] = smart_catchpointer(ptrs[2]);
    
    strcpy(ptrs[3], "testing");
    printf("%s\n", ptrs[1]);
    ptrs[1] = smart_catchpointer(ptrs[0]);
    printf("%s\n", ptrs[1]);
    
    printf("%p %p %p %p\n", ptrs[0], ptrs[1], ptrs[2], ptrs[3]);
    smart_free(ptrs[0]);
    smart_free(ptrs[1]);
    smart_free(ptrs[2]);
    smart_free(ptrs[3]);
    
    return 0;
}
