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
 **  @file   vdatatool.h
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

#ifndef VDATATOOL_H
#define VDATATOOL_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/logging.h"
#include "../vpatterndb/vcontainer.h"

Q_DECLARE_LOGGING_CATEGORY(vTool)

// We need QObject class because we use qobject_cast.
/**
 * @brief The VDataTool class need for getting access to data container of tool.
 */
class VDataTool : public QObject
{
    Q_OBJECT
public:
    explicit VDataTool(VContainer* data, QObject* parent = nullptr);
    virtual ~VDataTool() = default;
    VContainer getData() const;
    void setData(const VContainer* value);
    virtual quint32 referens() const;
    virtual void incrementReferens();
    virtual void decrementReferens();
    virtual void GroupVisibility(quint32 object, bool visible) = 0;

protected:
    /** @brief data container with data */
    VContainer data;

    /** @brief _referens keep count tools what use this tool. If value more than 1 you can't delete
     * tool. */
    quint32 _referens;

private:
    Q_DISABLE_COPY(VDataTool)
};

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief getData return data container.
 * @return container.
 */
inline VContainer VDataTool::getData() const { return data; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setData set data container.
 * @param value container.
 */
inline void VDataTool::setData(const VContainer* value) { data = *value; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief referens return count of referens.
 * @return count count of referens.
 */
inline quint32 VDataTool::referens() const { return _referens; }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief incrementReferens increment referens.
 */
inline void VDataTool::incrementReferens() { ++_referens; }

#endif   // VDATATOOL_H
