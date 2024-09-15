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
 **  @file   vtoolspline.h
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

#ifndef VTOOLSPLINE_H
#define VTOOLSPLINE_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QSharedPointer>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"


/**
 * @brief The VToolSpline class tool for creation spline. I mean bezier curve.
 */
class VToolSpline : public VAbstractSpline
{
    Q_OBJECT
public:
    virtual ~VToolSpline() = default;
    void setDialog() override;
    static VToolSpline* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolSpline* Create(
        const quint32 _id,
        VSpline* spline,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);
    static VToolSpline* Create(
        const quint32 _id,
        quint32 point1,
        quint32 point4,
        QString& a1,
        QString& a2,
        QString& l1,
        QString& l2,
        quint32 duplicate,
        const QString& color,
        const QString& penStyle,
        const QString& lineWeight,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);
    static const QString ToolType;
    static const QString OldToolType;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::Spline)
    };

    VSpline getSpline() const;
    void setSpline(const VSpline& spl);

    void ShowVisualization(bool show) override;

public slots:
    void ControlPointChangePosition(
        const qint32& indexSpline, const SplinePointPosition& position, const QPointF& pos);
    void EnableToolMove(bool move) override;

protected slots:
    void showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) override;

protected:
    void RemoveReferens() override;
    void SaveDialog(QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, QSharedPointer<VGObject>& obj) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void SetVisualization() override;
    void refreshCtrlPoints() override;

private:
    Q_DISABLE_COPY(VToolSpline)
    QPointF oldPosition;

    VToolSpline(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);

    bool IsMovable() const;
    void SetSplineAttributes(QDomElement& domElement, const VSpline& spl);
};

#endif   // VTOOLSPLINE_H
