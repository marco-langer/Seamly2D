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
 **  @file   vlinelength.h
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

#ifndef VLINELENGTH_H
#define VLINELENGTH_H

#include <QSharedDataPointer>
#include <QTypeInfo>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "vinternalvariable.h"

class VLengthLineData;
class VPointF;

class VLengthLine : public VInternalVariable
{
public:
    VLengthLine();
    VLengthLine(
        const VPointF& p1,
        const quint32& p1Id,
        const VPointF& p2,
        const quint32& p2Id,
        Unit patternUnit);
    VLengthLine(const VLengthLine& var);

    ~VLengthLine() override;

    VLengthLine& operator=(const VLengthLine& var);
    VLengthLine& operator=(VLengthLine&& var) noexcept;

    void Swap(VLengthLine& var) noexcept;

    bool Filter(quint32 id) override;

    quint32 GetP1Id() const;
    quint32 GetP2Id() const;

private:
    void SetValue(const VPointF& p1, const VPointF& p2);

    QSharedDataPointer<VLengthLineData> d;
};

Q_DECLARE_TYPEINFO(VLengthLine, Q_MOVABLE_TYPE);

#endif   // VLINELENGTH_H
