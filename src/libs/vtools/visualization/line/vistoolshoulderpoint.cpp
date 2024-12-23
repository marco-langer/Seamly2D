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
 **  @file   vistoolshoulderpoint.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 7, 2014
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

#include "vistoolshoulderpoint.h"

#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolshoulderpoint.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolShoulderPoint::VisToolShoulderPoint(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , lineP1Id(NULL_ID)
    , lineP2Id(NULL_ID)
    , point(nullptr)
    , line1P1(nullptr)
    , line1P2(nullptr)
    , line1(nullptr)
    , line2P2(nullptr)
    , line2(nullptr)
    , line3(nullptr)
    , length(0)
{
    line1P1 = InitPoint(supportColor, this);
    line1P2 = InitPoint(supportColor, this);   //-V656
    line1 = InitItem<VScaledLine>(supportColor, this);

    line2P2 = InitPoint(supportColor, this);
    line2 = InitItem<VScaledLine>(supportColor, this);
    line3 = InitItem<VScaledLine>(supportColor, this);   //-V656

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::RefreshGeometry()
{
    if (object1Id > NULL_ID) {
        const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
        DrawPoint(line1P1, static_cast<QPointF>(first), supportColor);

        if (lineP1Id <= NULL_ID) {
            DrawLine(
                line1,
                QLineF{ static_cast<QPointF>(first), Visualization::scenePos },
                supportColor,
                lineWeight);
        } else {
            const auto& second{ *Visualization::data->GeometricObject<VPointF>(lineP1Id) };
            DrawPoint(line1P2, static_cast<QPointF>(second), supportColor);

            DrawLine(
                line1,
                QLineF{ static_cast<QPointF>(first), static_cast<QPointF>(second) },
                supportColor,
                lineWeight);

            if (lineP2Id <= NULL_ID) {
                DrawLine(
                    line2,
                    QLineF{ static_cast<QPointF>(second), Visualization::scenePos },
                    supportColor,
                    lineWeight);
            } else {
                const auto& third{ *Visualization::data->GeometricObject<VPointF>(lineP2Id) };
                DrawPoint(line2P2, static_cast<QPointF>(third), supportColor);

                DrawLine(
                    line2,
                    QLineF{ static_cast<QPointF>(second), static_cast<QPointF>(third) },
                    supportColor,
                    lineWeight);

                if (not qFuzzyIsNull(length)) {
                    const QPointF fPoint{ VToolShoulderPoint::FindPoint(
                        static_cast<QPointF>(second),
                        static_cast<QPointF>(third),
                        static_cast<QPointF>(first),
                        length) };
                    const QLineF mainLine{ static_cast<QPointF>(second), fPoint };
                    DrawLine(this, mainLine, mainColor, lineWeight, lineStyle);

                    DrawPoint(point, mainLine.p2(), mainColor);
                    DrawLine(
                        line3,
                        QLineF{ static_cast<QPointF>(first), mainLine.p2() },
                        supportColor,
                        lineWeight,
                        Qt::DashLine);
                } else {
                    qreal angle =
                        QLineF{ static_cast<QPointF>(second), static_cast<QPointF>(third) }.angle();
                    const QPointF endRay{ Ray(static_cast<QPointF>(second), angle) };
                    const QLineF mainLine{ VGObject::BuildLine(
                        static_cast<QPointF>(second),
                        QLineF{ static_cast<QPointF>(second), endRay }.length(),
                        angle) };
                    DrawLine(this, mainLine, mainColor, lineWeight, lineStyle);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLineP1Id(const quint32& value) { lineP1Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLineP2Id(const quint32& value) { lineP2Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolShoulderPoint::setLength(const QString& expression)
{
    length = FindLength(expression, Visualization::data->DataVariables());
}
