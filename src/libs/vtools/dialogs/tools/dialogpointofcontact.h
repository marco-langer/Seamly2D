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
 **  @file   dialogpointofcontact.h
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

#ifndef DIALOGPOINTOFCONTACT_H
#define DIALOGPOINTOFCONTACT_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "dialogtool.h"
#include "ui_dialogpointofcontact.h"

namespace Ui {
class DialogPointOfContact;
}

/**
 * @brief The DialogPointOfContact class dialog for ToolPointOfContact. Help create point and edit
 * option.
 */
class DialogPointOfContact : public DialogTool
{
    Q_OBJECT
public:
    DialogPointOfContact(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogPointOfContact() override;

    void SetPointName(const QString& value);

    QString getRadius() const;
    void setRadius(const QString& value);

    quint32 getCenter() const;
    void setCenter(const quint32& value);

    quint32 GetFirstPoint() const;
    void SetFirstPoint(const quint32& value);

    quint32 GetSecondPoint() const;
    void SetSecondPoint(const quint32& value);
public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    /**
     * @brief DeployFormulaTextEdit grow or shrink formula input
     */
    void DeployFormulaTextEdit();
    /**
     * @brief FormulaTextChanged when formula text changes for validation and calc
     */
    void FormulaTextChanged();
    void PointNameChanged() override;
    void FXRadius();

protected:
    void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;
    void closeEvent(QCloseEvent* event) override;

private:
    Q_DISABLE_COPY(DialogPointOfContact)

    /** @brief ui keeps information about user interface */
    Ui::DialogPointOfContact* ui;

    /** @brief radius radius of arc */
    QString radius;

    /** @brief formulaBaseHeight base height defined by dialogui */
    int formulaBaseHeight;
};

#endif   // DIALOGPOINTOFCONTACT_H
