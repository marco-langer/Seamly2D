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
 **  @file   vtoolpointofcontact.h
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

#ifndef VTOOLPOINTOFCONTACT_H
#define VTOOLPOINTOFCONTACT_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

class VFormula;

/**
 * @brief The VToolPointOfContact class tool for creation point intersection arc ad line.
 */
class VToolPointOfContact : public VToolSinglePoint
{
    Q_OBJECT
public:
    void setDialog() override;

    static QPointF FindPoint(
        const qreal& radius,
        const QPointF& center,
        const QPointF& firstPoint,
        const QPointF& secondPoint);

    static VToolPointOfContact* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolPointOfContact* Create(
        const quint32 _id,
        QString& radius,
        const quint32& center,
        const quint32& firstPointId,
        const quint32& secondPointId,
        const QString& pointName,
        qreal mx,
        qreal my,
        bool showPointName,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::PointOfContact)
    };

    QString ArcCenterPointName() const;
    QString FirstPointName() const;
    QString SecondPointName() const;

    VFormula getArcRadius() const;
    void setArcRadius(const VFormula& value);

    quint32 getCenter() const;
    void setCenter(const quint32& value);

    quint32 GetFirstPointId() const;
    void SetFirstPointId(const quint32& value);

    quint32 GetSecondPointId() const;
    void SetSecondPointId(const quint32& value);

    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;
    QString makeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolPointOfContact)

    /** @brief radius string with formula radius arc. */
    QString arcRadius;

    /** @brief center id center arc point. */
    quint32 center;

    /** @brief firstPointId id first line point. */
    quint32 firstPointId;

    /** @brief secondPointId id second line point. */
    quint32 secondPointId;

    VToolPointOfContact(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const QString& radius,
        const quint32& center,
        const quint32& firstPointId,
        const quint32& secondPointId,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLPOINTOFCONTACT_H
