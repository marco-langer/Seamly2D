#include "math_test.h"

#include "math/math.h"


void MathTests::isFuzzyEqualTest_data()
{
    QTest::addColumn<double>("first");
    QTest::addColumn<double>("second");
    QTest::addColumn<bool>("expectedResult");

    constexpr double eps{ 1.0e-15 };

    QTest::newRow("zero ~== zero") << 0.0 << 0.0 << true;
    QTest::newRow("one ~== one") << 1.0 << 1.0 << true;
    QTest::newRow("one ~== two") << 1.0 << 2.0 << false;
    QTest::newRow("zero ~== one") << 0.0 << 1.0 << false;
    QTest::newRow("one ~== zero") << 1.0 << 0.0 << false;
    QTest::newRow("one ~== one + eps") << 1.0 << 1.0 + eps << true;
    QTest::newRow("zero ~== eps") << 0.0 << eps << true;
    QTest::newRow("eps ~== zero") << eps << 0.0 << true;
}


void MathTests::isFuzzyEqualTest()
{
    QFETCH(double, first);
    QFETCH(double, second);
    QFETCH(bool, expectedResult);

    QCOMPARE(math::isFuzzyEqual(first, second), expectedResult);
}
