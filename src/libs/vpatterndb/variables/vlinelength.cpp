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
 **  @file   vlinelength.cpp
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

#include "vlinelength.h"

#include <QLineF>
#include <QMessageLogger>
#include <QPointF>
#include <QString>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "vinternalvariable.h"
#include "vlinelength_p.h"

VLengthLine& VLengthLine::operator=(VLengthLine&& var) noexcept
{
    Swap(var);
    return *this;
}

void VLengthLine::Swap(VLengthLine& var) noexcept
{
    VInternalVariable::Swap(var);
    std::swap(d, var.d);
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine()
    : VInternalVariable()
    , d(new VLengthLineData)
{
    SetType(VarType::LineLength);
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine(
    const VPointF& p1,
    const quint32& p1Id,
    const VPointF& p2,
    const quint32& p2Id,
    Unit patternUnit)
    : VInternalVariable()
    , d(new VLengthLineData(p1Id, p2Id, patternUnit))
{
    SetType(VarType::LineLength);
    SetName(QString(line_ + "%1_%2").arg(p1.name(), p2.name()));
    SetValue(p1, p2);
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::VLengthLine(const VLengthLine& var)
    : VInternalVariable(var)
    , d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine& VLengthLine::operator=(const VLengthLine& var)
{
    if (&var == this) {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VLengthLine::~VLengthLine() = default;

//---------------------------------------------------------------------------------------------------------------------
bool VLengthLine::Filter(quint32 id) { return id == d->p1Id || id == d->p2Id; }

//---------------------------------------------------------------------------------------------------------------------
void VLengthLine::SetValue(const VPointF& p1, const VPointF& p2)
{
    VInternalVariable::SetValue(FromPixel(
        QLineF(static_cast<QPointF>(p1), static_cast<QPointF>(p2)).length(), d->patternUnit));
}

//---------------------------------------------------------------------------------------------------------------------
quint32 VLengthLine::GetP1Id() const { return d->p1Id; }

//---------------------------------------------------------------------------------------------------------------------
quint32 VLengthLine::GetP2Id() const { return d->p2Id; }
