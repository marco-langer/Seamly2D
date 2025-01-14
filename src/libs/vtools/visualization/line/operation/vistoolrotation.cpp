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
 **  @file   vistoolrotation.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2016
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

#include "vistoolrotation.h"

#include "math/math.h"

#include <QGraphicsLineItem>
#include <QGraphicsPathItem>
#include <QGuiApplication>
#include <QLineF>
#include <QPainterPath>
#include <Qt>

#include <limits>

#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vellipticalarc.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vgeometry/vspline.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "visoperation.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolRotation::VisToolRotation(const VContainer* data, QGraphicsItem* parent)
    : VisOperation(data, parent)
    , angle(std::numeric_limits<int>::min())
    , point(nullptr)
    , angleArc(nullptr)
    , xAxis(nullptr)
{
    point = InitPoint(supportColor2, this);
    angleArc = InitItem<VCurvePathItem>(supportColor2, this);
    xAxis = InitItem<VScaledLine>(supportColor2, this);
}

//---------------------------------------------------------------------------------------------------------------------
VisToolRotation::~VisToolRotation() = default;

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::RefreshGeometry()
{
    if (objects.isEmpty()) {
        return;
    }

    VPointF origin;
    qreal tempAngle = 0;

    if (object1Id != NULL_ID) {
        origin = *Visualization::data->GeometricObject<VPointF>(object1Id);
        DrawPoint(point, static_cast<QPointF>(origin), supportColor2);

        QLineF rLine;
        if (math::isFuzzyEqual(angle, std::numeric_limits<int>::min())) {
            rLine = QLineF{ static_cast<QPointF>(origin), Visualization::scenePos };

            if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) {
                rLine.setAngle(CorrectAngle(rLine.angle()));
            }

            rLine.setP2(Ray(static_cast<QPointF>(origin), rLine.angle()));
            tempAngle = rLine.angle();
        } else {
            rLine =
                QLineF{ static_cast<QPointF>(origin), Ray(static_cast<QPointF>(origin), angle) };
            tempAngle = angle;
        }

        DrawLine(this, rLine, supportColor2, lineWeight, Qt::DashLine);
        DrawLine(
            xAxis,
            QLineF{ static_cast<QPointF>(origin), Ray(static_cast<QPointF>(origin), 0) },
            supportColor2,
            lineWeight,
            Qt::DashLine);

        const VArc arc{ origin, defPointRadiusPixel * 2, 0, tempAngle };
        DrawPath(angleArc, arc.GetPath(), supportColor2, Qt::SolidLine, lineWeight, Qt::RoundCap);

        Visualization::toolTip = tr("Rotating angle = %1°, Hold <b>SHIFT</b> to constrain angle, "
                                    "<b>Mouse click</b> - finish creation")
                                     .arg(tempAngle);
    }

    int iPoint = -1;
    int iCurve = -1;
    for (int i = 0; i < objects.size(); ++i) {
        const quint32 id = objects.at(i);
        const auto& obj{ Visualization::data->GetGObject(id) };

        switch (static_cast<GOType>(obj.getType())) {
        case GOType::Point: {
            const auto& p{ *Visualization::data->GeometricObject<VPointF>(id) };

            ++iPoint;
            VScaledEllipse* point = GetPoint(static_cast<quint32>(iPoint), supportColor2);
            DrawPoint(point, static_cast<QPointF>(p), supportColor2);

            ++iPoint;
            point = GetPoint(static_cast<quint32>(iPoint), supportColor);

            if (object1Id != NULL_ID) {
                DrawPoint(
                    point,
                    static_cast<QPointF>(p.Rotate(static_cast<QPointF>(origin), tempAngle)),
                    supportColor);
            }
            break;
        }
        case GOType::Arc: {
            iCurve = AddCurve<VArc>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::EllipticalArc: {
            iCurve = AddCurve<VEllipticalArc>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::Spline: {
            iCurve = AddCurve<VSpline>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::SplinePath: {
            iCurve = AddCurve<VSplinePath>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::CubicBezier: {
            iCurve = AddCurve<VCubicBezier>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::CubicBezierPath: {
            iCurve =
                AddCurve<VCubicBezierPath>(tempAngle, static_cast<QPointF>(origin), id, iCurve);
            break;
        }
        case GOType::Unknown:
        case GOType::Curve:
        case GOType::Path:
        case GOType::AllCurves: break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::SetOriginPointId(quint32 value) { object1Id = value; }

//---------------------------------------------------------------------------------------------------------------------
QString VisToolRotation::Angle() const { return QString::number(line().angle()); }

//---------------------------------------------------------------------------------------------------------------------
void VisToolRotation::SetAngle(const QString& expression)
{
    angle = FindVal(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
template <class Item>
int VisToolRotation::AddCurve(qreal angle, const QPointF& origin, quint32 id, int i)
{
    const auto& curve{ *Visualization::data->template GeometricObject<Item>(id) };

    ++i;
    VCurvePathItem* path = GetCurve(static_cast<quint32>(i), supportColor2);
    DrawPath(
        path,
        curve.GetPath(),
        curve.DirectionArrows(),
        supportColor2,
        Qt::SolidLine,
        lineWeight,
        Qt::RoundCap);

    ++i;
    path = GetCurve(static_cast<quint32>(i), supportColor);
    if (object1Id != NULL_ID) {
        const Item rotated{ curve.Rotate(origin, angle) };
        DrawPath(
            path,
            rotated.GetPath(),
            rotated.DirectionArrows(),
            supportColor,
            Qt::SolidLine,
            lineWeight,
            Qt::RoundCap);
    }

    return i;
}
