#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "clockwork/runtime.h"
#include "clockwork/clockwork.h"
#include <sstream>
#include <atomic>
#include <thread>
#include <fstream>
#include <istream>
#include "clockwork/modeldata.h"
#include "clockwork/serializedmodel.h"
#include <dlpack/dlpack.h>
#include <tvm/runtime/module.h>
#include <tvm/runtime/registry.h>
#include <tvm/runtime/packed_func.h>
#include <pods/pods.h>
#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/streams.h>
#include "clockwork/tvm/decoupled_graph_runtime.h"
#include "clockwork/model.h"
#include <cuda_runtime.h>
#include <chrono>
#include "clockwork/util/util.h"
#include "clockwork/util/tvm_util.h"

using namespace clockwork;

struct ProfileData {
    std::chrono::high_resolution_clock::time_point cool, warm, malloced, hot, submitted, complete, warm2, warm2freed, cool2;
};

uint64_t nanos(std::chrono::high_resolution_clock::time_point t) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
}


void loadmodel() {
    tvmutil::initializeTVMCudaStream();
    util::setCurrentThreadMaxPriority();
    util::set_core(7);

	const int dtype_code = kDLFloat;
	const int dtype_bits = 32;
	const int dtype_lanes = 1;
	const int device_type = kDLGPU;
	const int device_id = 0;

	std::string model = "/home/jcmace/modelzoo/resnet18/tesla-m40_batchsize1/resnet18v2-batchsize1-optimized";


    ColdDiskModel* cold = new ColdDiskModel(
            model + ".so",
            model + ".clockwork",
            model + ".clockwork_params"
        );
    CoolModel* cool = cold->load();



    void* ptr;



    unsigned runs = 1000;
    std::vector<ProfileData> d(runs);
    for (unsigned i = 0; i < runs; i++) {
        if (i % 100 == 0) {
            std::cout << "Run " << i << std::endl;

            size_t free, total;
            CUDA_CALL(cudaMemGetInfo(&free, &total));
            std::cout << "   GPU " << (total-free) << " used" << std::endl;
        }
        d[i].cool = std::chrono::high_resolution_clock::now();

        WarmModel* warm = cool->load();
        d[i].warm = std::chrono::high_resolution_clock::now();

        if (i == 0) {
            CUDA_CALL(cudaMalloc(&ptr, warm->size()));
        }
        d[i].malloced = std::chrono::high_resolution_clock::now();

        HotModel* hot = warm->load(ptr);
        CUDA_CALL(cudaStreamSynchronize(tvm::runtime::ManagedCUDAThreadEntry::ThreadLocal()->stream));
        d[i].hot = std::chrono::high_resolution_clock::now();

        hot->call();
        d[i].submitted = std::chrono::high_resolution_clock::now();

        CUDA_CALL(cudaStreamSynchronize(tvm::runtime::ManagedCUDAThreadEntry::ThreadLocal()->stream));
        d[i].complete = std::chrono::high_resolution_clock::now();

        hot->unload();
        d[i].warm2 = std::chrono::high_resolution_clock::now();

        d[i].warm2freed = std::chrono::high_resolution_clock::now();

        warm->unload();
        d[i].cool2 = std::chrono::high_resolution_clock::now();
    }
    CUDA_CALL(cudaFree(ptr));

    std::ofstream out("times.out");
    out << "i" << "\t"
        << "t" << "\t"
        << "cool->warm" << "\t"
        << "warm->malloced" << "\t"
        << "malloced->hot" << "\t"
        << "hot->submitted" << "\t"
        << "submitted->complete" << "\t"
        << "complete->warm2" << "\t"
        << "warm2->freed" << "\t"
        << "freed->cool" << "\n";
    for (unsigned i = 10; i < d.size(); i++) {
        out << i << "\t"
            << nanos(d[i].cool) << "\t"
            << nanos(d[i].warm) - nanos(d[i].cool) << "\t"
            << nanos(d[i].malloced) - nanos(d[i].warm) << "\t"
            << nanos(d[i].hot) - nanos(d[i].malloced) << "\t"
            << nanos(d[i].submitted) - nanos(d[i].hot) << "\t"
            << nanos(d[i].complete) - nanos(d[i].submitted) << "\t"
            << nanos(d[i].warm2) - nanos(d[i].complete) << "\t"
            << nanos(d[i].warm2freed) - nanos(d[i].warm2) << "\t"
            << nanos(d[i].cool2) - nanos(d[i].warm2freed) << "\n";
    }
    out.close();
}

int main(int argc, char *argv[]) {
	std::cout << "begin" << std::endl;

	loadmodel();

	std::cout << "end" << std::endl;
}
