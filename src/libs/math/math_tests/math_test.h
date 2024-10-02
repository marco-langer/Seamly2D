#ifndef MATH_TEST_H
#define MATH_TEST_H

#include <QtTest/QtTest>


class MathTests : public QObject
{
    Q_OBJECT

private slots:
    void isFuzzyEqualTest_data();
    void isFuzzyEqualTest();
};

#endif   // MATH_TEST_H
