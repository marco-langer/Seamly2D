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
 **  @file   dialogpointofintersectioncurves.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 1, 2016
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

#ifndef DIALOGPOINTOFINTERSECTIONCURVES_H
#define DIALOGPOINTOFINTERSECTIONCURVES_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogPointOfIntersectionCurves;
}

class DialogPointOfIntersectionCurves : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogPointOfIntersectionCurves(
        const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogPointOfIntersectionCurves() override;

    void SetPointName(const QString& value);

    quint32 GetFirstCurveId() const;
    void SetFirstCurveId(const quint32& value);

    quint32 GetSecondCurveId() const;
    void SetSecondCurveId(const quint32& value);

    VCrossCurvesPoint GetVCrossPoint() const;
    void SetVCrossPoint(const VCrossCurvesPoint& vP);

    HCrossCurvesPoint GetHCrossPoint() const;
    void SetHCrossPoint(const HCrossCurvesPoint& hP);

public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;

protected:
    void ShowVisualization() override;

    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;
    void CheckState() final;

private slots:
    void CurveChanged();

private:
    Q_DISABLE_COPY(DialogPointOfIntersectionCurves)

    Ui::DialogPointOfIntersectionCurves* ui;
};

#endif   // DIALOGPOINTOFINTERSECTIONCURVES_H
