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
 **  @file   vcontour.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 1, 2015
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

#include "vcontour.h"

#include <QLineF>
#include <QPainterPath>
#include <QPoint>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>
#include <Qt>
#include <QtMath>

#include "vlayoutpiece.h"


VContour::VContour(int height, int width)
    : m_paperHeight{ height }
    , m_paperWidth{ width }
{
}


//---------------------------------------------------------------------------------------------------------------------
void VContour::SetContour(const QVector<QPointF>& contour) { m_globalContour = contour; }

//---------------------------------------------------------------------------------------------------------------------
const QVector<QPointF>& VContour::GetContour() const { return m_globalContour; }

//---------------------------------------------------------------------------------------------------------------------
quint32 VContour::GetShift() const { return m_shift; }

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetShift(quint32 shift) { m_shift = shift; }

//---------------------------------------------------------------------------------------------------------------------
int VContour::GetHeight() const { return m_paperHeight; }

//---------------------------------------------------------------------------------------------------------------------
void VContour::setHeight(int height) { m_paperHeight = height; }

//---------------------------------------------------------------------------------------------------------------------
int VContour::GetWidth() const { return m_paperWidth; }

//---------------------------------------------------------------------------------------------------------------------
void VContour::SetWidth(int width) { m_paperWidth = width; }

//---------------------------------------------------------------------------------------------------------------------
QSizeF VContour::GetSize() const { return QSizeF(m_paperWidth, m_paperHeight); }

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF>
VContour::UniteWithContour(const VLayoutPiece& detail, int globalI, int detJ, BestFrom type) const
{
    QVector<QPointF> newContour;
    if (m_globalContour.isEmpty())   //-V807
    {
        AppendWhole(newContour, detail, 0);
    } else {
        if (globalI <= 0 || globalI > GlobalEdgesCount()) {
            return QVector<QPointF>();
        }

        if (detJ <= 0 || detJ > detail.LayoutEdgesCount()) {
            return QVector<QPointF>();
        }

        int i2 = 0;
        if (globalI == m_globalContour.count()) {
            i2 = 0;
        } else {
            i2 = globalI;
        }

        int i = 0;
        while (i < m_globalContour.count()) {
            if (i == i2) {
                if (type == BestFrom::Rotation) {
                    AppendWhole(newContour, detail, detJ);
                } else {
                    int processedEdges = 0;
                    const int nD = detail.LayoutEdgesCount();
                    int j = detJ + 1;
                    do {
                        if (j > nD) {
                            j = 1;
                        }
                        if (j != detJ) {
                            const QVector<QPointF> points = CutEdge(detail.LayoutEdge(j));
                            for (int i = 0; i < points.size() - 1; ++i) {
                                newContour.append(points.at(i));
                            }
                        }
                        ++processedEdges;
                        ++j;
                    } while (processedEdges < nD);
                }
            }

            if (newContour.isEmpty() == false) {
                if (newContour.last() != m_globalContour.at(i)) {
                    newContour.append(m_globalContour.at(i));
                }
            } else {
                newContour.append(m_globalContour.at(i));
            }
            ++i;
        }
    }
    return newContour;
}

//---------------------------------------------------------------------------------------------------------------------
int VContour::GlobalEdgesCount() const
{
    if (m_globalContour.isEmpty()) {
        return 10;
    } else {
        return m_globalContour.count();
    }
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::GlobalEdge(int i) const
{
    if (m_globalContour.isEmpty())   //-V807
    {
        // Because sheet is blank we have one global edge for all cases - Ox axis.
        if (i < 1 || i > GlobalEdgesCount()) {   // Doesn't exist such edge
            return EmptySheetEdge();
        }

        const qreal nShift = EmptySheetEdge().length() / GlobalEdgesCount();
        return QLineF(nShift * (i - 1), 0, nShift * i, 0);
    } else {
        if (i < 1 || i > GlobalEdgesCount()) {   // Doesn't exist such edge
            return QLineF();
        }
        QLineF edge;
        if (i < GlobalEdgesCount()) {
            edge = QLineF(m_globalContour.at(i - 1), m_globalContour.at(i));
        } else {   // Closed countour
            edge = QLineF(m_globalContour.at(GlobalEdgesCount() - 1), m_globalContour.at(0));
        }
        return edge;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::CutEdge(const QLineF& edge) const
{
    QVector<QPointF> points;
    if (m_shift == 0) {
        points.append(edge.p1());
        points.append(edge.p2());
    } else {
        const int n = qFloor(edge.length() / m_shift);

        if (n <= 0) {
            points.append(edge.p1());
            points.append(edge.p2());
        } else {
            const qreal nShift = edge.length() / n;
            for (int i = 1; i <= n + 1; ++i) {
                QLineF l1 = edge;
                l1.setLength(nShift * (i - 1));
                points.append(l1.p2());
            }
        }
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
QVector<QPointF> VContour::CutEmptySheetEdge() const
{
    QVector<QPointF> points;
    const qreal nShift = EmptySheetEdge().length() / GlobalEdgesCount();
    for (int i = 1; i <= GlobalEdgesCount() + 1; ++i) {
        QLineF l1 = EmptySheetEdge();
        l1.setLength(nShift * (i - 1));
        points.append(l1.p2());
    }
    return points;
}

//---------------------------------------------------------------------------------------------------------------------
const QPointF& VContour::at(int i) const { return m_globalContour.at(i); }


//---------------------------------------------------------------------------------------------------------------------
QPainterPath VContour::ContourPath() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    const QVector<QPointF>& points = GetContour();
    path.moveTo(points.at(0));
    for (qint32 i = 1; i < points.count(); ++i) {
        path.lineTo(points.at(i));
    }
    path.lineTo(points.at(0));

    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void VContour::AppendWhole(QVector<QPointF>& contour, const VLayoutPiece& detail, int detJ) const
{
    int processedEdges = 0;
    const int nD = detail.LayoutEdgesCount();
    int j = detJ + 1;
    do {
        if (j > nD) {
            j = 1;
        }
        const QVector<QPointF> points = CutEdge(detail.LayoutEdge(j));
        for (int i = 0; i < points.size() - 1; ++i) {
            contour.append(points.at(i));
        }
        ++processedEdges;
        ++j;
    } while (processedEdges < nD);
}

//---------------------------------------------------------------------------------------------------------------------
QLineF VContour::EmptySheetEdge() const { return QLineF(0, 0, m_paperWidth - 5, 0); }
