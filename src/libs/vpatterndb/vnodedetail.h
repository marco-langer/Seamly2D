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
 **  @file   vnodedetail.h
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

#ifndef VNODEDETAIL_H
#define VNODEDETAIL_H

#include "../vmisc/def.h"

class VPieceNode;
class VContainer;

/**
 * @brief The VNodeDetail class keep information about detail node.
 */
class VNodeDetail final
{
public:
    /**
     * @brief VNodeDetail constructor.
     * @param id object id
     * @param typeTool type tool
     * @param typeNode type node
     * @param mx object bias x axis
     * @param my object bias y axis
     */
    VNodeDetail(
        quint32 id,
        Tool typeTool,
        NodeDetail typeNode,
        qreal mx = 0,
        qreal my = 0,
        bool reverse = false);

    /**
     * @brief getId return object id.
     * @return id.
     */
    quint32 getId() const;
    /**
     * @brief setId set object id.
     * @param value object id.
     */
    void setId(const quint32& value);
    /**
     * @brief getTypeTool return tool type.
     * @return tool type.
     */
    Tool getTypeTool() const;
    /**
     * @brief setTypeTool set tool type.
     * @param value tool type.
     */
    void setTypeTool(const Tool& value);
    /**
     * @brief getTypeNode return node type.
     * @return node type.
     */
    NodeDetail getTypeNode() const;
    /**
     * @brief setTypeNode set node type.
     * @param value node type.
     */
    void setTypeNode(const NodeDetail& value);
    /**
     * @brief getMx return object bias x axis.
     * @return bias x axis.
     */
    qreal getMx() const;
    /**
     * @brief setMx set object bias x axis.
     * @param value bias x axis.
     */
    void setMx(const qreal& value);
    /**
     * @brief getMy return object bias y axis.
     * @return bias y axis.
     */
    qreal getMy() const;
    /**
     * @brief setMy set object bias y axis.
     * @param value bias y axis.
     */
    void setMy(const qreal& value);

    bool getReverse() const;
    void setReverse(bool reverse);

    static QVector<VPieceNode>
    Convert(const VContainer* data, const QVector<VNodeDetail>& nodes, qreal width, bool closed);

private:
    quint32 m_id;
    Tool m_typeTool;
    NodeDetail m_typeNode;
    /**
     * @brief mx bias x axis.
     */
    qreal m_mx;
    /**
     * @brief my bias y axis.
     */
    qreal m_my;
    /**
     * @brief reverse true if need reverse points list for node.
     */
    bool m_reverse;
};

#endif   // VNODEDETAIL_H
