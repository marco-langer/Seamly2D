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
 **  @file   vabstractcurve_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 2, 2016
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

#ifndef VABSTRACTCURVE_P_H
#define VABSTRACTCURVE_P_H

#include <QSharedData>

#include "../ifc/ifcdef.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VAbstractCurveData : public QSharedData
{
public:
    VAbstractCurveData() = default;
    VAbstractCurveData(const VAbstractCurveData& curve)
        : QSharedData(curve)
        , duplicate(curve.duplicate)
        , color(curve.color)
        , penStyle(curve.penStyle)
        , lineWeight(curve.lineWeight)

    {}

    virtual ~VAbstractCurveData();

    /** @brief duplicate helps create unique name for curves that connects the same start and finish
     * points. */
    quint32 duplicate{ 0 };
    QString color{ ColorBlack };
    QString penStyle{ LineTypeSolidLine };
    QString lineWeight{ "0.35" };

private:
    VAbstractCurveData& operator=(const VAbstractCurveData&) = delete;
};

VAbstractCurveData::~VAbstractCurveData() = default;

QT_WARNING_POP

#endif   // VABSTRACTCURVE_P_H
