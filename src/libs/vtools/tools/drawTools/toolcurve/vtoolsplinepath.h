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
 **  @file   vtoolsplinepath.h
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

#ifndef VTOOLSPLINEPATH_H
#define VTOOLSPLINEPATH_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"

class VSplinePath;
template <class T>
class QSharedPointer;

/**
 * @brief The VToolSplinePath class tool for creation spline path.
 */
class VToolSplinePath : public VAbstractSpline
{
    Q_OBJECT
public:
    virtual ~VToolSplinePath() = default;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolSplinePath* Create(
        QSharedPointer<DialogTool> dialog,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data);
    static VToolSplinePath* Create(
        const quint32 _id,
        VSplinePath* path,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);
    static VToolSplinePath* Create(
        const quint32 _id,
        const QVector<quint32>& points,
        QVector<QString>& a1,
        QVector<QString>& a2,
        QVector<QString>& l1,
        QVector<QString>& l2,
        const QString& color,
        const QString& penStyle,
        const QString& lineWeight,
        quint32 duplicate,
        VMainGraphicsScene* scene,
        VAbstractPattern* doc,
        VContainer* data,
        const Document& parse,
        const Source& typeCreation);
    static const QString ToolType;
    static const QString OldToolType;
    static void
    UpdatePathPoints(VAbstractPattern* doc, QDomElement& element, const VSplinePath& path);
    virtual int type() const Q_DECL_OVERRIDE { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::SplinePath)
    };

    VSplinePath getSplinePath() const;
    void setSplinePath(const VSplinePath& splPath);

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;

signals:
    /**
     * @brief RefreshLine refresh control line.
     * @param indexSpline position spline in spline list.
     * @param pos position point in spline.
     * @param controlPoint new position control point.
     * @param splinePoint new position spline point.
     */
    void RefreshLine(
        const qint32& indexSpline,
        SplinePointPosition pos,
        const QPointF& controlPoint,
        const QPointF& splinePoint);
public slots:

    void ControlPointChangePosition(
        const qint32& indexSpline, const SplinePointPosition& position, const QPointF& pos);
    virtual void EnableToolMove(bool move) Q_DECL_OVERRIDE;

protected slots:
    virtual void
    showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id = NULL_ID) Q_DECL_OVERRIDE;

protected:
    virtual void RemoveReferens() Q_DECL_OVERRIDE;
    virtual void SaveDialog(QDomElement& domElement) Q_DECL_OVERRIDE;
    virtual void SaveOptions(QDomElement& tag, QSharedPointer<VGObject>& obj) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) Q_DECL_OVERRIDE;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) Q_DECL_OVERRIDE;
    virtual void SetVisualization() Q_DECL_OVERRIDE;
    virtual void refreshCtrlPoints() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolSplinePath)
    QPointF oldPosition;
    int splIndex;

    VToolSplinePath(
        VAbstractPattern* doc,
        VContainer* data,
        quint32 id,
        const Source& typeCreation,
        QGraphicsItem* parent = nullptr);

    bool IsMovable(int index) const;
    static void
    AddPathPoint(VAbstractPattern* doc, QDomElement& domElement, const VSplinePoint& splPoint);
    void
    UpdateControlPoints(const VSpline& spl, VSplinePath& splPath, const qint32& indexSpline) const;
    void SetSplinePathAttributes(QDomElement& domElement, const VSplinePath& path);
};

#endif   // VTOOLSPLINEPATH_H
