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
 **  @file   vtooldoublepoint.h
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

#ifndef VTOOLDOUBLEPOINT_H
#define VTOOLDOUBLEPOINT_H

#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QVariant>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vabstractpoint.h"
#include "../vmisc/def.h"

class VSimplePoint;

class VToolDoublePoint
    : public VAbstractPoint
    , public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDoublePoint(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 p1id,
        quint32 p2id,
        QGraphicsItem* parent = nullptr);
    virtual ~VToolDoublePoint() = default;

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::DoublePoint)
    };

    QString nameP1() const;
    void setNameP1(const QString& name);

    QString nameP2() const;
    void setNameP2(const QString& name);

    void GroupVisibility(quint32 object, bool visible) override;
    void setPointNamePosition(quint32 id, const QPointF& pos) override;
    bool isPointNameVisible(quint32 id) const override;
    void setPointNameVisiblity(quint32 id, bool visible) override;

public slots:
    void changePointName1Position(const QPointF& pos);
    void changePointName2Position(const QPointF& pos);
    void Disable(bool disable, const QString& draftBlockName) override;
    void EnableToolMove(bool move) override;
    void point1Chosen();
    void point2Chosen();
    void point1Selected(bool selected);
    void point2Selected(bool selected);
    void FullUpdateFromFile() override;
    void AllowHover(bool enabled) override;
    void AllowSelecting(bool enabled) override;
    void allowTextHover(bool enabled);
    void allowTextSelectable(bool enabled);
    void ToolSelectionType(const SelectionType& type) override;

protected:
    VSimplePoint* firstPoint;
    VSimplePoint* secondPoint;

    quint32 p1id;
    quint32 p2id;

    void updatePointNamePosition(quint32 id, const QPointF& pos) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void SaveOptions(QDomElement& tag, QSharedPointer<VGObject>& obj) override;
    void AddToFile() override;
    void updatePointNameVisibility(quint32 id, bool visible) override;
    QString complexToolTip(quint32 itemId) const;

private:
    Q_DISABLE_COPY(VToolDoublePoint)
};

#endif   // VTOOLDOUBLEPOINT_H
