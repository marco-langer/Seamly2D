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
 **  @file   vistoolspline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 8, 2014
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

#include "vistoolspline.h"

#include "math/math.h"

#include <QLineF>
#include <QPainterPath>
#include <QSharedPointer>
#include <Qt>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../visualization.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vcontrolpointspline.h"
#include "vispath.h"

const int EMPTY_ANGLE = -1;

//---------------------------------------------------------------------------------------------------------------------
VisToolSpline::VisToolSpline(const VContainer* data, QGraphicsItem* parent)
    : VisPath(data, parent)
    , object4Id(NULL_ID)
    , point1(nullptr)
    , point4(nullptr)
    , angle1(EMPTY_ANGLE)
    , angle2(EMPTY_ANGLE)
    , kAsm1(1)
    , kAsm2(1)
    , kCurve(1)
    , isLeftMousePressed(false)
    , p2Selected(false)
    , p3Selected(false)
    , p2()
    , p3()
    , controlPoints()
{
    point1 = InitPoint(supportColor, this);
    point4 = InitPoint(supportColor, this);   //-V656

    auto* controlPoint1 = new VControlPointSpline(1, SplinePointPosition::FirstPoint, this);
    controlPoint1->hide();
    controlPoints.append(controlPoint1);

    auto* controlPoint2 = new VControlPointSpline(1, SplinePointPosition::LastPoint, this);
    controlPoint2->hide();
    controlPoints.append(controlPoint2);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolSpline::~VisToolSpline() { emit ToolTip(""); }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::RefreshGeometry()
{
    // Radius of point circle, but little bigger. Need handle with hover sizes.
    const static qreal radius = defPointRadiusPixel * 1.5;

    if (object1Id > NULL_ID) {
        const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
        DrawPoint(point1, static_cast<QPointF>(first), supportColor);

        if (mode == Mode::Creation) {
            if (isLeftMousePressed && not p2Selected) {
                p2 = Visualization::scenePos;
                controlPoints[0]->refreshCtrlPoint(
                    1, SplinePointPosition::FirstPoint, p2, static_cast<QPointF>(first));

                if (not controlPoints[0]->isVisible()) {
                    if (QLineF{ static_cast<QPointF>(first), p2 }.length() > radius) {
                        controlPoints[0]->show();
                    } else {
                        p2 = static_cast<QPointF>(first);
                    }
                }
            } else {
                p2Selected = true;
            }
        }

        if (object4Id <= NULL_ID) {
            const VSpline spline{
                first, p2, Visualization::scenePos, VPointF{ Visualization::scenePos }
            };
            DrawPath(this, spline.GetPath(), mainColor, lineStyle, lineWeight, Qt::RoundCap);
        } else {
            const auto& second{ *Visualization::data->GeometricObject<VPointF>(object4Id) };
            DrawPoint(point4, static_cast<QPointF>(second), supportColor);

            if (mode == Mode::Creation) {
                if (isLeftMousePressed && not p3Selected) {
                    QLineF ctrlLine{ static_cast<QPointF>(second), Visualization::scenePos };
                    ctrlLine.setAngle(ctrlLine.angle() + 180);
                    p3 = ctrlLine.p2();
                    controlPoints[1]->refreshCtrlPoint(
                        1, SplinePointPosition::LastPoint, p3, static_cast<QPointF>(second));

                    if (not controlPoints[1]->isVisible()) {
                        if (QLineF{ static_cast<QPointF>(second), p3 }.length() > radius) {
                            controlPoints[1]->show();
                        } else {
                            p3 = static_cast<QPointF>(second);
                        }
                    }
                } else {
                    p3Selected = true;
                }
            }

            if (math::isFuzzyEqual(angle1, EMPTY_ANGLE)
                || math::isFuzzyEqual(angle2, EMPTY_ANGLE)) {
                DrawPath(
                    this,
                    VSpline{ first, p2, p3, second }.GetPath(),
                    mainColor,
                    lineStyle,
                    lineWeight,
                    Qt::RoundCap);
            } else {
                const VSpline spline{ first, second, angle1, angle2, kAsm1, kAsm2, kCurve };
                DrawPath(
                    this,
                    spline.GetPath(),
                    spline.DirectionArrows(),
                    mainColor,
                    lineStyle,
                    lineWeight,
                    Qt::RoundCap);
                Visualization::toolTip = tr("Hold <b>SHIFT</b> to constrain angle");
                emit ToolTip(Visualization::toolTip);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::setObject4Id(const quint32& value) { object4Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetAngle1(const qreal& value) { angle1 = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetAngle2(const qreal& value) { angle2 = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKAsm1(const qreal& value) { kAsm1 = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKAsm2(const qreal& value) { kAsm2 = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::SetKCurve(const qreal& value) { kCurve = value; }

//---------------------------------------------------------------------------------------------------------------------
const QPointF& VisToolSpline::GetP2() const { return p2; }

//---------------------------------------------------------------------------------------------------------------------
const QPointF& VisToolSpline::GetP3() const { return p3; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::MouseLeftPressed()
{
    if (mode == Mode::Creation) {
        isLeftMousePressed = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolSpline::MouseLeftReleased()
{
    if (mode == Mode::Creation) {
        isLeftMousePressed = false;
        RefreshGeometry();
    }
}
