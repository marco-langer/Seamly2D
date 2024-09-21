/******************************************************************************
 *   @file   vsplinepath.h
 **  @author Douglas S Caskey
 **  @date   22 Mar, 2023
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
 *****************************************************************************

 ************************************************************************
 **
 **  @file   vsplinepath.h
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

#ifndef VSPLINEPATH_H
#define VSPLINEPATH_H

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
#include "vspline.h"
#include "vsplinepoint.h"

class VSplinePathData;

/**
 * @brief The VSplinePath class keep information about splinePath.
 */
class VSplinePath : public VAbstractCubicBezierPath
{
    Q_DECLARE_TR_FUNCTIONS(VSplinePath)

public:
    explicit VSplinePath(quint32 idObject = 0, Draw mode = Draw::Calculation);

    VSplinePath(
        const QVector<VFSplinePoint>& points,
        qreal kCurve = 1,
        quint32 idObject = 0,
        Draw mode = Draw::Calculation);
    VSplinePath(
        const QVector<VSplinePoint>& points, quint32 idObject = 0, Draw mode = Draw::Calculation);
    VSplinePath(const VSplinePath& splPath);

    VSplinePath
    Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix = QString()) const;
    VSplinePath Flip(const QLineF& axis, const QString& prefix = QString()) const;
    VSplinePath Move(qreal length, qreal angle, const QString& prefix = QString()) const;
    ~VSplinePath() override;

    VSplinePoint& operator[](int indx);
    VSplinePath& operator=(const VSplinePath& path);
    VSplinePath& operator=(VSplinePath&& path) noexcept;

    void Swap(VSplinePath& path) noexcept;

    void append(const VSplinePoint& point);

    qint32 CountSubSpl() const override;
    qint32 CountPoints() const override;
    void Clear() override;
    VSpline GetSpline(qint32 index) const override;

    QVector<VSplinePoint> GetSplinePath() const override;
    QVector<VFSplinePoint> GetFSplinePath() const;

    qreal GetStartAngle() const override;
    qreal GetEndAngle() const override;

    qreal GetC1Length() const override;
    qreal GetC2Length() const override;

    void UpdatePoint(qint32 indexSpline, const SplinePointPosition& pos, const VSplinePoint& point);
    VSplinePoint GetSplinePoint(qint32 indexSpline, SplinePointPosition pos) const;

    const VSplinePoint& at(int indx) const;

    VPointF FirstPoint() const override;
    VPointF LastPoint() const override;

private:
    QSharedDataPointer<VSplinePathData> d;
};

Q_DECLARE_TYPEINFO(VSplinePath, Q_MOVABLE_TYPE);

#endif   // VSPLINEPATH_H
