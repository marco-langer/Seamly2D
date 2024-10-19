/******************************************************************************
 *   @file   preferencesconfigurationpage.h
 **  @author Douglas S Caskey
 **  @date   26 Oct, 2023
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
 **  @file   preferencesconfigurationpage.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 4, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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
 **  along with Valentina.  if not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef PREFERENCESCONFIGURATIONPAGE_H
#define PREFERENCESCONFIGURATIONPAGE_H

#include <QWidget>

namespace Ui {
class PreferencesConfigurationPage;
}

class PreferencesConfigurationPage : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesConfigurationPage(QWidget* parent = nullptr);
    ~PreferencesConfigurationPage() override;

    void Apply();

protected:
    void changeEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY(PreferencesConfigurationPage)
    Ui::PreferencesConfigurationPage* ui;
    bool m_langChanged{ false };
    bool m_unitChanged{ false };
    bool m_labelLangChanged{ false };
    bool m_selectionSoundChanged{ false };
    bool m_moveSuffixChanged{ false };
    bool m_rotateSuffixChanged{ false };
    bool m_mirrorByAxisSuffixChanged{ false };
    bool m_mirrorByLineSuffixChanged{ false };
    bool m_defaultExportFormatChanged{ false };

    void setPointNameComboBox(const QStringList& list);
    void InitUnits();
    void adjustTextColor();
};

#endif   // PREFERENCESCONFIGURATIONPAGE_H
