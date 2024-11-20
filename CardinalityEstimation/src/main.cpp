#include "../include/CardinalityEstimation.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <functional>

// Helper function to run a test case
void runTest(const std::string& testName, 
            int numTuples,
            std::function<std::tuple<int,int>()> generator) {
    CEEngine engine;
    
    std::cout << "\n=== " << testName << " ===" << std::endl;
    std::cout << "Inserting " << numTuples << " tuples..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numTuples; ++i) {
        engine.insertTuple(generator());
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    double estimate = engine.estimate();
    double error = std::abs(estimate - numTuples) / numTuples * 100;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Insertion time: " << duration.count() << "ms" << std::endl;
    std::cout << "True cardinality: " << numTuples << std::endl;
    std::cout << "Estimated cardinality: " << static_cast<int>(estimate) << std::endl;
    std::cout << "Error rate: " << error << "%" << std::endl;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Test 1: Uniform Distribution (Base case)
    {
        const int NUM_TUPLES = 1000000;
        const int VALUE_RANGE = 100000;
        std::uniform_int_distribution<> dis(0, VALUE_RANGE);
        
        runTest("Uniform Distribution", NUM_TUPLES,
                [&]() { return std::make_tuple(dis(gen), dis(gen)); });
    }
    
    // Test 2: Skewed Distribution (Zipfian-like)
    {
        const int NUM_TUPLES = 1000000;
        const int VALUE_RANGE = 100000;
        std::exponential_distribution<> exp_dis(0.0001);
        std::uniform_int_distribution<> offset_dis(0, 1000);
        
        runTest("Skewed Distribution", NUM_TUPLES,
                [&]() {
                    int val = static_cast<int>(exp_dis(gen)) % VALUE_RANGE;
                    return std::make_tuple(val, val + offset_dis(gen));
                });
    }
    
    // Test 3: Small Cardinality
    {
        const int NUM_TUPLES = 100;
        const int VALUE_RANGE = 50;
        std::uniform_int_distribution<> small_dis(0, VALUE_RANGE);
        
        runTest("Small Cardinality", NUM_TUPLES,
                [&]() { return std::make_tuple(small_dis(gen), small_dis(gen)); });
    }
    
    // Test 4: Large Cardinality
    {
        const int NUM_TUPLES = 10000000;
        const int VALUE_RANGE = 1000000;
        std::uniform_int_distribution<> large_dis(0, VALUE_RANGE);
        
        runTest("Large Cardinality", NUM_TUPLES,
                [&]() { return std::make_tuple(large_dis(gen), large_dis(gen)); });
    }
    
    // Test 5: Constant Values (Worst case)
    {
        const int NUM_TUPLES = 1000000;
        
        runTest("Constant Values", NUM_TUPLES,
                [&]() { return std::make_tuple(42, 42); });
    }
    
    // Test 6: Sequential Values
    {
        const int NUM_TUPLES = 1000000;
        int counter = 0;
        
        runTest("Sequential Values", NUM_TUPLES,
                [&]() { return std::make_tuple(counter++, counter++); });
    }
    
    // Test 7: Many Duplicates
    {
        const int NUM_TUPLES = 1000000;
        const int VALUE_RANGE = 1000;  // Very small range for many duplicates
        std::uniform_int_distribution<> dup_dis(0, VALUE_RANGE);
        
        runTest("Many Duplicates", NUM_TUPLES,
                [&]() { return std::make_tuple(dup_dis(gen), dup_dis(gen)); });
    }
    
    return 0;
}