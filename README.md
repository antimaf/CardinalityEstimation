# Cardinality Estimation Engine

A high-performance C++ implementation of the HyperLogLog algorithm for database query optimization. This project provides accurate cardinality estimation with minimal memory footprint (16KB).

## 🚀 Quick Start

### Prerequisites

- CMake (version 3.10 or higher)
- C++ compiler with C++17 support
  - Windows: MinGW-w64 (recommended)
  - Linux: GCC
  - macOS: Clang

For Windows users coming from other languages:
1. Download MinGW-w64 from [here](https://www.mingw-w64.org/downloads/)
2. Download CMake from [here](https://cmake.org/download/)
3. Add both to your system PATH

### Building the Project

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Generate build files
cmake -G "MinGW Makefiles" ..

# 3. Build the project
mingw32-make
```

## 📚 Project Structure

### Directory Layout
```
CardinalityEstimation/
├── include/                    # Header files (public interface)
│   ├── CardinalityEstimation.h  # Main public API
│   └── common/                  # Shared definitions
│       └── Root.h               # Base definitions
├── src/                        # Implementation files
│   ├── CEEngine.cpp            # HyperLogLog implementation
│   └── main.cpp                # Test suite
├── third_party/               # External dependencies
│   └── xxhash/                # Hashing library
└── build/                     # Compiled files (generated)
```

## 🔍 Algorithm Deep Dive

### HyperLogLog Implementation

The core algorithm works through these key steps:

1. **Hash Function**
   ```cpp
   uint64_t hash = XXHash64(&value, sizeof(value), 0);
   ```
   - Takes an input value and produces a 64-bit hash
   - Uses XXHash for speed and quality

2. **Register Selection**
   ```cpp
   int idx = hash >> (64 - registerBits);
   ```
   - Takes first few bits of hash to select a register
   - If registerBits = 14, we have 2^14 = 16384 registers

3. **Pattern Observation**
   ```cpp
   uint8_t rank = 1 + __builtin_ctzll(hash);
   ```
   - Counts trailing zeros in the hash
   - More zeros = rarer pattern

4. **Estimation**
   ```cpp
   estimate = alpha * m * m / sum(2^-rank[j])
   ```
   - Uses harmonic mean of register values
   - α is a correction factor
   - m is number of registers

### Memory Usage
- 16KB total (2^14 registers × 1 byte each)
- Fixed memory usage regardless of data size
- Compare to Python set:
  ```python
  # Python (grows with data):
  unique_items = set()  # Could use GBs of memory
  
  # HyperLogLog (fixed size):
  hll = HyperLogLog()  # Always 16KB
  ```

### Error Rate Analysis
1. **Standard Error**: ~1.04/√m
   - m = number of registers (16384)
   - Expected error ≈ 0.81%

2. **Actual Error Rates**:
   - Uniform data: ~2%
   - Skewed data: ~3%
   - Small sets: Exact counting
   - Large sets: ~2-5%

## 🛠 Core Functions

### CEEngine Class

```cpp
void insertTuple(const std::tuple<int, int>& tuple)
```
- **What it does**: Adds a new item to count
- **Usage example**: `engine.insertTuple({1, 2})`

```cpp
double estimate()
```
- **What it does**: Returns estimated unique count
- **Usage example**: `double count = engine.estimate()`

```cpp
void prepare()
```
- **What it does**: Resets the engine
- **Usage example**: `engine.prepare()`

## 🔧 Testing

The project includes comprehensive tests for:
1. Uniform Distribution
2. Skewed Distribution
3. Small Cardinality
4. Large Cardinality
5. Constant Values
6. Sequential Values
7. Many Duplicates

Run tests with:
```bash
cd build
./main
```

## 🚫 Common Errors 

1. **Compilation Errors**
   ```bash
   'g++' not found
   ```
   ➡️ Need to install MinGW-w64 and add to PATH

2. **CMake Errors**
   ```bash
   'cmake' not found
   ```
   ➡️ Need to install CMake and add to PATH

3. **Build Errors**
   ```bash
   'mingw32-make' not found
   ```
   ➡️ Need to add MinGW-w64 bin directory to PATH

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch
3. Make your changes
4. Submit a pull request

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.
