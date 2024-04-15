/******************************************************************************
 *   @file   vcubicbezierpath.h
 **  @author Douglas S Caskey
 **  @date   21 Mar, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program to create and model patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *****************************************************************************/

/************************************************************************
 **
 **  @file   vcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
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

#ifndef VCUBICBEZIERPATH_H
#define VCUBICBEZIERPATH_H

#include <QCoreApplication>
#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vabstractcubicbezierpath.h"
#include "vgeometrydef.h"
#include "vpointf.h"

class VCubicBezierPathData;

class VCubicBezierPath : public VAbstractCubicBezierPath
{
    Q_DECLARE_TR_FUNCTIONS(VCubicBezierPath)
public:
    explicit VCubicBezierPath(quint32 idObject = 0, Draw mode = Draw::Calculation);
    VCubicBezierPath(const VCubicBezierPath& curve);
    VCubicBezierPath(
        const QVector<VPointF>& points, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VCubicBezierPath
    Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix = QString()) const;
    VCubicBezierPath Flip(const QLineF& axis, const QString& prefix = QString()) const;
    VCubicBezierPath Move(qreal length, qreal angle, const QString& prefix = QString()) const;
    virtual ~VCubicBezierPath();

    VCubicBezierPath& operator=(const VCubicBezierPath& curve);
    VCubicBezierPath& operator=(VCubicBezierPath&& curve) noexcept;

    void Swap(VCubicBezierPath& curve) noexcept;

    VPointF& operator[](int indx);

    const VPointF& at(int indx) const;

    void append(const VPointF& point);

    virtual qint32 CountSubSpl() const override;
    virtual qint32 CountPoints() const override;
    virtual void Clear() override;
    virtual VSpline GetSpline(qint32 index) const override;
    virtual qreal GetStartAngle() const override;
    virtual qreal GetEndAngle() const override;

    virtual qreal GetC1Length() const override;
    virtual qreal GetC2Length() const override;

    virtual QVector<VSplinePoint> GetSplinePath() const override;
    QVector<VPointF> GetCubicPath() const;

    static qint32 CountSubSpl(qint32 size);
    static qint32 SubSplOffset(qint32 subSplIndex);
    static qint32 SubSplPointsCount(qint32 countSubSpl);

    virtual VPointF FirstPoint() const override;
    virtual VPointF LastPoint() const override;

private:
    QSharedDataPointer<VCubicBezierPathData> d;
};

Q_DECLARE_TYPEINFO(VCubicBezierPath, Q_MOVABLE_TYPE);

#endif   // VCUBICBEZIERPATH_H
