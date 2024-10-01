#include "geometry_test.h"

#include "geometry/geometry.h"

#include <cmath>


void GeometryTests::rotateTest_data()
{
    QTest::addColumn<QPointF>("pivot");
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<qreal>("degrees");
    QTest::addColumn<QPointF>("expectedResult");

    QTest::newRow("pivot equals point, 1st") << QPointF{} << QPointF{} << 90.0 << QPointF{};
    QTest::newRow("pivot equals point, 2st")
        << QPointF{ 1.0, 2.0 } << QPointF{ 1.0, 2.0 } << 90.0 << QPointF{ 1.0, 2.0 };
    QTest::newRow("origin pivot") << QPointF{} << QPointF{ 1.0, 2.0 } << 90.0
                                  << QPointF{ 2.0, -1.0 };
    QTest::newRow("clockwise") << QPointF{ 0.5, 0.5 } << QPointF{ 1.0, 2.0 } << 90.0
                               << QPointF{ 2.0, 0.0 };
}


void GeometryTests::rotateTest()
{
    QFETCH(QPointF, pivot);
    QFETCH(QPointF, point);
    QFETCH(qreal, degrees);
    QFETCH(QPointF, expectedResult);

    QCOMPARE(geo::rotate(pivot, point, degrees), expectedResult);
}


void GeometryTests::movedTest_data()
{
    QTest::addColumn<QPointF>("point");
    QTest::addColumn<qreal>("length");
    QTest::addColumn<qreal>("angle");
    QTest::addColumn<QPointF>("expectedMovedPoint");

    QTest::newRow("zero length") << QPointF{ 1.0, 1.0 } << 0.0 << 45.0 << QPointF{ 1.0, 1.0 };
    QTest::newRow("0 deg") << QPointF{ 1.0, 1.0 } << 1.0 << 0.0 << QPointF{ 2.0, 1.0 };
    QTest::newRow("45 deg") << QPointF{ 1.0, 1.0 } << 1.0 << 45.0
                            << QPointF{ 1.0 + 1.0 / std::sqrt(2.0), 1.0 - 1.0 / std::sqrt(2.0) };
}


void GeometryTests::movedTest()
{
    QFETCH(QPointF, point);
    QFETCH(qreal, length);
    QFETCH(qreal, angle);
    QFETCH(QPointF, expectedMovedPoint);

    QCOMPARE(geo::moved(point, length, angle), expectedMovedPoint);
}


void GeometryTests::distanceTest_data()
{
    QTest::addColumn<QPointF>("first");
    QTest::addColumn<QPointF>("second");
    QTest::addColumn<qreal>("expectedDistance");

    QTest::newRow("points equal") << QPointF{} << QPointF{} << 0.0;
    QTest::newRow("points differ") << QPointF{ 1.0, 1.0 } << QPointF{ 2.0, 2.0 } << std::sqrt(2.0);
}


void GeometryTests::distanceTest()
{
    QFETCH(QPointF, first);
    QFETCH(QPointF, second);
    QFETCH(qreal, expectedDistance);

    QCOMPARE(geo::distance(first, second), expectedDistance);
}


void GeometryTests::lengthTest_data()
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<qreal>("expectedLength");

    QTest::newRow("empty") << QVector<QPointF>{} << 0.0;
    QTest::newRow("single point") << QVector<QPointF>{ QPointF{ 1.1, 1.1 } } << 0.0;
    QTest::newRow("two points") << QVector<QPointF>{ QPointF{ 1.0, 1.0 }, QPointF{ 1.0, 2.0 } }
                                << 1.0;
    QTest::newRow("rectangle open") << QVector<QPointF>{
        QPointF{ 1.0, 1.0 }, QPointF{ 1.0, 2.0 }, QPointF{ 2.0, 2.0 }, QPointF{ 2.0, 1.0 }
    } << 3.0;
}


void GeometryTests::lengthTest()
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(qreal, expectedLength);

    QCOMPARE(geo::length(points), expectedLength);
}


