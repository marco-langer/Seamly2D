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
 **  @file   vlitepattern.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   9 8, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Seamly2D project
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

#ifndef VLITEPATTERN_H
#define VLITEPATTERN_H

#include "../ifc/xml/vabstractpattern.h"

class VLitePattern : public VAbstractPattern
{
    Q_OBJECT
public:
    explicit VLitePattern(QObject* parent = nullptr);

    void CreateEmptyFile() override;

    void IncrementReferens(quint32 id) const override;
    void DecrementReferens(quint32 id) const override;
    QStringList GetCurrentAlphabet() const override;
    QString
    GenerateLabel(const LabelType& type, const QString& reservedName = QString()) const override;
    QString GenerateSuffix(const QString& type) const override;

    void UpdateToolData(const quint32& id, VContainer* data) override;

public slots:
    void LiteParseTree(const Document& parse) override;

private:
    Q_DISABLE_COPY(VLitePattern)
};

#endif   // VLITEPATTERN_H
