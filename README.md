# upmem-cmake-template

A CMake project that demonstrates how to build and test a library for the UPMEM hardware.

## Requirements

- CMake 3.16+
- UPMEM SDK 2025.1
- Python 3.6 (required by the UPMEM simulator 😞)

## Getting started

### 1\. Clone the repository

```sh
git clone https://github.com/sigerror/upmem-cmake-template.git
cd upmem-cmake-template
```

### 2\. Configure

```sh
cmake -S . -B ./build
```

### 3\. Build

```sh
cmake --build ./build
```

### 4\. Run tests

```sh
ctest --test-dir ./build
```

## Config

| Variable            | Description                                   | Default value |
| ------------------- | --------------------------------------------- | ------------- |
| `MYLIB_NR_DPUS`     | Total number of DPUs available                | 16            |
| `MYLIB_NR_TASKLETS` | Number of tasklets per DPU (v1A: 24, v1B: 16) | 16            |
| `MYLIB_MIN_XFER_SZ` | Minimum memory tranfer size (bytes)           | 8             |

## Notes

### Python

Conda environment is not recommended, since it may shadow the libraries found by CMake (e.g. libgomp.so). Instead use the system or local installation of Python.

```sh
wget -qO- https://www.python.org/ftp/python/3.6.15/Python-3.6.15.tgz | tar -xzvf -
cd Python-3.6.15 && ./configure \
    --enable-shared \
    --enable-optimizations \
    --with-lto \
    --prefix=$HOME/.python
make -j$(nproc)
make altinstall
```

Usage:
```sh
cmake -S . -B ./build -DCMAKE_PREFIX_PATH=$HOME/.python
```

### CPM

Consider using [`CPM_SOURCE_CACHE`](https://github.com/cpm-cmake/CPM.cmake/tree/master?tab=readme-ov-file#cpm_source_cache) to avoid re-downloading dependencies.

```sh
cmake -S . -B ./build -DCPM_SOURCE_CACHE=$HOME/.cache/CPM
```
