/***************************************************************************
 **  @file   vtoolcubicbezier.cpp
 **  @author Douglas S Caskey
 **  @date   17 Sep, 2023
 **
 **  @copyright
 **  Copyright (C) 2017 - 2023 Seamly, LLC
 **  https://github.com/fashionfreedom/seamly2d
 **
 **  @brief
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
 **  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************

 ************************************************************************
 **  @file   vtoolcubicbezier.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   10 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vtoolcubicbezier.h"

#include <QDomElement>
#include <QPen>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <Qt>

#include "../../../dialogs/tools/dialogcubicbezier.h"
#include "../../../dialogs/tools/dialogtool.h"
#include "../../../visualization/path/vistoolcubicbezier.h"
#include "../../../visualization/visualization.h"
#include "../../vabstracttool.h"
#include "../ifc/exception/vexception.h"
#include "../vdrawtool.h"
#include "../vgeometry/../ifc/ifcdef.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vcubicbezier.h"
#include "../vgeometry/vgobject.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "vabstractspline.h"

const QString VToolCubicBezier::ToolType = QStringLiteral("cubicBezier");

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier::VToolCubicBezier(
    VAbstractPattern* doc,
    VContainer* data,
    quint32 id,
    const Source& typeCreation,
    QGraphicsItem* parent)
    : VAbstractSpline(doc, data, id, parent)
{
    sceneType = SceneObject::Spline;

    this->setFlag(QGraphicsItem::ItemIsFocusable, true);   // For keyboard input focus

    ToolCreation(typeCreation);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::setDialog()
{
    SCASSERT(not m_dialog.isNull())
    auto dialogTool = qobject_cast<DialogCubicBezier*>(m_dialog);
    SCASSERT(dialogTool != nullptr)
    const auto& spl{ *VAbstractTool::data.GeometricObject<VCubicBezier>(m_id) };
    dialogTool->SetSpline(spl);
    dialogTool->setLineColor(spl.getLineColor());
    dialogTool->setPenStyle(spl.GetPenStyle());
    dialogTool->setLineWeight(spl.getLineWeight());
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier* VToolCubicBezier::Create(
    QSharedPointer<DialogTool> dialog,
    VMainGraphicsScene* scene,
    VAbstractPattern* doc,
    VContainer* data)
{
    SCASSERT(not dialog.isNull())
    QSharedPointer<DialogCubicBezier> dialogTool = dialog.objectCast<DialogCubicBezier>();
    SCASSERT(not dialogTool.isNull())

    auto spline{ std::make_unique<VCubicBezier>(dialogTool->GetSpline()) };
    spline->setLineColor(dialogTool->getLineColor());
    spline->SetPenStyle(dialogTool->getPenStyle());
    spline->setLineWeight(dialogTool->getLineWeight());

    auto spl = Create(0, std::move(spline), scene, doc, data, Document::FullParse, Source::FromGui);

    if (spl != nullptr) {
        spl->m_dialog = dialogTool;
    }
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
VToolCubicBezier* VToolCubicBezier::Create(
    const quint32 _id,
    std::unique_ptr<VCubicBezier> spline,
    VMainGraphicsScene* scene,
    VAbstractPattern* doc,
    VContainer* data,
    const Document& parse,
    const Source& typeCreation)
{
    quint32 id = _id;
    VCubicBezier* splineObserver{ spline.get() };

    if (typeCreation == Source::FromGui) {
        id = data->AddGObject(std::move(spline));
        data->AddSpline(*data->GeometricObject<VAbstractBezier>(id), id);
    } else {
        data->UpdateGObject(id, std::move(spline));
        data->AddSpline(*data->GeometricObject<VAbstractBezier>(id), id);
        if (parse != Document::FullParse) {
            doc->UpdateToolData(id, data);
        }
    }

    if (parse == Document::FullParse) {
        VDrawTool::AddRecord(id, Tool::CubicBezier, doc);
        auto _spl = new VToolCubicBezier(doc, data, id, typeCreation);
        scene->addItem(_spl);
        InitSplineToolConnections(scene, _spl);
        VAbstractPattern::AddTool(id, _spl);
        doc->IncrementReferens(splineObserver->GetP1().getIdTool());
        doc->IncrementReferens(splineObserver->GetP1().getIdTool());
        doc->IncrementReferens(splineObserver->GetP1().getIdTool());
        doc->IncrementReferens(splineObserver->GetP4().getIdTool());
        return _spl;
    }
    return nullptr;
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCubicBezier::FirstPointName() const
{
    return VAbstractTool::data.GeometricObject<VCubicBezier>(m_id)->GetP1().name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCubicBezier::SecondPointName() const
{
    return VAbstractTool::data.GeometricObject<VCubicBezier>(m_id)->GetP2().name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCubicBezier::ThirdPointName() const
{
    return VAbstractTool::data.GeometricObject<VCubicBezier>(m_id)->GetP3().name();
}

//---------------------------------------------------------------------------------------------------------------------
QString VToolCubicBezier::ForthPointName() const
{
    return VAbstractTool::data.GeometricObject<VCubicBezier>(m_id)->GetP4().name();
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezier VToolCubicBezier::getSpline() const
{
    return *VAbstractTool::data.GeometricObject<VCubicBezier>(m_id);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::setSpline(const VCubicBezier& spl)
{
    auto& spline{ *VAbstractTool::data.GeometricObject<VCubicBezier>(m_id) };
    spline = spl;
    SaveOption(&spline);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::ShowVisualization(bool show)
{
    ShowToolVisualization<VisToolCubicBezier>(show);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::showContextMenu(QGraphicsSceneContextMenuEvent* event, quint32 id)
{
    Q_UNUSED(id)

    try {
        ContextMenu<DialogCubicBezier>(event);
    } catch (const VExceptionToolWasDeleted& error) {
        Q_UNUSED(error)
        return;   // Leave this method immediately!!!
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::RemoveReferens()
{
    const auto& spl{ *VAbstractTool::data.GeometricObject<VCubicBezier>(m_id) };
    doc->DecrementReferens(spl.GetP1().getIdTool());
    doc->DecrementReferens(spl.GetP2().getIdTool());
    doc->DecrementReferens(spl.GetP3().getIdTool());
    doc->DecrementReferens(spl.GetP4().getIdTool());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SaveDialog(QDomElement& domElement)
{
    SCASSERT(not m_dialog.isNull())
    auto dialogTool = qobject_cast<DialogCubicBezier*>(m_dialog);
    SCASSERT(dialogTool != nullptr)

    const VCubicBezier spl = dialogTool->GetSpline();

    SetSplineAttributes(domElement, spl);
    doc->SetAttribute(domElement, AttrColor, dialogTool->getLineColor());
    doc->SetAttribute(domElement, AttrLineWeight, dialogTool->getLineWeight());
    doc->SetAttribute(domElement, AttrPenStyle, dialogTool->getPenStyle());
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SaveOptions(QDomElement& tag, const VGObject* obj)
{
    VAbstractSpline::SaveOptions(tag, obj);

    const auto* spl{ dynamic_cast<const VCubicBezier*>(obj) };
    SCASSERT(spl)
    SetSplineAttributes(tag, *spl);
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SetVisualization()
{
    if (not vis.isNull()) {
        auto visual = qobject_cast<VisToolCubicBezier*>(vis);
        SCASSERT(visual != nullptr)

        const auto& spl{ *VAbstractTool::data.GeometricObject<VCubicBezier>(m_id) };
        visual->setObject1Id(spl.GetP1().id());
        visual->setObject2Id(spl.GetP2().id());
        visual->setObject3Id(spl.GetP3().id());
        visual->setObject4Id(spl.GetP4().id());
        visual->setLineStyle(lineTypeToPenStyle(spl.GetPenStyle()));
        visual->setLineWeight(spl.getLineWeight());
        visual->SetMode(Mode::Show);
        visual->RefreshGeometry();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::RefreshGeometry()
{
    this->setPath(VAbstractTool::data.GeometricObject<VCubicBezier>(m_id)->GetPath());

    SetVisualization();
}

//---------------------------------------------------------------------------------------------------------------------
void VToolCubicBezier::SetSplineAttributes(QDomElement& domElement, const VCubicBezier& spl)
{
    SCASSERT(doc != nullptr)

    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrPoint1, spl.GetP1().id());
    doc->SetAttribute(domElement, AttrPoint2, spl.GetP2().id());
    doc->SetAttribute(domElement, AttrPoint3, spl.GetP3().id());
    doc->SetAttribute(domElement, AttrPoint4, spl.GetP4().id());

    if (spl.GetDuplicate() > 0) {
        doc->SetAttribute(domElement, AttrDuplicate, spl.GetDuplicate());
    } else {
        if (domElement.hasAttribute(AttrDuplicate)) {
            domElement.removeAttribute(AttrDuplicate);
        }
    }
}
