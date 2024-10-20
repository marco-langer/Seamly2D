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
 **  @file   vsimplepoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 6, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
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

#ifndef VSIMPLEPOINT_H
#define VSIMPLEPOINT_H

#include <QColor>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "../vwidgets/vscenepoint.h"
#include "vabstractsimple.h"

class VGraphicsSimpleTextItem;
class VPointF;

class VSimplePoint
    : public VAbstractSimple
    , public VScenePoint
{
    Q_OBJECT
public:
    VSimplePoint(quint32 id, const QColor& currentColor, QObject* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::SimplePoint)
    };

    using VScenePoint::isOnlyPoint;
    using VScenePoint::setOnlyPoint;

    void SetVisualizationMode(bool value);
    bool IsVisualizationMode() const;

    void SetPointHighlight(bool value);

    void SetEnabled(bool enabled);
    void EnableToolMove(bool move);
    void allowTextHover(bool enabled);
    void allowTextSelectable(bool enabled);
    void ToolSelectionType(const SelectionType& type) override;

signals:
    /**
     * @brief Choosed send id when clicked.
     * @param id point id.
     */
    void Choosed(quint32 id);
    void Selected(bool selected, quint32 id);
    void nameChangedPosition(const QPointF& pos, quint32 id);

public slots:
    void deletePoint();
    void pointChosen();
    void pointSelected(bool selected);
    void pointnameChangedPosition(const QPointF& pos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    Q_DISABLE_COPY(VSimplePoint)

    bool m_visualizationMode{ false };
    bool m_alwaysHovered{ false };
};

#endif   // VSIMPLEPOINT_H
