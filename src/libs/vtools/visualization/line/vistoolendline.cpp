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
 **  @file   vistoolendline.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 7, 2014
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

#include "vistoolendline.h"

#include <QColor>
#include <QGuiApplication>
#include <QLineF>
#include <QPointF>
#include <QSharedPointer>
#include <Qt>

#include "../ifc/xml/vdomdocument.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include "../vpatterndb/vcontainer.h"
#include "visline.h"
#include "vtranslatevars.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolEndLine::VisToolEndLine(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , length(0)
    , angle(0)
    , point(nullptr)
{
    this->mainColor = Qt::red;

    point = InitPoint(mainColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::RefreshGeometry()
{
    const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
    QLineF line;
    if (qFuzzyIsNull(length)) {
        if (QGuiApplication::keyboardModifiers() == Qt::ShiftModifier) {
            line = QLineF{ static_cast<QPointF>(first), Visualization::scenePos };
            line.setAngle(CorrectAngle(line.angle()));
        } else {
            line = QLineF{ static_cast<QPointF>(first), Visualization::scenePos };
        }
    } else {
        line = VGObject::BuildLine(static_cast<QPointF>(first), length, angle);
        DrawPoint(point, line.p2(), mainColor);
    }
    DrawLine(this, line, mainColor, lineWeight, lineStyle);
    static const QString prefix = UnitsToStr(qApp->patternUnit(), true);
    Visualization::toolTip =
        tr("<b>Point Length and Angle</b>: angle = %1°, length = %2%3; "
           "Hold <b>SHIFT</b> to constrain angle, Press <b>ENTER</b> to finish tool creation")
            .arg(this->line().angle())
            .arg(qApp->translateVariables()->FormulaToUser(
                QString::number(qApp->fromPixel(this->line().length())),
                qApp->Settings()->getOsSeparator()))
            .arg(prefix);
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolEndLine::Angle() const { return QString("%1").arg(this->line().angle()); }

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::SetAngle(const QString& expression)
{
    angle = FindVal(expression, Visualization::data->DataVariables());
}

//---------------------------------------------------------------------------------------------------------------------
QString VisToolEndLine::Length() const
{
    return QString::number(qApp->fromPixel(this->line().length()));
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolEndLine::setLength(const QString& expression)
{
    length = FindLength(expression, Visualization::data->DataVariables());
}
