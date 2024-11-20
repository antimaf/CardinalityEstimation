# Cardinality Estimation Development Guide

This guide provides an in-depth explanation of the HyperLogLog algorithm implementation and the project structure.

## Table of Contents
- [HyperLogLog Algorithm](#hyperloglog-algorithm)
- [Project Structure](#project-structure)
- [Building From Scratch](#building-from-scratch)
- [File Deep Dive](#file-deep-dive)

## HyperLogLog Algorithm

### Overview
HyperLogLog (HLL) is a probabilistic algorithm used to estimate the number of unique elements (cardinality) in a large dataset. It's like having a super-efficient Python set() that uses constant memory.

### How It Works

1. **Hash Function**
   ```cpp
   uint64_t hash = XXHash64(&value, sizeof(value), 0);
   ```
   - Takes an input value and produces a 64-bit hash
   - Similar to Python's hash() but more uniform
   - We use XXHash for speed and quality

2. **Register Selection**
   ```cpp
   int idx = hash >> (64 - registerBits);
   ```
   - Takes first few bits of hash to select a register
   - If registerBits = 14, we have 2^14 = 16384 registers
   - Like distributing items into 16384 buckets

3. **Pattern Observation**
   ```cpp
   uint8_t rank = 1 + __builtin_ctzll(hash);
   ```
   - Counts trailing zeros in the hash
   - More zeros = rarer pattern
   - Like rolling a die and counting consecutive 6's

4. **Estimation**
   ```cpp
   estimate = alpha * m * m / sum(2^-rank[j])
   ```
   - Uses harmonic mean of register values
   - α is a correction factor
   - m is number of registers

### Memory Usage
- 16KB total (2^14 registers × 1 byte each)
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

## Project Structure

### Directory Layout Explained
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

## Building From Scratch

### 1. Setting Up CMake
CMake is like package.json for C++. It defines:
- Which files to compile
- How to compile them
- Dependencies needed

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(CardinalityEstimation)

# Like "type": "module" in package.json
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Like "dependencies" in package.json
add_library(xxhash STATIC
    third_party/xxhash/xxhash.c
)

# Like defining your main entry point
add_executable(main 
    src/main.cpp
    src/CEEngine.cpp
)
```

### 2. Header Files (.h)
Headers are like TypeScript interface definitions:

```cpp
// CardinalityEstimation.h
class CEEngine {
public:
    void insertTuple(tuple);  // Method declaration
};

// Similar to TypeScript:
interface CEEngine {
    insertTuple(tuple: [number, number]): void;
}
```

### 3. Implementation Files (.cpp)
Implementation files are like the actual JavaScript class:

```cpp
// CEEngine.cpp
#include "CardinalityEstimation.h"

void CEEngine::insertTuple(tuple t) {
    // Implementation here
}

// Similar to JavaScript:
class CEEngine {
    insertTuple(tuple) {
        // Same implementation
    }
}
```

## File Deep Dive

### 1. CardinalityEstimation.h
```cpp
class CEEngine {
    void insertTuple(const std::tuple<int, int>& tuple);
    double estimate();
    void prepare();
};
```
- Public interface for the engine
- Uses pImpl pattern for encapsulation
- Like a Python abstract base class

### 2. CEEngine.cpp
Core implementation containing:
1. **HyperLogLog Class**
   - Register management
   - Hash function implementation
   - Estimation algorithms
   - Bias correction

2. **CEEngine Implementation**
   - Tuple handling
   - Memory management
   - State tracking

### 3. main.cpp (Test Suite)
Contains comprehensive tests:
```cpp
void runTest(const std::string& testName, 
            int numTuples,
            std::function<std::tuple<int,int>()> generator)
```
- Test framework
- Data generators
- Performance measurements

Test Categories:
1. Uniform Distribution
   ```cpp
   std::uniform_int_distribution<> dis(0, VALUE_RANGE);
   ```
   - Random, evenly distributed values
   - Baseline performance test

2. Skewed Distribution
   ```cpp
   std::exponential_distribution<> exp_dis(0.0001);
   ```
   - Some values more common than others
   - Tests bias correction

3. Small/Large Cardinality
   - Tests scaling behavior
   - Checks estimation accuracy

4. Edge Cases
   - Constant values
   - Sequential values
   - Many duplicates

### 4. xxhash Library
- High-performance hashing
- Header-only implementation
- No external dependencies

## Performance Optimization Tips

1. **Memory Access**
   ```cpp
   // Bad (cache misses):
   for(each register)
     if(register[hash] < value)
       update

   // Good (sequential access):
   idx = hash >> bits
   registers[idx] = max(registers[idx], value)
   ```

2. **Bit Operations**
   ```cpp
   // Fast trailing zero count:
   __builtin_ctzll(hash)
   
   // Fast register selection:
   hash >> (64 - registerBits)
   ```

3. **Hash Function Choice**
   - XXHash: Fast, good distribution
   - MurmurHash: Alternative option
   - Custom hash: Possible but tricky

## Common Development Tasks

### Adding New Features
1. Update CardinalityEstimation.h
2. Implement in CEEngine.cpp
3. Add tests in main.cpp

### Debugging Tips
1. Use small datasets first
2. Check hash distribution
3. Validate register values
4. Compare with exact counting

### Performance Testing
1. Use different distributions
2. Measure memory usage
3. Profile critical sections
4. Compare with baselines
