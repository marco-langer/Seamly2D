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
 **  @file   vistoollineintersect.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 8, 2014
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

#ifndef VISTOOLLINEINTERSECT_H
#define VISTOOLLINEINTERSECT_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolLineIntersect : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolLineIntersect(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolLineIntersect() = default;

    void RefreshGeometry() override;

    void setLine1P2Id(const quint32& value);
    void setLine2P1Id(const quint32& value);
    void setLine2P2Id(const quint32& value);
    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolLineIntersect)
    };

private:
    Q_DISABLE_COPY(VisToolLineIntersect)
    quint32 line1P2Id;
    quint32 line2P1Id;
    quint32 line2P2Id;
    VScaledEllipse* point;
    VScaledEllipse* line1P1;
    VScaledEllipse* line1P2;
    VScaledLine* line1;
    VScaledEllipse* line2P1;
    VScaledEllipse* line2P2;
};

#endif   // VISTOOLLINEINTERSECT_H
