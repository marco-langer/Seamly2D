/***************************************************************************
 **  @file   PreferencesGraphicsViewPage.h
 **  @author Douglas S Caskey
 **  @date   26 Oct, 2023
 **
 **  @copyright
 **  Copyright (C) 2017 - 2023 Seamly, LLC
 **  https://github.com/fashionfreedom/seamly2d
 **
 **  @brief
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
 **  along with Seamly2D. if not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

#ifndef PREFERENCES_GRAPHICSVIEWPAGE_H
#define PREFERENCES_GRAPHICSVIEWPAGE_H

#include <QWidget>

namespace Ui {
class PreferencesGraphicsViewPage;
}

class PreferencesGraphicsViewPage : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesGraphicsViewPage(QWidget* parent = nullptr);
    ~PreferencesGraphicsViewPage() override;

    void Apply();

protected:
    void changeEvent(QEvent* event) override;

private:
    Q_DISABLE_COPY(PreferencesGraphicsViewPage)
    Ui::PreferencesGraphicsViewPage* ui;
    bool m_zrbPositiveColorChanged{ false };
    bool m_zrbNegativeColorChanged{ false };
    bool m_pointNameColorChanged{ false };
    bool m_pointNameHoverColorChanged{ false };
    bool m_orginAxisColorChanged{ false };
    bool m_primarySupportColorChanged{ false };
    bool m_secondarySupportColorChanged{ false };
    bool m_tertiarySupportColorChanged{ false };
};

#endif   // PREFERENCES_GRAPHICSVIEWPAGE_H
