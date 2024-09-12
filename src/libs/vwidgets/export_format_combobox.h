/******************************************************************************
 *   @file   export_format_combobox.h
 **  @author DS Caskey
 **  @date   Mar 15, 2022
 **
 **  @brief
 **  @copyright
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
 *****************************************************************************/


#ifndef EXPORT_FORMAT_COMBOBOX_H
#define EXPORT_FORMAT_COMBOBOX_H

#include <QComboBox>
#include <QWidget>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"

/**
 * A comboBox for choosing an export format type.
 */
class ExportFormatCombobox : public QComboBox
{
    Q_OBJECT

public:
    explicit ExportFormatCombobox(QWidget* parent = nullptr, const char* name = nullptr);

    LayoutExportFormat getExportFormat() const;
    void setExportFormat(LayoutExportFormat& format);
    static QString exportFormatDescription(LayoutExportFormat format);
    static QString makeHelpFormatList();
    void setCurrentToDefault();

private slots:
    void updateExportFormat(int index);

signals:
    void exportFormatChanged(const LayoutExportFormat& format);

private:
    void init();
    static QVector<std::pair<QString, LayoutExportFormat>> initFormats();
    static bool supportPSTest();
    static bool testPdf();

    static bool havePdf;
    static bool tested;
    LayoutExportFormat m_currentFormat{ LayoutExportFormat::SVG };
};

#endif   // EXPORT_FORMAT_COMBOBOX_H
