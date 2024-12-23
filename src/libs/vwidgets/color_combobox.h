/******************************************************************************
 *   color_combobox.h                                                         *
 *****************************************************************************/

#ifndef COLOR_COMBOBOX_H
#define COLOR_COMBOBOX_H

#include <QComboBox>
#include <QString>
#include <QWidget>


/**
 * A comboBox for choosing a color.
 */
class ColorComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit ColorComboBox(QWidget* parent = nullptr, const char* name = nullptr);
    ColorComboBox(int width, int height, QWidget* parent = nullptr, const char* name = nullptr);

    QString getColor() const;
    void setColor(const QString& color);
    void setItems(QMap<QString, QString> map);
    int getIconWidth() const;
    int getIconHeight() const;

private slots:
    void colorChanged(int index);

signals:
    void colorChangedSignal(const QString& color);

private:
    void init();

    QString m_currentColor{ QStringLiteral("black") };
    int m_iconWidth{ 40 };
    int m_iconHeight{ 14 };
};

#endif
