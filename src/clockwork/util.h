#ifndef _CLOCKWORK_UTIL_H_
#define _CLOCKWORK_UTIL_H_

#include <cstdint>
#include <string>
#include <cuda_runtime.h>
#include <chrono>


namespace clockwork {
namespace util {

// High-resolution timer, current time in nanoseconds
std::uint64_t now();

std::chrono::high_resolution_clock::time_point hrt();

std::string nowString();

void set_core(unsigned core);

void setCudaFlags();

std::string getGPUmodel(int deviceNumber);

extern "C" char* getGPUModelToBuffer(int deviceNumber, char* buf);

void setCurrentThreadMaxPriority();


void readFileAsString(const std::string &filename, std::string &dst);


void initializeCudaStream();
cudaStream_t Stream();

}
}


#endif