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
 **  @file   vtoollineintersect.h
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

#ifndef VTOOLLINEINTERSECT_H
#define VTOOLLINEINTERSECT_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"


/**
 * @brief The VToolLineIntersect class help find point intersection lines.
 */
class VToolLineIntersect : public VToolSinglePoint
{
    Q_OBJECT
public:
    void setDialog() override;

    static VToolLineIntersect* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolLineIntersect* Create(
        const quint32 _id,
        const quint32& p1Line1Id,
        const quint32& p2Line1Id,
        const quint32& p1Line2Id,
        const quint32& p2Line2Id,
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
        Type = UserType + static_cast<int>(Tool::LineIntersect)
    };

    QString Line1P1Name() const;
    QString Line1P2Name() const;
    QString Line2P1Name() const;
    QString Line2P2Name() const;

    quint32 GetP1Line1() const;
    void SetP1Line1(const quint32& value);

    quint32 GetP2Line1() const;
    void SetP2Line1(const quint32& value);

    quint32 GetP1Line2() const;
    void SetP1Line2(const quint32& value);

    quint32 GetP2Line2() const;
    void SetP2Line2(const quint32& value);

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
    Q_DISABLE_COPY(VToolLineIntersect)

    /** @brief p1Line1 id first point first line. */
    quint32 p1Line1;

    /** @brief p2Line1 id second point first line. */
    quint32 p2Line1;

    /** @brief p1Line2 id first point second line. */
    quint32 p1Line2;

    /** @brief p2Line2 id second point second line.*/
    quint32 p2Line2;

    VToolLineIntersect(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32& p1Line1,
        const quint32& p2Line1,
        const quint32& p1Line2,
        const quint32& p2Line2,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLLINEINTERSECT_H
