/******************************************************************************
 *   @file   dialogarc.h
 **  @author Douglas S Caskey
 **  @date   21 Mar, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program to create and model patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *****************************************************************************/

/************************************************************************
 **
 **  @file   dialogarc.h
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

#ifndef DIALOGARC_H
#define DIALOGARC_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/varc.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogArc;
}

/**
 * @brief The DialogArc class dialog for ToolArc. Help create arc and edit option.
 */
class DialogArc : public DialogTool
{
    Q_OBJECT
public:
    DialogArc(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    virtual ~DialogArc() override;

    VArc getArc() const;
    void setArc(const VArc& arc);

    quint32 getCenter() const;
    void setCenter(const quint32& value);

    QString getRadius() const;
    void setRadius(const QString& value);

    QString getF1() const;
    void setF1(const QString& value);

    QString getF2() const;
    void setF2(const QString& value);

    QString getPenStyle() const;
    void setPenStyle(const QString& value);

    QString getLineWeight() const;
    void setLineWeight(const QString& value);

    QString getLineColor() const;
    void setLineColor(const QString& value);

public slots:
    virtual void ChosenObject(quint32 id, const SceneObject& type) override;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployFormulaTextEdit();
    void DeployF1TextEdit();
    void DeployF2TextEdit();
    void RadiusChanged();
    void F1Changed();
    void F2Changed();

    void FXRadius();
    void FXF1();
    void FXF2();

protected:
    void pointNameChanged();
    virtual void CheckState() final;
    virtual void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    virtual void SaveData() override;
    virtual void closeEvent(QCloseEvent* event) override;

private:
    Q_DISABLE_COPY(DialogArc)

    /** @brief ui keeps information about user interface */
    Ui::DialogArc* ui;

    /** @brief flagRadius true if value of radius is correct */
    bool flagRadius;

    /** @brief flagF1 true if value of first angle is correct */
    bool flagF1;

    /** @brief flagF2 true if value of second angle is correct */
    bool flagF2;

    /** @brief timerRadius timer of check formula of radius */
    QTimer* timerRadius;

    /** @brief timerF1 timer of check formula of first angle */
    QTimer* timerF1;

    /** @brief timerF2 timer of check formula of second angle */
    QTimer* timerF2;

    /** @brief radius formula of radius */
    QString radius;

    /** @brief f1 formula of first angle */
    QString f1;

    /** @brief f2 formula of second angle */
    QString f2;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;
    int formulaBaseHeightF1;
    int formulaBaseHeightF2;

    qreal angleF1;
    qreal angleF2;
    VArc m_arc;
    qint32 m_Id;
    qint32 newDuplicate;

    void EvalRadius();
    void EvalF();
    void CheckAngles();
};

#endif   // DIALOGARC_H
