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
 **  @file   vcubicbezier.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 3, 2016
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

#ifndef VCUBICBEZIER_H
#define VCUBICBEZIER_H

#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vabstractcubicbezier.h"
#include "vgeometrydef.h"
#include "vpointf.h"

class VCubicBezierData;

class VCubicBezier : public VAbstractCubicBezier
{
public:
    VCubicBezier();
    VCubicBezier(const VCubicBezier& curve);
    VCubicBezier(
        const VPointF& p1,
        const VPointF& p2,
        const VPointF& p3,
        const VPointF& p4,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);
    VCubicBezier
    Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix = QString()) const;
    VCubicBezier Flip(const QLineF& axis, const QString& prefix = QString()) const;
    VCubicBezier Move(qreal length, qreal angle, const QString& prefix = QString()) const;
    virtual ~VCubicBezier();

    VCubicBezier& operator=(const VCubicBezier& curve);
    VCubicBezier& operator=(VCubicBezier&& curve) noexcept;

    void Swap(VCubicBezier& curve) noexcept;

    const VPointF& GetP1() const override;
    void SetP1(const VPointF& p);

    VPointF GetP2() const override;
    void SetP2(const VPointF& p);

    VPointF GetP3() const override;
    void SetP3(const VPointF& p);

    const VPointF& GetP4() const override;
    void SetP4(const VPointF& p);

    qreal GetStartAngle() const override;
    qreal GetEndAngle() const override;
    qreal GetLength() const override;
    QVector<QPointF> getPoints() const override;

    qreal GetC1Length() const override;
    qreal GetC2Length() const override;

protected:
    QPointF GetControlPoint1() const override;
    QPointF GetControlPoint2() const override;

private:
    QSharedDataPointer<VCubicBezierData> d;
};

Q_DECLARE_TYPEINFO(VCubicBezier, Q_MOVABLE_TYPE);

#endif   // VCUBICBEZIER_H
