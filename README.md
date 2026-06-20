## Performance Benchmarks
Tested via a deterministic randomized load-testing suite (`tests/benchmark.cpp`) simulating high-velocity market environments. Compiled with `-O3` aggressive optimizations.

* **Throughput:** 5.24 Million Operations / second
* **Average Latency:** 190.68 nanoseconds / order
* **Time Complexity:** $O(1)$ insertions, $O(1)$ cancellations

### To run the benchmark:
\`\`\`bash
g++ -std=c++17 -O3 tests/benchmark.cpp src/OrderBook.cpp -I include -o build/benchmark.exe
./build/benchmark.exe
\`\`\`