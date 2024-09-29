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


QRectF boundingRect(const QVector<QPointF>& points) { return QPolygonF{ points }.boundingRect(); }


QVector<QPointF> subPath(const QVector<QPointF>& path, int startIndex, int endIndex)
{
    if (path.isEmpty() || startIndex < 0 || startIndex >= path.size() || endIndex < 0
        || endIndex >= path.size() || startIndex == endIndex) {
        return path;
    }

    QVector<QPointF> subPath;
    int i = startIndex - 1;
    do {
        ++i;
        if (i >= path.size()) {
            i = 0;
        }
        subPath.append(path.at(i));
    } while (i != endIndex);

    return subPath;
}

}   // namespace geo
