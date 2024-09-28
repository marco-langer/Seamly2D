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
 **  @file   vtoolsinglepoint.h
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

#ifndef VTOOLSINGLEPOINT_H
#define VTOOLSINGLEPOINT_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vabstractpoint.h"
#include "../vmisc/def.h"
#include "../vwidgets/vscenepoint.h"


/**
 * @brief The VToolSinglePoint class parent for all tools what create points.
 */
class VToolSinglePoint
    : public VAbstractPoint
    , public VScenePoint
{
    Q_OBJECT
public:
    VToolSinglePoint(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        const QColor& lineColor,
        QGraphicsItem* parent = nullptr);
    virtual ~VToolSinglePoint() = default;

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::SinglePoint)
    };

    QString name() const;
    void setName(const QString& name);

    void SetEnabled(bool enabled);

    void GroupVisibility(quint32 object, bool visible) override;
    bool isPointNameVisible(quint32 id) const override;
    void setPointNameVisiblity(quint32 id, bool visible) override;
    void setPointNamePosition(quint32 id, const QPointF& pos) override;

public slots:
    void pointnameChangedPosition(const QPointF& pos);
    void Disable(bool disable, const QString& draftBlockName) override;
    void EnableToolMove(bool move) override;
    void pointChosen();
    void pointSelected(bool selected);
    void FullUpdateFromFile() override;
    void AllowHover(bool enabled) override;
    void AllowSelecting(bool enabled) override;
    void allowTextHover(bool enabled);
    void allowTextSelectable(bool enabled);
    void ToolSelectionType(const SelectionType& type) override;

protected:
    void updatePointNameVisibility(quint32 id, bool visible) override;
    void updatePointNamePosition(quint32 id, const QPointF& pos) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;

private:
    Q_DISABLE_COPY(VToolSinglePoint)
};

#endif   // VTOOLSINGLEPOINT_H
