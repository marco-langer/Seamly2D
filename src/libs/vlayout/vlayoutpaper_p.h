/***************************************************************************
 **  @file   vlayoutpaper_p.h
 **  @author Douglas S Caskey
 **  @date   Dec 27, 2022
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
**  @file   vlayoutpaper_p.h
**  @author Roman Telezhynskyi <dismine(at)gmail.com>
**  @date   8 1, 2015
**
**  @brief
**  @copyright
**  This source code is part of the Valentina project, a pattern making
**  program, whose allow create and modeling patterns of clothing.
**  Copyright (C) 2013-2015 Valentina project
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

#ifndef VLAYOUTPAPER_P_H
#define VLAYOUTPAPER_P_H

#include <QPointF>
#include <QSharedData>
#include <QVector>

#include "vcontour.h"
#include "vlayoutpiece.h"

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_GCC("-Wnon-virtual-dtor")

class VLayoutPaperData : public QSharedData
{
public:
    VLayoutPaperData() = default;
    VLayoutPaperData(int height, int width)
        : globalContour(VContour(height, width))
    {}

    VLayoutPaperData(const VLayoutPaperData& paper)
        : QSharedData(paper)
        , pieces(paper.pieces)
        , globalContour(paper.globalContour)
        , paperIndex(paper.paperIndex)
        , frame(paper.frame)
        , layoutWidth(paper.layoutWidth)
        , globalRotate(paper.globalRotate)
        , localRotate(paper.localRotate)
        , globalRotationIncrease(paper.globalRotationIncrease)
        , localRotationIncrease(paper.localRotationIncrease)
        , saveLength(paper.saveLength)
    {}

    ~VLayoutPaperData() {}

    /** @brief pieces list of arranged pieces. */
    QVector<VLayoutPiece> pieces;

    /** @brief globalContour list of global points contour. */
    VContour globalContour;

    quint32 paperIndex{ 0 };
    quint32 frame{ 0 };
    qreal layoutWidth{ 0 };
    bool globalRotate{ true };
    bool localRotate{ true };
    int globalRotationIncrease{ 180 };
    int localRotationIncrease{ 180 };
    bool saveLength{ false };

private:
    VLayoutPaperData& operator=(const VLayoutPaperData&) = delete;
};

QT_WARNING_POP

#endif   // VLAYOUTPAPER_P_H
