# Cardinality Estimation Algorithms Guide

## 1. HyperLogLog (HLL)

### Overview
HyperLogLog is one of the most popular cardinality estimation algorithms, providing a good balance between accuracy and memory usage.

### How it Works
1. Hash each element to get a binary string
2. Find the position of the leftmost '1' bit (ρ)
3. Use the first k bits to determine the register
4. Update the register with max(ρ) value seen

### Example Implementation
```cpp
class HyperLogLog {
private:
    vector<uint8_t> registers;
    int numRegisters;
    hash<string> hasher;
    
public:
    HyperLogLog(int precision) {
        numRegisters = 1 << precision;  // m = 2^p
        registers.resize(numRegisters, 0);
    }
    
    void add(const string& item) {
        size_t hash = hasher(item);
        uint32_t index = hash & (numRegisters - 1);  // First p bits
        uint8_t rank = countLeadingZeros(hash >> numRegisters) + 1;
        registers[index] = max(registers[index], rank);
    }
    
    double estimate() {
        double sum = 0;
        for (uint8_t val : registers) {
            sum += pow(2, -val);
        }
        double alpha = 0.7213 / (1 + 1.079 / numRegisters);
        return alpha * numRegisters * numRegisters / sum;
    }
};
```

## 2. Linear Counting

### Overview
Linear Counting is simpler than HLL but uses more memory for the same accuracy. It's good for smaller cardinalities.

### How it Works
1. Use a bit array initialized to 0
2. Hash each element to get an index
3. Set the corresponding bit to 1
4. Estimate based on the number of zeros remaining

### Example Implementation
```cpp
class LinearCounting {
private:
    vector<bool> bitArray;
    size_t m;  // size of bit array
    hash<string> hasher;
    
public:
    LinearCounting(size_t size) : m(size) {
        bitArray.resize(m, false);
    }
    
    void add(const string& item) {
        size_t hash = hasher(item);
        size_t index = hash % m;
        bitArray[index] = true;
    }
    
    double estimate() {
        size_t zeros = count(bitArray.begin(), bitArray.end(), false);
        return -m * log(static_cast<double>(zeros) / m);
    }
};
```

## 3. Adaptive Counting

### Overview
Adaptive Counting combines Linear Counting and HyperLogLog, switching between them based on cardinality range.

### How it Works
1. Use HyperLogLog by default
2. Switch to Linear Counting for small cardinalities
3. Use bias correction for better accuracy

### Example Implementation
```cpp
class AdaptiveCounting {
private:
    HyperLogLog hll;
    LinearCounting lc;
    const double THRESHOLD = 2.5;  // Switching threshold
    
public:
    AdaptiveCounting(int precision) 
        : hll(precision), 
          lc(1 << precision) {}
    
    void add(const string& item) {
        hll.add(item);
        lc.add(item);
    }
    
    double estimate() {
        double hllEstimate = hll.estimate();
        double lcEstimate = lc.estimate();
        
        // Switch based on estimate
        if (hllEstimate <= THRESHOLD * (1 << hll.precision)) {
            return lcEstimate;
        }
        return hllEstimate;
    }
};
```

## 4. KMV (K Minimum Values)

### Overview
KMV keeps track of the k smallest hash values seen, using their distribution to estimate cardinality.

### How it Works
1. Hash each element
2. Maintain a sorted set of k minimum hash values
3. Use the k-th minimum value to estimate total cardinality

### Example Implementation
```cpp
class KMinValues {
private:
    set<uint64_t> minValues;
    size_t k;
    hash<string> hasher;
    
public:
    KMinValues(size_t kSize) : k(kSize) {}
    
    void add(const string& item) {
        uint64_t hash = hasher(item);
        
        if (minValues.size() < k) {
            minValues.insert(hash);
        } else if (hash < *minValues.rbegin()) {
            minValues.erase(--minValues.end());
            minValues.insert(hash);
        }
    }
    
    double estimate() {
        if (minValues.size() < k) {
            return minValues.size();  // Exact count for small sets
        }
        
        uint64_t maxHash = *minValues.rbegin();
        return (k - 1) * (double)UINT64_MAX / maxHash;
    }
};
```

## Performance Comparison

### Memory Usage
1. **Linear Counting**: O(n) - Highest memory usage
2. **HyperLogLog**: O(2^p) where p is precision - Very efficient
3. **KMV**: O(k) where k is number of minimums - Moderate
4. **Adaptive Counting**: Similar to HLL + LC combined

### Accuracy
1. **HyperLogLog**: Standard error ≈ 1.04/√m
2. **Linear Counting**: Better for small cardinalities
3. **Adaptive Counting**: Best of both LC and HLL
4. **KMV**: Error ≈ 1/√k

### Processing Speed
1. **Linear Counting**: Fastest (simple bit operations)
2. **KMV**: Moderate (set maintenance overhead)
3. **HyperLogLog**: Fast (simple register updates)
4. **Adaptive Counting**: Slightly slower due to dual tracking

## Implementation Tips

1. **Hash Function Selection**
   - Use high-quality hash functions (MurmurHash3, XXHash)
   - Ensure good distribution across the value range
   - Consider using multiple hash functions for better accuracy

2. **Memory Optimization**
   - Use bit packing for registers when possible
   - Consider sparse representation for low cardinalities
   - Optimize data structure alignment

3. **Performance Optimization**
   - Use bit manipulation instead of floating-point when possible
   - Pre-compute common values and lookup tables
   - Consider SIMD instructions for parallel processing

4. **Error Handling**
   - Handle edge cases (empty set, single element)
   - Implement overflow protection
   - Consider using bias correction for small cardinalities
