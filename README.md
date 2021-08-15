# filters

## Build Test
```shell
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DFILTERS_TEST=1 -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

## Build Benchmark
```shell
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DFILTERS_TEST=1 -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Run Benchmarks
```shell
sudo perf stat -e cache-misses ./bloom_filter_bench
```