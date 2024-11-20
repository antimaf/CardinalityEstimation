#include "../include/CardinalityEstimation.h"
#include "../third_party/xxhash/xxhash.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

class HyperLogLog {
private:
    std::vector<uint8_t> registers;
    const int numRegisters;
    const int registerBits;
    std::unordered_map<uint64_t, size_t> valueFrequency;  // Track frequencies for bias correction
    const size_t maxTrackedValues = 10000;
    bool isExactCount = true;

    uint64_t hashTuple(uint64_t value) const {
        // Use different seeds for different hash functions to reduce collisions
        uint64_t hash1 = XXHash64(&value, sizeof(value), 0x123456789);
        uint64_t hash2 = XXHash64(&value, sizeof(value), 0x987654321);
        return hash1 ^ (hash2 >> 1);  // Combine hashes to reduce collisions
    }

public:
    HyperLogLog(int bits = 14) : numRegisters(1 << bits), registerBits(bits), registers(1 << bits, 0) {}

    void add(uint64_t value) {
        if (isExactCount) {
            valueFrequency[value]++;
            if (valueFrequency.size() > maxTrackedValues) {
                isExactCount = false;
                valueFrequency.clear();  // Free memory since we're switching to HLL
            }
            if (isExactCount) return;
        }

        uint64_t hash = hashTuple(value);
        int idx = hash >> (64 - registerBits);
        uint8_t rank = std::min(64 - registerBits, 1 + __builtin_ctzll(hash | (1ULL << (64 - registerBits))));
        registers[idx] = std::max(registers[idx], rank);
    }

    double estimate() const {
        // Use exact count if we're still tracking all values
        if (isExactCount) {
            return valueFrequency.size();
        }

        // Standard HyperLogLog estimation
        double sum = 0;
        int zeros = 0;
        double harmonicMean = 0;

        for (uint8_t r : registers) {
            double val = std::pow(2.0, -r);
            sum += val;
            harmonicMean += 1.0 / val;
            if (r == 0) zeros++;
        }
        
        // Calculate bias correction factor
        double alpha;
        switch (registerBits) {
            case 4:  alpha = 0.673; break;
            case 5:  alpha = 0.697; break;
            case 6:  alpha = 0.709; break;
            default: alpha = 0.7213 / (1.0 + 1.079 / numRegisters);
        }
        
        double estimate = alpha * numRegisters * numRegisters / sum;
        
        // Enhanced small range correction
        if (estimate <= 5.0 * numRegisters) {
            if (zeros > 0) {
                estimate = numRegisters * std::log(static_cast<double>(numRegisters) / zeros);
            }
        }
        // Large range correction with harmonic mean
        else if (estimate > (1LL << 32) / 30.0) {
            double harmonicEstimate = numRegisters * numRegisters / (harmonicMean / numRegisters);
            estimate = std::min(estimate, harmonicEstimate);
        }
        
        return std::max(1.0, estimate);  // Never return less than 1
    }

    void reset() {
        std::fill(registers.begin(), registers.end(), 0);
        valueFrequency.clear();
        isExactCount = true;
    }
};

class CEEngine::Impl {
private:
    HyperLogLog hll;
    std::vector<std::tuple<int, int>> tuples;

public:
    Impl() : hll(14) {}

    void insertTuple(const std::tuple<int, int>& tuple) {
        tuples.push_back(tuple);
        // Combine tuple values into a single 64-bit hash
        uint64_t combined = (static_cast<uint64_t>(std::get<0>(tuple)) << 32) | 
                           static_cast<uint64_t>(std::get<1>(tuple));
        hll.add(combined);
    }

    double estimate() {
        return hll.estimate();
    }

    void prepare() {
        tuples.clear();
        hll.reset();
    }
};

CEEngine::CEEngine() : pImpl(new Impl()) {}
CEEngine::~CEEngine() = default;

void CEEngine::insertTuple(const std::tuple<int, int>& tuple) {
    pImpl->insertTuple(tuple);
}

double CEEngine::estimate() {
    return pImpl->estimate();
}

void CEEngine::prepare() {
    pImpl->prepare();
}
