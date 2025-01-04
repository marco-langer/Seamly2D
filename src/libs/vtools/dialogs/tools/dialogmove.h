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
 **  @file   dialogmove.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   30 9, 2016
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

#ifndef DIALOGMOVING_H
#define DIALOGMOVING_H

#include <QList>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "../../tools/drawTools/operation/vabstractoperation.h"
#include "../vmisc/def.h"
#include "dialogtool.h"

namespace Ui {
class DialogMove;
}

class DialogMove : public DialogTool
{
    Q_OBJECT

public:
    explicit DialogMove(const VContainer* data, quint32 toolId, QWidget* parent = nullptr);
    ~DialogMove() override;

    QString GetAngle() const;
    void SetAngle(const QString& value);

    QString GetLength() const;
    void SetLength(const QString& value);

    QString getRotation() const;
    void setRotation(const QString& value);

    QString getSuffix() const;
    void setSuffix(const QString& value);

    quint32 getOriginPointId() const;
    void setOriginPointId(const quint32& value);

    void ShowDialog(bool click) override;

    QVector<SourceItem> getSourceObjects() const;
    void setSourceObjects(const QVector<SourceItem>& value);

public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;
    void SelectedObject(bool selected, quint32 id, quint32 tool) override;

private slots:
    void angleChanged();
    void lengthChanged();
    void rotationChanged();

    void editAngleFormula();
    void editLengthFormula();
    void editRotationFormula();

    void suffixChanged();
    void originChanged(const QString& text);

protected:
    void CheckState() final;
    void ShowVisualization() override;

    /** @brief SaveData Put dialog data in local variables */
    void SaveData() override;
    void closeEvent(QCloseEvent* event) override;

private:
    Q_DISABLE_COPY(DialogMove)
    Ui::DialogMove* ui;
    bool angleFlag;            //! @brief angleFlag true if value of angle is correct */
    QString angleFormula;      //! @brief angle formula of angle */
    QTimer* angleTimer;        //! @brief angleTimer timer of check formula of angle */
    bool lengthFlag;           //! @brief lengthFlag true if value of length is correct */
    QString lengthFormula;     //! @brief length formula of length */
    QTimer* lengthTimer;       //! @brief lengthTimer timer of check formula of length */
    bool rotationFlag;         //! @brief rotationFlag true if value of length is correct */
    QString rotationFormula;   //! @brief rotationFormula of rotation */
    QTimer* rotationTimer;     //! @brief rotationTimer timer of check formula of rotation */
    QVector<SourceItem> m_objects;
    bool stage1;
    bool stage2;
    QString m_suffix;
    bool useOriginPoint;

    void evaluateAngle();
    void evaluateLength();
    void evaluateRotation();
};

#endif   // DIALOGMOVING_H
