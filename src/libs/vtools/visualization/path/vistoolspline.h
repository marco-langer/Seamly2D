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
 **  @file   vistoolspline.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 8, 2014
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

#ifndef VISTOOLSPLINE_H
#define VISTOOLSPLINE_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "vispath.h"

class VControlPointSpline;

class VisToolSpline : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolSpline(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolSpline();

    void RefreshGeometry() override;

    void setObject4Id(const quint32& value);
    void SetAngle1(const qreal& value);
    void SetAngle2(const qreal& value);
    void SetKAsm1(const qreal& value);
    void SetKAsm2(const qreal& value);
    void SetKCurve(const qreal& value);

    const QPointF& GetP2() const;
    const QPointF& GetP3() const;

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolSpline)
    };
public slots:
    void MouseLeftPressed();
    void MouseLeftReleased();

protected:
    Q_DISABLE_COPY(VisToolSpline)
    quint32 object4Id;
    VScaledEllipse* point1;
    VScaledEllipse* point4;
    qreal angle1;
    qreal angle2;
    qreal kAsm1;
    qreal kAsm2;
    qreal kCurve;

    bool isLeftMousePressed;
    bool p2Selected;
    bool p3Selected;

    QPointF p2;
    QPointF p3;

    QVector<VControlPointSpline*> controlPoints;
};

#endif   // VISTOOLSPLINE_H
