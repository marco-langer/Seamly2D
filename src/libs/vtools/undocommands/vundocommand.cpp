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
 **  @file   vundocommand.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 7, 2014
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

#include "vundocommand.h"

#include <QApplication>
#include <QDomNode>

#include "../ifc/ifcdef.h"
#include "../tools/drawTools/operation/vabstractoperation.h"
#include "../vmisc/customevents.h"
#include "../vmisc/def.h"
#include "../vpatterndb/vnodedetail.h"
#include "../vpatterndb/vpiecenode.h"

Q_LOGGING_CATEGORY(vUndo, "v.undo")

//---------------------------------------------------------------------------------------------------------------------
VUndoCommand::VUndoCommand(const QDomElement& xml, VAbstractPattern* doc, QUndoCommand* parent)
    : QObject()
    , QUndoCommand(parent)
    , xml(xml)
    , doc(doc)
    , nodeId(NULL_ID)
    , redoFlag(false)
{
    SCASSERT(doc != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::RedoFullParsing()
{
    if (redoFlag) {
        emit NeedFullParsing();
    } else {
        QApplication::postEvent(doc, new LiteParseEvent());
    }
    redoFlag = true;
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::UndoDeleteAfterSibling(QDomNode& parentNode, const quint32& siblingId) const
{
    if (siblingId == NULL_ID) {
        parentNode.appendChild(xml);
    } else {
        const QDomElement refElement = doc->NodeById(siblingId);
        parentNode.insertAfter(xml, refElement);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::IncrementReferences(const QVector<quint32>& nodes) const
{
    for (quint32 nodeId : nodes) {
        doc->IncrementReferens(nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::DecrementReferences(const QVector<quint32>& nodes) const
{
    for (quint32 nodeId : nodes) {
        doc->DecrementReferens(nodeId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::IncrementReferences(const QVector<CustomSARecord>& nodes) const
{
    QVector<quint32> n;
    n.reserve(nodes.size());

    for (const CustomSARecord& node : nodes) {
        n.append(node.path);
    }

    IncrementReferences(n);
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::DecrementReferences(const QVector<CustomSARecord>& nodes) const
{
    QVector<quint32> n;
    n.reserve(nodes.size());

    for (const CustomSARecord& node : nodes) {
        n.append(node.path);
    }

    DecrementReferences(n);
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::IncrementReferences(const QVector<VPieceNode>& nodes) const
{
    QVector<quint32> n;
    n.reserve(nodes.size());

    for (const VPieceNode& node : nodes) {
        n.append(node.GetId());
    }

    IncrementReferences(n);
}

//---------------------------------------------------------------------------------------------------------------------
void VUndoCommand::DecrementReferences(const QVector<VPieceNode>& nodes) const
{
    QVector<quint32> n;
    n.reserve(nodes.size());

    for (const VPieceNode& node : nodes) {
        n.append(node.GetId());
    }

    DecrementReferences(n);
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VUndoCommand::getDestinationObject(quint32 idTool, quint32 idPoint) const
{
    const QDomElement tool = doc->elementById(idTool, VAbstractPattern::TagOperation);
    if (tool.isElement()) {
        QDomElement correctDest;
        const QDomNodeList nodeList = tool.childNodes();
        for (qint32 i = 0; i < nodeList.size(); ++i) {
            const QDomElement dest = nodeList.at(i).toElement();
            if (not dest.isNull() && dest.isElement()
                && dest.tagName() == VAbstractOperation::TagDestination) {
                correctDest = dest;
                break;
            }
        }

        if (not correctDest.isNull()) {
            const QDomNodeList destObjects = correctDest.childNodes();
            for (qint32 i = 0; i < destObjects.size(); ++i) {
                const QDomElement obj = destObjects.at(i).toElement();
                if (not obj.isNull() && obj.isElement()) {
                    const quint32 id = doc->GetParametrUInt(obj, AttrIdObject, NULL_ID_STR);
                    if (idPoint == id) {
                        return obj;
                    }
                }
            }
        }
    }

    return QDomElement();
}
