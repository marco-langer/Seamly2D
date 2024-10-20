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
 **  @file   vistooltruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2015
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

#ifndef VISTOOLTRUEDARTS_H
#define VISTOOLTRUEDARTS_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolTrueDarts : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolTrueDarts(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolTrueDarts() = default;

    void RefreshGeometry() override;

    void setObject2Id(const quint32& value);
    void setD1PointId(const quint32& value);
    void setD2PointId(const quint32& value);
    void setD3PointId(const quint32& value);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolTrueDarts)
    };

private:
    Q_DISABLE_COPY(VisToolTrueDarts)
    quint32 baseLineP2Id;
    quint32 dartP1Id;
    quint32 dartP2Id;
    quint32 dartP3Id;

    VScaledEllipse* point1;
    VScaledEllipse* point2;
    VScaledEllipse* baseLineP1;
    VScaledEllipse* baseLineP2;
    VScaledEllipse* dartP1;
    VScaledEllipse* dartP2;
    VScaledEllipse* dartP3;

    VScaledLine* lineblP1P1;
    VScaledLine* lineblP2P2;
    VScaledLine* p1d2;
    VScaledLine* d2p2;
};

#endif   // VISTOOLTRUEDARTS_H
