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
 **  @file   vistoollineintersectaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   19 10, 2014
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

#ifndef VISTOOLLINEINTERSECTAXIS_H
#define VISTOOLLINEINTERSECTAXIS_H

#include <QGraphicsItem>
#include <QLineF>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "visline.h"

class VisToolLineIntersectAxis : public VisLine
{
    Q_OBJECT
public:
    explicit VisToolLineIntersectAxis(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolLineIntersectAxis() = default;

    void RefreshGeometry() override;

    QString Angle() const;
    void SetAngle(const QString& expression);
    void setPoint2Id(const quint32& value);
    void setAxisPointId(const quint32& value);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolLineIntersectAxis)
    };

private:
    Q_DISABLE_COPY(VisToolLineIntersectAxis)
    quint32 point2Id;
    quint32 axisPointId;
    qreal angle;
    VScaledEllipse* point;
    VScaledEllipse* lineP1;
    VScaledEllipse* lineP2;
    VScaledEllipse* basePoint;
    VScaledLine* baseLine;
    VScaledLine* axisLine;
    VScaledLine* line_intersection;
    void ShowIntersection(const QLineF& axis_line, const QLineF& base_line);
};

#endif   // VISTOOLLINEINTERSECTAXIS_H
