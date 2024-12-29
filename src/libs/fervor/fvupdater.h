/***************************************************************************************************
 **
 **  Copyright (c) 2012 Linas Valiukas and others.
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a
 *copy of this
 **  software and associated documentation files (the "Software"), to deal in
 *the Software
 **  without restriction, including without limitation the rights to use, copy,
 *modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software,
 *and to
 **  permit persons to whom the Software is furnished to do so, subject to the
 *following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in
 *all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 *PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 *OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
 **
 ******************************************************************************************************/

#ifndef FVUPDATER_H
#define FVUPDATER_H

#include <QMetaObject>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>
#include <QtGlobal>

extern const QString defaultFeedURL;
class QFile;
class FvUpdater : public QObject
{
    Q_OBJECT

public:
    // Singleton
    static FvUpdater* sharedUpdater();
    static void drop();

    // Set / get feed URL
    void SetFeedURL(const QUrl& feedURL);
    void SetFeedURL(const QString& feedURL);
    QString GetFeedURL() const;

    bool IsDropOnFinish() const;
    void SetDropOnFinish(bool value);
signals:
    void setProgress(int);
public slots:
    // Check for updates
    bool CheckForUpdates(bool silentAsMuchAsItCouldGet = true);

    // Aliases
    bool CheckForUpdatesSilent();
    bool CheckForUpdatesNotSilent();

private slots:
    void httpFeedDownloadFinished();
    void networkError(QNetworkReply::NetworkError);
    void getFileSize();

private:
    //
    // Singleton business
    //
    Q_DISABLE_COPY(FvUpdater)
    FvUpdater();            // Hide main constructor
    virtual ~FvUpdater();   // Hide main destructor

    static QPointer<FvUpdater> m_Instance;   // Singleton instance

    // If true, don't show the error dialogs and the "no updates." dialog
    // (silentAsMuchAsItCouldGet from CheckForUpdates() goes here)
    // Useful for automatic update checking upon application startup.
    bool m_silentAsMuchAsItCouldGet;

    //
    // HTTP feed fetcher infrastructure
    //
    QUrl m_feedURL;   // Feed URL that will be fetched
    QNetworkAccessManager m_qnam;
    QPointer<QNetworkReply> m_reply;
    bool m_httpRequestAborted;
    bool m_dropOnFinish;
    int m_fileSize{};
    QString m_releaseName{};

    void startDownloadFeed(const QUrl& url);          // Start downloading feed
    void startDownloadFile(QUrl url, QString name);   // Start downloading file
    void fileDownloadFinished(QFile* downloadedFile, QString name);
    void cancelDownloadFeed();   // Stop downloading the current feed

    // Dialogs (notifications)
    // Show an error message
    void showErrorDialog(const QString& message, bool showEvenInSilentMode = false);
    // Show an informational message
    void showInformationDialog(const QString& message, bool showEvenInSilentMode = false);

    bool showConfirmationDialog(const QString& message, bool showEvenInSilentMode = false);


    // bool jsonParsing(); //
    bool releaseIsNewer(const QString& releaseTag);
    void getPLatformSpecificInstaller(QJsonArray assets);
};

#endif   // FVUPDATER_H
