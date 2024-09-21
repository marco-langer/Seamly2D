/******************************************************************************
 *   @file   vabstractcubicbezierpath.h
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
 *****************************************************************************

 ************************************************************************
 **
 **  @file   vabstractcubicbezierpath.h
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

#ifndef VABSTRACTCUBICBEZIERPATH_H
#define VABSTRACTCUBICBEZIERPATH_H

#include <QCoreApplication>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vabstractbezier.h"
#include "vgeometrydef.h"

class VPointF;
class VSpline;
class VSplinePoint;

class VAbstractCubicBezierPath : public VAbstractBezier
{
    Q_DECLARE_TR_FUNCTIONS(VAbstractCubicBezierPath)

public:
    VAbstractCubicBezierPath(
        const GOType& type,
        const quint32& idObject = NULL_ID,
        const Draw& mode = Draw::Calculation);
    VAbstractCubicBezierPath(const VAbstractCubicBezierPath& curve);
    VAbstractCubicBezierPath& operator=(const VAbstractCubicBezierPath& curve);

    virtual ~VAbstractCubicBezierPath();

    virtual qint32 CountSubSpl() const = 0;
    virtual qint32 CountPoints() const = 0;
    virtual void Clear() = 0;
    virtual VSpline GetSpline(qint32 index) const = 0;
    virtual QVector<VSplinePoint> GetSplinePath() const = 0;

    QPainterPath GetPath() const override;
    QVector<QPointF> getPoints() const override;
    qreal GetLength() const override;

    QVector<DirectionArrow> DirectionArrows() const override;

    int Segment(const QPointF& p) const;

    QPointF CutSplinePath(
        qreal length,
        qint32& p1,
        qint32& p2,
        QPointF& spl1p2,
        QPointF& spl1p3,
        QPointF& spl2p2,
        QPointF& spl2p3) const;

    QString NameForHistory(const QString& toolName) const override;

    virtual VPointF FirstPoint() const = 0;
    virtual VPointF LastPoint() const = 0;

protected:
    void CreateName() override;
};

#endif   // VABSTRACTCUBICBEZIERPATH_H
