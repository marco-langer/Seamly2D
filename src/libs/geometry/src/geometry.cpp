#include "geometry/geometry.h"

#include <QPolygonF>


namespace geo {

QRectF boundingRect(const QVector<QPointF>& points) { return QPolygonF{ points }.boundingRect(); }

}   // namespace geo
