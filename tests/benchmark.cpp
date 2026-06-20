#include "../include/OrderBook.h"
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>

int main() {
    OrderBook book;
    
    const int NUM_ORDERS = 1000000;
    
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<uint64_t> priceDist(14900, 15100);
    std::uniform_int_distribution<uint32_t> qtyDist(10, 1000);

    std::cout << "Starting High-Frequency Benchmark...\n";
    std::cout << "Injecting " << NUM_ORDERS << " randomized orders.\n\n";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Side side = (sideDist(rng) == 0) ? Side::BUY : Side::SELL;
        book.addOrder(i, side, priceDist(rng), qtyDist(rng));
    }

    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> diff = end - start;
    double seconds = diff.count();
    
    double opsPerSec = NUM_ORDERS / seconds;
    double avgLatencyNs = (seconds * 1e9) / NUM_ORDERS;

    std::cout << "========================================\n";
    std::cout << "          BENCHMARK RESULTS             \n";
    std::cout << "========================================\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Time:       " << seconds << " seconds\n";
    std::cout << "Throughput:       " << (opsPerSec / 1000000.0) << " Million Ops/sec\n";
    std::cout << "Average Latency:  " << avgLatencyNs << " nanoseconds/order\n";
    std::cout << "========================================\n";

    return 0;
}