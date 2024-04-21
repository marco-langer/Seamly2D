/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
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
 **  @file   VAbstractArcData.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 4, 2016
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

#ifndef VABSTRACTARC_P_H
#define VABSTRACTARC_P_H

#include "../vmisc/vabstractapplication.h"
#include "vgeometrydef.h"
#include "vpointf.h"
#include <QSharedData>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VAbstractArcData : public QSharedData
{
public:
    VAbstractArcData() = default;
    VAbstractArcData(
        const VPointF& center,
        qreal f1,
        const QString& formulaF1,
        qreal f2,
        const QString& formulaF2);
    VAbstractArcData(
        const QString& formulaLength, const VPointF& center, qreal f1, const QString& formulaF1);
    VAbstractArcData(const VPointF& center, qreal f1);
    VAbstractArcData(const VPointF& center, qreal f1, qreal f2);
    VAbstractArcData(const VAbstractArcData& arc);
    virtual ~VAbstractArcData();

    qreal f1{ 0.0 };   /** @brief f1 start angle in degree. */
    QString formulaF1; /** @brief formulaF1 formula for start angle. */
    qreal f2{ 0.0 };   /** @brief f2 end angle in degree. */
    QString formulaF2; /** @brief formulaF2 formula for end angle. */
    VPointF center;    /** @brief center center point of arc. */
    bool isFlipped{ false };
    QString formulaLength;

private:
    VAbstractArcData& operator=(const VAbstractArcData&) = delete;
};

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::VAbstractArcData(
    const VPointF& center, qreal f1, const QString& formulaF1, qreal f2, const QString& formulaF2)
    : f1(f1)
    , formulaF1(formulaF1)
    , f2(f2)
    , formulaF2(formulaF2)
    , center(center)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::VAbstractArcData(
    const QString& formulaLength, const VPointF& center, qreal f1, const QString& formulaF1)
    : f1(f1)
    , formulaF1(formulaF1)
    , formulaF2("0")
    , center(center)
    , formulaLength(formulaLength)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::VAbstractArcData(const VPointF& center, qreal f1)
    : f1(f1)
    , formulaF1(QString::number(f1))
    , formulaF2("0")
    , center(center)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::VAbstractArcData(const VPointF& center, qreal f1, qreal f2)
    : f1(f1)
    , formulaF1(QString::number(f1))
    , f2(f2)
    , formulaF2(QString::number(f2))
    , center(center)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::VAbstractArcData(const VAbstractArcData& arc)
    : QSharedData(arc)
    , f1(arc.f1)
    , formulaF1(arc.formulaF1)
    , f2(arc.f2)
    , formulaF2(arc.formulaF2)
    , center(arc.center)
    , isFlipped(arc.isFlipped)
    , formulaLength(arc.formulaLength)
{}

//---------------------------------------------------------------------------------------------------------------------
VAbstractArcData::~VAbstractArcData() {}

QT_WARNING_POP

#endif   // VABSTRACTARCDATA_H
