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
 **  @file   vcurvelength.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 8, 2014
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

#include "vcurvevariable.h"

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "vcurvevariable_p.h"
#include "vinternalvariable.h"

#ifdef Q_COMPILER_RVALUE_REFS
VCurveVariable& VCurveVariable::operator=(VCurveVariable&& var) noexcept
{
    Swap(var);
    return *this;
}
#endif

void VCurveVariable::Swap(VCurveVariable& var) noexcept
{
    VInternalVariable::Swap(var);
    std::swap(d, var.d);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveVariable::VCurveVariable()
    : VInternalVariable()
    , d(new VCurveVariableData)
{
    SetType(VarType::Unknown);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveVariable::VCurveVariable(const quint32& id, const quint32& parentId)
    : VInternalVariable()
    , d(new VCurveVariableData(id, parentId))
{
    SetType(VarType::Unknown);
}

//---------------------------------------------------------------------------------------------------------------------
VCurveVariable::VCurveVariable(const VCurveVariable& var)
    : VInternalVariable(var)
    , d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VCurveVariable& VCurveVariable::operator=(const VCurveVariable& var)
{
    if (&var == this) {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VCurveVariable::~VCurveVariable() {}

//---------------------------------------------------------------------------------------------------------------------
bool VCurveVariable::Filter(quint32 id)
{
    if (id == NULL_ID) {
        return false;
    }

    if (d->parentId != NULL_ID)   // Do not check if value zero
    {   // Not all curves have parents. Only those who was created after cutting the parent curve.
        return d->id == id || d->parentId == id;
    } else {
        return d->id == id;
    }
}

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VCurveVariable::GetId() const { return d->id; }

//---------------------------------------------------------------------------------------------------------------------
void VCurveVariable::SetId(const quint32& id) { d->id = id; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
quint32 VCurveVariable::GetParentId() const { return d->parentId; }

//---------------------------------------------------------------------------------------------------------------------
void VCurveVariable::SetParentId(const quint32& value) { d->parentId = value; }
