#include <CardinalityEstimation.h>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>

class TestDataExecuter : public DataExecuter {
private:
    std::vector<std::vector<int>> data;
    int numColumns;
    
public:
    TestDataExecuter(int rows, int cols) : numColumns(cols) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Generate some test data with different distributions
        for (int i = 0; i < rows; ++i) {
            std::vector<int> row(cols);
            
            // Column 0: Uniform distribution
            std::uniform_int_distribution<> uniform(0, rows/2);
            row[0] = uniform(gen);
            
            // Column 1: Normal distribution
            std::normal_distribution<> normal(rows/2, rows/4);
            row[1] = static_cast<int>(normal(gen));
            
            // Column 2: Skewed distribution
            std::exponential_distribution<> exp(2.0);
            row[2] = static_cast<int>(exp(gen) * rows/10);
            
            data.push_back(row);
        }
    }
    
    std::vector<int> getTuple(int idx) override {
        return data[idx];
    }
    
    int getNumTuples() override {
        return data.size();
    }
    
    int getNumDistinct(int columnIdx, int value, CompareOp op) {
        int count = 0;
        for (const auto& row : data) {
            if (op == EQUAL && row[columnIdx] == value) count++;
            else if (op == GREATER && row[columnIdx] > value) count++;
        }
        return count;
    }
};

void runTest(int numRows, int numCols) {
    std::cout << "\nRunning test with " << numRows << " rows and " << numCols << " columns\n";
    std::cout << "----------------------------------------\n";
    
    // Create test data
    auto dataExecuter = new TestDataExecuter(numRows, numCols);
    
    // Initialize CEEngine
    auto engine = new CEEngine(numRows, dataExecuter);
    engine->prepare();
    
    // Test different types of queries
    std::vector<std::pair<std::string, CompareExpression>> testQueries = {
        {"Uniform Equal", {0, EQUAL, numRows/4}},
        {"Uniform Greater", {0, GREATER, numRows/4}},
        {"Normal Equal", {1, EQUAL, numRows/2}},
        {"Normal Greater", {1, GREATER, numRows/2}},
        {"Skewed Equal", {2, EQUAL, numRows/10}},
        {"Skewed Greater", {2, GREATER, numRows/10}}
    };
    
    std::cout << std::setw(20) << "Query Type" 
              << std::setw(15) << "Estimated" 
              << std::setw(15) << "Actual"
              << std::setw(15) << "Error(%)\n";
    std::cout << std::string(65, '-') << "\n";
    
    for (const auto& test : testQueries) {
        std::vector<CompareExpression> quals = {test.second};
        
        // Get estimate
        int estimate = engine->query(quals);
        
        // Get actual count
        int actual = dataExecuter->getNumDistinct(
            test.second.columnIdx,
            test.second.value,
            test.second.compareOp
        );
        
        // Calculate error
        double error = std::abs(estimate - actual) * 100.0 / actual;
        
        std::cout << std::setw(20) << test.first
                  << std::setw(15) << estimate
                  << std::setw(15) << actual
                  << std::setw(15) << std::fixed << std::setprecision(2) << error << "\n";
    }
    
    delete engine;
    delete dataExecuter;
}

int main() {
    std::cout << "Cardinality Estimation Test\n";
    std::cout << "==========================\n";
    
    // Test with different dataset sizes
    runTest(1000, 3);    // Small dataset
    runTest(10000, 3);   // Medium dataset
    runTest(100000, 3);  // Large dataset
    
    return 0;
}
