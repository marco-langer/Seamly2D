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
 **  @file   stable.h
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

#ifndef STABLE_H
#define STABLE_H

/* Add C includes here */

#if defined __cplusplus
/* Add C++ includes here */
#    include <csignal>

/*In all cases we need include core header for getting defined values*/
#    ifdef QT_CORE_LIB
#        include <QtCore>
#    endif

#    ifdef QT_GUI_LIB
#        include <QtGui>
#    endif

#    ifdef QT_XML_LIB
#        include <QtXml>
#    endif

// In Windows you can't use same header in all modes.
#    if !defined(Q_OS_WIN)
#        ifdef QT_WIDGETS_LIB
#            include <QtWidgets>
#        endif

// Build doesn't work, if include this headers on Windows.
#        ifdef QT_XMLPATTERNS_LIB
#            include <QtXmlPatterns>
#        endif

#        ifdef QT_NETWORK_LIB
#            include <QtNetwork>
#        endif
#    endif /*Q_OS_WIN*/

#endif /*__cplusplus*/

#endif   // STABLE_H
