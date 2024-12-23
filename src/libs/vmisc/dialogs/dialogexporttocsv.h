/******************************************************************************
 *   @file   dialogexporttocsv.h
 **  @author Douglas S Caskey
 **  @date   12 oct, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program to create and model patterns of clothing.
 **  Copyright (C) 2017-2023 Seamly2D project
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
 *************************************************************************

************************************************************************
**
**  @file   dialogexporttocsv.h
**  @author Roman Telezhynskyi <dismine(at)gmail.com>
**  @date   1 6, 2016
**
**  @brief
**  @copyright
**  This source code is part of the Valentina project, a pattern making
**  program, whose allow create and modeling patterns of clothing.
**  Copyright (C) 2016 Valentina project
**  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
**
**  Valentina is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Valentina is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Valentina. If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#ifndef DIALOGEXPORTTOCSV_H
#define DIALOGEXPORTTOCSV_H

#include <QDialog>

namespace Ui {
class DialogExportToCSV;
}

class DialogExportToCSV : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExportToCSV(QWidget* parent = nullptr);
    virtual ~DialogExportToCSV();

    bool WithHeader() const;
    int SelectedMib() const;
    QChar Separator() const;

protected:
    void changeEvent(QEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    Q_DISABLE_COPY(DialogExportToCSV)
    Ui::DialogExportToCSV* ui;
    bool isInitialized;

    void SetSeparator(const QChar& separator);
};

#endif   // DIALOGEXPORTTOCSV_H
