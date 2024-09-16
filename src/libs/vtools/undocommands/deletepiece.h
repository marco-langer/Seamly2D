/***************************************************************************
 **  @file   deletepiece.h
 **  @author Douglas S Caskey
 **  @date   Dec 11, 2022
 **
 **  @copyright
 **  Copyright (C) 2017 - 2022 Seamly, LLC
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
 **************************************************************************/

/************************************************************************
**
**  @file
**  @author Roman Telezhynskyi <dismine(at)gmail.com>
**  @date   9 11, 2016
**
**  @brief
**  @copyright
**  This source code is part of the Valentine project, a pattern making
**  program, whose allow create and modeling patterns of clothing.
**  Copyright (C) 2016 Valentina project
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
**  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#ifndef DELETEPIECE_H
#define DELETEPIECE_H

#include <QtGlobal>

#include "../tools/pattern_piece_tool.h"
#include "vpiece.h"
#include "vundocommand.h"

class DeletePiece : public VUndoCommand
{
    Q_OBJECT
public:
    DeletePiece(
        VAbstractPattern* doc, quint32 id, const VPiece& piece, QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    Q_DISABLE_COPY(DeletePiece)

    QDomNode m_parentNode;
    quint32 m_siblingId;
    VPiece m_piece;
};

#endif   // DELETEPIECE_H
