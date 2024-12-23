/************************************************************************
 **
 **  @file   anchorpoint_visual.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
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

#ifndef ANCHOR_POINT_VISUAL_H
#define ANCHOR_POINT_VISUAL_H

#include "visline.h"

class VSimplePoint;

class AnchorPointVisual : public VisLine
{
    Q_OBJECT
public:
    explicit AnchorPointVisual(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~AnchorPointVisual();

    void RefreshGeometry() override;
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolAnchorPoint)
    };

private:
    Q_DISABLE_COPY(AnchorPointVisual)
    QPointer<VSimplePoint> m_point;
};

#endif   // ANCHOR_POINT_VISUAL_H
