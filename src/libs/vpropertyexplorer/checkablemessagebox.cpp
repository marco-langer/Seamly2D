/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** All rights reserved. This program and the accompanying materials are made
** available under the terms of the GNU Lesser General Public License (LGPL)
** version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful, but WITHOUT
** ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
** FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
** for more details.
**
****************************************************************************/

#include "checkablemessagebox.h"

#include <QAbstractButton>
#include <QApplication>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLatin1String>
#include <QPushButton>
#include <QSettings>
#include <QSize>
#include <QSizePolicy>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QVariant>
#include <Qt>

static const char kDoNotAskAgainKey[] = "DoNotAskAgain";

namespace Utils {

/*!
    \class CheckableMessageBox

    \brief The CheckableMessageBox class implements a message box suitable for
    questions with a
     "Do not ask me again" checkbox.

    Emulates the QMessageBox API with
    static conveniences. The message label can open external URLs.
*/
class CheckableMessageBoxPrivate
{
public:
    explicit CheckableMessageBoxPrivate(QDialog* q)
        : pixmapLabel(nullptr)
        , messageLabel(nullptr)
        , checkBox(nullptr)
        , buttonBox(nullptr)
        , clickedButton(nullptr)
    {
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

        pixmapLabel = new QLabel(q);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pixmapLabel->sizePolicy().hasHeightForWidth());
        pixmapLabel->setSizePolicy(sizePolicy);
        pixmapLabel->setVisible(false);

        QSpacerItem* pixmapSpacer =
            new QSpacerItem(0, 5, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        messageLabel = new QLabel(q);
        messageLabel->setMinimumSize(QSize(300, 0));
        messageLabel->setWordWrap(true);
        messageLabel->setOpenExternalLinks(true);
        messageLabel->setTextInteractionFlags(
            Qt::LinksAccessibleByKeyboard | Qt::LinksAccessibleByMouse);

        QSpacerItem* checkBoxRightSpacer =
            new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QSpacerItem* buttonSpacer =
            new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Minimum);

        checkBox = new QCheckBox(q);
        checkBox->setText(CheckableMessageBox::tr("Do not ask again"));

        buttonBox = new QDialogButtonBox(q);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        QVBoxLayout* verticalLayout = new QVBoxLayout();
        verticalLayout->addWidget(pixmapLabel);
        verticalLayout->addItem(pixmapSpacer);

        QHBoxLayout* horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->addLayout(verticalLayout);
        horizontalLayout_2->addWidget(messageLabel);

        QHBoxLayout* horizontalLayout = new QHBoxLayout();
        horizontalLayout->addWidget(checkBox);
        horizontalLayout->addItem(checkBoxRightSpacer);

        QVBoxLayout* verticalLayout_2 = new QVBoxLayout(q);
        verticalLayout_2->addLayout(horizontalLayout_2);
        verticalLayout_2->addLayout(horizontalLayout);
        verticalLayout_2->addItem(buttonSpacer);
        verticalLayout_2->addWidget(buttonBox);
    }

    QLabel* pixmapLabel;
    QLabel* messageLabel;
    QCheckBox* checkBox;
    QDialogButtonBox* buttonBox;
    QAbstractButton* clickedButton;

private:
    Q_DISABLE_COPY(CheckableMessageBoxPrivate)
};

CheckableMessageBox::CheckableMessageBox(QWidget* parent)
    : QDialog(parent)
    , d(new CheckableMessageBoxPrivate(this))
{
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(d->buttonBox, SIGNAL(accepted()), SLOT(accept()));
    connect(d->buttonBox, SIGNAL(rejected()), SLOT(reject()));
    connect(d->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(slotClicked(QAbstractButton*)));
}

CheckableMessageBox::~CheckableMessageBox() { delete d; }

void CheckableMessageBox::slotClicked(QAbstractButton* b) { d->clickedButton = b; }

QAbstractButton* CheckableMessageBox::clickedButton() const { return d->clickedButton; }

QDialogButtonBox::StandardButton CheckableMessageBox::clickedStandardButton() const
{
    if (d->clickedButton) {
        return d->buttonBox->standardButton(d->clickedButton);
    }
    return QDialogButtonBox::NoButton;
}

QString CheckableMessageBox::text() const { return d->messageLabel->text(); }

void CheckableMessageBox::setText(const QString& t) { d->messageLabel->setText(t); }

// cppcheck-suppress unusedFunction
/*
QPixmap CheckableMessageBox::iconPixmap() const
{
    return d->pixmapLabel->pixmap(Qt::ReturnByValueConstant::ReturnByValue);
}
*/

