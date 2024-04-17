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
 **  @file   vposter.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
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

#ifndef VPOSTER_H
#define VPOSTER_H

#include <QCoreApplication>
#include <QRect>
#include <QtGlobal>

#include "../vmisc/def.h"

class QGraphicsItem;
class QPrinter;
template <class T>
class QVector;

struct PosterData
{
    quint32 index{ 0 };   // paper index
    quint32 row{ 0 };     // positions in the grid
    quint32 column{ 0 };
    quint32 rows{ 0 };
    quint32 columns{ 0 };
    QRect rect;   // rect section
};

// cppcheck-suppress noConstructor
class VPoster
{
    Q_DECLARE_TR_FUNCTIONS(VPoster)
public:
    explicit VPoster(const QPrinter* printer);

    QVector<PosterData> Calc(const QRect& imageRect, int page, PageOrientation orientation) const;

    QVector<QGraphicsItem*> Borders(QGraphicsItem* parent, const PosterData& img, int sheets) const;

private:
    const QPrinter* printer;
    /**
     * @brief allowance is the width of the strip that holds the tiled
     * grid information and that is used for the gluing.
     */
    quint32 allowance;

    int CountRows(int height, PageOrientation orientation) const;
    int CountColumns(int width, PageOrientation orientation) const;

    PosterData Cut(int i, int j, const QRect& imageRect, PageOrientation orientation) const;

    QRect PageRect() const;

    static qreal ToPixel(qreal val);
};

#endif   // VPOSTER_H
