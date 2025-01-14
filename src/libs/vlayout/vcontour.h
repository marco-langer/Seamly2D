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
 **  @file   vcontour.h
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

#ifndef VCONTOUR_H
#define VCONTOUR_H

#include <QPointF>
#include <QSizeF>
#include <QVector>
#include <QtGlobal>

#include "vlayoutdef.h"

class QLineF;
class QRectF;
class QPainterPath;
class VLayoutPiece;

class VContour
{
public:
    VContour() = default;
    VContour(int height, int width);

    void SetContour(const QVector<QPointF>& contour);
    const QVector<QPointF>& GetContour() const;

    quint32 GetShift() const;
    void SetShift(quint32 shift);

    int GetHeight() const;
    void setHeight(int height);

    int GetWidth() const;
    void SetWidth(int width);

    QSizeF GetSize() const;

    QVector<QPointF>
    UniteWithContour(const VLayoutPiece& detail, int globalI, int detJ, BestFrom type) const;

    QLineF EmptySheetEdge() const;
    int GlobalEdgesCount() const;
    QLineF GlobalEdge(int i) const;
    QVector<QPointF> CutEdge(const QLineF& edge) const;
    QVector<QPointF> CutEmptySheetEdge() const;

    const QPointF& at(int i) const;

    QPainterPath ContourPath() const;

private:
    void AppendWhole(QVector<QPointF>& contour, const VLayoutPiece& detail, int detJ) const;

    /** @brief globalContour list of global points contour. */
    QVector<QPointF> m_globalContour;

    /** @brief paperHeight height of paper in pixels*/
    int m_paperHeight{ 0 };

    /** @brief paperWidth width of paper in pixels*/
    int m_paperWidth{ 0 };

    quint32 m_shift{ 0 };
};

#endif   // VCONTOUR_H
