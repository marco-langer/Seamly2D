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
 **  @date   3 11, 2016
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

#ifndef VPIECENODE_P_H
#define VPIECENODE_P_H

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"
#include <QDataStream>
#include <QSharedData>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VPieceNodeData : public QSharedData
{
public:
    VPieceNodeData()
        : m_id(NULL_ID)
        , m_typeTool(Tool::NodePoint)
        , m_reverse(false)
        , m_excluded(false)
        , m_isNotch(false)
        , m_isMainPathNode(true)
        , m_beforeWidthFormula(currentSeamAllowance)
        , m_afterWidthFormula(currentSeamAllowance)
        , m_angleType(PieceNodeAngle::ByLength)
        , m_notchType(stringToNotchType(qApp->Settings()->getDefaultNotchType()))
        , m_notchSubType(NotchSubType::Straightforward)
        , m_showNotch(qApp->Settings()->showSeamAllowanceNotch())
        , m_showSeamlineNotch(qApp->Settings()->showSeamlineNotch())
        , m_notchLength(qApp->Settings()->getDefaultNotchLength())
        , m_notchWidth(qApp->Settings()->getDefaultNotchWidth())
        , m_notchAngle(.000)
        , m_notchCount(1)
    {
    }

    VPieceNodeData(quint32 id, Tool typeTool, bool reverse)
        : m_id(id)
        , m_typeTool(typeTool)
        , m_reverse(reverse)
        , m_excluded(false)
        , m_isNotch(false)
        , m_isMainPathNode(true)
        , m_beforeWidthFormula(currentSeamAllowance)
        , m_afterWidthFormula(currentSeamAllowance)
        , m_angleType(PieceNodeAngle::ByLength)
        , m_notchType(stringToNotchType(qApp->Settings()->getDefaultNotchType()))
        , m_notchSubType(NotchSubType::Straightforward)
        , m_showNotch(qApp->Settings()->showSeamAllowanceNotch())
        , m_showSeamlineNotch(qApp->Settings()->showSeamlineNotch())
        , m_notchLength(qApp->Settings()->getDefaultNotchLength())
        , m_notchWidth(qApp->Settings()->getDefaultNotchWidth())
        , m_notchAngle(.000)
        , m_notchCount(1)
    {
        if (m_typeTool == Tool::NodePoint) {
            m_reverse = false;
        }
    }

    VPieceNodeData(const VPieceNodeData& node)
        : QSharedData(node)
        , m_id(node.m_id)
        , m_typeTool(node.m_typeTool)
        , m_reverse(node.m_reverse)
        , m_excluded(node.m_excluded)
        , m_isNotch(node.m_isNotch)
        , m_isMainPathNode(node.m_isMainPathNode)
        , m_beforeWidthFormula(node.m_beforeWidthFormula)
        , m_afterWidthFormula(node.m_afterWidthFormula)
        , m_angleType(node.m_angleType)
        , m_notchType(node.m_notchType)
        , m_notchSubType(node.m_notchSubType)
        , m_showNotch(node.m_showNotch)
        , m_showSeamlineNotch(node.m_showSeamlineNotch)
        , m_notchLength(node.m_notchLength)
        , m_notchWidth(node.m_notchWidth)
        , m_notchAngle(node.m_notchAngle)
        , m_notchCount(node.m_notchCount)
    {
    }

    ~VPieceNodeData() = default;

    quint32 m_id;      //! @brief id object id.
    Tool m_typeTool;   //! @brief typeTool type of tool
    bool m_reverse;    //! @brief reverse true if need reverse points list for node.
    bool m_excluded;   //! @brief m_excluded true if item excluded from main path. Excluded item is
                       //! not visible and also will not has affect on main path. Also include to
                       //! exist path items automatically setted excluded. */
    bool m_isNotch;    //! @brief m_isNotch has sense only for points. If true to seam allowance
                       //! should a notch should be added.
    bool m_isMainPathNode;   //! @brief m_isMainPathNode need fin know if allowed for this notch to
                             //! be double.

    QString m_beforeWidthFormula;
    QString m_afterWidthFormula;

    PieceNodeAngle m_angleType;

    NotchType m_notchType;
    NotchSubType m_notchSubType;
    bool m_showNotch;
    bool m_showSeamlineNotch;
    qreal m_notchLength;
    qreal m_notchWidth;
    qreal m_notchAngle;
    int m_notchCount;

private:
    VPieceNodeData& operator=(const VPieceNodeData&) = delete;
};

QT_WARNING_POP

#endif   // VPIECENODE_P_H