void GeometryTests::boundingRectTest_data()
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<QRectF>("expectedResult");

    QTest::newRow("empty") << QVector<QPointF>{} << QRectF{};
    QTest::newRow("single point, 1st quadrant")
        << QVector<QPointF>{ QPointF{ 1.1, 2.2 } }
        << QRectF{ QPointF{ 1.1, 2.2 }, QPointF{ 1.1, 2.2 } };
    QTest::newRow("single point, 2nd quadrant")
        << QVector<QPointF>{ QPointF{ -1.1, 2.2 } }
        << QRectF{ QPointF{ -1.1, 2.2 }, QPointF{ -1.1, 2.2 } };
    QTest::newRow("single point, 3rd quadrant")
        << QVector<QPointF>{ QPointF{ -1.1, -2.2 } }
        << QRectF{ QPointF{ -1.1, -2.2 }, QPointF{ -1.1, -2.2 } };
    QTest::newRow("single point, 4th quadrant")
        << QVector<QPointF>{ QPointF{ 1.1, -2.2 } }
        << QRectF{ QPointF{ 1.1, -2.2 }, QPointF{ 1.1, -2.2 } };
    QTest::newRow("1st quadrant") << QVector<QPointF>{
        QPointF{ 1.1, 2.2 }, QPointF{ 4.4, 3.3 }, QPointF{ 0.1, 5.2 }
    } << QRectF{ QPointF{ 0.1, 2.2 }, QPointF{ 4.4, 5.2 } };
    QTest::newRow("2nd quadrant") << QVector<QPointF>{
        QPointF{ -1.1, 2.2 }, QPointF{ -4.4, 3.3 }, QPointF{ -0.1, 5.2 }
    } << QRectF{ QPointF{ -4.4, 2.2 }, QPointF{ -0.1, 5.2 } };
    QTest::newRow("3rd quadrant") << QVector<QPointF>{
        QPointF{ -1.1, -2.2 }, QPointF{ -4.4, -3.3 }, QPointF{ -0.1, -5.2 }
    } << QRectF{ QPointF{ -4.4, -5.2 }, QPointF{ -0.1, -2.2 } };
    QTest::newRow("4th quadrant") << QVector<QPointF>{
        QPointF{ 1.1, -2.2 }, QPointF{ 4.4, -3.3 }, QPointF{ 0.1, -5.2 }
    } << QRectF{ QPointF{ 0.1, -5.2 }, QPointF{ 4.4, -2.2 } };
    QTest::newRow("1st + 2nd quadrant")
        << QVector<QPointF>{ QPointF{ 1.1, 2.2 }, QPointF{ -4.4, 3.3 }, QPointF{ 0.1, 5.2 } }
        << QRectF{ QPointF{ -4.4, 2.2 }, QPointF{ 1.1, 5.2 } };
    QTest::newRow("1st + 3rd quadrant")
        << QVector<QPointF>{ QPointF{ 1.1, 2.2 }, QPointF{ -4.4, -3.3 }, QPointF{ 0.1, 5.2 } }
        << QRectF{ QPointF{ -4.4, -3.3 }, QPointF{ 1.1, 5.2 } };
    QTest::newRow("1st + 4th quadrant")
        << QVector<QPointF>{ QPointF{ 1.1, 2.2 }, QPointF{ 4.4, -3.3 }, QPointF{ 0.1, 5.2 } }
        << QRectF{ QPointF{ 0.1, -3.3 }, QPointF{ 4.4, 5.2 } };
}


void GeometryTests::boundingRectTest()
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(QRectF, expectedResult);

    QCOMPARE(geo::boundingRect(points), expectedResult);
}


void GeometryTests::subPathTest_data()
{
    QTest::addColumn<QVector<QPointF>>("points");
    QTest::addColumn<int>("startIndex");
    QTest::addColumn<int>("endIndex");
    QTest::addColumn<QVector<QPointF>>("expectedSubPath");

    QTest::newRow("empty") << QVector<QPointF>{} << 0 << 0 << QVector<QPointF>{};
    QTest::newRow("full path") << QVector<QPointF>{
        { 0.0, 0.0 }, { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 }
    } << 0 << 3 << QVector<QPointF>{ { 0.0, 0.0 }, { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 } };
    QTest::newRow("part, no-wrap")
        << QVector<QPointF>{ { 0.0, 0.0 }, { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 }, { 4.0, 4.0 } }
        << 1 << 3 << QVector<QPointF>{ { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 } };
    QTest::newRow("part, wrap") << QVector<QPointF>{
        { 0.0, 0.0 }, { 1.0, 1.0 }, { 2.0, 2.0 }, { 3.0, 3.0 }, { 4.0, 4.0 }
    } << 3 << 1 << QVector<QPointF>{ { 3.0, 3.0 }, { 4.0, 4.0 }, { 0.0, 0.0 }, { 1.0, 1.0 } };
}


void GeometryTests::subPathTest()
{
    QFETCH(QVector<QPointF>, points);
    QFETCH(int, startIndex);
    QFETCH(int, endIndex);
    QFETCH(QVector<QPointF>, expectedSubPath);

    QCOMPARE(geo::subPath(points, startIndex, endIndex), expectedSubPath);
}

QTEST_MAIN(GeometryTests)
