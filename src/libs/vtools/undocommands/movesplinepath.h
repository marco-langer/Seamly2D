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
 **  @file   movesplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 6, 2014
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

#ifndef MOVESPLINEPATH_H
#define MOVESPLINEPATH_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vsplinepath.h"
#include "vundocommand.h"

class QGraphicsScene;

class MoveSplinePath : public VUndoCommand
{
    Q_OBJECT
public:
    MoveSplinePath(
        VAbstractPattern* doc,
        const VSplinePath& oldSplPath,
        const VSplinePath& newSplPath,
        const quint32& id,
        QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand* command) override;
    int id() const override;
    quint32 getSplinePathId() const;
    const VSplinePath& getNewSplinePath() const;

private:
    Q_DISABLE_COPY(MoveSplinePath)
    VSplinePath oldSplinePath;
    VSplinePath newSplinePath;
    QGraphicsScene* scene;
    void Do(const VSplinePath& splPath);
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MoveSplinePath::getSplinePathId() const { return nodeId; }

//---------------------------------------------------------------------------------------------------------------------
inline const VSplinePath& MoveSplinePath::getNewSplinePath() const { return newSplinePath; }

#endif   // MOVESPLINEPATH_H
