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
 **  @file   varcradius.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 5, 2015
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

#ifndef VARCRADIUS_H
#define VARCRADIUS_H

#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/ifcdef.h"
#include "../vmisc/def.h"
#include "vcurvevariable.h"

class VArc;
class VEllipticalArc;

class VArcRadius : public VCurveVariable
{
public:
    VArcRadius();
    VArcRadius(const quint32& id, const quint32& parentId, const VArc* arc, Unit patternUnit);
    VArcRadius(
        const quint32& id,
        const quint32& parentId,
        const VEllipticalArc* elArc,
        const int numberRadius,
        Unit patternUnit);
    VArcRadius(const VArcRadius& var);
    VArcRadius& operator=(const VArcRadius& var);
    virtual ~VArcRadius() Q_DECL_OVERRIDE;
};

#endif   // VARCRADIUS_H
