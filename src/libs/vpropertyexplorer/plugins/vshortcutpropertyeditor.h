/************************************************************************
 **
 **  @file   vshortcutpropertyeditor.h
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#ifndef VSHORTCUTPROPERTYEDITOR_H
#define VSHORTCUTPROPERTYEDITOR_H

#include <QKeySequence>
#include <QLineEdit>
#include <QMetaObject>
#include <QMimeData>
#include <QObject>
#include <QString>
#include <QToolButton>
#include <QWidget>
#include <QtGlobal>
#include <qcompilerdetection.h>

namespace VPE {


class VShortcutEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VShortcutEditWidget(QWidget* parent);
    virtual ~VShortcutEditWidget() Q_DECL_OVERRIDE;

    //! Needed for proper event handling
    virtual bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;

    //! Returns the currently set shortcut
    QString getShortcutAsString() const;

    //! Returns the currently set shortcut
    QKeySequence getShortcut();

signals:
    //! This signal is emitted when the user changed the current shortcut
    void dataChangedByUser(const QKeySequence& sequence, VShortcutEditWidget* editor);

    //! This signal is emitted whenever dataChangedByUser() gets emmitted
    //! and is connected to the delegate's commitData() signal
    void commitData(QWidget* editor);

public slots:
    //! Sets the shortcut
    //! \param shortcut The new shortcut
    //! \emit_signal If true, this will emit the dataChangedByUser()-signal
    void setShortcut(const QString& shortcut, bool emit_signal);

    //! Sets the shortcut
    //! \param shortcut The new shortcut
    //! \emit_signal If true, this will emit the dataChangedByUser()-signal
    void setShortcut(const QKeySequence& shortcut, bool emit_signal);

private slots:
    //! This slot is called when the user edits the line edit (e.g. by removing or pasting text
    //! using the mouse)
    void onTextEdited(const QString& text);

protected:
    //! The current key sequence
    QKeySequence CurrentKeySequence;

    //! The line to display and edit the key sequence
    QLineEdit* LineEdit;

private:
    Q_DISABLE_COPY(VShortcutEditWidget)
};

}   // namespace VPE

#endif   // VFILEPROPERTYEDITOR_H
