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
 **  @file   vistoolalongline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 7, 2014
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

#include "vistoolalongline.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolAlongLine::VisToolAlongLine(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , object2Id(NULL_ID)
    , point(nullptr)
    , lineP1(nullptr)
    , lineP2(nullptr)
    , line(nullptr)
    , length(0)
{
    this->mainColor = Qt::red;
    this->setZValue(2);   // Show on top real tool

    lineP1 = InitPoint(supportColor, this);
    lineP2 = InitPoint(supportColor, this);   //-V656
    line = InitItem<VScaledLine>(supportColor, this);
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setObject2Id(const quint32& value) { object2Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::setLength(const QString& expression)
{
    length = FindLength(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolAlongLine::RefreshGeometry()
{
    if (object1Id > NULL_ID) {
        const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
        DrawPoint(lineP1, static_cast<QPointF>(first), supportColor);

        if (object2Id <= NULL_ID) {
            DrawLine(
                line,
                QLineF{ static_cast<QPointF>(first), Visualization::scenePos },
                supportColor,
                lineWeight);
        } else {
            const auto& second{ *Visualization::data->GeometricObject<VPointF>(object2Id) };
            DrawPoint(lineP2, static_cast<QPointF>(second), supportColor);

            DrawLine(
                line,
                QLineF{ static_cast<QPointF>(first), static_cast<QPointF>(second) },
                supportColor,
                lineWeight);

            if (not qFuzzyIsNull(length)) {
                const QLineF mainLine{ VGObject::BuildLine(
                    static_cast<QPointF>(first), length, line->line().angle()) };
                DrawLine(this, mainLine, mainColor, lineWeight, lineStyle);

                DrawPoint(point, mainLine.p2(), mainColor);
            }
        }
    }
}
