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
 **  @file   vobjpaintdevice.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   6 12, 2014
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

#ifndef VOBJPAINTDEVICE_H
#define VOBJPAINTDEVICE_H

#include <QPaintDevice>
#include <QSharedPointer>
#include <QSize>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

class QIODevice;
class VObjEngine;

class VObjPaintDevice : public QPaintDevice
{
public:
    VObjPaintDevice();
    virtual ~VObjPaintDevice() Q_DECL_OVERRIDE;
    virtual QPaintEngine* paintEngine() const Q_DECL_OVERRIDE;

    QString getFileName() const;
    void setFileName(const QString& value);

    QSize getSize();
    void setSize(const QSize& size);

    QIODevice* getOutputDevice();
    void setOutputDevice(QIODevice* outputDevice);

    int getResolution() const;
    void setResolution(int dpi);

protected:
    virtual int metric(PaintDeviceMetric metric) const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VObjPaintDevice)
    QSharedPointer<VObjEngine> engine;
    QString fileName;
    uint owns_iodevice;
};

#endif   // VOBJPAINTDEVICE_H
