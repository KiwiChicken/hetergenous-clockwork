# Clockwork

A multi-tenant managed inference server, backed by a modified version of TVM.

If this README hasn't been updated, it means the system is still a work-in-progress.

# Pre-requisites

## CUDA

Make sure CUDA is installed and on your PATH

## Installing TVM

Clone our modified TVM and check out our modified branch:
```
git clone --recursive https://gitlab.mpi-sws.org/cld/ml/tvm
cd tvm
git checkout clockwork
```

Build TVM
```
mkdir build
cd build
cmake ..
make -j40
```

Set TVM_HOME environment variable
```
echo "export TVM_HOME=`pwd`" >> ~/.bashrc
source ~/.bashrc
```

Optionally you can add $TVM_HOME to your LD_LIBRARY_PATH and DYLD_LIBRARY_PATH environment variables

## Apt packages

The following apt packages pre-requisites:

Intel Threading Building Blocks
```
apt-get install libtbb-dev
```

# Building Clockwork

```
mkdir build
cd build
cmake ..
make
sudo make install
```

# Highly Recommended Environment Modifications

Clockwork is a high-performance system that depends upon predictability.  There are various tweaks to your environment that will make executions more predictable:

## 1. Disable CPU frequency autoscaling

Set the "performance" governor to prevent CPU clock scaling
```
echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
```

## 2. Increase file and memlock limits

Limits on the number of open files, and the amount of page-locked memory, reduce the total number of DNNs clockwork can keep in memory at any point in time.

Limits can be checked with the `ulimit` command (`ulimit -aH` lists hard limits, `ulimit -a` lists current)

Increase the RLIMIT_NOFILE (number of open files) and RLIMIT_MEMLOCK (amount of page-locked memory) to unlimited:
1. Open /etc/security/limits.conf
2. Add the following lines:
```
*            hard   memlock           unlimited
*            soft   memlock           unlimited
*            hard   nofile            unlimited
*            soft   nofile            unlimited
```
3. Modify /etc/systemd/user.conf and /etc/systemd/system.conf to add:
```
DefaultLimitNOFILE=65535
```
4. Restart

## 3. Disable CUDA JIT

None of the models we feed to Clockwork should have CUDA PTX code (JIT'able code) -- to make sure, set the CUDA_CACHE_DISABLE environment variable

## Disable GPU frequency autoscaling

Enable persistence mode
```
nvidia-smi -pm 1
```

Disable auto boost
```
nvidia-smi --auto-boost-default=DISABLED
```

List available GPU clock frequencies
```
nvidia-smi -q -d SUPPORTED_CLOCKS
```

Pick a memory and graphics clock frequency (usually the highest), e.g.
```
    Supported Clocks
        Memory                      : 3004 MHz
            Graphics                : 1114 MHz
```

Set the default application clock and system clock to those highest values
```
nvidia-smi -ac 3004,1114
nvidia-smi -lgc 1114
```

## Check

Some of these values can be checked by running the Clockwork profiler with:
```
./profile [check]
```
