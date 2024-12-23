/************************************************************************
 **
 **  @file   vgrainlineitem.h
 **  @author Bojan Kverh
 **  @date   September 10, 2016
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

#ifndef VGRAINLINEITEM_H
#define VGRAINLINEITEM_H

#include "../vmisc/def.h"
#include "../vpatterndb/floatItemData/vgrainlinedata.h"
#include "vpieceitem.h"

class VGrainlineItem : public VPieceItem
{
    Q_OBJECT
public:
    explicit VGrainlineItem(QGraphicsItem* pParent = nullptr);

    QPainterPath shape() const override;

    void paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget) override;
    void UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength, ArrowType eAT);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::GrainlineItem)
    };

    bool isContained(const QPointF& pt, qreal dRot, qreal& dX, qreal& dY) const;

signals:
    void itemResized(qreal dLength);
    void itemRotated(qreal dRot, const QPointF& ptNewPos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* pME) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pME) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pME) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* pME) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* pME) override;
    void Update() override;
    void UpdateRectangle();

    double GetAngle(const QPointF& pt) const override;

    QPointF Rotate(const QPointF& pt, const QPointF& ptCenter, qreal dAng) const;
    QPointF GetInsideCorner(int i, qreal dDist) const;

private:
    Q_DISABLE_COPY(VGrainlineItem)
    qreal m_dRotation{ 0.0 };
    qreal m_dStartRotation{ 0.0 };
    qreal m_dLength{ 0.0 };
    QPolygonF m_polyBound;
    QPointF m_ptStartPos;
    QPointF m_ptStartMove;
    qreal m_dScale{ 1.0 };
    QPolygonF m_polyResize;
    qreal m_dStartLength{ 0.0 };
    QPointF m_ptStart;
    QPointF m_ptFinish;
    QPointF m_ptCenter;
    qreal m_dAngle{ 0.0 };
    ArrowType m_eArrowType{ ArrowType::atBoth };
    qreal m_penWidth{ 3.0 };

    qreal GetScale() const;

    QLineF MainLine() const;
    QPolygonF FirstArrow(qreal dArrLen) const;
    QPolygonF SecondArrow(qreal dArrLen) const;

    QPainterPath MainShape() const;

    void allUserModifications(const QPointF& pos);
    void userRotateAndMove();
    void userMoveAndResize(const QPointF& pos);

    void UpdatePolyResize();
};

#endif   // VGRAINLINEITEM_H
