#include "algorithm_tests.h"

#include "core_utils/algorithm.h"

void AlgorithmTests::areUniqueTest()
{
    QVERIFY(areUnique(1, 2, 3));
    QVERIFY(!areUnique(3, 2, 3));
}
