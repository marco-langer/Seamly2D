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
 **  @file   vnodepoint.h
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

#ifndef VNODEPOINT_H
#define VNODEPOINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "../vwidgets/vscenepoint.h"
#include "vabstractnode.h"

/**
 * @brief The VNodePoint class point detail node.
 */
class VNodePoint
    : public VAbstractNode
    , public VScenePoint
{
    Q_OBJECT
public:
    static void Create(
        VAbstractPattern* doc,
        VContainer* data,
        VMainGraphicsScene* scene,
        quint32 id,
        quint32 idPoint,
        const Document& parse,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::NodePoint)
    };
    QString getTagName() const override;

    void setPointNamePosition(quint32 id, const QPointF& pos) override;
    void setPointNameVisiblity(quint32 id, bool visible) override;

signals:
    /**
     * @brief showContextMenu emit when need show tool context menu.
     * @param event context menu event.
     */
    // void ShowOptions();
    // void ToggleInLayout(bool checked);
    // void Delete();

public slots:
    void FullUpdateFromFile() override;
    void nameChangedPosition(const QPointF& pos);
    void pointChosen();
    void EnableToolMove(bool move);
    void AllowHover(bool enabled) override;
    void AllowSelecting(bool enabled) override;
    void allowTextHover(bool enabled);
    void allowTextSelectable(bool enabled);

protected:
    void AddToFile() override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void ShowNode() override;
    void HideNode() override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    Q_DISABLE_COPY(VNodePoint)

    VNodePoint(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 idPoint,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0,
        QObject* qoParent = nullptr,
        QGraphicsItem* parent = nullptr);
};

#endif   // VNODEPOINT_H
