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
 **  @file   vtooltruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
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

#ifndef VTOOLTRUEDARTS_H
#define VTOOLTRUEDARTS_H

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
#include "vtooldoublepoint.h"


class VToolTrueDarts : public VToolDoublePoint
{
    Q_OBJECT
public:
    static void FindPoint(
        const QPointF& baseLineP1,
        const QPointF& baseLineP2,
        const QPointF& dartP1,
        const QPointF& dartP2,
        const QPointF& dartP3,
        QPointF& p1,
        QPointF& p2);
    void setDialog() override;

    static VToolTrueDarts* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolTrueDarts* Create(
        quint32 _id,
        const quint32& _p1id,
        const quint32& _p2id,
        const quint32& baseLineP1Id,
        const quint32& baseLineP2Id,
        const quint32& dartP1Id,
        const quint32& dartP2Id,
        const quint32& dartP3Id,
        const QString& point1Name,
        qreal mx1,
        qreal my1,
        bool showPointName1,
        const QString& point2Name,
        qreal mx2,
        qreal my2,
        bool showPointName2,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::TrueDarts)
    };

    void ShowVisualization(bool show) override;

    QString BaseLineP1Name() const;
    QString BaseLineP2Name() const;
    QString DartP1Name() const;
    QString DartP2Name() const;
    QString DartP3Name() const;

    quint32 GetBaseLineP1Id() const;
    void SetBaseLineP1Id(const quint32& value);

    quint32 GetBaseLineP2Id() const;
    void SetBaseLineP2Id(const quint32& value);

    quint32 GetDartP1Id() const;
    void SetDartP1Id(const quint32& value);

    quint32 GetDartP2Id() const;
    void SetDartP2Id(const quint32& value);

    quint32 GetDartP3Id() const;
    void SetDartP3Id(const quint32& value);

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;

private:
    Q_DISABLE_COPY(VToolTrueDarts)
    quint32 baseLineP1Id;
    quint32 baseLineP2Id;
    quint32 dartP1Id;
    quint32 dartP2Id;
    quint32 dartP3Id;

    VToolTrueDarts(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32& p1id,
        const quint32& p2id,
        const quint32& baseLineP1Id,
        const quint32& baseLineP2Id,
        const quint32& dartP1Id,
        const quint32& dartP2Id,
        const quint32& dartP3Id,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLTRUEDARTS_H
