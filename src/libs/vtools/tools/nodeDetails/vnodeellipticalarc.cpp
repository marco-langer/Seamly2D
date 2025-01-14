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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 12, 2016
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

#include "vnodeellipticalarc.h"

#include <QDomElement>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vdomdocument.h"
#include "../vabstracttool.h"
#include "../vdatatool.h"
#include "vabstractnode.h"

const QString VNodeEllipticalArc::ToolType = QStringLiteral("modeling");

//---------------------------------------------------------------------------------------------------------------------
void VNodeEllipticalArc::Create(
    VAbstractPattern* doc,
    VContainer* data,
    quint32 id,
    quint32 idArc,
    const Document& parse,
    const Source& typeCreation,
    const QString& blockName,
    const quint32& idTool)
{
    if (parse == Document::FullParse) {
        VAbstractTool::AddRecord(id, Tool::NodeElArc, doc);
        VNodeEllipticalArc* arc =
            new VNodeEllipticalArc(doc, data, id, idArc, typeCreation, blockName, idTool, doc);

        VAbstractPattern::AddTool(id, arc);
        if (idTool != NULL_ID) {
            // Some nodes we don't show on scene. Tool that create this nodes must free memory.
            VDataTool* tool = VAbstractPattern::getTool(idTool);
            SCASSERT(tool != nullptr)
            arc->setParent(tool);   // Adopted by a tool
        } else {
            // Help to delete the node before each FullParse
            doc->AddToolOnRemove(arc);
        }
    } else {
        doc->UpdateToolData(id, data);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VNodeEllipticalArc::getTagName() const { return VAbstractPattern::TagElArc; }

//---------------------------------------------------------------------------------------------------------------------
void VNodeEllipticalArc::AllowHover(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeEllipticalArc::AllowSelecting(bool enabled)
{
    Q_UNUSED(enabled)
    // do nothing
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeEllipticalArc::AddToFile()
{
    QDomElement domElement = doc->createElement(getTagName());

    doc->SetAttribute(domElement, VDomDocument::AttrId, m_id);
    doc->SetAttribute(domElement, AttrType, ToolType);
    doc->SetAttribute(domElement, AttrIdObject, idNode);
    if (idTool != NULL_ID) {
        doc->SetAttribute(domElement, AttrIdTool, idTool);
    }

    AddToModeling(domElement);
}

//---------------------------------------------------------------------------------------------------------------------
VNodeEllipticalArc::VNodeEllipticalArc(
    VAbstractPattern* doc,
    VContainer* data,
    quint32 id,
    quint32 idArc,
    const Source& typeCreation,
    const QString& blockName,
    const quint32& idTool,
    QObject* qoParent)
    : VAbstractNode(doc, data, id, idArc, blockName, idTool, qoParent)
{
    ToolCreation(typeCreation);
}
