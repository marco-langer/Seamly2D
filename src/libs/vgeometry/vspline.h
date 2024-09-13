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
 **  @file   vspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VSPLINE_H
#define VSPLINE_H

#include <QLineF>
#include <QMetaType>
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

class VSplineData;

/**
 * @brief VSpline class that implements the spline.
 */
class VSpline : public VAbstractCubicBezier
{
public:
    VSpline();
    VSpline(const VSpline& spline);
    VSpline(
        const VPointF& p1,
        const VPointF& p4,
        qreal angle1,
        qreal angle2,
        qreal kAsm1,
        qreal kAsm2,
        qreal kCurve,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);
    VSpline(
        const VPointF& p1,
        const QPointF& p2,
        const QPointF& p3,
        const VPointF& p4,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);
    VSpline(
        const VPointF& p1,
        const VPointF& p4,
        qreal angle1,
        const QString& angle1Formula,
        qreal angle2,
        const QString& angle2Formula,
        qreal c1Length,
        const QString& c1LengthFormula,
        qreal c2Length,
        const QString& c2LengthFormula,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);
    VSpline
    Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix = QString()) const;
    VSpline Flip(const QLineF& axis, const QString& prefix = QString()) const;
    VSpline Move(qreal length, qreal angle, const QString& prefix = QString()) const;
    virtual ~VSpline();

    VSpline& operator=(const VSpline& spline);
    VSpline& operator=(VSpline&& spline) noexcept;

    void Swap(VSpline& spline) noexcept;

    VPointF GetP1() const override;
    void SetP1(const VPointF& p);

    VPointF GetP2() const override;
    VPointF GetP3() const override;

    VPointF GetP4() const override;
    void SetP4(const VPointF& p);

    qreal GetStartAngle() const override;
    qreal GetEndAngle() const override;

    QString GetStartAngleFormula() const;
    QString GetEndAngleFormula() const;

    void SetStartAngle(qreal angle, const QString& formula);
    void SetEndAngle(qreal angle, const QString& formula);

    qreal GetC1Length() const override;
    qreal GetC2Length() const override;

    QString GetC1LengthFormula() const;
    QString GetC2LengthFormula() const;

    void SetC1Length(qreal length, const QString& formula);
    void SetC2Length(qreal length, const QString& formula);

    qreal GetLength() const override;
    qreal GetKasm1() const;
    qreal GetKasm2() const;
    qreal GetKcurve() const;

    using VAbstractCubicBezier::CutSpline;
    QPointF CutSpline(qreal length, VSpline& spl1, VSpline& spl2) const;

    QVector<QPointF> getPoints() const override;
    // cppcheck-suppress unusedFunction
    static QVector<QPointF> SplinePoints(
        const QPointF& p1,
        const QPointF& p4,
        qreal angle1,
        qreal angle2,
        qreal kAsm1,
        qreal kAsm2,
        qreal kCurve);
    qreal ParamT(const QPointF& pBt) const;

protected:
    QPointF GetControlPoint1() const override;
    QPointF GetControlPoint2() const override;

private:
    QSharedDataPointer<VSplineData> d;
    QVector<qreal> CalcT(
        qreal curveCoord1,
        qreal curveCoord2,
        qreal curveCoord3,
        qreal curveCoord4,
        qreal pointCoord) const;
    static qint32 Cubic(QVector<qreal>& x, qreal a, qreal b, qreal c);
    static int Sign(long double ld);
};

Q_DECLARE_METATYPE(VSpline)
Q_DECLARE_TYPEINFO(VSpline, Q_MOVABLE_TYPE);

#endif   // VSPLINE_H
