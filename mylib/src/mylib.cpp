#include <mylib/mylib.h>
#include <vector>

#include "kernel/sum.h"

namespace {
int ceil_div(const int x, const int y) {
    return (x + y - 1) / y;
}
}

int mylib_sum(const int n, const int* src) {
    const int blk_sz = [&]() {
        const int elem_per_dpu = MYLIB_NR_TASKLETS * MYLIB_MIN_XFER_SZ / sizeof(int);
        return ceil_div(ceil_div(n, MYLIB_NR_DPUS), elem_per_dpu) * elem_per_dpu;
    }();
    const int n_blks = n / blk_sz;

    struct dpu_set_t set;
    DPU_ASSERT(dpu_alloc(n_blks, nullptr, &set));
    DPU_ASSERT(dpu_load_from_incbin(set, &mylib_kernel_sum, nullptr));

    DPU_ASSERT(dpu_broadcast_to(set, "n", 0, &blk_sz, sizeof(blk_sz), DPU_XFER_DEFAULT));

    // Distribute `src` over DPUs
    struct dpu_set_t dpu;
    int i;
    DPU_FOREACH(set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, const_cast<int*>(&src[i * blk_sz])));
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_TO_DPU, DPU_MRAM_HEAP_POINTER_NAME, 0, blk_sz * sizeof(int),
        DPU_XFER_DEFAULT));

    DPU_ASSERT(dpu_launch(set, DPU_SYNCHRONOUS));

    // Gather results from DPUs
    std::vector<int> out(n_blks * 2);
    DPU_FOREACH(set, dpu, i) {
        DPU_ASSERT(dpu_prepare_xfer(dpu, &out[i * 2]));
    }
    DPU_ASSERT(dpu_push_xfer(set, DPU_XFER_FROM_DPU, DPU_MRAM_HEAP_POINTER_NAME, 0, sizeof(int) * 2,
        DPU_XFER_DEFAULT));

    DPU_ASSERT(dpu_free(set));

    // Further reduction on CPU
    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < out.size(); ++i) {
        sum += out[i];
    }
    return sum;
}
