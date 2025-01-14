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
 **  @file   vcontrolpointspline.cpp
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

#include "vcontrolpointspline.h"

#include <QBrush>
#include <QEvent>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QLineF>
#include <QList>
#include <QMessageLogger>
#include <QPen>
#include <QPoint>
#include <QPolygonF>
#include <QRectF>
#include <Qt>

#include "../vgeometry/vgobject.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/global.h"
#include "scalesceneitems.h"
#include "vgraphicssimpletextitem.h"
#include "vmaingraphicsscene.h"
#include "vmaingraphicsview.h"

//---------------------------------------------------------------------------------------------------------------------
VControlPointSpline::VControlPointSpline(
    const qint32& indexSpline, SplinePointPosition position, QGraphicsItem* parent)
    : SceneRect(QColor(Qt::red), parent)
    , indexSpline(indexSpline)
    , position(position)
{
    init();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VControlPointSpline constructor.
 * @param indexSpline index spline in list.
 * @param position position point in spline.
 * @param controlPoint control point.
 * @param splinePoint spline point.
 * @param parent parent object.
 */
VControlPointSpline::VControlPointSpline(
    const qint32& indexSpline,
    SplinePointPosition position,
    const QPointF& controlPoint,
    const QPointF& splinePoint,
    bool freeAngle,
    bool freeLength,
    QGraphicsItem* parent)
    : SceneRect(QColor(Qt::red), parent)
    , indexSpline(indexSpline)
    , position(position)
    , freeAngle(freeAngle)
    , freeLength(freeLength)
{
    init();

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
    this->setPos(controlPoint);

    setCtrlLine(controlPoint, splinePoint);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::paint(
    QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen lPen = controlLine->pen();
    lPen.setColor(correctColor(controlLine, Qt::black));
    controlLine->setPen(lPen);

    SceneRect::paint(painter, option, widget);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief hoverEnterEvent handle hover enter events.
 * @param event hover move event.
 */
void VControlPointSpline::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    if (freeAngle || freeLength) {
        SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
    }
    SceneRect::hoverEnterEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    if (freeAngle || freeLength) {
        setCursor(QCursor());
    }
    SceneRect::hoverLeaveEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief itemChange handle item change.
 * @param change change.
 * @param value value.
 * @return value.
 */
QVariant
VControlPointSpline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    switch (change) {
    case ItemPositionChange: {
        if (not freeAngle || not freeLength) {
            const QPointF splPoint = controlLine->line().p1() + pos();

            QLineF newLine(splPoint, value.toPointF());   // value - new position.
            QLineF oldLine(splPoint, pos());              // pos() - old position.

            if (not freeAngle) {
                newLine.setAngle(oldLine.angle());
            }

            if (not freeLength) {
                newLine.setLength(oldLine.length());
            }

            return newLine.p2();
        }

        break;
    }
    case ItemPositionHasChanged: {
        // Each time we move something we call recalculation scene rect. In some cases this can
        // cause moving objects positions. And this cause infinite redrawing. That's why we wait the
        // finish of saving the last move.
        static bool changeFinished = true;
        if (changeFinished) {
            changeFinished = false;
            // value - new position.
            emit ControlPointChangePosition(indexSpline, position, value.toPointF());
            if (scene()) {
                const QList<QGraphicsView*> viewList = scene()->views();
                if (not viewList.isEmpty()) {
                    if (QGraphicsView* view = viewList.at(0)) {
                        const int xmargin = 50;
                        const int ymargin = 50;

                        const QRectF viewRect = VMainGraphicsView::SceneVisibleArea(view);
                        const QRectF itemRect = mapToScene(boundingRect()).boundingRect();

                        // If item's rect is bigger than view's rect ensureVisible works very
                        // unstable.
                        if (itemRect.height() + 2 * ymargin < viewRect.height()
                            && itemRect.width() + 2 * xmargin < viewRect.width()) {
                            view->ensureVisible(itemRect, xmargin, ymargin);
                        } else {
                            // Ensure visible only small rect around a cursor
                            VMainGraphicsScene* currentScene =
                                qobject_cast<VMainGraphicsScene*>(scene());
                            SCASSERT(currentScene)
                            const QPointF cursorPosition = currentScene->getScenePos();
                            view->ensureVisible(
                                QRectF(cursorPosition.x() - 5, cursorPosition.y() - 5, 10, 10));
                        }
                    }
                }
            }
            changeFinished = true;
        }
        break;
    }
    default: break;
    }
    return SceneRect::itemChange(change, value);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton
        && event->type() != QEvent::GraphicsSceneMouseDoubleClick) {
        if (freeAngle || freeLength) {
            SetItemOverrideCursor(this, cursorArrowCloseHand, 1, 1);
        }
    }
    SceneRect::mousePressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::LeftButton
        && event->type() != QEvent::GraphicsSceneMouseDoubleClick) {
        if (freeAngle || freeLength) {
            SetItemOverrideCursor(this, cursorArrowOpenHand, 1, 1);
        }
    }
    SceneRect::mouseReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    emit showContextMenu(event);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::init()
{
    setOnlyPoint(true);
    this->setBrush(QBrush(Qt::NoBrush));
    this->setZValue(100);

    controlLine = new VScaledLine(this);
    controlLine->setBasicWidth(widthHairLine);
    controlLine->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
}

//---------------------------------------------------------------------------------------------------------------------
void VControlPointSpline::setCtrlLine(const QPointF& controlPoint, const QPointF& splinePoint)
{
    QPointF p1, p2;
    VGObject::LineIntersectCircle(
        QPointF(),
        scaledRadius(sceneScale(scene())),
        QLineF(QPointF(), splinePoint - controlPoint),
        p1,
        p2);
    controlLine->setLine(QLineF(splinePoint - controlPoint, p1));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief refreshCtrlPoint refresh the control point.
 * @param indexSpline index spline in list.
 * @param pos position point in spline.
 * @param controlPoint control point.
 * @param splinePoint spline point.
 */
void VControlPointSpline::refreshCtrlPoint(
    const qint32& indexSpline,
    SplinePointPosition pos,
    const QPointF& controlPoint,
    const QPointF& splinePoint,
    bool freeAngle,
    bool freeLength)
{
    if (this->indexSpline == indexSpline && this->position == pos) {
        this->freeAngle = freeAngle;
        this->freeLength = freeLength;
        this->setPos(controlPoint);
        setCtrlLine(controlPoint, splinePoint);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setEnabledPoint disable or enable control point.
 * @param enable true - enable.
 */
void VControlPointSpline::setEnabledPoint(bool enable)
{
    setEnabled(enable);
    controlLine->setEnabled(enable);

    setFlag(QGraphicsItem::ItemIsSelectable, enable);
    setFlag(QGraphicsItem::ItemIsMovable, enable);
    setAcceptHoverEvents(enable);
}
