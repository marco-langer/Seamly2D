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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   8 2, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Seamly2D project
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

#ifndef VLAYOUTPIECEPATH_H
#define VLAYOUTPIECEPATH_H

#include <QPointF>
#include <QVector>


class QPainterPath;

class VLayoutPiecePath
{
public:
    VLayoutPiecePath() = default;
    VLayoutPiecePath(
        const QVector<QPointF>& points, bool cut, Qt::PenStyle penStyle = Qt::SolidLine);

    QPainterPath GetPainterPath() const;

    QVector<QPointF> Points() const;
    void SetPoints(const QVector<QPointF>& points);

    Qt::PenStyle PenStyle() const;
    void SetPenStyle(const Qt::PenStyle& penStyle);

    bool IsCutPath() const;
    void SetCutPath(bool cut);

private:
    /** @brief m_points list of path points. */
    QVector<QPointF> m_points;

    /** @brief m_penStyle path pen style. */
    Qt::PenStyle m_penStyle{ Qt::SolidLine };

    bool m_cut{ false };
};

#endif   // VLAYOUTPIECEPATH_H
