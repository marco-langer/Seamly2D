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
 **  @file   vabstractarc.h
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

#ifndef VABSTRACTARC_H
#define VABSTRACTARC_H

#include <QSharedDataPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "vabstractcurve.h"
#include "vgeometrydef.h"

class VAbstractArcData;
class VPointF;

class VAbstractArc : public VAbstractCurve
{
public:
    explicit VAbstractArc(
        const GOType& type,
        const quint32& idObject = NULL_ID,
        const Draw& mode = Draw::Calculation);

    VAbstractArc(
        const GOType& type,
        const VPointF& center,
        qreal f1,
        const QString& formulaF1,
        qreal f2,
        const QString& formulaF2,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);

    VAbstractArc(
        const GOType& type,
        const VPointF& center,
        qreal f1,
        qreal f2,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);

    VAbstractArc(
        const GOType& type,
        const QString& formulaLength,
        const VPointF& center,
        qreal f1,
        const QString& formulaF1,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);

    VAbstractArc(
        const GOType& type,
        const VPointF& center,
        qreal f1,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);

    explicit VAbstractArc(const VAbstractArc& arc);
    virtual ~VAbstractArc();

    VAbstractArc& operator=(const VAbstractArc& arc);
    VAbstractArc& operator=(VAbstractArc&& arc) noexcept;

    void Swap(VAbstractArc& arc) noexcept;

    const QString& GetFormulaF1() const;
    void SetFormulaF1(const QString& formula, qreal value);
    qreal GetStartAngle() const override;

    const QString& GetFormulaF2() const;
    void SetFormulaF2(const QString& formula, qreal value);
    qreal GetEndAngle() const override;

    virtual VPointF GetCenter() const;
    void SetCenter(const VPointF& point);

    const QString& GetFormulaLength() const;
    void SetFormulaLength(const QString& formula, qreal value);

    void setId(const quint32& id) override;
    QString NameForHistory(const QString& toolName) const override;

    bool IsFlipped() const;
    qreal AngleArc() const;

protected:
    void SetFlipped(bool value);
    virtual void FindF2(qreal length) = 0;
    void SetFormulaLength(const QString& formula);

private:
    QSharedDataPointer<VAbstractArcData> d;
};

#endif   // VABSTRACTARC_H
