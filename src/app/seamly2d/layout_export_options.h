#ifndef LAYOUT_EXPORT_OPTIONS_H
#define LAYOUT_EXPORT_OPTIONS_H

#include <QString>

#include "../vgeometry/vgeometrydef.h"
#include "../vmisc/def.h"

struct LayoutExportOptions final
{
    QString path;
    QString fileName;
    LayoutExportFormat format{ LayoutExportFormat::SVG };
    Draw mode{ Draw::Layout };
    bool isBinaryDxfFormat{ false };
    bool isTextAsPaths{ false };
};

QString exportFormatSuffix(LayoutExportFormat format);

#endif   // LAYOUT_EXPORT_OPTIONS_H
