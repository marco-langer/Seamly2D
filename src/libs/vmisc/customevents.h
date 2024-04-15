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
 **  @file   customevents.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   23 6, 2014
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

#ifndef CUSTOMEVENTS_H
#define CUSTOMEVENTS_H

#include <QEvent>
#include <qcompilerdetection.h>

enum CustomEventType
{
    UndoEventType = 1,
    LiteParseEventType = 2
};

// Define undo event identifier
const QEvent::Type UNDO_EVENT =
    static_cast<QEvent::Type>(QEvent::User + CustomEventType::UndoEventType);

class UndoEvent : public QEvent
{
public:
    UndoEvent()
        : QEvent(UNDO_EVENT)
    {}

    virtual ~UndoEvent() = default;
};

const QEvent::Type LITE_PARSE_EVENT =
    static_cast<QEvent::Type>(QEvent::User + CustomEventType::LiteParseEventType);

class LiteParseEvent : public QEvent
{
public:
    LiteParseEvent()
        : QEvent(LITE_PARSE_EVENT)
    {}

    virtual ~LiteParseEvent() = default;
};

#endif   // CUSTOMEVENTS_H
