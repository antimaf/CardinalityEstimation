# Cardinality Estimation Engine

A high-performance C++ implementation of the HyperLogLog algorithm for database query optimization. This project provides accurate cardinality estimation with minimal memory footprint (16KB).

## 🚀 Quick Start

### Prerequisites

- CMake (version 3.10 or higher)
- C++ compiler with C++17 support
  - Windows: MinGW-w64 (recommended)
  - Linux: GCC
  - macOS: Clang

For Windows users coming from Python/JavaScript:
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

### Core Components

#### 1. CEEngine (Cardinality Estimation Engine)
The main interface for cardinality estimation. Think of this like a Python class that manages everything:

```cpp
CEEngine engine;                    // Like: engine = CardinalityEngine()
engine.insertTuple({1, 2});        // Like: engine.insert_tuple((1, 2))
double estimate = engine.estimate() // Like: estimate = engine.estimate()
```

#### 2. HyperLogLog Implementation
The core algorithm that makes everything work. Similar to how you might use a Python set for counting unique items, but much more memory-efficient:

```python
# Python equivalent (simplified):
unique_items = set()
unique_items.add(item)  # Uses lots of memory
count = len(unique_items)

# Our C++ version:
CEEngine engine;        // Uses only 16KB
engine.insertTuple(item)
count = engine.estimate()
```

### File Structure

```
CardinalityEstimation/
├── include/                    # Header files (like Python module definitions)
│   ├── CardinalityEstimation.h  # Main interface
│   └── common/                  # Common utilities
├── src/                        # Source files (implementations)
│   ├── CEEngine.cpp            # Main engine implementation
│   └── main.cpp                # Test suite and examples
├── third_party/               # External libraries
│   └── xxhash/                # Fast hashing library
├── CMakeLists.txt            # Build configuration (like package.json)
└── README.md                 # This file
```

## 🔍 Key Features Explained

### 1. Memory Efficiency
- Uses only 16KB of memory regardless of data size
- In Python terms, it's like having a set() that never grows beyond 16KB
- Perfect for large databases where memory is crucial

### 2. Fast Insertion
- O(1) insertion time
- Can process millions of items per second
- No slowdown as data grows

### 3. Accuracy
- Typically within 2% error rate
- Automatically adjusts for different data patterns
- Handles edge cases (duplicates, constant values)

## 🛠 Core Functions Explained

### CEEngine Class

```cpp
void insertTuple(const std::tuple<int, int>& tuple)
```
- **What it does**: Adds a new item to count
- **Python equivalent**: `my_set.add(item)`
- **Usage example**: `engine.insertTuple({1, 2})`

```cpp
double estimate()
```
- **What it does**: Returns estimated unique count
- **Python equivalent**: `len(my_set)`
- **Usage example**: `double count = engine.estimate()`

```cpp
void prepare()
```
- **What it does**: Resets the engine
- **Python equivalent**: `my_set.clear()`
- **Usage example**: `engine.prepare()`

### Internal HyperLogLog Class

```cpp
void add(uint64_t value)
```
- **What it does**: Processes a single value
- **How it works**: Uses probabilistic counting
- **Memory trick**: Only stores the pattern of bits

```cpp
double estimate() const
```
- **What it does**: Calculates final estimate
- **How it works**: Uses statistical formulas
- **Smart feature**: Auto-adjusts for data patterns

## 📊 Performance Characteristics

- **Memory**: Fixed 16KB (vs. growing with data in Python sets)
- **Speed**: ~5 million items/second
- **Accuracy**: 
  - Normal data: ~2% error
  - Small sets: Exact counting
  - Large sets: ~2-5% error
  - Duplicates: Handled automatically

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

## 🚫 Common Errors for Python/JS Developers

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
