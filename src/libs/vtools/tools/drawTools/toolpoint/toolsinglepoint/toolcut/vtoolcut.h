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
 **  @file   vtoolcut.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   25 6, 2014
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

#ifndef VTOOLCUT_H
#define VTOOLCUT_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../../../../../visualization/visualization.h"
#include "../../../../vdatatool.h"
#include "../../../toolcurve/vabstractspline.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "../vtoolsinglepoint.h"

class VFormula;

class VToolCut : public VToolSinglePoint
{
    Q_OBJECT
public:
    VToolCut(
        VAbstractPattern* doc,
        VContainer* data,
        const quint32& id,
        const QString& formula,
        const quint32& curveCutId,
        QGraphicsItem* parent = nullptr);

    int type() const override { return Type; }
    enum
    {
        Type = UserType + static_cast<int>(Tool::Cut)
    };

    VFormula GetFormula() const;
    void SetFormula(const VFormula& value);

    QString CurveName() const;

    quint32 getCurveCutId() const;
    void setCurveCutId(const quint32& value);

public slots:
    void Disable(bool disable, const QString& draftBlockName) override;
    void piecesMode(bool mode) override;
    void FullUpdateFromFile() override;

protected:
    /** @brief formula keep formula of length */
    QString formula;

    quint32 curveCutId;
    bool m_piecesMode;

    void RefreshGeometry();
    void RemoveReferens() override;

    template <typename T>
    void ShowToolVisualization(bool show);

private:
    Q_DISABLE_COPY(VToolCut)
};

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void VToolCut::ShowToolVisualization(bool show)
{
    if (show) {
        if (vis.isNull()) {
            AddVisualization<T>();
            SetVisualization();
        } else {
            if (T* visual = qobject_cast<T*>(vis)) {
                visual->show();
            }
        }
    } else {
        delete vis;
    }

    VDataTool* parent =
        VAbstractPattern::getTool(VAbstractTool::data.GetGObject(curveCutId).getIdTool());
    if (VAbstractSpline* parentCurve = qobject_cast<VAbstractSpline*>(parent)) {
        m_piecesMode ? parentCurve->ShowHandles(m_piecesMode) : parentCurve->ShowHandles(show);
    }
}
#endif   // VTOOLCUT_H
