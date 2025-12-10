#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <memory.h>
#include "parser.hpp"

inline uint64_t load_be48_v2(const std::byte* p) {
    uint64_t v;

    memcpy(&v, p, 6);
    v = __builtin_bswap64(v);
    return v >> 16;
}


class Handler {
public:
    void handle(ITCH::Message msg) {
        std::cout << char(msg.type) << '\n';
        messages_num++;
    }

    long long messages_num = 0;
};

int main() {
    std::ifstream file("../data/01302019.NASDAQ_ITCH50",
                       std::ios::binary | std::ios::in);

    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::vector<std::byte> src_buf;
    try {
        src_buf.resize(1ull << 30);
    } catch (const std::bad_alloc&) {
        std::cerr << "Allocation failed\n";
        return 1;
    }

    file.read(reinterpret_cast<char*>(src_buf.data()), src_buf.size());
    std::size_t bytes_read = file.gcount();

    const std::byte* src = src_buf.data();

    Handler handler{};
    ITCH::ItchParser parser;
    parser.parse(src, 1 * 1024 * 1024 * 1024, handler);

    std::cout << handler.messages_num << '\n';

    return 0;
}
