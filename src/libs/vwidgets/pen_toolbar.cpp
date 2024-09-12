/******************************************************************************
 *   @file   pen_toolbar.cpp
 **  @author DS Caskey
 **  @date   Jan 16, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
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
 *************************************************************************/

#include "pen_toolbar.h"

#include "../vmisc/logging.h"
#include "../vmisc/vabstractapplication.h"

#include <QToolButton>

Q_LOGGING_CATEGORY(penToolBar, "pentoolbar")

/**
 * Constructor.
 */
PenToolBar::PenToolBar(const QString& title, QWidget* parent)
    : QToolBar(title, parent)
    , m_colorBox(new ColorComboBox{ 40, 14, this, "colorbox" })
    , m_lineTypeBox(new LineTypeComboBox{ 40, 14, this, "lineTypebox" })
    , m_lineWeightBox(new LineWeightComboBox{ 40, 14, this, "lineWeightbox" })
{
    m_currentPen.color = qApp->Settings()->getDefaultLineColor();
    m_currentPen.lineType = qApp->Settings()->getDefaultLineType();
    m_currentPen.lineWeight = qApp->Settings()->getDefaultLineWeight();

    int index = m_colorBox->findData(m_currentPen.color);
    if (index != -1) {
        m_colorBox->setCurrentIndex(index);
    }
    index = m_lineTypeBox->findData(m_currentPen.lineType);
    if (index != -1) {
        m_lineTypeBox->setCurrentIndex(index);
    }
    index = m_lineWeightBox->findData(m_currentPen.lineWeight);
    if (index != -1) {
        m_lineWeightBox->setCurrentIndex(index);
    }

    addWidget(m_colorBox);
    m_colorBox->setToolTip(tr("Current line color"));
    connect(m_colorBox, &ColorComboBox::colorChangedSignal, this, &PenToolBar::colorChanged);

    addWidget(m_lineTypeBox);
    m_lineTypeBox->setToolTip(tr("Current line type"));
    connect(m_lineTypeBox, &LineTypeComboBox::lineTypeChanged, this, &PenToolBar::lineTypeChanged);

    addWidget(m_lineWeightBox);
    m_lineWeightBox->setToolTip(tr("Current line weight"));
    connect(
        m_lineWeightBox,
        &LineWeightComboBox::lineWeightChanged,
        this,
        &PenToolBar::lineWeightChanged);

    QToolButton* resetButton = new QToolButton;
    resetButton->setIcon(QIcon(":/icons/win.icon.theme/24x24/actions/edit-undo.png"));
    resetButton->setToolTip(tr("Reset current pen to defaults"));
    addWidget(resetButton);
    connect(resetButton, &QToolButton::clicked, this, &PenToolBar::penReset);

    QToolButton* savePresetButton = new QToolButton;
    savePresetButton->setIcon(QIcon(":/icons/win.icon.theme/24x24/actions/document-save-as.png"));
    savePresetButton->setToolTip(tr("Save current pen preset"));
    addWidget(savePresetButton);
    connect(savePresetButton, &QToolButton::clicked, this, &PenToolBar::savePreset);
}


/**
 * Called when the linetype was changed by the user.
 */
void PenToolBar::lineTypeChanged(const QString& type)
{
    qCDebug(penToolBar, "PenToolBar::lineTypeChanged - Pen type changed\n");
    m_currentPen.lineType = type;

    emit penChanged(m_currentPen);
}

/**
 * Called when the color was changed by the user.
 */
void PenToolBar::colorChanged(const QString& color)
{
    qCDebug(penToolBar, "PenToolBar::colorChanged - Pen color changed\n");
    m_currentPen.color = color;

    emit penChanged(m_currentPen);
}

/**
 * Called when the width was changed by the user.
 */
void PenToolBar::lineWeightChanged(const qreal& weight)
{
    qCDebug(penToolBar, "PenToolBar::lineWeightChanged - Pen width changed\n");
    m_currentPen.lineWeight = weight;

    emit penChanged(m_currentPen);
}

/**
 * @brief penReset resets the current pen to the preferred defaults.
 */
void PenToolBar::penReset()
{
    qCDebug(penToolBar, "Pen Reset");
    blockSignals(true);

    int index = m_colorBox->findData(qApp->Settings()->getDefaultLineColor());
    if (index != -1) {
        m_colorBox->setCurrentIndex(index);
        m_currentPen.color = m_colorBox->getColor();
    }
    index = m_lineTypeBox->findData(qApp->Settings()->getDefaultLineType());
    if (index != -1) {
        m_lineTypeBox->setCurrentIndex(index);
        m_currentPen.lineType = m_lineTypeBox->getLineType();
    }
    index = m_lineWeightBox->findData(qApp->Settings()->getDefaultLineWeight());
    if (index != -1) {
        m_lineWeightBox->setCurrentIndex(index);
        m_currentPen.lineWeight = m_lineWeightBox->getLineWeight();
    }

    blockSignals(false);
    emit penChanged(m_currentPen);
}

/**
 * savePreset() Save the current pen to the preferences.
 */
void PenToolBar::savePreset()
{
    qApp->Settings()->setDefaultLineColor(m_currentPen.color);
    qApp->Settings()->setDefaultLineType(m_currentPen.lineType);
    qApp->Settings()->setDefaultLineWeight(m_currentPen.lineWeight);
}
