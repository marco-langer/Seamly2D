/******************************************************************************
 *   @file   dialogellipticalarc.h
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
 *****************************************************************************

 ************************************************************************
 **
 **  @file   dialogellipticalarc.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   15 9, 2016
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

#ifndef DIALOGELLIPTICALARC_H
#define DIALOGELLIPTICALARC_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vellipticalarc.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogEllipticalArc;
}

class DialogEllipticalArc : public DialogTool
{
    Q_OBJECT
public:
    DialogEllipticalArc(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogEllipticalArc() override;

    const VEllipticalArc& getArc() const;
    void setArc(const VEllipticalArc& arc);

    quint32 GetCenter() const;
    void SetCenter(const quint32& value);

    QString GetRadius1() const;
    void SetRadius1(const QString& value);

    QString GetRadius2() const;
    void SetRadius2(const QString& value);

    QString GetF1() const;
    void SetF1(const QString& value);

    QString GetF2() const;
    void SetF2(const QString& value);

    QString GetRotationAngle() const;
    void SetRotationAngle(const QString& value);

    QString getPenStyle() const;
    void setPenStyle(const QString& value);

    QString getLineWeight() const;
    void setLineWeight(const QString& value);

    QString getLineColor() const;
    void setLineColor(const QString& value);


public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployRadius1TextEdit();
    void DeployRadius2TextEdit();
    void DeployF1TextEdit();
    void DeployF2TextEdit();
    void DeployRotationAngleTextEdit();

    void Radius1Changed();
    void Radius2Changed();
    void F1Changed();
    void F2Changed();
    void RotationAngleChanged();

    void FXRadius1();
    void FXRadius2();
    void FXF1();
    void FXF2();
    void FXRotationAngle();

protected:
    void pointNameChanged();
    void CheckState() final;
    void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;
    void closeEvent(QCloseEvent* event) override;

private:
    Q_DISABLE_COPY(DialogEllipticalArc)

    /** @brief ui keeps information about user interface */
    Ui::DialogEllipticalArc* ui;

    /** @brief flagRadius1 true if value of radius1 is correct */
    bool flagRadius1;

    /** @brief flagRadius2 true if value of radius2 is correct */
    bool flagRadius2;

    /** @brief flagF1 true if value of first angle is correct */
    bool flagF1;

    /** @brief flagF2 true if value of second angle is correct */
    bool flagF2;

    /** @brief flagRotationAngle true if value of rotation angle is correct */
    bool flagRotationAngle;

    /** @brief timerRadius1 timer of check formula of radius1 */
    QTimer* timerRadius1;

    /** @brief timerRadius2 timer of check formula of radius2 */
    QTimer* timerRadius2;

    /** @brief timerF1 timer of check formula of first angle */
    QTimer* timerF1;

    /** @brief timerF2 timer of check formula of second angle */
    QTimer* timerF2;

    /** @brief timerRotationAngle timer of check formula of rotation angle */
    QTimer* timerRotationAngle;

    /** @brief radius1 formula of radius1 */
    QString radius1;

    /** @brief radius2 formula of radius2 */
    QString radius2;

    /** @brief f1 formula of first angle */
    QString f1;

    /** @brief f2 formula of second angle */
    QString f2;

    /** @brief rotationAngle formula of rotation angle */
    QString rotationAngle;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeightRadius1;
    int formulaBaseHeightRadius2;
    int formulaBaseHeightF1;
    int formulaBaseHeightF2;
    int formulaBaseHeightRotationAngle;

    qreal angleF1;
    qreal angleF2;
    qreal angleRotation;
    VEllipticalArc m_arc;
    qint32 m_Id;
    qint32 newDuplicate;

    void EvalRadiuses();
    void EvalAngles();
    void CheckAngles();
    void collapseFormula(QPlainTextEdit* textEdit, QPushButton* pushButton, int height);
};

#endif   // DIALOGELLIPTICALARC_H
