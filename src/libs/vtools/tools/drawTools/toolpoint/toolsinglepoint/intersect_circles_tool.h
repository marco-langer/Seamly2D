/**************************************************************************
 **
 **  @file   intersect_circles_tool.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   29 5, 2015
 **
 **  @author Douglas S. Caskey
 **  @date   7.16.2022
 **
 **  @copyright
 **  Copyright (C) 2013-2022 Seamly2D project.
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published
 **  by the Free Software Foundation, either version 3 of the License,
 **  or (at your option) any later version.
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

#ifndef INTERSECT_CIRCLES_TOOL_H
#define INTERSECT_CIRCLES_TOOL_H

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"

#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

class VFormula;

class IntersectCirclesTool : public VToolSinglePoint
{
    Q_OBJECT

public:
    void setDialog() override;

    static IntersectCirclesTool* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);

    static IntersectCirclesTool* Create(
        const quint32 _id,
        const QString& pointName,
        quint32 firstCircleCenterId,
        quint32 secondCircleCenterId,
        QString& firstCircleRadius,
        QString& secondCircleRadius,
        CrossCirclesPoint crossPoint,
        qreal mx,
        qreal my,
        bool showPointName,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static QPointF FindPoint(
        const QPointF& c1Point,
        const QPointF& c2Point,
        qreal c1Radius,
        qreal c2Radius,
        const CrossCirclesPoint crossPoint);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::PointOfIntersectionCircles)
    };

    QString FirstCircleCenterPointName() const;
    QString SecondCircleCenterPointName() const;

    quint32 GetFirstCircleCenterId() const;
    void SetFirstCircleCenterId(const quint32& value);

    quint32 GetSecondCircleCenterId() const;
    void SetSecondCircleCenterId(const quint32& value);

    VFormula GetFirstCircleRadius() const;
    void SetFirstCircleRadius(const VFormula& value);

    VFormula GetSecondCircleRadius() const;
    void SetSecondCircleRadius(const VFormula& value);

    CrossCirclesPoint GetCrossCirclesPoint() const;
    void setCirclesCrossPoint(const CrossCirclesPoint& value);

    void ShowVisualization(bool show) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SetVisualization() override;

private:
    Q_DISABLE_COPY(IntersectCirclesTool)

    quint32 firstCircleCenterId;
    quint32 secondCircleCenterId;

    QString firstCircleRadius;
    QString secondCircleRadius;

    CrossCirclesPoint crossPoint;

    IntersectCirclesTool(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32 firstCircleCenterId,
        quint32 secondCircleCenterId,
        const QString& firstCircleRadius,
        const QString& secondCircleRadius,
        CrossCirclesPoint crossPoint,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // INTERSECT_CIRCLES_TOOL_H
