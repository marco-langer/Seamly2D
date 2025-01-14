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
 **  @file   vtoolpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 1, 2016
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

#ifndef VTOOLPOINTOFINTERSECTIONCURVES_H
#define VTOOLPOINTOFINTERSECTIONCURVES_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vtoolsinglepoint.h"


class VToolPointOfIntersectionCurves : public VToolSinglePoint
{
    Q_OBJECT
public:
    void setDialog() override;

    static VToolPointOfIntersectionCurves* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolPointOfIntersectionCurves* Create(
        const quint32 _id,
        const QString& pointName,
        quint32 firstCurveId,
        quint32 secondCurveId,
        VCrossCurvesPoint vCrossPoint,
        HCrossCurvesPoint hCrossPoint,
        qreal mx,
        qreal my,
        bool showPointName,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);

    static QPointF FindPoint(
        const QVector<QPointF>& curve1Points,
        const QVector<QPointF>& curve2Points,
        VCrossCurvesPoint vCrossPoint,
        HCrossCurvesPoint hCrossPoint);

    static const QString ToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::PointOfIntersectionCurves)
    };

    QString FirstCurveName() const;
    QString SecondCurveName() const;

    quint32 GetFirstCurveId() const;
    void SetFirstCurveId(const quint32& value);

    quint32 GetSecondCurveId() const;
    void SetSecondCurveId(const quint32& value);

    VCrossCurvesPoint GetVCrossPoint() const;
    void SetVCrossPoint(const VCrossCurvesPoint& value);

    HCrossCurvesPoint GetHCrossPoint() const;
    void SetHCrossPoint(const HCrossCurvesPoint& value);

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
    Q_DISABLE_COPY(VToolPointOfIntersectionCurves)

    quint32 firstCurveId;
    quint32 secondCurveId;

    VCrossCurvesPoint vCrossPoint;
    HCrossCurvesPoint hCrossPoint;

    explicit VToolPointOfIntersectionCurves(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const quint32 firstCurveId,
        quint32 secondCurveId,
        VCrossCurvesPoint vCrossPoint,
        HCrossCurvesPoint hCrossPoint,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);
};

#endif   // VTOOLPOINTOFINTERSECTIONCURVES_H
