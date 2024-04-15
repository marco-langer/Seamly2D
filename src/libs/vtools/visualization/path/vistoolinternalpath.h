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
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   22 11, 2016
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

#ifndef VISTOOLINTERNALPATH_H
#define VISTOOLINTERNALPATH_H

#include <QtCore/QObject>
#include <QtGlobal>

#include "../vpatterndb/vpiecepath.h"
#include "vispath.h"

class VSimplePoint;

class VisToolInternalPath : public VisPath
{
    Q_OBJECT
public:
    VisToolInternalPath(const VContainer* data, QGraphicsItem* parent = nullptr);
    virtual ~VisToolInternalPath() = default;

    virtual void RefreshGeometry() override;
    void SetPath(const VPiecePath& path);
    virtual int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Vis::ToolInternalPath)
    };

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Q_DISABLE_COPY(VisToolInternalPath)
    QVector<VSimplePoint*> m_points;

    VScaledLine* m_line;

    VPiecePath m_path;

    VSimplePoint* GetPoint(quint32 i, const QColor& color);

    void HideAllItems();
};

#endif   // VISTOOLINTERNALPATH_H
