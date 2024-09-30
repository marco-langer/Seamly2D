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
 **  @file   vtoolrecord.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VTOOLRECORD_H
#define VTOOLRECORD_H

#include <QString>
#include <QtGlobal>

#include "../ifcdef.h"
#include "../vmisc/def.h"

/**
 * @brief The VToolRecord class record about tool in history.
 */
struct VToolRecord final
{
    VToolRecord() = default;
    VToolRecord(const quint32& id, const Tool& typeTool, const QString& nameDraw);
    bool operator==(const VToolRecord& record) const;

    /** @brief id tool id. */
    quint32 id{ NULL_ID };

    /** @brief typeTool tool type. */
    Tool typeTool{ Tool::Arrow };

    /** @brief nameDraw pattern peace name. */
    QString nameDraw;
};

//---------------------------------------------------------------------------------------------------------------------
inline bool VToolRecord::operator==(const VToolRecord& record) const
{
    // Id should be enough
    return id == record.id /* && typeTool == record.getTypeTool() && nameDraw ==
                                   record.getDraftBlockName()*/
        ;
}

#endif   // VTOOLRECORD_H
