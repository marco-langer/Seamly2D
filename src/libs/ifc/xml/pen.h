#ifndef PEN_H
#define PEN_H

#include <QString>

struct Pen final
{
    QString color{ QStringLiteral(u"black") };
    qreal lineWeight{ 1.20 };
    QString lineType{ QStringLiteral(u"solidline") };
};

#endif   // PEN_H
