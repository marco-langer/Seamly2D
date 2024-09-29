#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QPointF>
#include <QRectF>
#include <QVector>


namespace geo {

QPointF rotate(const QPointF& pivot, const QPointF& point, qreal degrees);

qreal distance(const QPointF& first, const QPointF& second);

qreal length(const QVector<QPointF>& points);

QRectF boundingRect(const QVector<QPointF>& points);

QVector<QPointF> subPath(const QVector<QPointF>& path, int startIndex, int endIndex);

}   // namespace geo

#endif   // GEOMETRY_H
