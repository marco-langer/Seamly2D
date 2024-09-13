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
 **  @file   anchorpoint_dialog.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   31 1, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Seamly2D project
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

#ifndef DIALOGPIN_H
#define DIALOGPIN_H

#include "dialogtool.h"

namespace Ui {
class AnchorPointDialog;
}

class AnchorPointDialog : public DialogTool
{
    Q_OBJECT
public:
    explicit AnchorPointDialog(const VContainer* data, quint32 toolId, QWidget* parent = nullptr);
    virtual ~AnchorPointDialog();

    void EnbleShowMode(bool disable);

    quint32 GetPieceId() const;
    void SetPieceId(quint32 id);

    quint32 GetPointId() const;
    void SetPointId(quint32 id);

    void SetPiecesList(const QVector<quint32>& list) override;

public slots:
    void ChosenObject(quint32 id, const SceneObject& type) override;

protected:
    void CheckState() final;
    void ShowVisualization() override;

private:
    Q_DISABLE_COPY(AnchorPointDialog)
    Ui::AnchorPointDialog* ui;
    bool m_showMode;
    bool m_flagPoint;

    void CheckPieces();
    void CheckPoint();
};

#endif   // DIALOGPIN_H
