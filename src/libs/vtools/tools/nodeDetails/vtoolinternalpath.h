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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   24 11, 2016
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

#ifndef VTOOLINTERNALPATH_H
#define VTOOLINTERNALPATH_H

#include <QtGlobal>

#include "vabstractnode.h"

class DialogTool;

class VToolInternalPath
    : public VAbstractNode
    , public QGraphicsPathItem
{
    Q_OBJECT
public:
    static VToolInternalPath* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolInternalPath* Create(
        quint32 _id,
        const VPiecePath& path,
        quint32 pieceId,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0);

    virtual int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::InternalPath)
    };
    virtual QString getTagName() const override;

    virtual void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr) override;

    virtual void incrementReferens() override;
    virtual void decrementReferens() override;

    static void AddAttributes(
        VAbstractPattern* doc, QDomElement& domElement, quint32 id, const VPiecePath& path);
public slots:
    virtual void FullUpdateFromFile() override;
    virtual void AllowHover(bool enabled) override;
    virtual void AllowSelecting(bool enabled) override;

protected:
    virtual void AddToFile() override;
    virtual void ShowNode() override;
    virtual void HideNode() override;
    virtual void ToolCreation(const Source& typeCreation) override;

private:
    Q_DISABLE_COPY(VToolInternalPath)

    quint32 m_pieceId;

    VToolInternalPath(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 pieceId,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0,
        QObject* qoParent = nullptr,
        QGraphicsItem* parent = nullptr);

    void RefreshGeometry();

    void IncrementNodes(const VPiecePath& path) const;
    void DecrementNodes(const VPiecePath& path) const;
};

#endif   // VTOOLINTERNALPATH_H
