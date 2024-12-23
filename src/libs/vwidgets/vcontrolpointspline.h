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
 **  @file   vcontrolpointspline.h
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

#ifndef VCONTROLPOINTSPLINE_H
#define VCONTROLPOINTSPLINE_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vgeometrydef.h"
#include "../vgeometry/vsplinepath.h"
#include "../vmisc/def.h"
#include "scene_rect.h"

/**
 * @brief The VControlPointSpline class control spline point.
 */
class VControlPointSpline
    : public QObject
    , public SceneRect
{
    Q_OBJECT
public:
    VControlPointSpline(
        const qint32& indexSpline, SplinePointPosition position, QGraphicsItem* parent = nullptr);
    VControlPointSpline(
        const qint32& indexSpline,
        SplinePointPosition position,
        const QPointF& controlPoint,
        const QPointF& splinePoint,
        bool freeAngle,
        bool freeLength,
        QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ControlPointSpline)
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
        override;
signals:
    /**
     * @brief ControlPointChangePosition emit when control point change position.
     * @param indexSpline index spline in list.
     * @param position position point in spline.
     * @param pos new posotion.
     */
    void ControlPointChangePosition(
        const qint32& indexSpline, SplinePointPosition position, const QPointF& pos);
    /**
     * @brief showContextMenu emit when need show tool's context menu.
     * @param event context menu event.
     */
    void showContextMenu(QGraphicsSceneContextMenuEvent* event);

public slots:
    void refreshCtrlPoint(
        const qint32& indexSpline,
        SplinePointPosition pos,
        const QPointF& controlPoint,
        const QPointF& splinePoint,
        bool freeAngle = true,
        bool freeLength = true);
    void setEnabledPoint(bool enable);

protected:
    /** @brief controlLine pointer to line control point. */
    VScaledLine* controlLine{ nullptr };

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    Q_DISABLE_COPY(VControlPointSpline)
    /** @brief indexSpline index spline in list.. */
    qint32 indexSpline;

    /** @brief position position point in spline. */
    SplinePointPosition position;

    bool freeAngle{ true };
    bool freeLength{ true };

    void init();
    void setCtrlLine(const QPointF& controlPoint, const QPointF& splinePoint);
};

#endif   // VCONTROLPOINTSPLINE_H
