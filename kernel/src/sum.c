#include <alloc.h>
#include <barrier.h>
#include <defs.h>
#include <mram.h>

BARRIER_INIT(sync_all, NR_TASKLETS);

__host int n;
int sum[NR_TASKLETS];

int main(void) {
    const int tid = me();
    const int n_per_tasklet = n / NR_TASKLETS;

    int* src_mram = (int*) (DPU_MRAM_HEAP_POINTER + tid * n_per_tasklet * sizeof(int));
    int* src_wram = (int*) mem_alloc(n_per_tasklet * sizeof(int));
    mram_read((__mram_ptr void*) src_mram, src_wram, n_per_tasklet * sizeof(int));

    int acc = 0;
    for (int i = 0; i < n_per_tasklet; ++i) {
        acc += src_wram[i];
    }
    sum[tid] = acc;

    barrier_wait(&sync_all);

    for (int s = NR_TASKLETS / 2; s > 1; s /= 2) {
        if (tid < s) {
            sum[tid] += sum[tid + s];
        }
        barrier_wait(&sync_all);
    }

    if (tid == 0) {
        mram_write(sum, (__mram_ptr void*) DPU_MRAM_HEAP_POINTER, sizeof(int) * 2);
    }
    return 0;
}
