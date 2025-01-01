/***************************************************************************************************
 **
 **  Copyright (C) 2015 Roman Telezhynskyi <dismine(at)gmail.com>
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#ifndef QMUDEF_H
#define QMUDEF_H

#include <QChar>
#include <QLocale>
#include <QString>
#include <QtGlobal>
#include <qcompilerdetection.h>

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wattributes")

#define INIT_LOCALE_VARIABLES(locale)                        \
    const QChar positiveSign = (locale).positiveSign();      \
    const QChar negativeSign = (locale).negativeSign();      \
    const QChar sign0 = (locale).toString(0).at(0);          \
    const QChar sign1 = (locale).toString(1).at(0);          \
    const QChar sign2 = (locale).toString(2).at(0);          \
    const QChar sign3 = (locale).toString(3).at(0);          \
    const QChar sign4 = (locale).toString(4).at(0);          \
    const QChar sign5 = (locale).toString(5).at(0);          \
    const QChar sign6 = (locale).toString(6).at(0);          \
    const QChar sign7 = (locale).toString(7).at(0);          \
    const QChar sign8 = (locale).toString(8).at(0);          \
    const QChar sign9 = (locale).toString(9).at(0);          \
    const QChar expUpper = (locale).exponential().toUpper(); \
    const QChar expLower = (locale).exponential().toLower(); \
    const QChar decimalPoint = (locale).decimalPoint();      \
    const QChar groupSeparator = (locale).groupSeparator()

QString NameRegExp();

QT_WARNING_POP

Q_REQUIRED_RESULT static inline bool QmuFuzzyComparePossibleNulls(double p1, double p2);
static inline bool QmuFuzzyComparePossibleNulls(double p1, double p2)
{
    if (qFuzzyIsNull(p1)) {
        return qFuzzyIsNull(p2);
    } else if (qFuzzyIsNull(p2)) {
        return false;
    } else {
        return qFuzzyCompare(p1, p2);
    }
}

int ReadVal(
    const QString& formula,
    qreal& val,
    const QLocale& locale,
    const QChar& decimal,
    const QChar& thousand);

#endif   // QMUDEF_H
