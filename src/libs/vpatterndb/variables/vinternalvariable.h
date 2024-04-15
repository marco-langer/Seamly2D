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
 **  @file   vinternalvariable.h
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

#ifndef VINTERNALVARIABLE_H
#define VINTERNALVARIABLE_H

#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QtGlobal>

#include "../vmisc/def.h"

class VInternalVariableData;

class VInternalVariable
{
public:
    VInternalVariable();
    VInternalVariable(const VInternalVariable& var);

    virtual ~VInternalVariable();

    VInternalVariable& operator=(const VInternalVariable& var);
    VInternalVariable& operator=(VInternalVariable&& var) noexcept;

    void Swap(VInternalVariable& var) noexcept;

    virtual qreal GetValue() const;
    virtual qreal* GetValue();

    QString GetName() const;
    void SetName(const QString& name);

    VarType GetType() const;
    void SetType(const VarType& type);

    virtual bool Filter(quint32 id);

    virtual bool isNotUsed() const;

protected:
    void SetValue(const qreal& value);

private:
    QSharedDataPointer<VInternalVariableData> d;
};

Q_DECLARE_TYPEINFO(VInternalVariable, Q_MOVABLE_TYPE);

#endif   // VINTERNALVARIABLE_H
