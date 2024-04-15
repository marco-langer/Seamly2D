/************************************************************************
 **
 **  @file   vpiecelabeldata.h
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

#ifndef VPATTERNPIECEDATA_H
#define VPATTERNPIECEDATA_H

#include <QString>

#include "floatitemdef.h"
#include "vpatternlabeldata.h"

class VPieceLabelDataPrivate;
struct VLabelTemplateLine;

/**
 * @brief The VPieceLabelData class holds some information about a single
 * piece like letter, name, material type, cut number and placement.
 */
class VPieceLabelData : public VPatternLabelData
{
public:
    VPieceLabelData();
    VPieceLabelData(const VPieceLabelData& data);

    virtual ~VPieceLabelData();

    VPieceLabelData& operator=(const VPieceLabelData& data);
#ifdef Q_COMPILER_RVALUE_REFS
    VPieceLabelData& operator=(VPieceLabelData&& data) Q_DECL_NOTHROW;
#endif

    void Swap(VPieceLabelData& data) Q_DECL_NOTHROW;

    void Clear();

    // methods, which operate on other members
    QString GetLetter() const;
    void SetLetter(const QString& qsLetter);

    QString GetAnnotation() const;
    void SetAnnotation(const QString& val);

    QString GetOrientation() const;
    void SetOrientation(const QString& val);

    QString GetRotationWay() const;
    void SetRotationWay(const QString& val);

    QString GetTilt() const;
    void SetTilt(const QString& val);

    QString GetFoldPosition() const;
    void SetFoldPosition(const QString& val);

    int GetQuantity() const;
    void SetQuantity(int val);

    bool IsOnFold() const;
    void SetOnFold(bool onFold);

    QVector<VLabelTemplateLine> GetLabelTemplate() const;
    void SetLabelTemplate(const QVector<VLabelTemplateLine>& lines);

private:
    QSharedDataPointer<VPieceLabelDataPrivate> d;
};

#endif   // VPATTERNPIECEDATA_H
