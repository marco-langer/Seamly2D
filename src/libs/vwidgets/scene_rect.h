/***************************************************************************
 *                                                                         *
 *   @file   scene_rect.cpp                                                *
 *   @author DSCaskey                                                      *
 *   @date   3.30.2021                                                     *
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
 **************************************************************************/

#ifndef SCENE_RECT_H
#define SCENE_RECT_H

#include <QGraphicsRectItem>
#include <QtGlobal>

#include "../vmisc/def.h"

class VPointF;
class VScaledLine;

class SceneRect : public QGraphicsRectItem
{
public:
    explicit SceneRect(const QColor& lineColor, QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ScenePoint)
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
        override;
    void refreshPointGeometry(const VPointF& point);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void setOnlyPoint(bool value);
    bool isOnlyPoint() const;

    void setPointColor(const QString& value);

private:
    Q_DISABLE_COPY(SceneRect)

    void setRectPen(qreal scale);

    QColor m_rectColor; /** @brief m_rectColor color of point. */
    bool m_onlyPoint{ false };
    bool m_isHovered{ false };
    bool m_showPointName{ true };
};

#endif   // SCENE_RECT_H
