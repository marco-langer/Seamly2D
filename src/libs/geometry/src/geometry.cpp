#include "geometry/geometry.h"

#include <QLineF>
#include <QPolygonF>

#include <cmath>


namespace geo {

QPointF rotate(const QPointF& pivot, const QPointF& point, qreal degrees)
{
    QLineF axis{ pivot, point };
    axis.setAngle(axis.angle() + degrees);
    return axis.p2();
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

}   // namespace geo
