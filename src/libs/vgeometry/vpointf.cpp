/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
 *                                                                         *
 ***************************************************************************
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 **************************************************************************

 ************************************************************************
 **
 **  @file   vpointf.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vpointf.h"
#include "vpointf_p.h"

#include "geometry/geometry.h"

#include <QLineF>
#include <QPointF>
#include <QString>
#include <QTransform>

VPointF& VPointF::operator=(VPointF&& point) noexcept
{
    Swap(point);
    return *this;
}

void VPointF::Swap(VPointF& point) noexcept
{
    VGObject::Swap(point);
    std::swap(d, point.d);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF creat empty point
 */
VPointF::VPointF()
    : VGObject(GOType::Point, 0, Draw::Calculation)
    , d(new VPointFData)
{}

//---------------------------------------------------------------------------------------------------------------------
VPointF::VPointF(const VPointF& point)
    : VGObject(point)
    , d(point.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPointF::VPointF(const QPointF& point)
    : VGObject(VPointF())
    , d(new VPointFData(point))
{}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF create new point
 * @param x x coordinate
 * @param y y coordinate
 * @param name point label
 * @param mx offset name respect to x
 * @param my offset name respect to y
 */
VPointF::VPointF(
    qreal x, qreal y, const QString& name, qreal mx, qreal my, quint32 idObject, const Draw& mode)
    : VGObject(GOType::Point, idObject, mode)
    , d(new VPointFData(x, y, mx, my))
{
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VPointF create new point
 * @param point point
 * @param name point label
 * @param mx offset name respect to x
 * @param my offset name respect to y
 */
VPointF::VPointF(
    const QPointF& point,
    const QString& name,
    qreal mx,
    qreal my,
    quint32 idObject,
    const Draw& mode)
    : VGObject(GOType::Point, idObject, mode)
    , d(new VPointFData(point, mx, my))
{
    setName(name);
}

//---------------------------------------------------------------------------------------------------------------------
VPointF::~VPointF() = default;

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief operator = assignment operator
 * @param point point
 * @return point
 */
VPointF& VPointF::operator=(const VPointF& point)
{
    if (&point == this) {
        return *this;
    }
    VGObject::operator=(point);
    d = point.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF::operator QPointF() const { return toQPointF(); }

//---------------------------------------------------------------------------------------------------------------------
VPointF VPointF::Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix) const
{
    const QPointF newPoint{ geo::rotate(originPoint, toQPointF(), degrees) };
    VPointF rotated(newPoint, name() + prefix, mx(), my());
    rotated.setShowPointName(isShowPointName());
    return rotated;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VPointF::Flip(const QLineF& axis, const QString& prefix) const
{
    const QPointF newPoint = FlipPF(axis, toQPointF());
    VPointF flipped(newPoint, name() + prefix, mx(), my());
    flipped.setShowPointName(isShowPointName());
    return flipped;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VPointF::Move(qreal length, qreal angle, const QString& prefix) const
{
    const QPointF newPoint{ geo::moved(toQPointF(), length, angle) };
    VPointF moved(newPoint, name() + prefix, mx(), my());
    moved.setShowPointName(isShowPointName());
    return moved;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mx return offset name respect to x
 * @return offset
 */
qreal VPointF::mx() const { return d->_mx; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief my return offset name respect to y
 * @return offset
 */
qreal VPointF::my() const { return d->_my; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMx set offset name respect to x
 * @param mx offset
 */
void VPointF::setMx(qreal mx) { d->_mx = mx; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setMy set offset name respect to y
 * @param my offset
 */
void VPointF::setMy(qreal my) { d->_my = my; }

//---------------------------------------------------------------------------------------------------------------------
QPointF VPointF::toQPointF() const { return QPointF(d->_x, d->_y); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief x return x coordinate
 * @return value
 */
qreal VPointF::x() const { return d->_x; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setX set x coordinate
 * @param value x coordinate
 */
void VPointF::setX(const qreal& value) { d->_x = value; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief y return y coordinate
 * @return value
 */
qreal VPointF::y() const { return d->_y; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setY set y coordinate
 * @param value y coordinate
 */
void VPointF::setY(const qreal& value) { d->_y = value; }

//---------------------------------------------------------------------------------------------------------------------
bool VPointF::isShowPointName() const { return d->m_showPointName; }

//---------------------------------------------------------------------------------------------------------------------
void VPointF::setShowPointName(bool show) { d->m_showPointName = show; }


//---------------------------------------------------------------------------------------------------------------------
QPointF VPointF::FlipPF(const QLineF& axis, const QPointF& point)
{
    const QTransform transform = flipTransform(axis);
    return transform.map(point);
}
