#include "document_validation.h"

#include "../exception/vexception.h"

#include <QAbstractMessageHandler>
#include <QFile>
#include <QLoggingCategory>
#include <QTextDocument>
#include <QXmlSchema>
#include <QXmlSchemaValidator>


Q_LOGGING_CATEGORY(vXMLSchema, "v.xml_schema")

namespace {

// This class need for validation pattern file using XSD shema
class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler()
        : QAbstractMessageHandler()
        , m_messageType(QtMsgType())
        , m_description()
        , m_sourceLocation(QSourceLocation())
    {
    }

    QString statusMessage() const;
    qint64 line() const;
    qint64 column() const;

protected:
    // cppcheck-suppress unusedFunction
    virtual void handleMessage(
        QtMsgType type,
        const QString& description,
        const QUrl& identifier,
        const QSourceLocation& sourceLocation) override;

private:
    QtMsgType m_messageType;
    QString m_description;
    QSourceLocation m_sourceLocation;
};


QString MessageHandler::statusMessage() const
{
    QTextDocument doc;
    doc.setHtml(m_description);
    return doc.toPlainText();
}


qint64 MessageHandler::line() const { return m_sourceLocation.line(); }


qint64 MessageHandler::column() const { return m_sourceLocation.column(); }


// cppcheck-suppress unusedFunction
void MessageHandler::handleMessage(
    QtMsgType type,
    const QString& description,
    const QUrl& identifier,
    const QSourceLocation& sourceLocation)
{
    Q_UNUSED(type)
    Q_UNUSED(identifier)

    m_messageType = type;
    m_description = description;
    m_sourceLocation = sourceLocation;
}

}   // namespace

void validateXMLSchema(const QString& schemaFilePath, const QString& filePath)
{
    qCDebug(vXMLSchema, "Validation xml file %s.", qUtf8Printable(filePath));
    QFile pattern(filePath);
    // cppcheck-suppress ConfigurationNotChecked
    if (pattern.open(QIODevice::ReadOnly) == false) {
        const QString errorMsg(
            QObject::tr("Can't open file %1:\n%2.").arg(filePath).arg(pattern.errorString()));
        throw VException(errorMsg);
    }

    QFile fileSchema(schemaFilePath);
    // cppcheck-suppress ConfigurationNotChecked
    if (fileSchema.open(QIODevice::ReadOnly) == false) {
        pattern.close();
        const QString errorMsg(QObject::tr("Can't open schema file %1:\n%2.")
                                   .arg(schemaFilePath)
                                   .arg(fileSchema.errorString()));
        throw VException(errorMsg);
    }

    MessageHandler messageHandler;
    QXmlSchema sch;
    sch.setMessageHandler(&messageHandler);
    if (sch.load(&fileSchema, QUrl::fromLocalFile(fileSchema.fileName())) == false) {
        pattern.close();
        fileSchema.close();
        VException e(messageHandler.statusMessage());
        e.AddMoreInformation(
            QObject::tr("Could not load schema file '%1'.").arg(fileSchema.fileName()));
        throw e;
    }
    qCDebug(vXMLSchema, "Schema loaded.");

    bool errorOccurred = false;
    if (sch.isValid() == false) {
        errorOccurred = true;
    } else {
        QXmlSchemaValidator validator(sch);
        if (validator.validate(&pattern, QUrl::fromLocalFile(pattern.fileName())) == false) {
            errorOccurred = true;
        }
    }

    if (errorOccurred) {
        pattern.close();
        fileSchema.close();
        VException e(messageHandler.statusMessage());
        e.AddMoreInformation(QObject::tr("Validation error file %3 in line %1 column %2")
                                 .arg(messageHandler.line())
                                 .arg(messageHandler.column())
                                 .arg(filePath));
        throw e;
    }
    pattern.close();
    fileSchema.close();
}
