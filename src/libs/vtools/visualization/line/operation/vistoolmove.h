/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
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
 **  @date   1 10, 2016
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

#ifndef VISTOOLMOVE_H
#define VISTOOLMOVE_H

#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "visoperation.h"

class VisToolMove : public VisOperation
{
    Q_OBJECT
public:
    explicit VisToolMove(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolMove();

    void RefreshGeometry() override;

    QString Angle() const;
    void SetAngle(const QString& expression);

    QString Rotation() const;
    void setRotation(const QString& expression);

    QString Length() const;
    qreal LengthValue() const;
    void SetLength(const QString& expression);

    void setOriginPointId(quint32 value);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolMove)
    };

private:
    Q_DISABLE_COPY(VisToolMove)
    qreal angle;
    qreal length;
    qreal rotationAngle;
    VScaledEllipse* originPointItem;
    VScaledEllipse* rotationOriginPointItem;
    VScaledEllipse* rotationFinishPointItem;
    ArrowedLineItem* moveLineItem;
    VScaledLine* rotationLineItem;
    QPointF m_origin;
    QPointF m_rotationPoint;

    template <class Item>
    QGraphicsPathItem* AddOriginCurve(quint32 id, int& i);

    template <class Item>
    int AddDestinationCurve(
        qreal angle,
        qreal length,
        quint32 id,
        int i,
        qreal rotationAngle,
        const QPointF& rotationOrigin);

    void createOriginObjects(int& iPoint, int& iCurve);
    void createRotatedObjects(
        int& iPoint,
        int& iCurve,
        qreal length,
        qreal angle,
        qreal rotationAngle,
        const QPointF& rotationOrigin);
};

#endif   // VISTOOLMOVE_H
