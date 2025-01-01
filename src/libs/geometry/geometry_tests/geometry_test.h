#ifndef GEOMETRY_TEST_H
#define GEOMETRY_TEST_H

#include <QtTest/QtTest>


class GeometryTests : public QObject
{
    Q_OBJECT

private slots:
    void rotateTest_data();
    void rotateTest();

    void movedTest_data();
    void movedTest();

    void distanceTest_data();
    void distanceTest();

    void lengthTest_data();
    void lengthTest();

    void boundingRectTest_data();
    void boundingRectTest();

    void subPathTest_data();
    void subPathTest();

    void calculateSignedAreaTest_data();
    void calculateSignedAreaTest();
};

#endif   // GEOMETRY_TEST_H
