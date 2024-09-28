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
 **  @file   vgraphicslineitem.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 7, 2014
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

#include "vistoolline.h"

#include <QGraphicsLineItem>
#include <QPen>
#include <QPointF>
#include <QSharedPointer>

#include "../ifc/ifcdef.h"
#include "../vgeometry/vpointf.h"
#include "../visualization.h"
#include "../vmisc/vabstractapplication.h"
#include "../vpatterndb/vcontainer.h"
#include "visualization/line/visline.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolLine::VisToolLine(const VContainer* data, QGraphicsItem* parent)
    : VisLine(data, parent)
    , point2Id(NULL_ID)
{
    this->mainColor = Qt::red;
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::RefreshGeometry()
{
    QLineF line;
    const auto& first{ *Visualization::data->GeometricObject<VPointF>(object1Id) };
    if (point2Id == NULL_ID) {
        line = QLineF{ static_cast<QPointF>(first), Visualization::scenePos };
    } else {
        const auto& second{ *Visualization::data->GeometricObject<VPointF>(point2Id) };
        line = QLineF{ static_cast<QPointF>(first), static_cast<QPointF>(second) };
    }
    DrawLine(this, line, mainColor, lineWeight, lineStyle);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::setPoint2Id(const quint32& value) { point2Id = value; }

//---------------------------------------------------------------------------------------------------------------------
void VisToolLine::DrawLine(
    VScaledLine* lineItem,
    const QLineF& line,
    const QColor& color,
    const qreal& lineWeight,
    Qt::PenStyle style)
{
    SCASSERT(lineItem != nullptr)

    QPen visPen = lineItem->pen();
    visPen.setColor(color);
    visPen.setStyle(style);
    visPen.setWidthF(lineWeight);

    lineItem->setPen(visPen);
    lineItem->setLine(line);
}
