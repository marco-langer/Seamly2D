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
 **  @file   vistoolcurveintersectaxis.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 10, 2014
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

#include "vistoolcurveintersectaxis.h"

#include "math/math.h"

#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QLineF>
#include <QPainterPath>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../../tools/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolcurveintersectaxis.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolCurveIntersectAxis::VisToolCurveIntersectAxis(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , axisPointId(NULL_ID)
    , angle(-1)
    , point(nullptr)
    , basePoint(nullptr)
    , baseLine(nullptr)
    , axisLine(nullptr)
    , visCurve(nullptr)
{
    this->mainColor = Qt::red;

    visCurve = InitItem<VCurvePathItem>(Qt::darkGreen, this);
    basePoint = InitPoint(supportColor, this);
    baseLine = InitItem<VScaledLine>(supportColor, this);
    axisLine = InitItem<VScaledLine>(supportColor, this);   //-V656
    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::RefreshGeometry()
{
    if (object1Id > NULL_ID) {
        const auto& curve{ *Visualization::data->GeometricObject<VAbstractCurve>(object1Id) };
        DrawPath(
            visCurve,
            curve.GetPath(),
            curve.DirectionArrows(),
            supportColor,
            Qt::SolidLine,
            lineWeight,
            Qt::RoundCap);

        if (axisPointId > NULL_ID) {
            QLineF axis;
            const auto& first{ *Visualization::data->GeometricObject<VPointF>(axisPointId) };
            if (math::isFuzzyEqual(angle, -1.0)) {
                axis = Axis(static_cast<QPointF>(first), Visualization::scenePos);
            } else {
                axis = Axis(static_cast<QPointF>(first), angle);
            }
            DrawPoint(basePoint, static_cast<QPointF>(first), mainColor);
            DrawLine(axisLine, axis, supportColor, lineWeight, Qt::DashLine);

            const std::optional<QPointF> intersectPoint{ VToolCurveIntersectAxis::findPoint(
                static_cast<QPointF>(first), axis.angle(), curve) };
            if (intersectPoint) {
                QLineF axis_line(static_cast<QPointF>(first), *intersectPoint);
                DrawLine(this, axis_line, mainColor, lineWeight, lineStyle);
                DrawPoint(point, *intersectPoint, mainColor);
            }

            Visualization::toolTip = tr("<b>Intersection curve and axis</b>: angle = %1°, "
                                        "Hold <b>SHIFT</b> to constrain angle, "
                                        "Press <b>ENTER</b> to finish tool creation ")
                                         .arg(this->line().angle());
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolCurveIntersectAxis::Angle() const { return QString("%1").arg(this->line().angle()); }

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::SetAngle(const QString& expression)
{
    angle = FindVal(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolCurveIntersectAxis::setAxisPointId(const quint32& value) { axisPointId = value; }
