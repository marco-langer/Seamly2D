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
 **  @file   vtoolcutarc.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 1, 2014
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

#ifndef VTOOLCUTARC_H
#define VTOOLCUTARC_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolcut.h"

template <class T>
class QSharedPointer;

/**
 * @brief The VToolCutArc class tool for cutting arc.
 */
class VToolCutArc : public VToolCut
{
    Q_OBJECT
public:
    void setDialog() override;

    static VToolCutArc* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolCutArc* Create(
        const quint32 _id,
        const QString& pointName,
        QString& formula,
        quint32 arcId,
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
        Type = UserType + static_cast<int>(Tool::CutArc)
    };
    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, QSharedPointer<VGObject>& obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;
    QString makeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolCutArc)

    VToolCutArc(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const QString& formula,
        const quint32& arcId,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};
#endif   // VTOOLCUTARC_H
