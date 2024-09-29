#include "geometry/geometry.h"

#include <QLineF>
#include <QPolygonF>


namespace geo {

QPointF rotate(const QPointF& pivot, const QPointF& point, qreal degrees)
{
    QLineF axis{ pivot, point };
    axis.setAngle(axis.angle() + degrees);
    return axis.p2();
}


QRectF boundingRect(const QVector<QPointF>& points) { return QPolygonF{ points }.boundingRect(); }

}   // namespace geo
