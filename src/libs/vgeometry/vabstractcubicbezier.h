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
 **  @file   vabstractcubicbezier.h
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

#ifndef VABSTRACTCUBICBEZIER_H
#define VABSTRACTCUBICBEZIER_H

#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "vabstractbezier.h"
#include "vgeometrydef.h"

class VPointF;

class VAbstractCubicBezier : public VAbstractBezier
{
public:
    VAbstractCubicBezier(
        const GOType& type,
        const quint32& idObject = NULL_ID,
        const Draw& mode = Draw::Calculation);
    VAbstractCubicBezier(const VAbstractCubicBezier& curve);
    VAbstractCubicBezier& operator=(const VAbstractCubicBezier& curve);
    virtual ~VAbstractCubicBezier();

    virtual const VPointF& GetP1() const = 0;
    virtual VPointF GetP2() const = 0;
    virtual VPointF GetP3() const = 0;
    virtual const VPointF& GetP4() const = 0;

    QPointF CutSpline(
        qreal length, QPointF& spl1p2, QPointF& spl1p3, QPointF& spl2p2, QPointF& spl2p3) const;

    QString NameForHistory(const QString& toolName) const override;

    qreal GetParmT(qreal length) const;
    qreal LengthT(qreal t) const;

protected:
    void CreateName() override;

    static qreal CalcSqDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    static void PointBezier_r(
        qreal x1,
        qreal y1,
        qreal x2,
        qreal y2,
        qreal x3,
        qreal y3,
        qreal x4,
        qreal y4,
        qint16 level,
        QVector<qreal>& px,
        QVector<qreal>& py);
    static QVector<QPointF> GetCubicBezierPoints(
        const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4);
    static qreal
    LengthBezier(const QPointF& p1, const QPointF& p2, const QPointF& p3, const QPointF& p4);

    virtual QPointF GetControlPoint1() const = 0;
    virtual QPointF GetControlPoint2() const = 0;
};

#endif   // VABSTRACTCUBICBEZIER_H
