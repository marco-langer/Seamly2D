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
 **  @file   vistoolnormal.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 8, 2014
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

#include "vistoolnormal.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolnormal.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolNormal::VisToolNormal(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , object2Id(NULL_ID)
    , point(nullptr)
    , lineP1(nullptr)
    , lineP2(nullptr)
    , line(nullptr)
    , length(0)
    , angle(0)
{
    this->mainColor = Qt::red;

    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);   //-V656
    line = InitItem<VScaledLine>(supportColor, this);

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::RefreshGeometry()
{
    if (object1Id > NULL_ID) {
        const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
        DrawPoint(lineP1, static_cast<QPointF>(first), supportColor);

        if (object2Id <= NULL_ID) {
            const QLineF line_mouse{ static_cast<QPointF>(first), Visualization::scenePos };
            DrawLine(line, line_mouse, supportColor, lineWeight);

            QLineF normal = line_mouse.normalVector();
            QPointF endRay = Ray(normal.p1(), normal.angle());
            DrawLine(this, QLineF(normal.p1(), endRay), mainColor, lineWeight);
        } else {
            const auto& second{ *Visualization::data->GeometricObject<VPointF>(object2Id) };
            DrawPoint(lineP2, static_cast<QPointF>(second), supportColor);

            const QLineF line_mouse{ static_cast<QPointF>(first), static_cast<QPointF>(second) };
            DrawLine(line, line_mouse, supportColor, lineWeight);

            if (qFuzzyIsNull(length)) {
                QLineF normal = line_mouse.normalVector();
                QPointF endRay = Ray(normal.p1(), normal.angle());
                DrawLine(this, QLineF(normal.p1(), endRay), mainColor, lineWeight);
            } else {
                const QPointF fPoint{ VToolNormal::FindPoint(
                    static_cast<QPointF>(first), static_cast<QPointF>(second), length, angle) };
                const QLineF mainLine{ static_cast<QPointF>(first), fPoint };
                DrawLine(this, mainLine, mainColor, lineWeight, lineStyle);

                DrawPoint(point, mainLine.p2(), mainColor);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::setObject2Id(const quint32& value) { object2Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::setLength(const QString& expression)
{
    length = FindLength(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VisToolNormal::GetAngle() const { return angle; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolNormal::SetAngle(const qreal& value) { angle = value; }
