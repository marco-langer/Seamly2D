#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QLineF>
#include <QPointF>
#include <QRectF>
#include <QVector>

namespace geo {

QPointF rotate(const QPointF& pivot, const QPointF& point, qreal degrees);

QPointF moved(const QPointF& originPoint, qreal length, qreal angle);

qreal distance(const QPointF& first, const QPointF& second);

qreal length(const QVector<QPointF>& points);

QRectF boundingRect(const QVector<QPointF>& points);

QVector<QPointF> subPath(const QVector<QPointF>& path, int startIndex, int endIndex);

/**
 * @brief Calculates the signed area of the poylgon via triangle area sum
 *
 * @pre the polygon spanned by \p points must not contain intersections
 */
qreal signedArea(const QVector<QPointF>& points);

/**
 * @brief Calculates the area of the poylgon via triangle area sum
 *
 * @pre the polygon spanned by \p points must not contain intersections
 */
qreal area(const QVector<QPointF>& points);

/**
 * @brief Returns true if the polygon is clockwise, false otherwise.
 *
 * @pre the polygon spanned by \p points must not contain intersections
 */
bool isClockwise(const QVector<QPointF>& points);

qreal angleBetweenBisectors(const QLineF& b1, const QLineF& b2);

}   // namespace geo

#endif   // GEOMETRY_H
