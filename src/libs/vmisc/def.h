/************************************************************************
 **  @file   def.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   11 4, 2015
 **
 **  @author Douglas S Caskey
 **  @date   7.31.2022
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 - 2022 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef DEF_H
#define DEF_H

#include <QLineF>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <Qt>
#include <QtGlobal>
#include <csignal>
#include <qcompilerdetection.h>
#ifdef Q_OS_WIN
#    include <windows.h>
#endif /* Q_OS_WIN */


#ifdef Q_CC_MSVC
#    include <ciso646>
#endif /* Q_CC_MSVC */

class QComboBox;
class QMarginsF;
class VTranslateMeasurements;
class QGraphicsItem;

inline constexpr int SceneSize{ 50'000 };
inline constexpr int HANDLE_SIZE{ 12 };

enum class Position : char
{
    TopLeft = 0,
    Top,
    TopRight,
    Right,
    BottomRight,
    Bottom,
    BottomLeft,
    Left,
    Center
};

enum class PaperSizeFormat : char
{
    A0 = 0,
    A1,
    A2,
    A3,
    A4,
    Letter,
    Legal,
    Tabloid,
    AnsiC,
    AnsiD,
    AnsiE,
    Roll24in,   // Be careful when changing order roll type
    Roll30in,   // Used also for showing icon
    Roll36in,
    Roll42in,
    Roll44in,
    Custom
};

enum class LayoutExportFormat : char
{
    SVG = 0,
    PDF = 1,
    PDFTiled = 2,
    PNG = 3,
    JPG = 4,
    BMP = 5,
    PPM = 6,
    OBJ = 7, /* Wavefront OBJ*/
    PS = 8,
    EPS = 9,
    DXF_AC1006_Flat = 10, /* R10. */
    DXF_AC1009_Flat = 11, /* R11 & R12. */
    DXF_AC1012_Flat = 12, /* R13. */
    DXF_AC1014_Flat = 13, /* R14. */
    DXF_AC1015_Flat = 14, /* ACAD 2000. */
    DXF_AC1018_Flat = 15, /* ACAD 2004. */
    DXF_AC1021_Flat = 16, /* ACAD 2007. */
    DXF_AC1024_Flat = 17, /* ACAD 2010. */
    DXF_AC1027_Flat = 18, /* ACAD 2013. */
    DXF_AC1006_AAMA = 19, /* R10. */
    DXF_AC1009_AAMA = 20, /* R11 & R12. */
    DXF_AC1012_AAMA = 21, /* R13. */
    DXF_AC1014_AAMA = 22, /* R14. */
    DXF_AC1015_AAMA = 23, /* ACAD 2000. */
    DXF_AC1018_AAMA = 24, /* ACAD 2004. */
    DXF_AC1021_AAMA = 25, /* ACAD 2007. */
    DXF_AC1024_AAMA = 26, /* ACAD 2010. */
    DXF_AC1027_AAMA = 27, /* ACAD 2013. */
    DXF_AC1006_ASTM = 28, /* R10. */
    DXF_AC1009_ASTM = 29, /* R11 & R12. */
    DXF_AC1012_ASTM = 30, /* R13. */
    DXF_AC1014_ASTM = 31, /* R14. */
    DXF_AC1015_ASTM = 32, /* ACAD 2000. */
    DXF_AC1018_ASTM = 33, /* ACAD 2004. */
    DXF_AC1021_ASTM = 34, /* ACAD 2007. */
    DXF_AC1024_ASTM = 35, /* ACAD 2010. */
    DXF_AC1027_ASTM = 36, /* ACAD 2013. */
    TIF = 37,             /* TIFF */
    COUNT                 /*Use only for validation*/
};

enum class NodeDetail : char
{
    Contour,
    Modeling
};
enum class SceneObject : char
{
    Point,
    Line,
    Spline,
    Arc,
    ElArc,
    SplinePath,
    Piece,
    Unknown
};
enum class MeasurementsType : char
{
    Multisize,
    Individual,
    Unknown
};
enum class Unit : char
{
    Mm = 0,
    Cm,
    Inch,
    Px,
    LAST_UNIT_DO_NOT_USE
};
enum class Source : char
{
    FromGui,
    FromFile,
    FromTool
};
enum class NodeUsage : bool
{
    NotInUse = false,
    InUse = true
};
enum class SelectionType : bool
{
    ByMousePress,
    ByMouseRelease
};

enum class PageOrientation : bool
{
    Portrait = true,
    Landscape = false
};

enum class PieceNodeAngle : unsigned char
{
    ByLength = 0,
    ByPointsIntersection,
    ByFirstEdgeSymmetry,
    BySecondEdgeSymmetry,
    ByFirstEdgeRightAngle,
    BySecondEdgeRightAngle
};

struct DraftImage
{
    quint32 id{ 0 };
    QString name;
    QString filename;
    QPixmap pixmap;
    bool locked{ false };
    Position origin{ Position::Center };
    quint32 anchor{ 0 };
    qreal xPos{ 0.0 };
    qreal yPos{ 0.0 };
    qreal width{ 0.0 };
    qreal height{ 0.0 };
    qreal xScale{ 0.0 };
    qreal yScale{ 0.0 };
    bool aspectLocked{ false };
    Unit units{ Unit::Px };
    qreal rotation{ 0.0 };
    bool visible{ true };
    qreal opacity{ 100.0 };
    qreal order{ 0.0 };
};

Q_DECLARE_METATYPE(DraftImage)
Q_DECLARE_TYPEINFO(DraftImage, Q_MOVABLE_TYPE);

enum class NotchType : unsigned char
{
    Slit = 0,   // Default
    TNotch,
    VInternal,
    VExternal,
    UNotch,
    Castle,
    Diamond
};

QString notchTypeToString(NotchType type);
NotchType stringToNotchType(const QString& value);

enum class NotchSubType : unsigned char
{
    Straightforward = 0,   // Default
    Bisector,
    Intersection
};

QString notchSubTypeToString(NotchSubType type);
NotchSubType stringToNotchSubType(const QString& value);


Unit StrToUnits(const QString& unit);
QString UnitsToStr(const Unit& unit, const bool translate = false);


enum class PiecePathIncludeType : unsigned char
{
    AsMainPath = 0,
    AsCustomSA = 1
};

enum class PiecePathType : unsigned char
{
    PiecePath = 0,
    CustomSeamAllowance = 1,
    InternalPath = 2,
    Unknown = 3
};

using ToolVisHolderType = int;
enum class Tool : ToolVisHolderType
{
    Arrow,
    SinglePoint,
    DoublePoint,
    LinePoint,
    AbstractSpline,
    Cut,
    BasePoint,
    EndLine,
    Line,
    AlongLine,
    ShoulderPoint,
    Normal,
    Bisector,
    LineIntersect,
    Spline,
    CubicBezier,
    CutSpline,
    CutArc,
    Arc,
    ArcWithLength,
    SplinePath,
    CubicBezierPath,
    CutSplinePath,
    PointOfContact,
    Piece,
    InternalPath,
    NodePoint,
    NodeArc,
    NodeElArc,
    NodeSpline,
    NodeSplinePath,
    Height,
    Triangle,
    LineIntersectAxis,
    PointOfIntersectionArcs,
    PointOfIntersectionCircles,
    PointOfIntersectionCurves,
    CurveIntersectAxis,
    ArcIntersectAxis,
    PointOfIntersection,
    PointFromCircleAndTangent,
    PointFromArcAndTangent,
    TrueDarts,
    Union,
    Group,
    Rotation,
    MirrorByLine,
    MirrorByAxis,
    Move,
    Midpoint,
    EllipticalArc,
    AnchorPoint,
    InsertNodes,
    BackgroundImage,
    LAST_ONE_DO_NOT_USE   // add new stuffs above this, this constant must be last and never used
};

enum class Vis : ToolVisHolderType
{
    ControlPointSpline = static_cast<ToolVisHolderType>(Tool::LAST_ONE_DO_NOT_USE),
    GraphicsSimpleTextItem,
    SimplePoint,
    SimpleCurve,
    ScaledLine,
    ScaledEllipse,
    Line,
    Path,
    Operation,
    ToolAlongLine,
    ToolArc,
    ToolArcWithLength,
    ToolBisector,
    ToolCutArc,
    ToolEndLine,
    ToolHeight,
    ToolLine,
    ToolLineIntersect,
    ToolNormal,
    ToolPointOfContact,
    ToolPointOfIntersection,
    ToolPointOfIntersectionArcs,
    ToolPointOfIntersectionCircles,
    ToolPointOfIntersectionCurves,
    ToolPointFromCircleAndTangent,
    ToolPointFromArcAndTangent,
    ToolShoulderPoint,
    ToolSpline,
    ToolCubicBezier,
    ToolCubicBezierPath,
    ToolTriangle,
    ToolCutSpline,
    ToolSplinePath,
    ToolCutSplinePath,
    ToolLineIntersectAxis,
    ToolCurveIntersectAxis,
    ToolTrueDarts,
    ToolRotation,
    ToolMirrorByLine,
    ToolMirrorByAxis,
    ToolMove,
    ToolEllipticalArc,
    ToolPiece,
    ToolInternalPath,
    ToolAnchorPoint,
    PieceAnchors,
    NoBrush,
    CurvePathItem,
    GrainlineItem,
    PieceItem,
    TextGraphicsItem,
    ScenePoint,
    ArrowedLineItem,
    BackgroundImageItem,
    ResizeHandlesItem,
    LAST_ONE_DO_NOT_USE   // add new types above this, this constant must be last and never used
};

enum class VarType : char
{
    Measurement,
    Variable,
    LineLength,
    CurveLength,
    CurveCLength,
    LineAngle,
    CurveAngle,
    ArcRadius,
    Unknown
};

inline constexpr int heightStep{ 6 };
enum class GHeights : unsigned char
{
    ALL,
    H50 = 50,
    H56 = 56,
    H62 = 62,
    H68 = 68,
    H74 = 74,
    H80 = 80,
    H86 = 86,
    H92 = 92,
    H98 = 98,
    H104 = 104,
    H110 = 110,
    H116 = 116,
    H122 = 122,
    H128 = 128,
    H134 = 134,
    H140 = 140,
    H146 = 146,
    H152 = 152,
    H158 = 158,
    H164 = 164,
    H170 = 170,
    H176 = 176,
    H182 = 182,
    H188 = 188,
    H194 = 194,
    H200 = 200
};

inline constexpr int sizeStep{ 2 };
enum class GSizes : unsigned char
{
    ALL,
    S22 = 22,
    S24 = 24,
    S26 = 26,
    S28 = 28,
    S30 = 30,
    S32 = 32,
    S34 = 34,
    S36 = 36,
    S38 = 38,
    S40 = 40,
    S42 = 42,
    S44 = 44,
    S46 = 46,
    S48 = 48,
    S50 = 50,
    S52 = 52,
    S54 = 54,
    S56 = 56,
    S58 = 58,
    S60 = 60,
    S62 = 62,
    S64 = 64,
    S66 = 66,
    S68 = 68,
    S70 = 70,
    S72 = 72
};

/* QImage supports a maximum of 32768x32768 px images (signed short).
 * This follows from the condition: width * height * colordepth < INT_MAX (4 billion) -> 32768 *
 * 32768 * 4 = 4 billion. The second condition is of course that malloc is able to allocate the
 * requested memory.
 *
 * If you really need bigger images you will have to use another wrapper or split into multiple
 * QImage's.
 */
inline constexpr int QIMAGE_MAX{ 32'768 };


extern const QString LONG_OPTION_NO_HDPI_SCALING;
bool IsOptionSet(int argc, char* argv[], const char* option);
void initHighDpiScaling(int argc, char* argv[]);

// functions
extern const QString degTorad_F;
extern const QString radTodeg_F;
extern const QString sin_F;
extern const QString cos_F;
extern const QString tan_F;
extern const QString asin_F;
extern const QString acos_F;
extern const QString atan_F;
extern const QString sinh_F;
extern const QString cosh_F;
extern const QString tanh_F;
extern const QString asinh_F;
extern const QString acosh_F;
extern const QString atanh_F;
extern const QString sinD_F;
extern const QString cosD_F;
extern const QString tanD_F;
extern const QString asinD_F;
extern const QString acosD_F;
extern const QString atanD_F;
extern const QString log2_F;
extern const QString log10_F;
extern const QString log_F;
extern const QString ln_F;
extern const QString exp_F;
extern const QString sqrt_F;
extern const QString sign_F;
extern const QString rint_F;
extern const QString abs_F;
extern const QString min_F;
extern const QString max_F;
extern const QString sum_F;
extern const QString avg_F;
extern const QString fmod_F;

extern const QStringList builInFunctions;

// Postfix operators
extern const QString cm_Oprt;
extern const QString mm_Oprt;
extern const QString in_Oprt;

extern const QStringList builInPostfixOperators;

// Placeholders
extern const QString pl_size;
extern const QString pl_height;
extern const QString pl_date;
extern const QString pl_time;
extern const QString pl_patternName;
extern const QString pl_patternNumber;
extern const QString pl_author;
extern const QString pl_customer;
extern const QString pl_pExt;
extern const QString pl_pFileName;
extern const QString pl_mFileName;
extern const QString pl_mExt;
extern const QString pl_pLetter;
extern const QString pl_pAnnotation;
extern const QString pl_pOrientation;
extern const QString pl_pRotation;
extern const QString pl_pTilt;
extern const QString pl_pFoldPosition;
extern const QString pl_pName;
extern const QString pl_pQuantity;
extern const QString pl_mFabric;
extern const QString pl_mLining;
extern const QString pl_mInterfacing;
extern const QString pl_mInterlining;
extern const QString pl_wCut;
extern const QString pl_wOnFold;

extern const QStringList labelTemplatePlaceholders;

extern const QString cursorArrowOpenHand;
extern const QString cursorArrowCloseHand;
extern const QString cursorResizeArrow;

extern const QString degreeSymbol;
extern const QString trueStr;
extern const QString falseStr;

extern const QString strSlit;

extern const QString strStraightforward;
extern const QString strBisector;
extern const QString strIntersection;

extern const QString unitMM;
extern const QString unitCM;
extern const QString unitINCH;
extern const QString unitPX;

extern const QString valExt;
extern const QString vitExt;
extern const QString vstExt;
extern const QString sm2dExt;
extern const QString smisExt;
extern const QString smmsExt;

void SetItemOverrideCursor(
    QGraphicsItem* item, const QString& pixmapPath, int hotX = -1, int hotY = -1);

extern const qreal PrintDPI;

[[nodiscard]] double ToPixel(double val, const Unit& unit);
[[nodiscard]] double FromPixel(double pix, const Unit& unit);

[[nodiscard]] qreal UnitConvertor(qreal value, const Unit& from, const Unit& to);
[[nodiscard]] QMarginsF UnitConvertor(const QMarginsF& margins, const Unit& from, const Unit& to);

void InitLanguages(QComboBox* combobox);
[[nodiscard]] QStringList SupportedLocales();

QString makeHeaderName(const QString& name);
[[nodiscard]] QString strippedName(const QString& fullFileName);
[[nodiscard]] QString RelativeMPath(const QString& patternPath, const QString& absoluteMPath);
[[nodiscard]] QString AbsoluteMPath(const QString& patternPath, const QString& relativeMPath);

void ShowInGraphicalShell(const QString& filePath);

inline constexpr qreal accuracyPointOnLine{ (0.1555 /*mm*/ / 25.4) * 96.0 };


[[nodiscard]] inline bool
VFuzzyComparePoints(const QPointF& p1, const QPointF& p2, qreal accuracy = accuracyPointOnLine)
{
    return QLineF(p1, p2).length() <= accuracy;
}


/**
 * @brief The CustomSA struct contains record about custom seam allowance (SA).
 */
struct CustomSARecord
{
    quint32 startPoint{ 0 };
    quint32 path{ 0 };
    quint32 endPoint{ 0 };
    bool reverse{ false };
    PiecePathIncludeType includeType{ PiecePathIncludeType::AsCustomSA };
};

Q_DECLARE_METATYPE(CustomSARecord)
Q_DECLARE_TYPEINFO(CustomSARecord, Q_MOVABLE_TYPE);

#endif   // DEF_H
