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
 **  @file   vistoolcubicbezier.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Seamly2D project
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

#include "vistoolcubicbezier.h"

#include <QGraphicsLineItem>
#include <QLineF>
#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/scalesceneitems.h"
#include "vispath.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCubicBezier::VisToolCubicBezier(const VContainer* data, QGraphicsItem* parent)
    : VisPath(data, parent)
    , object2Id(NULL_ID)
    , object3Id(NULL_ID)
    , object4Id(NULL_ID)
    , point1(nullptr)
    , point2(nullptr)
    , point3(nullptr)
    , point4(nullptr)
    , helpLine1(nullptr)
    , helpLine2(nullptr)
{
    helpLine1 = InitItem<VScaledLine>(mainColor, this);
    helpLine2 = InitItem<VScaledLine>(mainColor, this);

    point1 = InitPoint(supportColor, this);
    point2 = InitPoint(supportColor, this);   //-V656
    point3 = InitPoint(supportColor, this);   //-V656
    point4 = InitPoint(supportColor, this);   //-V656
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::RefreshGeometry()
{
    if (object1Id > NULL_ID) {
        const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
        DrawPoint(point1, static_cast<QPointF>(first), Qt::DashLine);

        if (object2Id <= NULL_ID) {
            DrawLine(
                helpLine1,
                QLineF{ static_cast<QPointF>(first), Visualization::scenePos },
                mainColor,
                lineWeight,
                Qt::DashLine);
        } else {
            const auto& second{ *Visualization::data->GeometricObject<VPointF>(object2Id) };
            DrawPoint(point2, static_cast<QPointF>(second), supportColor);
            DrawLine(
                helpLine1,
                QLineF{ static_cast<QPointF>(first), static_cast<QPointF>(second) },
                mainColor,
                lineWeight,
                Qt::DashLine);

            if (object3Id <= NULL_ID) {
                const VCubicBezier spline{ first,
                                           second,
                                           VPointF(Visualization::scenePos),
                                           VPointF(Visualization::scenePos) };
                DrawPath(this, spline.GetPath(), mainColor, lineStyle, lineWeight, Qt::RoundCap);
            } else {
                const auto& third{ *Visualization::data->GeometricObject<VPointF>(object3Id) };
                DrawPoint(point3, static_cast<QPointF>(third), supportColor);

                if (object4Id <= NULL_ID) {
                    const VCubicBezier spline{
                        first, second, third, VPointF(Visualization::scenePos)
                    };
                    DrawPath(
                        this, spline.GetPath(), mainColor, lineStyle, lineWeight, Qt::RoundCap);
                    DrawLine(
                        helpLine2,
                        QLineF{ static_cast<QPointF>(third), Visualization::scenePos },
                        mainColor,
                        lineWeight,
                        Qt::DashLine);
                } else {
                    const auto& fourth{ *Visualization::data->GeometricObject<VPointF>(object4Id) };
                    DrawPoint(point4, static_cast<QPointF>(fourth), supportColor);
                    DrawLine(
                        helpLine2,
                        QLineF{ static_cast<QPointF>(fourth), static_cast<QPointF>(third) },
                        mainColor,
                        lineWeight,
                        Qt::DashLine);

                    const VCubicBezier spline{ first, second, third, fourth };
                    DrawPath(
                        this,
                        spline.GetPath(),
                        spline.DirectionArrows(),
                        mainColor,
                        lineStyle,
                        lineWeight,
                        Qt::RoundCap);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject2Id(const quint32& value) { object2Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject3Id(const quint32& value) { object3Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolCubicBezier::setObject4Id(const quint32& value) { object4Id = value; }
