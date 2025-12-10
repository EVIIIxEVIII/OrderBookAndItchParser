#include <benchmark/benchmark.h>
#include <endian.h>
#include <memory.h>

static void BM_bswap32(benchmark::State& state) {
    uint32_t t = 87760787;

    for (auto _ : state) {
        benchmark::DoNotOptimize(t);
        uint32_t res = 0;
        res = __builtin_bswap32(t);
        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();
    }
}

static void BM_bswap64(benchmark::State& state) {
    uint64_t t = 87760787;

    for (auto _ : state) {
        benchmark::DoNotOptimize(t);
        uint64_t res = 0;
        res = __builtin_bswap64(t);
        benchmark::DoNotOptimize(res);
        benchmark::ClobberMemory();
    }
}


inline uint64_t load_be48(const std::byte* p) {
    return (uint64_t(p[0]) << 40) |
           (uint64_t(p[1]) << 32) |
           (uint64_t(p[2]) << 24) |
           (uint64_t(p[3]) << 16) |
           (uint64_t(p[4]) << 8)  |
           uint64_t(p[5]);
}

inline uint64_t load_be48_v2(const std::byte* p) {
    uint64_t v;
    memcpy(&v, p, 6);
    v = __builtin_bswap64(v);
    return v >> 16;
}

static void BM_load_be48(benchmark::State& state) {
    std::array<std::byte, 6> buf{
        std::byte{0x01}, std::byte{0x23}, std::byte{0x45},
        std::byte{0x67}, std::byte{0x89}, std::byte{0xAB}
    };

    const std::byte* p = buf.data();
    uint64_t sink = 0;

    for (auto _ : state) {
        benchmark::DoNotOptimize(sink = load_be48(p));
    }

    benchmark::DoNotOptimize(sink);
}


static void BM_load_be48_v2(benchmark::State& state) {
    std::array<std::byte, 6> buf{
        std::byte{0x01}, std::byte{0x23}, std::byte{0x45},
        std::byte{0x67}, std::byte{0x89}, std::byte{0xAB}
    };

    const std::byte* p = buf.data();
    uint64_t sink = 0;

    for (auto _ : state) {
        benchmark::DoNotOptimize(sink = load_be48_v2(p));
    }

    benchmark::DoNotOptimize(sink);
}

static void BM_load_be48_v3(benchmark::State& state) {
    std::array<std::byte, 8> buf{
        std::byte{0x01}, std::byte{0x23}, std::byte{0x45},
        std::byte{0x67}, std::byte{0x89}, std::byte{0xAB}, std::byte{0x89}, std::byte{0xAB}
    };

    const std::byte* p = buf.data();
    uint64_t sink = 0;

    for (auto _ : state) {
        benchmark::DoNotOptimize(sink = be64toh(uint64_t(*p)) >> 16);
    }

    benchmark::DoNotOptimize(sink);
}

BENCHMARK(BM_load_be48);
BENCHMARK(BM_load_be48_v2);
BENCHMARK(BM_load_be48_v3);

BENCHMARK(BM_bswap32);
BENCHMARK(BM_bswap64);
