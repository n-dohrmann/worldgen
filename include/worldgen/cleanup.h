
#ifndef CLEANUP_H
#define CLEANUP_H

#define CLEANUP_MAX_SIZE 256

// cleanup function type
typedef void (*cleanup_fn)(void*);

typedef struct {
    cleanup_fn cleanup;
    void *data;
} Cleanup;

typedef struct {
    Cleanup fns[CLEANUP_MAX_SIZE];
    int size;
} CleanupStack;

// defer memory cleanup action
void defer(cleanup_fn cleanup, void *data);

// singleton stack
CleanupStack cl_stack;

void clean_all();

#endif // CLEANUP_H
