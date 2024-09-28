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
 **  @file   vabstractspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 3, 2014
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

#ifndef VABSTRACTSPLINE_H
#define VABSTRACTSPLINE_H

#include <QDomElement>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QMetaObject>
#include <QObject>
#include <QPainterPath>
#include <QPointF>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../../../visualization/line/visline.h"
#include "../vdrawtool.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vmaingraphicsview.h"

class VControlPointSpline;

class VAbstractSpline
    : public VDrawTool
    , public QGraphicsPathItem
{
    Q_OBJECT
public:
    VAbstractSpline(
        VAbstractPattern* doc, VContainer* data, quint32 id, QGraphicsItem* parent = nullptr);
    virtual ~VAbstractSpline() = default;

    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    int type() const override { return Type; }

    enum
    {
        Type = UserType + static_cast<int>(Tool::AbstractSpline)
    };

    QString getTagName() const override;
    void ShowHandles(bool show);

    QString getLineColor() const;
    void setLineColor(const QString& value);

    QString GetPenStyle() const;
    void SetPenStyle(const QString& value);

    QString getLineWeight() const;
    void setLineWeight(const QString& value);

    QString name() const;

    void GroupVisibility(quint32 object, bool visible) override;

public slots:
    void FullUpdateFromFile() override;
    void Disable(bool disable, const QString& draftBlockName) override;
    void piecesMode(bool mode) override;
    void AllowHover(bool enabled) override;
    void AllowSelecting(bool enabled) override;
    QString makeToolTip() const override;

signals:
    /**
     * @brief setEnabledPoint disable control points.
     * @param enable enable or diasable points.
     */
    void setEnabledPoint(bool enable);

protected:
    /**
     * @brief controlPoints list pointers of control points.
     */
    QVector<VControlPointSpline*> controlPoints;

    SceneObject sceneType;
    bool m_isHovered;
    bool m_piecesMode;

    /**
     * @brief RefreshGeometry  refresh item on scene.
     */
    virtual void RefreshGeometry();

    void ShowTool(quint32 id, bool enable) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void ReadToolAttributes(const QDomElement& domElement) override;
    void SaveOptions(QDomElement& tag, const VGObject* obj) override;
    virtual void refreshCtrlPoints();
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    VSpline CorrectedSpline(
        const VSpline& spline, const SplinePointPosition& position, const QPointF& pos) const;

    template <typename T>
    void ShowToolVisualization(bool show);

    template <typename T>
    static void InitSplineToolConnections(VMainGraphicsScene* scene, T* tool);

    template <typename T>
    static void InitSplinePathToolConnections(VMainGraphicsScene* scene, T* tool);

    template <typename T>
    static void InitArcToolConnections(VMainGraphicsScene* scene, T* tool);

    template <typename T>
    static void InitElArcToolConnections(VMainGraphicsScene* scene, T* tool);

private:
    Q_DISABLE_COPY(VAbstractSpline)

    void InitDefShape();
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void VAbstractSpline::ShowToolVisualization(bool show)
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

    if (m_piecesMode) {
        ShowHandles(m_piecesMode);
    } else {
        ShowHandles(show);
    }

    if (QGraphicsScene* sc =
            scene()) {   // Showing/hiding control points require recalculation scene size.
        VMainGraphicsView::NewSceneRect(sc, qApp->getSceneView(), this);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitSplineToolConnections(VMainGraphicsScene* scene, T* tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplineItemHover, tool, &T::AllowHover);
    QObject::connect(
        scene, &VMainGraphicsScene::EnableSplineItemSelection, tool, &T::AllowSelecting);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitSplinePathToolConnections(VMainGraphicsScene* scene, T* tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableSplinePathItemHover, tool, &T::AllowHover);
    QObject::connect(
        scene, &VMainGraphicsScene::EnableSplinePathItemSelection, tool, &T::AllowSelecting);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitArcToolConnections(VMainGraphicsScene* scene, T* tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemHover, tool, &T::AllowHover);
    QObject::connect(scene, &VMainGraphicsScene::EnableArcItemSelection, tool, &T::AllowSelecting);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VAbstractSpline::InitElArcToolConnections(VMainGraphicsScene* scene, T* tool)
{
    SCASSERT(scene != nullptr)
    SCASSERT(tool != nullptr)

    InitDrawToolConnections(scene, tool);
    QObject::connect(scene, &VMainGraphicsScene::EnableElArcItemHover, tool, &T::AllowHover);
    QObject::connect(
        scene, &VMainGraphicsScene::EnableElArcItemSelection, tool, &T::AllowSelecting);
}

#endif   // VABSTRACTSPLINE_H