void CheckableMessageBox::setIconPixmap(const QPixmap& p)
{
    d->pixmapLabel->setPixmap(p);
    d->pixmapLabel->setVisible(!p.isNull());
}

bool CheckableMessageBox::isChecked() const { return d->checkBox->isChecked(); }

void CheckableMessageBox::setChecked(bool s) { d->checkBox->setChecked(s); }

QString CheckableMessageBox::checkBoxText() const { return d->checkBox->text(); }

void CheckableMessageBox::setCheckBoxText(const QString& t) { d->checkBox->setText(t); }

// cppcheck-suppress unusedFunction
bool CheckableMessageBox::isCheckBoxVisible() const { return d->checkBox->isVisible(); }

void CheckableMessageBox::setCheckBoxVisible(bool v) { d->checkBox->setVisible(v); }

QDialogButtonBox::StandardButtons CheckableMessageBox::standardButtons() const
{
    return d->buttonBox->standardButtons();
}

void CheckableMessageBox::setStandardButtons(QDialogButtonBox::StandardButtons s)
{
    d->buttonBox->setStandardButtons(s);
}

QPushButton* CheckableMessageBox::button(QDialogButtonBox::StandardButton b) const
{
    return d->buttonBox->button(b);
}

QPushButton* CheckableMessageBox::addButton(const QString& text, QDialogButtonBox::ButtonRole role)
{
    return d->buttonBox->addButton(text, role);
}

QDialogButtonBox::StandardButton CheckableMessageBox::defaultButton() const
{
    for (QAbstractButton* b : d->buttonBox->buttons())
        if (QPushButton* pb = qobject_cast<QPushButton*>(b)) {
            if (pb->isDefault()) {
                return d->buttonBox->standardButton(pb);
            }
        }
    return QDialogButtonBox::NoButton;
}

void CheckableMessageBox::setDefaultButton(QDialogButtonBox::StandardButton s)
{
    if (QPushButton* b = d->buttonBox->button(s)) {
        b->setDefault(true);
        b->setFocus();
    }
}

QDialogButtonBox::StandardButton CheckableMessageBox::question(
    QWidget* parent,
    const QString& title,
    const QString& question,
    const QString& checkBoxText,
    bool* checkBoxSetting,
    QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton)
{
    CheckableMessageBox mb(parent);
    mb.setWindowTitle(title);
    mb.setIconPixmap(QMessageBox::standardIcon(QMessageBox::Question));
    mb.setText(question);
    mb.setCheckBoxText(checkBoxText);
    mb.setChecked(*checkBoxSetting);
    mb.setStandardButtons(buttons);
    mb.setDefaultButton(defaultButton);
    mb.exec();
    *checkBoxSetting = mb.isChecked();
    return mb.clickedStandardButton();
}

QDialogButtonBox::StandardButton CheckableMessageBox::information(
    QWidget* parent,
    const QString& title,
    const QString& text,
    const QString& checkBoxText,
    bool* checkBoxSetting,
    QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton)
{
    CheckableMessageBox mb(parent);
    mb.setWindowTitle(title);
    mb.setIconPixmap(QMessageBox::standardIcon(QMessageBox::Information));
    mb.setText(text);
    mb.setCheckBoxText(checkBoxText);
    mb.setChecked(*checkBoxSetting);
    mb.setStandardButtons(buttons);
    mb.setDefaultButton(defaultButton);
    mb.exec();
    *checkBoxSetting = mb.isChecked();
    return mb.clickedStandardButton();
}

// cppcheck-suppress unusedFunction
QMessageBox::StandardButton
CheckableMessageBox::dialogButtonBoxToMessageBoxButton(QDialogButtonBox::StandardButton db)
{
    return static_cast<QMessageBox::StandardButton>(int(db));
}

bool CheckableMessageBox::askAgain(QSettings* settings, const QString& settingsSubKey)
{
    // QTC_CHECK(settings);
    if (settings) {
        settings->beginGroup(QLatin1String(kDoNotAskAgainKey));
        bool shouldNotAsk = settings->value(settingsSubKey, false).toBool();
        settings->endGroup();
        if (shouldNotAsk) {
            return false;
        }
    }
    return true;
}

void CheckableMessageBox::initDoNotAskAgainMessageBox(
    CheckableMessageBox& messageBox,
    const QString& title,
    const QString& text,
    QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton,
    DoNotAskAgainType type)
{
    messageBox.setWindowTitle(title);
    messageBox.setIconPixmap(QMessageBox::standardIcon(
        type == Information ? QMessageBox::Information : QMessageBox::Question));
    messageBox.setText(text);
    messageBox.setCheckBoxVisible(true);
    messageBox.setCheckBoxText(
        type == Information ? CheckableMessageBox::msgDoNotShowAgain()
                            : CheckableMessageBox::msgDoNotAskAgain());
    messageBox.setChecked(false);
    messageBox.setStandardButtons(buttons);
    messageBox.setDefaultButton(defaultButton);
}

