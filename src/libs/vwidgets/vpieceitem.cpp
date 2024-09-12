/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                             *
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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Seamly2D project
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

#include "vpieceitem.h"

#include <QGraphicsScene>
#include <QtMath>

//---------------------------------------------------------------------------------------------------------------------
VPieceItem::VPieceItem(QGraphicsItem* pParent)
    : QGraphicsObject(pParent)
{
    m_rectBoundingBox.setTopLeft(QPointF(0, 0));
    setAcceptHoverEvents(true);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief boundingRect returns the item bounding box
 * @return item bounding box
 */
QRectF VPieceItem::boundingRect() const { return m_rectBoundingBox; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Reset resets the item, putting the mode and z coordinate to normal and redraws it
 */
void VPieceItem::Reset()
{
    if (QGraphicsScene* toolScene = scene()) {
        toolScene->clearSelection();
    }
    m_eMode = mNormal;
    m_bReleased = false;
    Update();
    setZValue(m_inactiveZ);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief IsIdle returns the idle flag.
 * @return true, if item mode is normal and false otherwise.
 */
bool VPieceItem::IsIdle() const { return m_eMode == mNormal; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief GetAngle calculates the angle between the line, which goes from rotation center to pt and
 * x axis
 * @param pt point of interest
 * @return the angle between line from rotation center and point of interest and x axis
 */
double VPieceItem::GetAngle(const QPointF& pt) const
{
    const double dX = pt.x() - m_ptRotCenter.x();
    const double dY = pt.y() - m_ptRotCenter.y();

    if (fabs(dX) < 1 && fabs(dY) < 1) {
        return 0;
    } else {
        return qAtan2(dY, dX);
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPieceItem::MoveTypes VPieceItem::GetMoveType() const { return m_moveType; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceItem::SetMoveType(const VPieceItem::MoveTypes& moveType)
{
    m_moveType = moveType;
    setAcceptHoverEvents(m_moveType != NotMovable);
}
