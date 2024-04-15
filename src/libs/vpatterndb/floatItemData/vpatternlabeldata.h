/************************************************************************
 **
 **  @file   vpatternlabeldata.h
 **  @author Bojan Kverh
 **  @date   June 16, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2022 Seamly2D project
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

#ifndef VPATTERNINFOGEOMETRY_H
#define VPATTERNINFOGEOMETRY_H

#include <QPointF>

#include "vabstractfloatitemdata.h"

class VPatternLabelDataPrivate;

/**
 * @brief The VPatternLabelData class holds the information about pattern info label geometry
 */
class VPatternLabelData : public VAbstractFloatItemData
{
public:
    VPatternLabelData();
    VPatternLabelData(const VPatternLabelData& data);

    virtual ~VPatternLabelData();

    VPatternLabelData& operator=(const VPatternLabelData& data);
#ifdef Q_COMPILER_RVALUE_REFS
    VPatternLabelData& operator=(VPatternLabelData&& data) noexcept;
#endif

    void Swap(VPatternLabelData& data) noexcept;

    // methods, which set up label parameters
    QString GetLabelWidth() const;
    void SetLabelWidth(const QString& dLabelW);

    QString GetLabelHeight() const;
    void SetLabelHeight(const QString& dLabelH);

    int getFontSize() const;
    void SetFontSize(int iSize);

    QString GetRotation() const;
    void SetRotation(const QString& dRot);

    quint32 centerAnchorPoint() const;
    void setCenterAnchorPoint(const quint32& centerAnchorPoint);

    quint32 topLeftAnchorPoint() const;
    void setTopLeftAnchorPoint(const quint32& topLeftAnchorPoint);

    quint32 bottomRightAnchorPoint() const;
    void setBottomRightAnchorPoint(const quint32& bottomRightAnchorPoint);

private:
    QSharedDataPointer<VPatternLabelDataPrivate> d;
};

#endif   // VPATTERNINFOGEOMETRY_H
