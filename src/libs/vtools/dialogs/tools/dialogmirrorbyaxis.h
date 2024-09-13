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
 **  @file   dialogmirrorbyaxis.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 9, 2016
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

#ifndef DIALOGMIRRORBYAXIS_H
#define DIALOGMIRRORBYAXIS_H

#include "dialogtool.h"

#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../../tools/drawTools/operation/vabstractoperation.h"
#include "../vmisc/def.h"

namespace Ui {
class DialogMirrorByAxis;
}

class DialogMirrorByAxis : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogMirrorByAxis(
        const VContainer* data, const quint32& toolId, QWidget* parent = nullptr);
    virtual ~DialogMirrorByAxis();

    quint32 getOriginPointId() const;
    void setOriginPointId(quint32 value);

    AxisType getAxisType() const;
    void setAxisType(AxisType type);

    QString getSuffix() const;
    void setSuffix(const QString& value);

    QVector<SourceItem> getSourceObjects() const;
    void setSourceObjects(const QVector<SourceItem>& value);

    void ShowDialog(bool click) override;

public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void SelectedObject(bool selected, quint32 id, quint32 tool) override;

private slots:
    void suffixChanged();

protected:
    void CheckState() final;
    void ShowVisualization() override;

    /** @brief SaveData Put dialog data in local variables */
    void SaveData() override;

private slots:
    void pointChanged();

private:
    Q_DISABLE_COPY(DialogMirrorByAxis)

    Ui::DialogMirrorByAxis* ui;
    QVector<SourceItem> m_objects;
    bool stage1;
    QString m_suffix;

    static void fillComboBoxAxisType(QComboBox* box);
};

#endif   // DIALOGMIRRORBYAXIS_H
