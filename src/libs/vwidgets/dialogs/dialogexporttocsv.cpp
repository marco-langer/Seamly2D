/******************************************************************************
 *   @file   dialogexporttocsv.cpp
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
**  @file   dialogexporttocsv.cpp
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

#include "dialogexporttocsv.h"
#include "ui_dialogexporttocsv.h"

#include "../vmisc/scassert.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/vcommonsettings.h"

#include <QPushButton>
#include <QShowEvent>
#include <QTextCodec>

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::DialogExportToCSV(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogExportToCSV)
    , isInitialized(false)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->checkBoxWithHeader->setChecked(qApp->Settings()->GetCSVWithHeader());

    for (int mib : QTextCodec::availableMibs()) {
        ui->comboBoxCodec->addItem(QTextCodec::codecForMib(mib)->name(), mib);
    }

    ui->comboBoxCodec->setCurrentIndex(
        ui->comboBoxCodec->findData(qApp->Settings()->GetCSVCodec()));

    SetSeparator(qApp->Settings()->GetCSVSeparator());

    QPushButton* bDefaults = ui->buttonBox->button(QDialogButtonBox::RestoreDefaults);
    SCASSERT(bDefaults != nullptr)
    connect(bDefaults, &QPushButton::clicked, this, [this]() {
        ui->checkBoxWithHeader->setChecked(qApp->Settings()->GetDefCSVWithHeader());
        ui->comboBoxCodec->setCurrentIndex(
            ui->comboBoxCodec->findData(qApp->Settings()->GetDefCSVCodec()));

        SetSeparator(qApp->Settings()->GetDefCSVSeparator());
    });
}

//---------------------------------------------------------------------------------------------------------------------
DialogExportToCSV::~DialogExportToCSV() { delete ui; }

//---------------------------------------------------------------------------------------------------------------------
bool DialogExportToCSV::WithHeader() const { return ui->checkBoxWithHeader->isChecked(); }

//---------------------------------------------------------------------------------------------------------------------
int DialogExportToCSV::SelectedMib() const { return ui->comboBoxCodec->currentData().toInt(); }

//---------------------------------------------------------------------------------------------------------------------
QChar DialogExportToCSV::Separator() const
{
    if (ui->radioButtonTab->isChecked()) {
        return QChar('\t');
    } else if (ui->radioButtonSemicolon->isChecked()) {
        return QChar(';');
    } else if (ui->radioButtonSpace->isChecked()) {
        return QChar(' ');
    } else {
        return QChar(',');
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi(this);
    }

    // remember to call base class implementation
    QDialog::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::showEvent(QShowEvent* event)
{
    QDialog::showEvent(event);
    if (event->spontaneous()) {
        return;
    }

    if (isInitialized) {
        return;
    }
    // do your init stuff here

    setMaximumSize(size());
    setMinimumSize(size());

    isInitialized = true;   // first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void DialogExportToCSV::SetSeparator(const QChar& separator)
{
    switch (separator.toLatin1()) {
    case '\t': ui->radioButtonTab->setChecked(true); break;
    case ';': ui->radioButtonSemicolon->setChecked(true); break;
    case ' ': ui->radioButtonSpace->setChecked(true); break;
    case ',':
    default: ui->radioButtonComma->setChecked(true); break;
    }
}
