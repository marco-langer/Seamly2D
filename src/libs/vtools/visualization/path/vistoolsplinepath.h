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
 **  @file   vistoolsplinepath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 9, 2014
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

#ifndef VISTOOLSPLINEPATH_H
#define VISTOOLSPLINEPATH_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vsplinepath.h"
#include "../vmisc/def.h"
#include "vispath.h"

class VControlPointSpline;

class VisToolSplinePath : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolSplinePath(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolSplinePath();

    void RefreshGeometry() override;

    void setPath(const VSplinePath& value);
    const VSplinePath& getPath();

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolSplinePath)
    };
signals:
    void PathChanged(const VSplinePath& path);

public slots:
    void MouseLeftPressed();
    void MouseLeftReleased();

protected:
    Q_DISABLE_COPY(VisToolSplinePath)
    QVector<VScaledEllipse*> points;
    QVector<VControlPointSpline*> ctrlPoints;
    VCurvePathItem* newCurveSegment;
    VSplinePath path;

    bool isLeftMousePressed;
    bool pointIsSelected;

    QPointF ctrlPoint;

    VScaledEllipse* getPoint(quint32 i);
    void Creating(const QPointF& pSpl, int size);
};

#endif   // VISTOOLSPLINEPATH_H
