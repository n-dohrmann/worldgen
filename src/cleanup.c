
#include <worldgen/cleanup.h>

CleanupStack cl_stack = { 0 };

void defer(cleanup_fn cleanup, void* data)
{
    if (cl_stack.size >= CLEANUP_MAX_SIZE) {
        return;
    }
    cl_stack.fns[cl_stack.size++] = (Cleanup) { cleanup, data };
}

void clean_all(void)
{
    for (int i = cl_stack.size - 1; i >= 0; i--) {
        cl_stack.fns[i].cleanup(cl_stack.fns[i].data);
    }
    cl_stack = (CleanupStack) { 0 };
}
