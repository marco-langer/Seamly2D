/***************************************************************************
 **  @file   vpiecepath_p.h
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
 **************************************************************************

 ************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#ifndef VPIECEPATH_P_H
#define VPIECEPATH_P_H

#include <QSharedData>
#include <QVector>

#include "vpiecenode.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VPiecePathData : public QSharedData
{
public:
    VPiecePathData()
        : m_nodes()
        , m_type(PiecePathType::Unknown)
        , m_name()
        , m_penType(Qt::SolidLine)
        , m_cut(false)
    {}

    explicit VPiecePathData(PiecePathType type)
        : m_nodes()
        , m_type(type)
        , m_name()
        , m_penType(Qt::SolidLine)
        , m_cut(false)
    {}

    VPiecePathData(const VPiecePathData& path)
        : QSharedData(path)
        , m_nodes(path.m_nodes)
        , m_type(path.m_type)
        , m_penType(path.m_penType)
        , m_cut(path.m_cut)
    {}

    ~VPiecePathData();

    QVector<VPieceNode> m_nodes;
    PiecePathType m_type;
    QString m_name;
    Qt::PenStyle m_penType;
    bool m_cut;

private:
    VPiecePathData& operator=(const VPiecePathData&) = delete;
};

VPiecePathData::~VPiecePathData() = default;

QT_WARNING_POP

#endif   // VPIECEPATH_P_H
