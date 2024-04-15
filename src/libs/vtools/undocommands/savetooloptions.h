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
 **  @file   savetooloptions.h
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

#ifndef SAVETOOLOPTIONS_H
#define SAVETOOLOPTIONS_H

#include <QDomElement>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vundocommand.h"

class SaveToolOptions : public VUndoCommand
{
    Q_OBJECT
public:
    SaveToolOptions(
        const QDomElement& oldXml,
        const QDomElement& newXml,
        VAbstractPattern* doc,
        const quint32& id,
        QUndoCommand* parent = nullptr);
    virtual ~SaveToolOptions() override;
    virtual void undo() override;
    virtual void redo() override;
    virtual bool mergeWith(const QUndoCommand* command) override;
    virtual int id() const override;
    QDomElement getNewXml() const;
    quint32 getToolId() const;

private:
    Q_DISABLE_COPY(SaveToolOptions)
    const QDomElement oldXml;
    QDomElement newXml;
};

//---------------------------------------------------------------------------------------------------------------------
inline QDomElement SaveToolOptions::getNewXml() const { return newXml; }

//---------------------------------------------------------------------------------------------------------------------
inline quint32 SaveToolOptions::getToolId() const { return nodeId; }

#endif   // SAVETOOLOPTIONS_H
