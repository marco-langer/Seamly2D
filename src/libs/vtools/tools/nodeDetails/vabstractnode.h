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
 **  @file   vabstractnode.h
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

#ifndef VABSTRACTNODE_H
#define VABSTRACTNODE_H

#include <QColor>
#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vabstracttool.h"

enum class ParentType : bool
{
    Scene,
    Item
};

/**
 * @brief The VAbstractNode class parent class for all detail node.
 */
class VAbstractNode : public VAbstractTool
{
    Q_OBJECT
public:
    VAbstractNode(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32& idNode,
        const QString& blockName = QString(),
        const quint32& idTool = 0,
        QObject* parent = nullptr);
    virtual ~VAbstractNode() = default;
    static const QString AttrIdTool;
    void ShowVisualization(bool show) override;
    void incrementReferens() override;
    void decrementReferens() override;

    ParentType GetParentType() const;
    void SetParentType(const ParentType& value);

    void GroupVisibility(quint32 object, bool visible) override;

    bool IsExluded() const;
    void SetExluded(bool exluded);

protected:
    ParentType parentType;

    /** @brief idNodenode id. */
    quint32 idNode;

    /** @brief idTool id tool. */
    quint32 idTool;

    QString m_blockName;

    bool m_exluded;

    void AddToModeling(const QDomElement& domElement);
    void ToolCreation(const Source& typeCreation) override;
    void SetVisualization() override {}

    virtual void ShowNode() = 0;
    virtual void HideNode() = 0;

private:
    Q_DISABLE_COPY(VAbstractNode)
};

#endif   // VABSTRACTNODE_H
