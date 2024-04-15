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
 **  @file   movedetail.h
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

#ifndef MOVEDETAIL_H
#define MOVEDETAIL_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vundocommand.h"

class QGraphicsScene;

class MovePiece : public VUndoCommand
{
    Q_OBJECT
public:
    MovePiece(
        VAbstractPattern* doc,
        const double& x,
        const double& y,
        const quint32& id,
        QGraphicsScene* scene,
        QUndoCommand* parent = nullptr);
    virtual ~MovePiece();

    virtual void undo() override;
    virtual void redo() override;
    // cppcheck-suppress unusedFunction
    virtual bool mergeWith(const QUndoCommand* command) override;
    virtual int id() const override;
    quint32 getPieceId() const;
    double getNewX() const;
    double getNewY() const;

private:
    Q_DISABLE_COPY(MovePiece)

    double m_oldX;
    double m_oldY;
    double m_newX;
    double m_newY;
    QGraphicsScene* m_scene;

    void SaveCoordinates(QDomElement& domElement, double x, double y);
};

//---------------------------------------------------------------------------------------------------------------------
inline quint32 MovePiece::getPieceId() const { return nodeId; }

//---------------------------------------------------------------------------------------------------------------------
inline double MovePiece::getNewX() const { return m_newX; }

//---------------------------------------------------------------------------------------------------------------------
inline double MovePiece::getNewY() const { return m_newY; }

#endif   // MOVEDETAIL_H
