/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                             *
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
 **  @file   deltool.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 6, 2014
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

#include "deltool.h"

#include <QDomElement>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/logging.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
DelTool::DelTool(VAbstractPattern* doc, quint32 id, QUndoCommand* parent)
    : VUndoCommand(QDomElement(), doc, parent)
    , parentNode(QDomNode())
    , siblingId(NULL_ID)
    , activeBlockName(doc->getActiveDraftBlockName())
{
    setText(tr("delete tool"));
    nodeId = id;
    siblingId = doc->SiblingNodeId(nodeId);
    parentNode = doc->ParentNodeById(nodeId);
    xml = doc->CloneNodeById(nodeId);
}

//---------------------------------------------------------------------------------------------------------------------
DelTool::~DelTool() {}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::undo()
{
    qCDebug(vUndo, "Undo.");

    UndoDeleteAfterSibling(parentNode, siblingId);
    emit NeedFullParsing();
    // Keep last!
    doc->setCurrentDraftBlock(activeBlockName);   // Without this user will not see this change
}

//---------------------------------------------------------------------------------------------------------------------
void DelTool::redo()
{
    qCDebug(vUndo, "Redo.");

    // Keep first!
    doc->setCurrentDraftBlock(activeBlockName);   // Without this user will not see this change
    QDomElement domElement = doc->NodeById(nodeId);
    parentNode.removeChild(domElement);
    emit NeedFullParsing();
}
