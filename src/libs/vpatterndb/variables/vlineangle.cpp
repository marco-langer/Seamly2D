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
 **  @file   vlineangle.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   28 7, 2014
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

#include "vlineangle.h"

#include <QLineF>
#include <QMessageLogger>
#include <QPointF>
#include <QString>
#include <QtMath>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "vinternalvariable.h"
#include "vlineangle_p.h"

VLineAngle& VLineAngle::operator=(VLineAngle&& var) noexcept
{
    Swap(var);
    return *this;
}

void VLineAngle::Swap(VLineAngle& var) noexcept
{
    VInternalVariable::Swap(var);
    std::swap(d, var.d);
}

//---------------------------------------------------------------------------------------------------------------------
VLineAngle::VLineAngle()
    : VInternalVariable()
    , d(new VLineAngleData)
{
    SetType(VarType::LineAngle);
}

//---------------------------------------------------------------------------------------------------------------------
VLineAngle::VLineAngle(
    const VPointF& p1, const quint32& p1Id, const VPointF& p2, const quint32& p2Id)
    : VInternalVariable()
    , d(new VLineAngleData(p1Id, p2Id))
{
    SetType(VarType::LineAngle);

    SetName(QString(angleLine_ + "%1_%2").arg(p1.name(), p2.name()));
    SetValue(p1, p2);
}

//---------------------------------------------------------------------------------------------------------------------
VLineAngle::VLineAngle(const VLineAngle& var)
    : VInternalVariable(var)
    , d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLineAngle& VLineAngle::operator=(const VLineAngle& var)
{
    if (&var == this) {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLineAngle::~VLineAngle() = default;

//---------------------------------------------------------------------------------------------------------------------
bool VLineAngle::Filter(quint32 id) { return id == d->p1Id || id == d->p2Id; }

//---------------------------------------------------------------------------------------------------------------------
void VLineAngle::SetValue(const VPointF& p1, const VPointF& p2)
{
    // Correct angle. Try avoid results like 6,7563e-15.
    const qreal angle =
        qFloor(QLineF(static_cast<QPointF>(p1), static_cast<QPointF>(p2)).angle() * 100000.)
        / 100000.;
    VInternalVariable::SetValue(angle);
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VLineAngle::GetP1Id() const { return d->p1Id; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VLineAngle::GetP2Id() const { return d->p2Id; }
