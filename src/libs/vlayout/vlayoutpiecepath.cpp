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
 **  @date   8 2, 2017
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

#include "vlayoutpiecepath.h"

#include <QPainterPath>

//---------------------------------------------------------------------------------------------------------------------
VLayoutPiecePath::VLayoutPiecePath(const QVector<QPointF>& points, bool cut, Qt::PenStyle penStyle)
    : m_points{ points }
    , m_penStyle{ penStyle }
    , m_cut{ cut }
{}


//---------------------------------------------------------------------------------------------------------------------
QPainterPath VLayoutPiecePath::GetPainterPath() const
{
    QPainterPath path;
    if (not m_points.isEmpty()) {
        path.addPolygon(QPolygonF(m_points));
        path.setFillRule(Qt::WindingFill);
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VLayoutPiecePath::Points() const { return m_points; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetPoints(const QVector<QPointF>& points) { m_points = points; }

//---------------------------------------------------------------------------------------------------------------------
Qt::PenStyle VLayoutPiecePath::PenStyle() const { return m_penStyle; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetPenStyle(const Qt::PenStyle& penStyle) { m_penStyle = penStyle; }

//---------------------------------------------------------------------------------------------------------------------
bool VLayoutPiecePath::IsCutPath() const { return m_cut; }

//---------------------------------------------------------------------------------------------------------------------
void VLayoutPiecePath::SetCutPath(bool cut) { m_cut = cut; }
