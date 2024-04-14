/************************************************************************
 **
 **  @file   vdxfpaintdevice.h
 **  @author Valentina Zhuravska <zhuravska19(at)gmail.com>
 **  @date   12 812, 2015
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

#ifndef VDXFPAINTDEVICE_H
#define VDXFPAINTDEVICE_H

#include <QPaintDevice>
#include <QSize>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

#include "dxfdef.h"
#include "libdxfrw/drw_base.h"

class VDxfEngine;
class VLayoutPiece;

class VDxfPaintDevice : public QPaintDevice
{
public:
    VDxfPaintDevice();
    virtual ~VDxfPaintDevice() override;
    virtual QPaintEngine* paintEngine() const override;

    QString getFileName() const;
    void setFileName(const QString& value);

    QSize getSize();
    void setSize(const QSize& size);

    double getResolution() const;
    void setResolution(double dpi);

    DRW::Version GetVersion() const;
    void setVersion(DRW::Version version);

    void SetBinaryFormat(bool binary);
    bool IsBinaryFormat() const;

    void setMeasurement(const VarMeasurement& var);
    void setInsunits(const VarInsunits& var);

    bool ExportToAAMA(const QVector<VLayoutPiece>& details) const;

protected:
    virtual int metric(PaintDeviceMetric metric) const override;

private:
    Q_DISABLE_COPY(VDxfPaintDevice)
    VDxfEngine* engine;
    QString fileName;
};

#endif   // VDXFPAINTDEVICE_H
