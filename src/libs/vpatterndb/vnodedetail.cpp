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
 **  @file   vnodedetail.cpp
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

#include "vnodedetail.h"
#include "../vgeometry/vpointf.h"
#include "../vpatterndb/vcontainer.h"
#include "vnodedetail_p.h"
#include "vpiecenode.h"
#include "vpiecepath.h"

#include "math/math.h"

#include <QLineF>
#include <QVector>

namespace {
//---------------------------------------------------------------------------------------------------------------------
bool IsOX(const QLineF& line)
{
    return math::isFuzzyEqual(line.angle(), 0.0) || math::isFuzzyEqual(line.angle(), 360.0)
        || math::isFuzzyEqual(line.angle(), 180.0);
}

//---------------------------------------------------------------------------------------------------------------------
bool IsOY(const QLineF& line)
{
    return math::isFuzzyEqual(line.angle(), 90.0) || math::isFuzzyEqual(line.angle(), 270.0);
}

//---------------------------------------------------------------------------------------------------------------------
QString LocalWidth(const QLineF& line, const QLineF& movedLine)
{
    if (math::isFuzzyEqual(line.angle(), movedLine.angle())) {
        return QString().setNum(movedLine.length());
    } else {   // different direction means value is negative
        return QString("0");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ConvertBefore(VPieceNode& node, const QLineF& line, qreal mX, qreal mY)
{
    if (not qFuzzyIsNull(mX) && IsOX(line)) {
        const QLineF movedLine(line.p1().x(), line.p1().y(), line.p2().x() + mX, line.p2().y());
        node.setBeforeSAFormula(LocalWidth(line, movedLine));
    } else if (not qFuzzyIsNull(mY) && IsOY(line)) {
        const QLineF movedLine(line.p1().x(), line.p1().y(), line.p2().x(), line.p2().y() + mY);
        node.setBeforeSAFormula(LocalWidth(line, movedLine));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void ConvertAfter(VPieceNode& node, const QLineF& line, qreal mX, qreal mY)
{
    if (not qFuzzyIsNull(mX) && IsOX(line)) {
        const QLineF movedLine(line.p1().x(), line.p1().y(), line.p2().x() + mX, line.p2().y());
        node.setAfterSAFormula(LocalWidth(line, movedLine));
    } else if (not qFuzzyIsNull(mY) && IsOY(line)) {
        const QLineF movedLine(line.p1().x(), line.p1().y(), line.p2().x(), line.p2().y() + mY);
        node.setAfterSAFormula(LocalWidth(line, movedLine));
    }
}
}   // namespace

VNodeDetail& VNodeDetail::operator=(VNodeDetail&& node) noexcept
{
    Swap(node);
    return *this;
}

void VNodeDetail::Swap(VNodeDetail& node) noexcept { std::swap(d, node.d); }

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail()
    : d(new VNodeDetailData)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(
    quint32 id, Tool typeTool, NodeDetail typeNode, qreal mx, qreal my, bool reverse)
    : d(new VNodeDetailData(id, typeTool, typeNode, mx, my, reverse))
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::VNodeDetail(const VNodeDetail& node)
    : d(node.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail& VNodeDetail::operator=(const VNodeDetail& node)
{
    if (&node == this) {
        return *this;
    }
    d = node.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VNodeDetail::~VNodeDetail() = default;

//---------------------------------------------------------------------------------------------------------------------
quint32 VNodeDetail::getId() const { return d->id; }

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setId(const quint32& value) { d->id = value; }

//---------------------------------------------------------------------------------------------------------------------
Tool VNodeDetail::getTypeTool() const { return d->typeTool; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VNodeDetail::setTypeTool(const Tool& value) { d->typeTool = value; }

//---------------------------------------------------------------------------------------------------------------------
NodeDetail VNodeDetail::getTypeNode() const { return d->typeNode; }

//---------------------------------------------------------------------------------------------------------------------
// cppcheck-suppress unusedFunction
void VNodeDetail::setTypeNode(const NodeDetail& value) { d->typeNode = value; }

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMx() const { return d->mx; }

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMx(const qreal& value) { d->mx = value; }

//---------------------------------------------------------------------------------------------------------------------
qreal VNodeDetail::getMy() const { return d->my; }

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setMy(const qreal& value) { d->my = value; }

//---------------------------------------------------------------------------------------------------------------------
bool VNodeDetail::getReverse() const
{
    if (getTypeTool() == Tool::NodePoint) {
        return false;
    } else {
        return d->reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VNodeDetail::setReverse(bool reverse)
{
    if (getTypeTool() == Tool::NodePoint) {
        d->reverse = false;
    } else {
        d->reverse = reverse;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPieceNode> VNodeDetail::Convert(
    const VContainer* data, const QVector<VNodeDetail>& nodes, qreal width, bool closed)
{
    if (width < 0) {
        width = 0;
    }

    VPiecePath path;
    for (int i = 0; i < nodes.size(); ++i) {
        const VNodeDetail& node = nodes.at(i);
        path.Append(VPieceNode(node.getId(), node.getTypeTool(), node.getReverse()));
    }

    if (path.PathPoints(data).size() > 2) {
        for (int i = 0; i < nodes.size(); ++i) {
            const VNodeDetail& node = nodes.at(i);
            if (node.getTypeTool() == Tool::NodePoint) {
                if (not qFuzzyIsNull(node.getMx()) || not qFuzzyIsNull(node.getMy())) {
                    const QPointF previosPoint = path.NodePreviousPoint(data, i);
                    const QPointF nextPoint = path.NodeNextPoint(data, i);

                    const QPointF point = data->GeometricObject<VPointF>(node.getId())->toQPointF();

                    QLineF lineBefore(point, previosPoint);
                    lineBefore.setAngle(lineBefore.angle() - 90);
                    lineBefore.setLength(width);

                    ConvertBefore(path[i], lineBefore, node.getMx(), node.getMy());

                    QLineF lineAfter(point, nextPoint);
                    lineAfter.setAngle(lineAfter.angle() + 90);
                    lineAfter.setLength(width);

                    ConvertAfter(path[i], lineAfter, node.getMx(), node.getMy());
                }
            }
        }
    }

    if (not closed && path.CountNodes() > 1) {
        path[0].setBeforeSAFormula("0");
        path[path.CountNodes() - 1].setAfterSAFormula("0");
    }

    return path.GetNodes();
}
