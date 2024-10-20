/************************************************************************
 **
 **  @file   vistoolellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   24 10, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Seamly2D project
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
#ifndef VISTOOLELLIPTICALARC_H
#define VISTOOLELLIPTICALARC_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "vispath.h"

class VisToolEllipticalArc : public VisPath
{
    Q_OBJECT
public:
    explicit VisToolEllipticalArc(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolEllipticalArc() = default;

    void RefreshGeometry() override;
    void setRadius1(const QString& expression);
    void setRadius2(const QString& expression);
    void setF1(const QString& expression);
    void setF2(const QString& expression);
    void setRotationAngle(const QString& expression);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolEllipticalArc)
    };

private:
    Q_DISABLE_COPY(VisToolEllipticalArc)
    VScaledEllipse* arcCenter;
    qreal radius1;
    qreal radius2;
    qreal f1;
    qreal f2;
    qreal rotationAngle;
};

#endif   // VISTOOLELLIPTICALARC_H
