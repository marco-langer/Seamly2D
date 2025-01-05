/******************************************************************************
 *   @file   vabstractcurve.h
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
 **  @file   vabstractcurve.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
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

#ifndef VABSTRACTCURVE_H
#define VABSTRACTCURVE_H

#include <QPointF>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "vgeometrydef.h"
#include "vgobject.h"

using DirectionArrow = QPair<QLineF, QLineF>;

class QPainterPath;
class VAbstractCurveData;

class VAbstractCurve : public VGObject
{
public:
    explicit VAbstractCurve(
        const GOType& type,
        const quint32& idObject = NULL_ID,
        const Draw& mode = Draw::Calculation);
    explicit VAbstractCurve(const VAbstractCurve& curve);
    ~VAbstractCurve() override;

    VAbstractCurve& operator=(const VAbstractCurve& curve);
    VAbstractCurve& operator=(VAbstractCurve&& curve) noexcept;

    void Swap(VAbstractCurve& curve) noexcept;

    virtual QVector<QPointF> getPoints() const = 0;
    static QVector<QPointF> GetSegmentPoints(
        const QVector<QPointF>& points,
        const QPointF& begin,
        const QPointF& end,
        bool reverse = false);
    QVector<QPointF>
    GetSegmentPoints(const QPointF& begin, const QPointF& end, bool reverse = false) const;

    virtual QPainterPath GetPath() const;
    virtual qreal GetLength() const = 0;
    qreal GetLengthByPoint(const QPointF& point) const;
    virtual QVector<QPointF> IntersectLine(const QLineF& line) const;
    virtual bool IsIntersectLine(const QLineF& line) const;

    static bool isPointOnCurve(const QVector<QPointF>& points, const QPointF& p);
    bool isPointOnCurve(const QPointF& p) const;

    virtual qreal GetStartAngle() const = 0;
    virtual qreal GetEndAngle() const = 0;

    quint32 GetDuplicate() const;
    void SetDuplicate(quint32 number);

    const QString& getLineColor() const;
    void setLineColor(const QString& color);

    const QString& GetPenStyle() const;
    void SetPenStyle(const QString& penStyle);

    const QString& getLineWeight() const;
    void setLineWeight(const QString& lineWeight);

    static qreal PathLength(const QVector<QPointF>& path);

    QPointF getFirstPoint() const;
    QPointF getLastPoint() const;

    static QVector<QPointF> CurveIntersectLine(const QVector<QPointF>& points, const QLineF& line);

    virtual QString NameForHistory(const QString& toolName) const = 0;
    virtual QVector<DirectionArrow> DirectionArrows() const;
    static QPainterPath ShowDirection(const QVector<DirectionArrow>& arrows, qreal width);

    static const qreal lengthCurveDirectionArrow;

protected:
    virtual void CreateName() = 0;

private:
    QSharedDataPointer<VAbstractCurveData> d;

    static QVector<QPointF>
    FromBegin(const QVector<QPointF>& points, const QPointF& begin, bool* ok = nullptr);
    static QVector<QPointF>
    ToEnd(const QVector<QPointF>& points, const QPointF& end, bool* ok = nullptr);
};

Q_DECLARE_TYPEINFO(VAbstractCurve, Q_MOVABLE_TYPE);

#endif   // VABSTRACTCURVE_H
