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
 **  @file   vnodesplinepath.h
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

#ifndef VNODESPLINEPATH_H
#define VNODESPLINEPATH_H

#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstractnode.h"

/**
 * @brief The VNodeSplinePath class spline path detail node.
 */
class VNodeSplinePath : public VAbstractNode
{
    Q_OBJECT
public:
    static void Create(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 idSpline,
        const Document& parse,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0);

    static const QString ToolType;
    QString getTagName() const override;
public slots:
    void FullUpdateFromFile() override {}
    void AllowHover(bool enabled) override;
    void AllowSelecting(bool enabled) override;

protected:
    void AddToFile() override;
    void ShowNode() override {}
    void HideNode() override {}

private:
    Q_DISABLE_COPY(VNodeSplinePath)

    VNodeSplinePath(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        quint32 idSpline,
        const Source& typeCreation,
        const QString& blockName = QString(),
        const quint32& idTool = 0,
        QObject* qoParent = nullptr);
};

#endif   // VNODESPLINEPATH_H
