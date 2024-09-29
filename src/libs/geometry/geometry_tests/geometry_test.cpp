#include "geometry_test.h"

#include "geometry/geometry.h"


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


QTEST_MAIN(GeometryTests)
