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
 **  @file   vcurvelength.h
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

#ifndef VCURVEVARIABLE_H
#define VCURVEVARIABLE_H

#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vinternalvariable.h"

class VCurveVariableData;

class VCurveVariable : public VInternalVariable
{
public:
    VCurveVariable();
    VCurveVariable(const quint32& id, const quint32& parentId);
    VCurveVariable(const VCurveVariable& var);

    virtual ~VCurveVariable() override;

    VCurveVariable& operator=(const VCurveVariable& var);
#ifdef Q_COMPILER_RVALUE_REFS
    VCurveVariable& operator=(VCurveVariable&& var) noexcept;
#endif

    void Swap(VCurveVariable& var) noexcept;

    virtual bool Filter(quint32 id) override;

    quint32 GetId() const;
    void SetId(const quint32& id);

    quint32 GetParentId() const;
    void SetParentId(const quint32& value);

private:
    QSharedDataPointer<VCurveVariableData> d;
};

Q_DECLARE_TYPEINFO(VCurveVariable, Q_MOVABLE_TYPE);

#endif   // VCURVEVARIABLE_H
