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
 **  @file   varcradius.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
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

#include "varcradius.h"

#include <QMessageLogger>
#include <QString>

#include "../ifc/ifcdef.h"
#include "../vgeometry/varc.h"
#include "../vgeometry/vellipticalarc.h"


//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius()
    : VCurveVariable()
{
    SetType(VarType::ArcRadius);
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius(
    const quint32& id, const quint32& parentId, const VArc& arc, Unit patternUnit)
    : VCurveVariable(id, parentId)
{
    SetType(VarType::ArcRadius);
    SetName(radius_V + QString("%1").arg(arc.name()));
    SetValue(FromPixel(arc.GetRadius(), patternUnit));
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius(
    const quint32& id,
    const quint32& parentId,
    const VEllipticalArc& elArc,
    const int numberRadius,
    Unit patternUnit)
    : VCurveVariable(id, parentId)
{
    SetType(VarType::ArcRadius);
    SetName(radius_V + QString("%1%2").arg(numberRadius).arg(elArc.name()));
    if (numberRadius == 1) {
        SetValue(FromPixel(elArc.GetRadius1(), patternUnit));
    } else {
        SetValue(FromPixel(elArc.GetRadius2(), patternUnit));
    }
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::VArcRadius(const VArcRadius& var)
    : VCurveVariable(var)
{}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius& VArcRadius::operator=(const VArcRadius& var)
{
    if (&var == this) {
        return *this;
    }
    VCurveVariable::operator=(var);
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VArcRadius::~VArcRadius() = default;
