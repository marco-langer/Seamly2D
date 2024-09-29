#ifndef GEOMETRY_TEST_H
#define GEOMETRY_TEST_H

#include <QtTest/QtTest>


class GeometryTests : public QObject
{
    Q_OBJECT

private slots:
    void rotateTest_data();
    void rotateTest();

    void distanceTest_data();
    void distanceTest();

    void lengthTest_data();
    void lengthTest();

    void boundingRectTest_data();
    void boundingRectTest();
};

#endif   // GEOMETRY_TEST_H
