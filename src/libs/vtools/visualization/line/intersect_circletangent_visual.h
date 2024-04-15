/**************************************************************************
 **
 **  @file   intersect_circletangent_visual.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 6, 2015
 **
 **  @author Douglas S. Caskey
 **  @date   7.16.2022
 **
 **  @copyright
 **  Copyright (C) 2013-2022 Seamly2D project.
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published
 **  by the Free Software Foundation, either version 3 of the License,
 **  or (at your option) any later version.
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

#ifndef INTERSECT_CIRCLETANGENT_VISUAL_H
#define INTERSECT_CIRCLETANGENT_VISUAL_H

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "visline.h"

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

class IntersectCircleTangentVisual : public VisLine
{
    Q_OBJECT
public:
    explicit IntersectCircleTangentVisual(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~IntersectCircleTangentVisual() = default;

    virtual void RefreshGeometry() override;

    void setObject2Id(const quint32& value);
    void setCRadius(const QString& value);
    void setCrossPoint(const CrossCirclesPoint& value);

    virtual int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolPointFromCircleAndTangent)
    };

private:
    Q_DISABLE_COPY(IntersectCircleTangentVisual)
    quint32 object2Id;
    qreal cRadius;
    CrossCirclesPoint crossPoint;
    VScaledEllipse* point;
    VScaledEllipse* tangent;
    VScaledEllipse* cCenter;
    QGraphicsEllipseItem* cPath;
    VScaledLine* tangent2;
    QColor m_secondarySupportColor;

    void FindRays(const QPointF& p, const QPointF& center, qreal radius);
};

#endif   // INTERSECT_CIRCLETANGENT_VISUAL_H
