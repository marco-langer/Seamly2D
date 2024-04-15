/************************************************************************
 **
 **  @file   vgrainlinedata.h
 **  @author Bojan Kverh
 **  @date   September 06, 2016
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

#ifndef VGRAINLINEGEOMETRY_H
#define VGRAINLINEGEOMETRY_H

#include <QPointF>
#include <QString>

#include "floatitemdef.h"
#include "vabstractfloatitemdata.h"

class VGrainlineDataPrivate;

/**
 * @brief The VGrainlineData class holds information about a grainline like
 * position, size, rotation and visibility
 */
class VGrainlineData : public VAbstractFloatItemData
{
public:
    VGrainlineData();
    VGrainlineData(const VGrainlineData& data);

    virtual ~VGrainlineData();

    VGrainlineData& operator=(const VGrainlineData& data);
#ifdef Q_COMPILER_RVALUE_REFS
    VGrainlineData& operator=(VGrainlineData&& data) Q_DECL_NOTHROW;
#endif

    void Swap(VGrainlineData& data) Q_DECL_NOTHROW;

    // methods, which set and return values of different parameters
    QString GetLength() const;
    void SetLength(const QString& qsLen);

    QString GetRotation() const;
    void SetRotation(const QString& qsRot);

    ArrowType GetArrowType() const;
    void SetArrowType(ArrowType eAT);

    quint32 centerAnchorPoint() const;
    void setCenterAnchorPoint(quint32 centerAnchor);

    quint32 topAnchorPoint() const;
    void setTopAnchorPoint(quint32 topAnchorPoint);

    quint32 bottomAnchorPoint() const;
    void setBottomAnchorPoint(quint32 bottomAnchorPoint);

private:
    QSharedDataPointer<VGrainlineDataPrivate> d;
};

#endif   // VGRAINLINEGEOMETRY_H
