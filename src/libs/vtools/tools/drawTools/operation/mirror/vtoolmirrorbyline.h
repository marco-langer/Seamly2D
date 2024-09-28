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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Seamly2D project
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

#ifndef VTOOLMIRRORBYLINE_H
#define VTOOLMIRRORBYLINE_H

#include <QSharedPointer>
#include <QtGlobal>

#include "vabstractmirror.h"

class VToolMirrorByLine : public VAbstractMirror
{
    Q_OBJECT

public:
    virtual ~VToolMirrorByLine() = default;
    void setDialog() override;
    static VToolMirrorByLine* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolMirrorByLine* Create(
        const quint32 _id,
        quint32 firstLinePointId,
        quint32 secondLinePointId,
        const QString& suffix,
        const QVector<SourceItem>& source,
        const QVector<DestinationItem>& destination,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static const QString ToolType;

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::MirrorByLine)
    };

    QString firstLinePointName() const;
    QString secondLinePointName() const;

    quint32 getFirstLinePointId() const;
    void setFirstLinePointId(const quint32& value);

    quint32 getSecondLinePointId() const;
    void setSecondLinePointId(const quint32& value);

    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void SetVisualization() override;
    void SaveDialog(QDomElement& domElement) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    QString makeToolTip() const override;

private:
    Q_DISABLE_COPY(VToolMirrorByLine)

    quint32 m_firstLinePointId;
    quint32 m_secondLinePointId;

    VToolMirrorByLine(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 firstLinePointId,
        quint32 secondLinePointId,
        const QString& suffix,
        const QVector<SourceItem>& source,
        const QVector<DestinationItem>& destination,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLMIRRORBYLINE_H
