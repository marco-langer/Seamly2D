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
 **  @file   vabstractpoint.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 6, 2015
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

#ifndef VABSTRACTPOINT_H
#define VABSTRACTPOINT_H

#include <QColor>
#include <QMetaObject>
#include <QObject>
#include <QPen>
#include <QPointF>
#include <QString>
#include <Qt>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../../../visualization/visualization.h"
#include "../../vabstracttool.h"
#include "../ifc/ifcdef.h"
#include "../vdrawtool.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"

class VAbstractPoint : public VDrawTool
{
    Q_OBJECT
public:
    VAbstractPoint(VAbstractPattern* doc, VContainer* data, quint32 id);
    virtual ~VAbstractPoint() = default;

    QString getTagName() const override;

    template <typename T>
    void ShowToolVisualization(bool show);

public slots:
    void ShowTool(quint32 id, bool enable) override;
    void deletePoint();

protected:
    void SetPointName(quint32 id, const QString& name);
    virtual void updatePointNamePosition(quint32 id, const QPointF& pos) = 0;


    template <typename T>
    static void InitToolConnections(VMainGraphicsScene* scene, T* tool);

private:
    Q_DISABLE_COPY(VAbstractPoint)
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::ShowToolVisualization(bool show)
{
    if (show) {
        if (vis.isNull()) {
            AddVisualization<T>();
            SetVisualization();
        } else {
            if (T* visual = qobject_cast<T*>(vis)) {
                visual->show();
            }
        }
    } else {
        delete vis;
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractPoint::InitToolConnections(VMainGraphicsScene* scene, T* tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnablePointItemHover, tool, &T::AllowHover);
    QObject::connect(
        scene, &VMainGraphicsScene::EnablePointItemSelection, tool, &T::AllowSelecting);
    QObject::connect(scene, &VMainGraphicsScene::enableTextItemHover, tool, &T::allowTextHover);
    QObject::connect(
        scene, &VMainGraphicsScene::enableTextItemSelection, tool, &T::allowTextSelectable);
}

#endif   // VABSTRACTPOINT_H
