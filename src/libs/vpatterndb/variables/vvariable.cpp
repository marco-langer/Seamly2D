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
 **  @file   vvariable.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 7, 2014
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

#include "vvariable.h"

#include <QMessageLogger>
#include <QtDebug>

#include "vinternalvariable.h"
#include "vvariable_p.h"

#ifdef Q_COMPILER_RVALUE_REFS
VVariable& VVariable::operator=(VVariable&& var) noexcept
{
    Swap(var);
    return *this;
}
#endif

void VVariable::Swap(VVariable& var) noexcept
{
    VInternalVariable::Swap(var);
    std::swap(d, var.d);
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable()
    : VInternalVariable()
    , d(new VVariableData)
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const QString& name, const QString& description)
    : VInternalVariable()
    , d(new VVariableData(description))
{
    SetName(name);
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::VVariable(const VVariable& var)
    : VInternalVariable(var)
    , d(var.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VVariable& VVariable::operator=(const VVariable& var)
{
    if (&var == this) {
        return *this;
    }
    VInternalVariable::operator=(var);
    d = var.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VVariable::~VVariable() {}

//---------------------------------------------------------------------------------------------------------------------
QString VVariable::GetDescription() const { return d->description; }


//---------------------------------------------------------------------------------------------------------------------
void VVariable::SetDescription(const QString& desc) { d->description = desc; }
