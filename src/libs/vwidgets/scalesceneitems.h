/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   21 6, 2017
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

#ifndef SCALESCENEITEMS_H
#define SCALESCENEITEMS_H

#include <QGraphicsLineItem>

#include "global.h"

#include "../vmisc/def.h"
#include "vcurvepathitem.h"

class VCurvePathItem;

class VScaledLine : public QGraphicsLineItem
{
public:
    explicit VScaledLine(QGraphicsItem* parent = nullptr);
    explicit VScaledLine(const QLineF& line, QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ScaledLine)
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
        override;

    qreal GetBasicWidth() const;
    void setBasicWidth(const qreal& value);

private:
    Q_DISABLE_COPY(VScaledLine)

    qreal basicWidth{ widthMainLine };
};

class ArrowedLineItem : public QGraphicsLineItem
{
public:
    explicit ArrowedLineItem(QGraphicsItem* parent = nullptr);
    ArrowedLineItem(const QLineF& line, QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ArrowedLineItem)
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
        override;

private:
    Q_DISABLE_COPY(ArrowedLineItem)
    void drawArrow(const QLineF& axis, QPainterPath& path, const qreal& arrow_size);
    VCurvePathItem* m_arrows;
};

class VScaledEllipse : public QGraphicsEllipseItem
{
public:
    explicit VScaledEllipse(QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ScaledEllipse)
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
        override;

private:
    Q_DISABLE_COPY(VScaledEllipse)
};

#endif   // SCALESCENEITEMS_H
