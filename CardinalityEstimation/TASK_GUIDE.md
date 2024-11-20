# Cardinality Estimation Project Guide

## Overview
This project focuses on implementing a Cardinality Estimation Engine (CEEngine) for approximating the number of distinct elements in large datasets. The goal is to create an efficient implementation that balances accuracy and resource usage.

### Problem Background
Cardinality estimation is a fundamental problem in database systems and data processing. It involves estimating the number of distinct elements in a dataset without storing all unique values, which is crucial for query optimization and data analytics.

## Technical Requirements

### Interface Definition
The `CEEngine` class must implement the following interfaces:
1. `Init()`: Initialize the estimation engine
2. `Update(const std::string& item)`: Process a new item
3. `Estimate()`: Return the estimated cardinality
4. `Reset()`: Clear all state and prepare for new estimation

### Performance Metrics
Your implementation will be evaluated on:
1. **Accuracy**: Mean relative error of estimation
2. **Memory Usage**: Efficiency of memory utilization
3. **Processing Speed**: Time taken for updates and estimation
4. **Resource Limits**:
   - Maximum memory usage: 10MB
   - Processing time constraints will be enforced

## Getting Started

### Project Setup
1. The project structure:
   - `include/`: Header files
   - `src/`: Source files
   - `test/`: Test cases and benchmarks

### Local Development Environment
```bash
mkdir build
cd build
cmake ..
make
./main
```

## Implementation Guidelines

### Key Requirements
1. **Focus Area**: 
   - Implement the `CEEngine` class
   - Optimize for both accuracy and performance
2. **File Modifications**:
   - ✅ Modify `CEEngine` class
   - ✅ Add new supporting classes
   - ❌ Do not modify other defined files

### Algorithm Considerations
1. **Recommended Approaches**:
   - HyperLogLog
   - Linear Counting
   - Adaptive Counting
   - KMV (K Minimum Values)
2. **Optimization Tips**:
   - Use bit manipulation for efficiency
   - Consider hash function selection carefully
   - Implement memory-efficient data structures

### Code Organization
1. **Header Files**:
   - Place in `include/` directory
   - Use proper include paths:
     - For `include/header_1.h` → `#include "header_1.h"`
     - For `include/my_folder/header_2.h` → `#include "my_folder/header_2.h"`

2. **Source Files**:
   - Place in `src/` directory
   - Organize related functionality in subdirectories

### Important Restrictions
1. **Logging**: Minimize for performance
2. **Third-party Libraries**: 
   - Avoid external dependencies
   - Include source code if necessary
3. **Threading**: Multi-threading prohibited
4. **Memory Management**:
   - Avoid memory leaks
   - Implement efficient cleanup in destructor

## Testing Strategy
1. **Local Testing**:
   - Test with various data distributions
   - Verify memory usage
   - Benchmark performance
2. **Edge Cases**:
   - Empty input
   - Large cardinalities
   - Repeated elements
   - Special characters

## Submission Process
1. Package the project:
   - Create tar.gz of `CardinalityEstimation` directory
   - Verify structure and completeness
2. Final Checklist:
   - All required interfaces implemented
   - Code compiles without warnings
   - Memory leaks addressed
   - Documentation complete

## Evaluation Process
1. Automated Testing:
   - Interface compliance
   - Performance benchmarks
   - Memory usage monitoring
2. Scoring Criteria:
   - Accuracy (40%)
   - Memory efficiency (30%)
   - Processing speed (30%)

## Development Tips
1. CMakeLists.txt can be modified for local testing
2. Report base code issues promptly
3. Focus on algorithmic efficiency
4. Test thoroughly before submission

## Support
For technical issues or clarifications:
1. Review the complete challenge guidebook
2. Contact competition support staff
3. Check for any published FAQs or updates
