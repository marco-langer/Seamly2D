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
 **  @file   dialogcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
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

#ifndef DIALOGCUBICBEZIERPATH_H
#define DIALOGCUBICBEZIERPATH_H

#include <QMetaObject>
#include <QObject>
#include <QSet>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../vgeometry/vcubicbezierpath.h"
#include "../vgeometry/vpointf.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogCubicBezierPath;
}

class DialogCubicBezierPath : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogCubicBezierPath(
        const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    ~DialogCubicBezierPath() override;

    const VCubicBezierPath& GetPath() const;
    void SetPath(const VCubicBezierPath& value);

    QString getPenStyle() const;
    void setPenStyle(const QString& value);

    QString getLineWeight() const;
    void setLineWeight(const QString& value);

    QString getLineColor() const;
    void setLineColor(const QString& value);


public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void ShowDialog(bool click) override;

protected:
    void ShowVisualization() override;
    void SaveData() override;

private slots:
    void PointChanged(int row);
    void currentPointChanged(int index);

private:
    Q_DISABLE_COPY(DialogCubicBezierPath)
    Ui::DialogCubicBezierPath* ui;

    /** @brief path cubic bezier path */
    VCubicBezierPath path;

    qint32 newDuplicate;

    void NewItem(const VPointF& point);
    void DataPoint(const VPointF& p);
    void SavePath();
    QSet<quint32> AllPathBackboneIds() const;
    bool IsPathValid() const;
    VCubicBezierPath ExtractPath() const;
};

#endif   // DIALOGCUBICBEZIERPATH_H
