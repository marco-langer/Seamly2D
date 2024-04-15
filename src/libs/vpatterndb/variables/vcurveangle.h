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
 **  @file   vcurveangle.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   1 6, 2015
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

#ifndef VCURVEANGLE_H
#define VCURVEANGLE_H

#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vcurvevariable.h"

class VAbstractCurve;
class VSpline;

enum class CurveAngle : char
{
    StartAngle,
    EndAngle
};

class VCurveAngle : public VCurveVariable
{
public:
    VCurveAngle();
    VCurveAngle(
        const quint32& id, const quint32& parentId, const VAbstractCurve* curve, CurveAngle angle);
    VCurveAngle(
        const quint32& id,
        const quint32& parentId,
        const QString& baseCurveName,
        const VSpline& spl,
        CurveAngle angle,
        qint32 segment);
    VCurveAngle(const VCurveAngle& var);
    VCurveAngle& operator=(const VCurveAngle& var);
    virtual ~VCurveAngle() Q_DECL_OVERRIDE;
};

#endif   // VCURVEANGLE_H
