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
    virtual ~VGrainlineItem() Q_DECL_EQ_DEFAULT;

    virtual QPainterPath shape() const Q_DECL_OVERRIDE;

    virtual void
    paint(QPainter* pP, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget) Q_DECL_OVERRIDE;
    void UpdateGeometry(const QPointF& ptPos, qreal dRotation, qreal dLength, ArrowType eAT);

    virtual int type() const Q_DECL_OVERRIDE { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::GrainlineItem)
    };

    bool isContained(const QPointF& pt, qreal dRot, qreal& dX, qreal& dY) const;

signals:
    void itemResized(qreal dLength);
    void itemRotated(qreal dRot, const QPointF& ptNewPos);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* pME) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* pME) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* pME) Q_DECL_OVERRIDE;
    virtual void Update() Q_DECL_OVERRIDE;
    void UpdateRectangle();

    virtual double GetAngle(const QPointF& pt) const Q_DECL_OVERRIDE;

    QPointF Rotate(const QPointF& pt, const QPointF& ptCenter, qreal dAng) const;
    QPointF GetInsideCorner(int i, qreal dDist) const;

private:
    Q_DISABLE_COPY(VGrainlineItem)
    qreal m_dRotation;
    qreal m_dStartRotation;
    qreal m_dLength;
    QPolygonF m_polyBound;
    QPointF m_ptStartPos;
    QPointF m_ptStartMove;
    qreal m_dScale;
    QPolygonF m_polyResize;
    qreal m_dStartLength;
    QPointF m_ptStart;
    QPointF m_ptFinish;
    QPointF m_ptCenter;
    qreal m_dAngle;
    ArrowType m_eArrowType;
    qreal m_penWidth;

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
