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
 **  @file   vpointf.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
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

#ifndef VPOINTF_H
#define VPOINTF_H

#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>
#include <QTypeInfo>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "vgeometrydef.h"
#include "vgobject.h"

class VPointFData;

QT_WARNING_PUSH
QT_WARNING_DISABLE_INTEL(2304)

/**
 * @brief The VPointF class keep data of point.
 */
class VPointF : public VGObject
{
public:
    VPointF();
    VPointF(const VPointF& point);
    explicit VPointF(const QPointF& point);
    VPointF(
        qreal x,
        qreal y,
        const QString& name,
        qreal mx,
        qreal my,
        quint32 idObject = 0,
        const Draw& mode = Draw::Calculation);
    VPointF(
        const QPointF& point,
        const QString& name,
        qreal mx,
        qreal my,
        quint32 idObject = 0,
        const Draw& mode = Draw::Calculation);
    ~VPointF() override;

    VPointF& operator=(const VPointF& point);
    VPointF& operator=(VPointF&& point) noexcept;

    void Swap(VPointF& point) noexcept;

    explicit operator QPointF() const;
    VPointF
    Rotate(const QPointF& originPoint, qreal degrees, const QString& prefix = QString()) const;
    VPointF Flip(const QLineF& axis, const QString& prefix = QString()) const;
    VPointF Move(qreal length, qreal angle, const QString& prefix = QString()) const;
    qreal mx() const;
    qreal my() const;
    void setMx(qreal mx);
    void setMy(qreal my);
    QPointF toQPointF() const;
    qreal x() const;
    void setX(const qreal& value);
    qreal y() const;
    void setY(const qreal& value);

    bool isShowPointName() const;
    void setShowPointName(bool show);

    static QPointF FlipPF(const QLineF& axis, const QPointF& point);

private:
    QSharedDataPointer<VPointFData> d;
};

Q_DECLARE_METATYPE(VPointF)
Q_DECLARE_TYPEINFO(VPointF, Q_MOVABLE_TYPE);

QT_WARNING_POP

#endif   // VPOINTF_H
