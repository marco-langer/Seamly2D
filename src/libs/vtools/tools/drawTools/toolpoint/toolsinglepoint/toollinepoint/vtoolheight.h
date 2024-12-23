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
 **  @file   vtoolheight.h
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

#ifndef VTOOLHEIGHT_H
#define VTOOLHEIGHT_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoollinepoint.h"


/**
 * @brief The VToolHeight class tool for creation point of height. Help find point of projection
 * onto line.
 */
class VToolHeight : public VToolLinePoint
{
    Q_OBJECT
public:
    void setDialog() override;

    static VToolHeight* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolHeight* Create(
        const quint32 _id,
        const QString& pointName,
        const QString& lineType,
        const QString& lineWeight,
        const QString& lineColor,
        quint32 basePointId,
        quint32 p1LineId,
        quint32 p2LineId,
        qreal mx,
        qreal my,
        bool showPointName,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static QPointF FindPoint(const QLineF& line, const QPointF& point);
    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::Height)
    };

    QString FirstLinePointName() const;
    QString SecondLinePointName() const;

    quint32 GetP1LineId() const;
    void SetP1LineId(const quint32& value);

    quint32 GetP2LineId() const;
    void SetP2LineId(const quint32& value);

    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;
    QString makeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolHeight)

    quint32 p1LineId; /** @brief p1LineId id first point of line.  */
    quint32 p2LineId; /** @brief p2LineId id second point of line. */

    VToolHeight(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const QString& lineType,
        const QString& lineWeight,
        const QString& lineColor,
        const quint32& basePointId,
        const quint32& p1LineId,
        const quint32& p2LineId,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLHEIGHT_H
