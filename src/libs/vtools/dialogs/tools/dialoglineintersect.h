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
 **  @file   dialoglineintersect.h
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

#ifndef DIALOGLINEINTERSECT_H
#define DIALOGLINEINTERSECT_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogLineIntersect;
}

/**
 * @brief The DialogLineIntersect class dialog for ToolLineIntersect. Help create point and edit
 * option.
 */
class DialogLineIntersect : public DialogTool
{
    Q_OBJECT
public:
    DialogLineIntersect(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogLineIntersect() override;

    quint32 GetP1Line1() const;
    void SetP1Line1(const quint32& value);

    quint32 GetP2Line1() const;
    void SetP2Line1(const quint32& value);

    quint32 GetP1Line2() const;
    void SetP1Line2(const quint32& value);

    quint32 GetP2Line2() const;
    void SetP2Line2(const quint32& value);

    void SetPointName(const QString& value);
public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void PointChanged();
    void PointNameChanged() override;

protected:
    void ShowVisualization() override;
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;

private:
    Q_DISABLE_COPY(DialogLineIntersect)

    /** @brief ui keeps information about user interface */
    Ui::DialogLineIntersect* ui;

    /** @brief flagPoint keep state of point */
    bool flagPoint;

    void CheckState() final;
    bool CheckIntersecion();
};

#endif   // DIALOGLINEINTERSECT_H
