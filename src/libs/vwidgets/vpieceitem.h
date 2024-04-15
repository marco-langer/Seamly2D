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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Seamly2D project
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

#ifndef VPIECEITEM_H
#define VPIECEITEM_H

#include <QGraphicsObject>
#include <QtCore/qglobal.h>

#include "../vmisc/def.h"

class VPieceItem : public QGraphicsObject
{
    Q_OBJECT
public:
    enum MoveType
    {
        NotMovable = 0x0,
        IsRotatable = 0x1,   // 0001
        IsResizable = 0x2,   // 0010
        IsMovable = 0x4,     // 0100
        AllModifications = IsRotatable | IsResizable | IsMovable,
        Error = 0x8   // 1000
    };
    Q_DECLARE_FLAGS(MoveTypes, MoveType)

    explicit VPieceItem(QGraphicsItem* pParent = nullptr);
    virtual ~VPieceItem();

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;

    virtual void Update() = 0;

    void Reset();
    bool IsIdle() const;

    VPieceItem::MoveTypes GetMoveType() const;
    void SetMoveType(const VPieceItem::MoveTypes& moveType);

    virtual int type() const Q_DECL_OVERRIDE { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::PieceItem)
    };

signals:
    void itemMoved(const QPointF& ptPos);

protected:
    enum Mode
    {
        mNormal,
        mMove,
        mResize,
        mRotate
    };
    QRectF m_rectBoundingBox;
    Mode m_eMode;
    bool m_bReleased;
    QPointF m_ptRotCenter;
    VPieceItem::MoveTypes m_moveType;

    qreal m_inactiveZ;

    virtual double GetAngle(const QPointF& pt) const;

private:
    Q_DISABLE_COPY(VPieceItem)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(VPieceItem::MoveTypes)

#endif   // VPIECEITEM_H
