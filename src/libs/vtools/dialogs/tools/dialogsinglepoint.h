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
 **  @file   dialogsinglepoint.h
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

#ifndef DIALOGSINGLEPOINT_H
#define DIALOGSINGLEPOINT_H

#include <QMetaObject>
#include <QObject>
#include <QPointF>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "dialogtool.h"

namespace Ui {
class DialogSinglePoint;
}

/**
 * @brief The DialogSinglePoint class dialog for ToolSinglePoint. Help create point and edit option.
 */
class DialogSinglePoint : public DialogTool
{
    Q_OBJECT
public:
    DialogSinglePoint(const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogSinglePoint() override;

    void SetData(const QString& name, const QPointF& point);
    const QPointF& GetPoint() const;

public slots:
    void mousePress(const QPointF& scenePos);

protected:
    /**
     * @brief SaveData Put dialog data in local variables
     */
    void SaveData() override;

private:
    Q_DISABLE_COPY(DialogSinglePoint)

    /** @brief ui keeps information about user interface */
    Ui::DialogSinglePoint* ui;

    /** @brief point data of point */
    QPointF point;
};

#endif   // DIALOGSINGLEPOINT_H
