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
 **  @file   dialoggroup.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   4 4, 2016
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

#ifndef DIALOGGROUP_H
#define DIALOGGROUP_H

#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "dialogtool.h"

namespace Ui {
class EditGroupDialog;
}

class EditGroupDialog : public DialogTool
{
    Q_OBJECT

public:
    explicit EditGroupDialog(
        const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~EditGroupDialog();

    void SetName(const QString& name);
    QString GetName() const;

    QMap<quint32, quint32> GetGroup() const;

    void ShowDialog(bool click) override;

public slots:
    void SelectedObject(bool selected, quint32 object, quint32 tool) override;

private slots:
    void NameChanged();

private:
    Q_DISABLE_COPY(EditGroupDialog)
    Ui::EditGroupDialog* ui;
    QMap<quint32, quint32> group;
};

#endif   // DIALOGGROUP_H
