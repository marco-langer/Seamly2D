#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPointF>
#include <QRectF>
#include <QVector>


namespace geo {

QRectF boundingRect(const QVector<QPointF>& points);

}

#endif   // GEOMETRY_H
