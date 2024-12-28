#ifndef FILE_DIALOG_H
#define FILE_DIALOG_H

#include <QFileDialog>
#include <QString>
#include <QWidget>

[[nodiscard]] QString fileDialog(
    QWidget* parent,
    const QString& title,
    const QString& dir,
    const QString& filter,
    QString* selectedFilter,
    QFileDialog::Options options,
    QFileDialog::FileMode mode,
    QFileDialog::AcceptMode accept);

#endif   // FILE_DIALOG_H
