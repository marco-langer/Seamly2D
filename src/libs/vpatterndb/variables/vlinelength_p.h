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
 **  @file   vlinelength_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
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

#ifndef VLINELENGTH_P_H
#define VLINELENGTH_P_H

#include <QSharedData>

#include "../ifc/ifcdef.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VLengthLineData : public QSharedData
{
public:
    VLengthLineData()
        : p1Id(NULL_ID)
        , p2Id(NULL_ID)
        , patternUnit(Unit::Cm)
    {}

    VLengthLineData(const quint32& p1Id, const quint32& p2Id, Unit patternUnit)
        : p1Id(p1Id)
        , p2Id(p2Id)
        , patternUnit(patternUnit)
    {}

    VLengthLineData(const VLengthLineData& var)
        : QSharedData(var)
        , p1Id(var.p1Id)
        , p2Id(var.p2Id)
        , patternUnit(var.patternUnit)
    {}

    virtual ~VLengthLineData();

    quint32 p1Id;
    quint32 p2Id;
    Unit patternUnit;

private:
    VLengthLineData& operator=(const VLengthLineData&) = delete;
};

VLengthLineData::~VLengthLineData() = default;

QT_WARNING_POP

#endif   // VLINELENGTH_P_H
