#include "geometry/geometry.h"

#include "math/math.h"

#include <QPolygonF>

#include <cmath>

namespace geo {

QPointF rotate(const QPointF& pivot, const QPointF& point, qreal degrees)
{
    QLineF axis{ pivot, point };
    axis.setAngle(axis.angle() + degrees);
    return axis.p2();
}

QPointF moved(const QPointF& originPoint, qreal length, qreal angle)
{
    QLineF line{ originPoint.x(), originPoint.y(), originPoint.x() + length, originPoint.y() };
    line.setAngle(angle);
    return line.p2();
}

qreal distance(const QPointF& first, const QPointF& second)
{
    return std::sqrt(
        (first.x() - second.x()) * (first.x() - second.x())
        + (first.y() - second.y()) * (first.y() - second.y()));
}

qreal length(const QVector<QPointF>& points)
{
    if (points.empty()) {
        return 0.0;
    }

    qreal length{ 0.0 };
    for (int i = 0; i < points.size() - 1; ++i) {
        length += distance(points[i], points[i + 1]);
    }

    return length;
}

QRectF boundingRect(const QVector<QPointF>& points)
{   // TODO remove QPolygonF depedency (QPolygonF is part ofQtGui, but this lib should be QtCore
    // only)
    return QPolygonF{ points }.boundingRect();
}

QVector<QPointF> subPath(const QVector<QPointF>& path, int startIndex, int endIndex)
{
    const int pathLength{ path.size() };

    if (path.isEmpty() || startIndex < 0 || startIndex >= pathLength || endIndex < 0
        || endIndex >= pathLength || startIndex == endIndex) {
        return path;
    }

    const int subPathLength{ startIndex < endIndex ? endIndex - startIndex + 1
                                                   : pathLength - startIndex + endIndex + 1 };

    QVector<QPointF> subPath;
    subPath.reserve(subPathLength);

    int i = startIndex - 1;
    do {
        ++i;
        if (i >= pathLength) {
            i = 0;
        }
        subPath.append(path.at(i));
    } while (i != endIndex);

    return subPath;
}

qreal signedArea(const QVector<QPointF>& points)
{
    const int n{ points.size() };

    if (n < 3) {
        return 0.0;
    }

    // Calculation a polygon area through the sum of the areas of triangles
    qreal summedArea{ 0.0 };

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            // if i == 0, then y[i-1] replace on y[n-1]
            const qreal s{ points.at(i).x() * (points.at(n - 1).y() - points.at(i + 1).y()) };
            summedArea += s;
        } else {
            if (i == n - 1) {
                // if i == n-1, then y[i+1] replace on y[0]
                const qreal s{ points.at(i).x() * (points.at(i - 1).y() - points.at(0).y()) };
                summedArea += s;
            } else {
                const qreal s{ points.at(i).x() * (points.at(i - 1).y() - points.at(i + 1).y()) };
                summedArea += s;
            }
        }
    }

    return summedArea / 2.0;
}

qreal area(const QVector<QPointF>& points) { return qAbs(signedArea(points)); }

bool isClockwise(const QVector<QPointF>& points)
{
    if (points.count() < 3) {
        return false;
    }

    return signedArea(points) < 0;
}

qreal angleBetweenBisectors(const QLineF& b1, const QLineF& b2)
{
    const QLineF newB2 = b2.translated(-(b2.p1().x() - b1.p1().x()), -(b2.p1().y() - b1.p1().y()));

    qreal angle1 = newB2.angleTo(b1);
    if (math::isFuzzyEqual(angle1, 360.0)) {
        angle1 = 0;
    }

    qreal angle2 = b1.angleTo(newB2);
    if (math::isFuzzyEqual(angle2, 360.0)) {
        angle2 = 0;
    }

    if (angle1 <= angle2) {
        return angle1;
    } else {
        return angle2;
    }
}

}   // namespace geo
