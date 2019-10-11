# Smart Pointers

## What is this?
This is an implementation of smart pointers in C. Pointers use reference counting to know when to free a memory region.

## What this is NOT
- NOT thread
- NOT production ready
    - Because of the way the smart pointers are implemented, a pointer is prepended with a "smart pointer header". To access this pointer the passed ptr address is decremented by the size of the structure. This can cause some problems be reading memory regions that should not be accessible.
    - Faking the "smart pointer header" signature can cause unintended consequences when interacting with a pointer.

## Documentation
```
void *smart_malloc(size_t size)
    - Returns a memory region (like normal malloc)

```
```
void smart_free(void *ptr)
    - Frees a memory region
    - If reference counts still exist for the region, "free" will not be called

```
```
void *smart_calloc(size_t nmemb, size_t size)
    - Returns a memory region zeroed out (like normal calloc)

```
```
void *smart_realloc(void *ptr, size_t size)
    - Returns a memory region to a new location (like normal realloc)
    - Calls smart_free on ptr
```
```
void *smart_reference(void *ptr)
    - Add new reference count to smart pointer

```
```
void *smart_catchpointer(void *ptr)
    - Catch up to latest pointer given a non-free'd pointer
    - Prevents problem of calling realloc and forgetting to update another pointer to new address
```
