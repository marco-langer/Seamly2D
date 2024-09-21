/******************************************************************************
 *   @file   vtoolpointofintersectionarcs.h
 **  @author Douglas S Caskey
 **  @date   30 Apr, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program to create and model patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
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
 *************************************************************************

 ************************************************************************
 **
 **  @file   vtoolpointofintersectionarcs.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 5, 2015
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

#ifndef VTOOLPOINTOFINTERSECTIONARCS_H
#define VTOOLPOINTOFINTERSECTIONARCS_H

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


class VToolPointOfIntersectionArcs : public VToolSinglePoint
{
    Q_OBJECT

public:
    void setDialog() override;

    static VToolPointOfIntersectionArcs* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolPointOfIntersectionArcs* Create(
        const quint32 _id,
        const QString& pointName,
        const quint32& firstArcId,
        const quint32& secondArcId,
        CrossCirclesPoint pType,
        qreal mx,
        qreal my,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static QPointF FindPoint(const VArc* arc1, const VArc* arc2, const CrossCirclesPoint pType);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::PointOfIntersectionArcs)
    };

    QString FirstArcName() const;
    QString SecondArcName() const;

    quint32 GetFirstArcId() const;
    void SetFirstArcId(const quint32& value);

    quint32 GetSecondArcId() const;
    void SetSecondArcId(const quint32& value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void setCirclesCrossPoint(const CrossCirclesPoint& value);

    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, QSharedPointer<VGObject>& obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;

private:
    Q_DISABLE_COPY(VToolPointOfIntersectionArcs)

    /** @brief firstArcId id first arc. */
    quint32 firstArcId;

    /** @brief secondArcId id second arc. */
    quint32 secondArcId;

    CrossCirclesPoint crossPoint;

    VToolPointOfIntersectionArcs(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32& firstArcId,
        const quint32& secondArcId,
        CrossCirclesPoint pType,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLPOINTOFINTERSECTIONARCS_H
