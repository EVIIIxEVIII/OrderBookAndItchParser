#include <benchmark/benchmark.h>
#include <fstream>
#include <vector>
#include <cstddef>
#include <iostream>

#include "parser_v2.hpp"

class Handler {
public:
    void handle(ITCHv2::Message msg) {
        messages_num++;
    }

    long long messages_num = 0;
};

static const std::vector<std::byte> first_chunk(size_t size) {
    std::vector<std::byte> buf = [size] {
        std::ifstream file("../data/01302019.NASDAQ_ITCH50",
                           std::ios::binary);
        if (!file) {
            throw std::runtime_error("Failed to open ITCH file");
        }

        std::vector<std::byte> tmp(size);
        file.read(reinterpret_cast<char*>(tmp.data()),
                  static_cast<std::streamsize>(tmp.size()));
        std::streamsize got = file.gcount();
        if (got <= 0) {
            throw std::runtime_error("Failed to read first chunk");
        }
        tmp.resize(static_cast<std::size_t>(got));
        return tmp;
    }();
    return buf;
}


static void BM_ParseMsg_v2(benchmark::State& state) {
    static const std::vector<std::byte> buf = first_chunk(4096);
    ITCHv2::ItchParser parser;

    const std::byte* vsrc = buf.data();
    ITCHv2::MessageType last_type = ITCHv2::MessageType::SYSTEM_EVENT;

    for (auto _ : state) {
        auto msg =  parser.parseMsg(vsrc);
        benchmark::DoNotOptimize(msg);
    }

    state.SetItemsProcessed(state.iterations());
}

static void BM_Parse_v2(benchmark::State& state) {
    const auto size = state.range(0);
    std::vector<std::byte> src_buf = first_chunk(size);

    const std::byte* src = src_buf.data();

    ITCHv2::ItchParser parser;
    for (auto _ : state) {
        Handler handler{};
        parser.parse(src, size, handler);
        benchmark::DoNotOptimize(handler);
    }

    state.SetBytesProcessed(size * state.iterations());
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_Parse_v2)
    ->Args({4096})
    ->Args({8192})
    ->Args({16 * 1024})
    ->Args({32 * 1024})
    ->Args({64 * 1024})
    ->Args({128 * 1024});
BENCHMARK(BM_ParseMsg_v2);