void CheckableMessageBox::doNotAskAgain(QSettings* settings, const QString& settingsSubKey)
{
    if (!settings) {
        return;
    }

    settings->beginGroup(QLatin1String(kDoNotAskAgainKey));
    settings->setValue(settingsSubKey, true);
    settings->endGroup();
}

/*!
    Shows a message box with given \a title and \a text, and a \gui {Do not ask again} check box.
    If the user checks the check box and accepts the dialog with the \a acceptButton,
    further invocations of this function with the same \a settings and \a settingsSubKey will not
    show the dialog, but instantly return \a acceptButton.

    Returns the clicked button, or QDialogButtonBox::NoButton if the user rejects the dialog
    with the escape key, or \a acceptButton if the dialog is suppressed.
*/
QDialogButtonBox::StandardButton
// cppcheck-suppress unusedFunction
CheckableMessageBox::doNotAskAgainQuestion(
    QWidget* parent,
    const QString& title,
    const QString& text,
    QSettings* settings,
    const QString& settingsSubKey,
    QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton,
    QDialogButtonBox::StandardButton acceptButton)

{
    if (!askAgain(settings, settingsSubKey)) {
        return acceptButton;
    }

    CheckableMessageBox messageBox(parent);
    initDoNotAskAgainMessageBox(messageBox, title, text, buttons, defaultButton, Question);
    messageBox.exec();
    if (messageBox.isChecked() && (messageBox.clickedStandardButton() == acceptButton)) {
        doNotAskAgain(settings, settingsSubKey);
    }

    return messageBox.clickedStandardButton();
}

/*!
    Shows a message box with given \a title and \a text, and a \gui {Do not show again} check box.
    If the user checks the check box and quits the dialog, further invocations of this
    function with the same \a settings and \a settingsSubKey will not show the dialog, but instantly
   return.

    Returns the clicked button, or QDialogButtonBox::NoButton if the user rejects the dialog
    with the escape key, or \a defaultButton if the dialog is suppressed.
*/
QDialogButtonBox::StandardButton
// cppcheck-suppress unusedFunction
CheckableMessageBox::doNotShowAgainInformation(
    QWidget* parent,
    const QString& title,
    const QString& text,
    QSettings* settings,
    const QString& settingsSubKey,
    QDialogButtonBox::StandardButtons buttons,
    QDialogButtonBox::StandardButton defaultButton)

{
    if (!askAgain(settings, settingsSubKey)) {
        return defaultButton;
    }

    CheckableMessageBox messageBox(parent);
    initDoNotAskAgainMessageBox(messageBox, title, text, buttons, defaultButton, Information);
    messageBox.exec();
    if (messageBox.isChecked()) {
        doNotAskAgain(settings, settingsSubKey);
    }

    return messageBox.clickedStandardButton();
}

/*!
    Resets all suppression settings for doNotAskAgainQuestion() found in \a settings,
    so all these message boxes are shown again.
 */
// cppcheck-suppress unusedFunction
void CheckableMessageBox::resetAllDoNotAskAgainQuestions(QSettings* settings)
{
    // Q_ASSERT(settings, return);
    settings->beginGroup(QLatin1String(kDoNotAskAgainKey));
    settings->remove(QString());
    settings->endGroup();
}

/*!
    Returns whether any message boxes from doNotAskAgainQuestion() are suppressed
    in the \a settings.
*/
// cppcheck-suppress unusedFunction
bool CheckableMessageBox::hasSuppressedQuestions(QSettings* settings)
{
    // Q_ASSERT(settings, return false);
    bool hasSuppressed = false;
    settings->beginGroup(QLatin1String(kDoNotAskAgainKey));
    for (const QString& subKey : settings->childKeys()) {
        if (settings->value(subKey, false).toBool()) {
            hasSuppressed = true;
            break;
        }
    }
    settings->endGroup();
    return hasSuppressed;
}

/*!
    Returns the standard \gui {Do not ask again} check box text.
    \sa doNotAskAgainQuestion()
*/
QString CheckableMessageBox::msgDoNotAskAgain()
{
    return QApplication::translate("Utils::CheckableMessageBox", "Do not &ask again");
}

/*!
    Returns the standard \gui {Do not show again} check box text.
    \sa doNotShowAgainInformation()
*/
QString CheckableMessageBox::msgDoNotShowAgain()
{
    return QApplication::translate("Utils::CheckableMessageBox", "Do not &show again");
}

}   // namespace Utils
