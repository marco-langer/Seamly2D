/************************************************************************
 **
 **  @file   vpiecelabeldata.cpp
 **  @author Bojan Kverh
 **  @date   June 16, 2016
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

#include "vpiecelabeldata.h"
#include "vpiecelabeldata_p.h"

#include <QList>

VPieceLabelData& VPieceLabelData::operator=(VPieceLabelData&& data) noexcept
{
    Swap(data);
    return *this;
}

void VPieceLabelData::Swap(VPieceLabelData& data) noexcept
{
    VPatternLabelData::Swap(data);
    std::swap(d, data.d);
}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::VPieceLabelData()
    : VPatternLabelData()
    , d(new VPieceLabelDataPrivate())
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::VPieceLabelData(const VPieceLabelData& data)
    : VPatternLabelData(data)
    , d(data.d)
{}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData& VPieceLabelData::operator=(const VPieceLabelData& data)
{
    if (&data == this) {
        return *this;
    }
    VPatternLabelData::operator=(data);
    d = data.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VPieceLabelData::~VPieceLabelData() = default;

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::Clear() { d->m_qsLetter.clear(); }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetLetter() const { return d->m_qsLetter; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetLetter(const QString& qsLetter) { d->m_qsLetter = qsLetter.left(3); }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetAnnotation() const { return d->m_annotation; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetAnnotation(const QString& val) { d->m_annotation = val; }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetOrientation() const { return d->m_orientation; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetOrientation(const QString& val) { d->m_orientation = val; }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetRotationWay() const { return d->m_rotation; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetRotationWay(const QString& val) { d->m_rotation = val; }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetTilt() const { return d->m_tilt; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetTilt(const QString& val) { d->m_tilt = val; }

//---------------------------------------------------------------------------------------------------------------------
QString VPieceLabelData::GetFoldPosition() const { return d->m_foldPosition; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetFoldPosition(const QString& val) { d->m_foldPosition = val; }

//---------------------------------------------------------------------------------------------------------------------
int VPieceLabelData::GetQuantity() const { return d->m_quantity; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetQuantity(int val)
{
    if (val >= 1) {
        d->m_quantity = val;
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VPieceLabelData::IsOnFold() const { return d->m_onFold; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetOnFold(bool onFold) { d->m_onFold = onFold; }

//---------------------------------------------------------------------------------------------------------------------
QVector<VLabelTemplateLine> VPieceLabelData::GetLabelTemplate() const { return d->m_lines; }

//---------------------------------------------------------------------------------------------------------------------
void VPieceLabelData::SetLabelTemplate(const QVector<VLabelTemplateLine>& lines)
{
    d->m_lines = lines;
}
