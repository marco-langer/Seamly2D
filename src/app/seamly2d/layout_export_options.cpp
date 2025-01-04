#include "layout_export_options.h"

QString exportFormatSuffix(LayoutExportFormat format)
{
    switch (format) {
    case LayoutExportFormat::SVG: return QStringLiteral(".svg");
    case LayoutExportFormat::PDF:
    case LayoutExportFormat::PDFTiled: return QStringLiteral(u".pdf");
    case LayoutExportFormat::PNG: return QStringLiteral(u".png");
    case LayoutExportFormat::JPG: return QStringLiteral(u".jpg");
    case LayoutExportFormat::BMP: return QStringLiteral(u".bmp");
    case LayoutExportFormat::PPM: return QStringLiteral(u".ppm");
    case LayoutExportFormat::OBJ: return QStringLiteral(u".obj");
    case LayoutExportFormat::PS: return QStringLiteral(u".ps");
    case LayoutExportFormat::EPS: return QStringLiteral(u".eps");
    case LayoutExportFormat::TIF: return QStringLiteral(u".tif");
    case LayoutExportFormat::DXF_AC1006_Flat:
    case LayoutExportFormat::DXF_AC1009_Flat:
    case LayoutExportFormat::DXF_AC1012_Flat:
    case LayoutExportFormat::DXF_AC1014_Flat:
    case LayoutExportFormat::DXF_AC1015_Flat:
    case LayoutExportFormat::DXF_AC1018_Flat:
    case LayoutExportFormat::DXF_AC1021_Flat:
    case LayoutExportFormat::DXF_AC1024_Flat:
    case LayoutExportFormat::DXF_AC1027_Flat:
    case LayoutExportFormat::DXF_AC1006_AAMA:
    case LayoutExportFormat::DXF_AC1009_AAMA:
    case LayoutExportFormat::DXF_AC1012_AAMA:
    case LayoutExportFormat::DXF_AC1014_AAMA:
    case LayoutExportFormat::DXF_AC1015_AAMA:
    case LayoutExportFormat::DXF_AC1018_AAMA:
    case LayoutExportFormat::DXF_AC1021_AAMA:
    case LayoutExportFormat::DXF_AC1024_AAMA:
    case LayoutExportFormat::DXF_AC1027_AAMA:
    case LayoutExportFormat::DXF_AC1006_ASTM:
    case LayoutExportFormat::DXF_AC1009_ASTM:
    case LayoutExportFormat::DXF_AC1012_ASTM:
    case LayoutExportFormat::DXF_AC1014_ASTM:
    case LayoutExportFormat::DXF_AC1015_ASTM:
    case LayoutExportFormat::DXF_AC1018_ASTM:
    case LayoutExportFormat::DXF_AC1021_ASTM:
    case LayoutExportFormat::DXF_AC1024_ASTM:
    case LayoutExportFormat::DXF_AC1027_ASTM: return QStringLiteral(u".dxf");
    case LayoutExportFormat::COUNT: Q_UNREACHABLE();
    }

    Q_UNREACHABLE();
}
