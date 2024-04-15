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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 9, 2016
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

#ifndef VCURVECLENGTH_H
#define VCURVECLENGTH_H

#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vcurvevariable.h"

enum class CurveCLength : char
{
    C1,
    C2
};

class VAbstractBezier;
class VSpline;

class VCurveCLength : public VCurveVariable
{
public:
    VCurveCLength();
    VCurveCLength(
        const quint32& id,
        const quint32& parentId,
        const VAbstractBezier* curve,
        CurveCLength cType,
        Unit patternUnit);
    VCurveCLength(
        const quint32& id,
        const quint32& parentId,
        const QString& baseCurveName,
        const VSpline& spl,
        CurveCLength cType,
        Unit patternUnit,
        qint32 segment);
    VCurveCLength(const VCurveCLength& var);
    VCurveCLength& operator=(const VCurveCLength& var);
    virtual ~VCurveCLength() override;
};

#endif   // VCURVECLENGTH_H
