#include "file_dialog.h"

QString fileDialog(
    QWidget* parent,
    const QString& title,
    const QString& dir,
    const QString& filter,
    QString* selectedFilter,
    QFileDialog::Options options,
    QFileDialog::FileMode mode,
    QFileDialog::AcceptMode accept)
{
    QFileDialog dialog(parent, title, dir, filter);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dialog.setOptions(options);
    dialog.setFileMode(mode);
    dialog.setAcceptMode(accept);
    dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
    if (selectedFilter && !selectedFilter->isEmpty()) {
        dialog.selectNameFilter(*selectedFilter);
    }

    QUrl selectedUrl;
    if (dialog.exec() == QDialog::Accepted) {
        if (selectedFilter) {
            *selectedFilter = dialog.selectedNameFilter();
        }
        selectedUrl = dialog.selectedUrls().value(0);
    } else {
        selectedUrl = QUrl();
    }

    if (selectedUrl.isLocalFile() || selectedUrl.isEmpty()) {
        return selectedUrl.toLocalFile();
    } else {
        return selectedUrl.toString();
    }
}
