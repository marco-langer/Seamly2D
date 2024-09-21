/************************************************************************
 **  @file   main.cpp
 **  @author Douglas S Caskey
 **  @date   5 Jan, 2024
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
 ************************************************************************

 ************************************************************************
 **
 **  @file   main.cpp
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

/*
 * @brief entry point of Seamly2D application
 * @return non-zero value is code of the error
 */

#include "../vpatterndb/vpiecenode.h"
#include "core/application_2d.h"
#include "dialogs/welcome_dialog.h"
#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>   // For QT_REQUIRE_VERSION
#include <QTimer>

//---------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE(cursor);
    Q_INIT_RESOURCE(icon);
    Q_INIT_RESOURCE(schema);
    Q_INIT_RESOURCE(theme);
    Q_INIT_RESOURCE(flags);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(toolicon);
    Q_INIT_RESOURCE(sounds);

    QT_REQUIRE_VERSION(argc, argv, "5.15.2");

    // Need to internally move a node inside a piece main path
    qRegisterMetaTypeStreamOperators<VPieceNode>("VPieceNode");

    //------------------------------------------------------------------------
    // On macOS, correct WebView / QtQuick compositing and stacking requires running
    // Qt in layer-backed mode, which again requires rendering on the Gui thread.
    qWarning("Seamly2D: Setting QT_MAC_WANTS_LAYER=1 and QSG_RENDER_LOOP=basic");
    qputenv("QT_MAC_WANTS_LAYER", "1");
    //------------------------------------------------------------------------

#ifndef Q_OS_MAC   // supports natively
    initHighDpiScaling(argc, argv);
#endif   // Q_OS_MAC

    Application2D app(argc, argv);
    app.initOptions();

    auto settings = qApp->Seamly2DSettings();
    // its named showWelcome, but true means "do not show welcome again" and thus we invert it here
    bool showWelcome = !settings->getShowWelcome();

    if (showWelcome) {
        SeamlyWelcomeDialog* dialog = new SeamlyWelcomeDialog();
        dialog->setAttribute(Qt::WA_DeleteOnClose, true);
        dialog->exec();
        app.loadTranslations(settings->getLocale());
    }

    MainWindow window;
#if !defined(Q_OS_MAC)
    app.setWindowIcon(QIcon(":/icon/64x64/icon64x64.png"));
#endif   // !defined(Q_OS_MAC)
    app.setMainWindow(&window);

    int msec = 0;
    // Before we load pattern show window.
    if (Application2D::isGUIMode()) {
        window.show();
        msec = 15;   // set delay for correct the first fitbest zoom
    }

    QTimer::singleShot(msec, &window, &MainWindow::processCommandLine);

    return app.exec();
}
