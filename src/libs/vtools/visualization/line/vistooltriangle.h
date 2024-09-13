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
 **  @file   vistooltriangle.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   13 8, 2014
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

#ifndef VISTOOLTRIANGLE_H
#define VISTOOLTRIANGLE_H

#include <QColor>
#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <Qt>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolTriangle : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolTriangle(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolTriangle() = default;

    void RefreshGeometry() override;

    void setObject2Id(const quint32& value);
    void setHypotenuseP1Id(const quint32& value);
    void setHypotenuseP2Id(const quint32& value);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolTriangle)
    };

private:
    Q_DISABLE_COPY(VisToolTriangle)
    quint32 object2Id;   // axis second point
    quint32 hypotenuseP1Id;
    quint32 hypotenuseP2Id;
    VScaledEllipse* point;
    VScaledEllipse* axisP1;
    VScaledEllipse* axisP2;
    ArrowedLineItem* axis;
    VScaledEllipse* hypotenuseP1;
    VScaledEllipse* hypotenuseP2;
    VScaledLine* foot1;
    VScaledLine* foot2;
};

#endif   // VISTOOLTRIANGLE_H
