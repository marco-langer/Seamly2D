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
 **  @file   dialogtruedarts.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 6, 2015
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

#ifndef DIALOGTRUEDARTS_H
#define DIALOGTRUEDARTS_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogTrueDarts;
}

class DialogTrueDarts : public DialogTool
{
    Q_OBJECT

public:
    DialogTrueDarts(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogTrueDarts() override;

    QString GetFirstNewDartPointName();
    QString GetSecondNewDartPointName();
    void SetNewDartPointNames(const QString& firstPoint, const QString& secondPoint);

    quint32 GetFirstBasePointId() const;
    void SetFirstBasePointId(const quint32& value);

    quint32 GetSecondBasePointId() const;
    void SetSecondBasePointId(const quint32& value);

    quint32 GetFirstDartPointId() const;
    void SetFirstDartPointId(const quint32& value);

    quint32 GetSecondDartPointId() const;
    void SetSecondDartPointId(const quint32& value);

    quint32 GetThirdDartPointId() const;
    void SetThirdDartPointId(const quint32& value);

    void SetChildrenId(const quint32& ch1, const quint32& ch2);
public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void PointNameChanged() override;
    void NameDartPoint1Changed();
    void NameDartPoint2Changed();

protected:
    void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;
    void CheckState() final;

private:
    Q_DISABLE_COPY(DialogTrueDarts)
    Ui::DialogTrueDarts* ui;

    QString d1PointName;
    QString d2PointName;

    quint32 ch1;
    quint32 ch2;

    bool flagName1;
    bool flagName2;

    void NameChanged(
        QLabel* labelEditNamePoint,
        const QString& pointD1Name,
        const QString& pointD2Name,
        QLineEdit* secondPointName,
        bool& flagName);

    void FillComboBoxs(const quint32& ch1, const quint32& ch2);

    void CheckName(
        QLineEdit* edit,
        QLabel* labelEditNamePoint,
        const QString& pointD1Name,
        const QString& pointD2Name,
        QLineEdit* secondPointName,
        bool& flagName);
};

#endif   // DIALOGTRUEDARTS_H
