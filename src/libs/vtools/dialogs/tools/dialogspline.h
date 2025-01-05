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
 **  @file   dialogspline.h
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

#ifndef DIALOGSPLINE_H
#define DIALOGSPLINE_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vspline.h"
#include "../vmisc/def.h"
#include "dialogtool.h"


namespace Ui {
class DialogSpline;
}

/**
 * @brief The DialogSpline class dialog for ToolSpline. Help create spline and edit option.
 */
class DialogSpline : public DialogTool
{
    Q_OBJECT
public:
    DialogSpline(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogSpline() override;

    const VSpline& GetSpline() const;
    void SetSpline(const VSpline& spline);

    QString getPenStyle() const;
    void setPenStyle(const QString& value);

    QString getLineWeight() const;
    void setLineWeight(const QString& value);

    QString getLineColor() const;
    void setLineColor(const QString& value);

public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void PointNameChanged() override;
    void ShowDialog(bool click) override;

protected:
    void CheckState() final;
    void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void DeployAngle1TextEdit();
    void DeployAngle2TextEdit();
    void DeployLength1TextEdit();
    void DeployLength2TextEdit();

    void Angle1Changed();
    void Angle2Changed();
    void Length1Changed();
    void Length2Changed();

    void FXAngle1();
    void FXAngle2();
    void FXLength1();
    void FXLength2();

private:
    Q_DISABLE_COPY(DialogSpline)

    Ui::DialogSpline* ui; /** @brief ui keeps information about user interface */
    VSpline spl;          /** @brief spl spline */
    qint32 newDuplicate;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeightAngle1;
    int formulaBaseHeightAngle2;
    int formulaBaseHeightLength1;
    int formulaBaseHeightLength2;

    /** @brief timerAngle1 timer of check first angle formula */
    QTimer* timerAngle1;
    QTimer* timerAngle2;
    QTimer* timerLength1;
    QTimer* timerLength2;

    /** @brief flagAngle1 true if value of first angle is correct */
    bool flagAngle1;
    bool flagAngle2;
    bool flagLength1;
    bool flagLength2;

    const VPointF& GetP1() const;
    const VPointF& GetP4() const;

    void EvalAngle1();
    void EvalAngle2();
    void EvalLength1();
    void EvalLength2();

    VSpline CurrentSpline() const;
};

#endif   // DIALOGSPLINE_H
