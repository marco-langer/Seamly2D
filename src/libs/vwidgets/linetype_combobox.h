/******************************************************************************
 *   linetype_combobox.h
 **  @author DS Caskey
 **  @date   April 6, 2021
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

#ifndef LINETYPE_COMBOBOX_H
#define LINETYPE_COMBOBOX_H

#include "../ifc/ifcdef.h"

#include <QComboBox>
#include <QIcon>
#include <QPen>
#include <QWidget>


class QIcon;

/**
 * A comboBox for choosing a line type.
 */
class LineTypeComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit LineTypeComboBox(QWidget* parent = nullptr);
    LineTypeComboBox(int width, int height, QWidget* parent = nullptr, const char* name = nullptr);

    QString getLineType() const { return m_currentLineType; }
    void setLineType(const QString& type);

private slots:
    void updateLineType(int index);

signals:
    void lineTypeChanged(const QString& type);

private:
    void init();
    QIcon createIcon(const QString& type);

    QString m_currentLineType{ LineTypeSolidLine };
    int m_iconWidth{ 40 };
    int m_iconHeight{ 14 };
};

#endif
