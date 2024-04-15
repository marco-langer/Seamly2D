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
 **  @file   addtocalc.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 6, 2014
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

#include "addtocalc.h"

#include <QDomNode>

#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/customevents.h"
#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"
#include "../vwidgets/vmaingraphicsview.h"
#include "vundocommand.h"

//---------------------------------------------------------------------------------------------------------------------
AddToCalc::AddToCalc(const QDomElement& xml, VAbstractPattern* doc, QUndoCommand* parent)
    : VUndoCommand(xml, doc, parent)
    , activeBlockName(doc->getActiveDraftBlockName())
    , cursor(doc->getCursor())
{
    setText(tr("add object"));
    nodeId = doc->getParameterId(xml);
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::undo()
{
    qCDebug(vUndo, "Undo.");

    doc->changeActiveDraftBlock(activeBlockName);   // Without this user will not see this change

    QDomElement calcElement;
    if (doc->getActiveNodeElement(VAbstractPattern::TagCalculation, calcElement)) {
        QDomElement domElement = doc->elementById(nodeId);
        if (domElement.isElement()) {
            if (calcElement.removeChild(domElement).isNull()) {
                qCDebug(vUndo, "Can't delete node.");
                return;
            }
        } else {
            qCDebug(vUndo, "Can't get tool by id = %u.", nodeId);
            return;
        }
    } else {
        qCDebug(vUndo, "Can't find tag Calculation.");
        return;
    }
    emit NeedFullParsing();
    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
    doc->setCurrentDraftBlock(activeBlockName);   // Return current pattern piece after undo
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::redo()
{
    qCDebug(vUndo, "Redo.");

    doc->changeActiveDraftBlock(activeBlockName);   // Without this user will not see this change
    doc->setCursor(cursor);

    QDomElement calcElement;
    if (doc->getActiveNodeElement(VAbstractPattern::TagCalculation, calcElement)) {
        if (cursor == NULL_ID) {
            calcElement.appendChild(xml);
        } else {
            QDomElement refElement = doc->elementById(cursor);
            if (refElement.isElement()) {
                calcElement.insertAfter(xml, refElement);
            } else {
                qCDebug(vUndo, "Can not find the element after which you want to insert.");
                return;
            }
        }
    } else {
        qCDebug(vUndo, "Can't find tag Calculation.");
        return;
    }
    RedoFullParsing();
    VMainGraphicsView::NewSceneRect(qApp->getCurrentScene(), qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void AddToCalc::RedoFullParsing()
{
    if (redoFlag) {
        emit NeedFullParsing();
        doc->setCurrentDraftBlock(activeBlockName);   // Return current pattern piece after undo
    } else {
        QApplication::postEvent(doc, new LiteParseEvent());
    }
    redoFlag = true;
}
