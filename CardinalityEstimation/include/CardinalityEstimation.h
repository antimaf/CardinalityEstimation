#ifndef CARDINALITY_ESTIMATION_H
#define CARDINALITY_ESTIMATION_H

#include <memory>
#include <tuple>
#include <vector>

class CEEngine {
public:
    CEEngine();
    ~CEEngine();

    // Insert a new tuple
    void insertTuple(const std::tuple<int, int>& tuple);

    // Estimate current cardinality
    double estimate();

    // Prepare/reset the engine
    void prepare();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // CARDINALITY_ESTIMATION_H