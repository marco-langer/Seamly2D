/******************************************************************************
 *   @file   mainwindow.cpp
 **  @author Douglas S Caskey
 **  @date   17 Sep, 2023
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Seamly2D project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2022 Seamly2D project
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
 *************************************************************************

 ************************************************************************
 **
 **  @file   mainwindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../ifc/exception/vexceptionconversionerror.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../ifc/exception/vexceptionobjecterror.h"
#include "../ifc/exception/vexceptionundo.h"
#include "../ifc/exception/vexceptionwrongid.h"
#include "../ifc/xml/individual_size_converter.h"
#include "../ifc/xml/multi_size_converter.h"
#include "../ifc/xml/vpatternconverter.h"
#include "../qmuparser/qmuparsererror.h"
#include "../tools/image_item.h"
#include "../vformat/measurements.h"
#include "../vgeometry/vspline.h"
#include "../vmisc/customevents.h"
#include "../vmisc/def.h"
#include "../vmisc/logging.h"
#include "../vmisc/projectversion.h"
#include "../vmisc/qxtcsvmodel.h"
#include "../vmisc/vsettings.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vpropertyexplorer/checkablemessagebox.h"
#include "../vtools/dialogs/support/editlabeltemplate_dialog.h"
#include "../vtools/dialogs/tooldialogs.h"
#include "../vtools/tools/drawTools/drawtools.h"
#include "../vtools/undocommands/addgroup.h"
#include "../vtools/undocommands/label/showpointname.h"
#include "../vtools/undocommands/rename_draftblock.h"
#include "../vwidgets/dialogs/dialogexporttocsv.h"
#include "../vwidgets/dialogs/file_dialog.h"
#include "../vwidgets/mouse_coordinates.h"
#include "../vwidgets/pen_toolbar.h"
#include "../vwidgets/vmaingraphicsscene.h"
#include "../vwidgets/vwidgetpopup.h"
#include "core/application_2d.h"
#include "core/vtooloptionspropertybrowser.h"
#include "dialogs/calculator_dialog.h"
#include "dialogs/decimalchart_dialog.h"
#include "dialogs/dialogs.h"
#include "options.h"
#include "tools/nodeDetails/anchorpoint_tool.h"
#include "tools/nodeDetails/vtoolinternalpath.h"
#include "tools/pattern_piece_tool.h"
#include "tools/union_tool.h"
#include "version.h"

#include <QAction>
#include <QComboBox>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFontComboBox>
#include <QImageReader>
#include <QInputDialog>
#include <QMessageBox>
#include <QProcess>
#include <QScrollBar>
#include <QSettings>
#include <QSharedPointer>
#include <QShowEvent>
#include <QTextCodec>
#include <QTimer>
#include <QToolBar>
#include <QUndoStack>
#include <QtDebug>
#include <QtGlobal>

#include <chrono>
#include <memory>
#include <thread>

#if defined(Q_OS_MAC)
#    include <QDrag>
#    include <QMimeData>
#endif   // defined(Q_OS_MAC)

QT_WARNING_PUSH
QT_WARNING_DISABLE_CLANG("-Wmissing-prototypes")
QT_WARNING_DISABLE_INTEL(1418)

Q_LOGGING_CATEGORY(vMainWindow, "v.mainwindow")

QT_WARNING_POP

const QString autosavePrefix = QStringLiteral(".autosave");

// String below need for getting translation for key Ctrl
const QString strQShortcut = QStringLiteral("QShortcut");   // Context
const QString strCtrl = QStringLiteral("Ctrl");             // String

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MainWindow constructor.
 * @param parent parent widget.
 */
MainWindow::MainWindow(QWidget* parent)
    : MainWindowsNoGUI(parent)
    , m_ui(new Ui::MainWindow)
{
    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_recentFileActs[i] = nullptr;
    }

    createActions();
    initializeScenes();
    doc = new VPattern(pattern, &m_mode, m_draftScene, m_pieceScene);
    connect(doc, &VPattern::ClearMainWindow, this, &MainWindow::Clear);
    connect(doc, &VPattern::patternChanged, this, &MainWindow::patternChangesWereSaved);
    connect(doc, &VPattern::UndoCommand, this, &MainWindow::fullParseFile);
    connect(doc, &VPattern::setGuiEnabled, this, &MainWindow::setGuiEnabled);
    connect(doc, &VPattern::CheckLayout, this, [this]() {
        if (pattern->DataPieces().count() == 0) {
            if (!m_ui->showDraftMode->isChecked()) {
                showDraftMode(true);
            }
        }
    });
    connect(doc, &VPattern::setCurrentDraftBlock, this, &MainWindow::changeDraftBlockGlobally);
    connect(doc, &VPattern::CheckLayout, this, [&]() {
        this->updateZoomToPointComboBox(draftPointNamesList());
    });
    qApp->setCurrentDocument(doc);
    qApp->setCurrentData(pattern);

    initializeDocksContain();
    createMenus();
    initializeDraftToolBar();
    initializePointNameToolBar();
    initializeModesToolBar();
    initializeToolButtons();
    showMaximized();
    initPenToolBar();

    m_helpLabel = new QLabel(QObject::tr("Create new pattern piece to start working."));
    m_ui->statusBar->addWidget(m_helpLabel);

    initializeToolsToolBar();

    connect(
        qApp->getUndoStack(),
        &QUndoStack::cleanChanged,
        this,
        &MainWindow::patternChangesWereSaved);

    initializeAutoSave();

    m_ui->draft_ToolBox->setCurrentIndex(0);

    ReadSettings();
    initializeToolBarVisibility();

    setCurrentFile("");
    WindowsLocale();

    connect(m_ui->listWidget, &QListWidget::currentRowChanged, this, &MainWindow::showLayoutPages);

    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::MeasurementsChanged);
    connect(qApp, &QApplication::focusChanged, this, [this](QWidget* old, QWidget* now) {
        if (old == nullptr && isAncestorOf(now) == true) {   // focus IN
            static bool asking = false;
            if (!asking && m_changes && not m_changesAsked) {
                asking = true;
                m_changesAsked = true;
                const auto answer = QMessageBox::question(
                    this,
                    tr("Measurements"),
                    tr("Measurements were changed. Do you want to sync measurements now?"),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
                if (answer == QMessageBox::Yes) {
                    SyncMeasurements();
                }
                asking = false;
            }
        }

        // In case we will need it
        // else if (isAncestorOf(old) == true && now == nullptr)
        // focus OUT
    });

#if defined(Q_OS_MAC)
    // On Mac default icon size is 32x32.
    m_ui->draft_ToolBar->setIconSize(QSize(24, 24));
    m_ui->mode_ToolBar->setIconSize(QSize(24, 24));
    m_ui->edit_Toolbar->setIconSize(QSize(24, 24));
    m_ui->zoom_ToolBar->setIconSize(QSize(24, 24));

    setUnifiedTitleAndToolBarOnMac(true);

    // Mac OS Dock Menu
    QMenu* menu = new QMenu(this);

    QAction* newPattern_Action = menu->addAction(tr("New pattern"));
    newPattern_Action->setMenuRole(QAction::NoRole);
    connect(newPattern_Action, &QAction::triggered, this, &MainWindow::New);

    QAction* openPattern_Action = menu->addAction(tr("Open pattern"));
    openPattern_Action->setMenuRole(QAction::NoRole);
    connect(openPattern_Action, &QAction::triggered, this, &MainWindow::Open);

    QAction* openSeamlyMe_Action = menu->addAction(tr("Create/Edit measurements"));
    openSeamlyMe_Action->setMenuRole(QAction::NoRole);
    connect(openSeamlyMe_Action, &QAction::triggered, this, &MainWindow::CreateMeasurements);

    QAction* appPreferences_Action = menu->addAction(tr("Preferences"));
    appPreferences_Action->setMenuRole(QAction::NoRole);
    connect(appPreferences_Action, &QAction::triggered, this, &MainWindow::Preferences);

    menu->setAsDockMenu();

#endif   // defined(Q_OS_MAC)
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::addDraftBlock(const QString& blockName)
{
    if (doc->appendDraftBlock(blockName) == false) {
        qCWarning(
            vMainWindow, "Error creating draft block with the name %s.", qUtf8Printable(blockName));
        return;
    }

    if (m_draftBlockComboBox->count() == 0) {
        m_draftScene->initializeOrigins();
        m_draftScene->enablePiecesMode(qApp->Seamly2DSettings()->getShowControlPoints());
        m_pieceScene->initializeOrigins();
    }

    m_draftBlockComboBox->blockSignals(true);
    m_draftBlockComboBox->addItem(blockName);

    pattern->ClearGObjects();
    // Create single point
    emit m_ui->view->itemClicked(nullptr);   // hide options previous tool
    const QString label = doc->GenerateLabel(LabelType::NewPatternPiece);
    const QPointF startPosition = draftBlockStartPosition();

    auto spoint = VToolBasePoint::Create(
        0,
        blockName,
        std::make_unique<VPointF>(startPosition.x(), startPosition.y(), label, 5, 10),
        m_draftScene,
        doc,
        pattern,
        Document::FullParse,
        Source::FromGui);
    emit m_ui->view->itemClicked(spoint);

    setToolsEnabled(true);
    setWidgetsEnabled(true);

    const qint32 index = m_draftBlockComboBox->findText(blockName);
    if (index != -1) {   // -1 for not found
        m_draftBlockComboBox->setCurrentIndex(index);
    } else {
        m_draftBlockComboBox->setCurrentIndex(0);
    }
    m_draftBlockComboBox->blockSignals(false);

    // Show best for new PP
    VMainGraphicsView::NewSceneRect(m_ui->view->scene(), m_ui->view, spoint);
    m_ui->view->zoom100Percent();

    m_ui->newDraft_Action->setEnabled(true);
    m_helpLabel->setText("");
    m_groupsWidget->updateGroups();
}

//---------------------------------------------------------------------------------------------------------------------
QPointF MainWindow::draftBlockStartPosition() const
{
    const qreal originX = 30.0;
    const qreal originY = 40.0;
    const qreal margin = 40.0;
    if (m_draftBlockComboBox->count() > 1) {
        const QRectF rect = m_draftScene->visibleItemsBoundingRect();
        if (rect.width() <= rect.height()) {
            return QPointF(rect.width() + margin, originY);
        } else {
            return QPointF(originX, rect.height() + margin);
        }
    } else {
        return QPointF(originX, originY);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeScenes()
{
    m_draftScene = new VMainGraphicsScene(this);
    currentScene = m_draftScene;
    qApp->setCurrentScene(&currentScene);
    connect(this, &MainWindow::EnableItemMove, m_draftScene, &VMainGraphicsScene::EnableItemMove);
    connect(this, &MainWindow::ItemsSelection, m_draftScene, &VMainGraphicsScene::ItemsSelection);

    connect(
        this,
        &MainWindow::EnableLabelSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleLabelSelection);
    connect(
        this,
        &MainWindow::EnablePointSelection,
        m_draftScene,
        &VMainGraphicsScene::TogglePointSelection);
    connect(
        this,
        &MainWindow::EnableLineSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleLineSelection);
    connect(
        this,
        &MainWindow::EnableArcSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleArcSelection);
    connect(
        this,
        &MainWindow::EnableElArcSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleElArcSelection);
    connect(
        this,
        &MainWindow::EnableSplineSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleSplineSelection);
    connect(
        this,
        &MainWindow::EnableSplinePathSelection,
        m_draftScene,
        &VMainGraphicsScene::ToggleSplinePathSelection);

    connect(
        this, &MainWindow::EnableLabelHover, m_draftScene, &VMainGraphicsScene::ToggleLabelHover);
    connect(
        this, &MainWindow::EnablePointHover, m_draftScene, &VMainGraphicsScene::TogglePointHover);
    connect(this, &MainWindow::EnableLineHover, m_draftScene, &VMainGraphicsScene::ToggleLineHover);
    connect(this, &MainWindow::EnableArcHover, m_draftScene, &VMainGraphicsScene::ToggleArcHover);
    connect(
        this, &MainWindow::EnableElArcHover, m_draftScene, &VMainGraphicsScene::ToggleElArcHover);
    connect(
        this, &MainWindow::EnableSplineHover, m_draftScene, &VMainGraphicsScene::ToggleSplineHover);
    connect(
        this,
        &MainWindow::EnableSplinePathHover,
        m_draftScene,
        &VMainGraphicsScene::ToggleSplinePathHover);

    connect(m_draftScene, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

    m_pieceScene = new VMainGraphicsScene(this);
    connect(this, &MainWindow::EnableItemMove, m_pieceScene, &VMainGraphicsScene::EnableItemMove);

    connect(
        this,
        &MainWindow::EnableNodeLabelSelection,
        m_pieceScene,
        &VMainGraphicsScene::ToggleNodeLabelSelection);
    connect(
        this,
        &MainWindow::EnableNodePointSelection,
        m_pieceScene,
        &VMainGraphicsScene::ToggleNodePointSelection);
    connect(
        this,
        &MainWindow::enablePieceSelection,
        m_pieceScene,
        &VMainGraphicsScene::togglePieceSelection);

    connect(
        this,
        &MainWindow::EnableNodeLabelHover,
        m_pieceScene,
        &VMainGraphicsScene::ToggleNodeLabelHover);
    connect(
        this,
        &MainWindow::EnableNodePointHover,
        m_pieceScene,
        &VMainGraphicsScene::ToggleNodePointHover);
    connect(
        this, &MainWindow::enablePieceHover, m_pieceScene, &VMainGraphicsScene::togglePieceHover);

    connect(m_pieceScene, &VMainGraphicsScene::mouseMove, this, &MainWindow::MouseMove);

    m_ui->view->setScene(currentScene);

    m_draftScene->setCurrentTransform(m_ui->view->transform());
    m_pieceScene->setCurrentTransform(m_ui->view->transform());

    connect(
        m_ui->view, &VMainGraphicsView::mouseRelease, this, [this]() { EndVisualization(true); });
    connect(
        m_ui->view,
        &VMainGraphicsView::signalZoomScaleChanged,
        this,
        &MainWindow::zoomScaleChanged);

    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    m_ui->view->setSizePolicy(policy);
    qApp->setSceneView(m_ui->view);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<MeasurementDoc> MainWindow::openMeasurementFile(const QString& fileName)
{
    std::unique_ptr<MeasurementDoc> measurements;
    if (fileName.isEmpty()) {
        return measurements;
    }

    try {
        measurements = std::make_unique<MeasurementDoc>(pattern);
        measurements->setSize(VContainer::rsize());
        measurements->setHeight(VContainer::rheight());
        measurements->setXMLContent(fileName);

        if (measurements->Type() == MeasurementsType::Unknown) {
            VException exception(tr("Measurement file has unknown format."));
            throw exception;
        }

        if (measurements->Type() == MeasurementsType::Multisize) {
            MultiSizeConverter converter(fileName);
            measurements->setXMLContent(converter.Convert());   // Read again after conversion
        } else {
            IndividualSizeConverter converter(fileName);
            measurements->setXMLContent(converter.Convert());   // Read again after conversion
        }

        if (!measurements->eachKnownNameIsValid()) {
            VException exception(tr("Measurement file contains invalid known measurement(s)."));
            throw exception;
        }

        checkRequiredMeasurements(*measurements);

        if (measurements->Type() == MeasurementsType::Multisize) {
            if (measurements->measurementUnits() == Unit::Inch) {
                qCCritical(
                    vMainWindow,
                    "%s\n\n%s",
                    qUtf8Printable(tr("Wrong units.")),
                    qUtf8Printable(tr("Application doesn't support multisize table with inches.")));
                measurements->clear();
                if (!Application2D::isGUIMode()) {
                    qApp->exit(V_EX_DATAERR);
                }
                return measurements;
            }
        }
    }

    catch (VException& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("File exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        measurements->clear();
        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return measurements;
    }
    return measurements;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::loadMeasurements(const QString& fileName)
{
    const std::unique_ptr<MeasurementDoc> measurements = openMeasurementFile(fileName);

    if (measurements->isNull()) {
        return false;
    }

    if (qApp->patternUnit() == Unit::Inch && measurements->Type() == MeasurementsType::Multisize) {
        qWarning() << tr("Gradation doesn't support inches");
        return false;
    }

    try {
        qApp->setPatternType(measurements->Type());
        initializeStatusToolBar();

        pattern->ClearVariables(VarType::Measurement);
        measurements->readMeasurements();
    }

    catch (VExceptionEmptyParameter& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("File exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    if (measurements->Type() == MeasurementsType::Multisize) {
        VContainer::setSize(UnitConvertor(
            measurements->BaseSize(),
            measurements->measurementUnits(),
            *measurements->GetData()->GetPatternUnit()));

        qCInfo(vMainWindow, "Multisize file %s was loaded.", qUtf8Printable(fileName));

        VContainer::setHeight(UnitConvertor(
            measurements->BaseHeight(),
            measurements->measurementUnits(),
            *measurements->GetData()->GetPatternUnit()));

        doc->SetPatternWasChanged(true);
        emit doc->UpdatePatternLabel();
    } else if (measurements->Type() == MeasurementsType::Individual) {
        setSizeHeightForIndividualM();

        qCInfo(vMainWindow, "Individual file %s was loaded.", qUtf8Printable(fileName));
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::updateMeasurements(const QString& fileName, int size, int height)
{
    const std::unique_ptr<MeasurementDoc> measurements = openMeasurementFile(fileName);

    if (measurements->isNull()) {
        return false;
    }

    if (qApp->patternType() != measurements->Type()) {
        qCCritical(
            vMainWindow, "%s", qUtf8Printable(tr("Measurement files types have not match.")));
        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_DATAERR);
        }
        return false;
    }

    try {
        pattern->ClearVariables(VarType::Measurement);
        measurements->readMeasurements();
    }

    catch (VExceptionEmptyParameter& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("File exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    if (measurements->Type() == MeasurementsType::Multisize) {
        VContainer::setSize(size);
        VContainer::setHeight(height);

        doc->SetPatternWasChanged(true);
        emit doc->UpdatePatternLabel();
    } else if (measurements->Type() == MeasurementsType::Individual) {
        setSizeHeightForIndividualM();
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::checkRequiredMeasurements(const MeasurementDoc& measurements)
{
    const QStringList tempMeasurements = measurements.ListAll();
    auto docMeasurements = doc->ListMeasurements();
    const QSet<QString> match =
        QSet<QString>(docMeasurements.begin(), docMeasurements.end())
            .subtract(QSet<QString>(tempMeasurements.begin(), tempMeasurements.end()));
    if (!match.isEmpty()) {
        QList<QString> list = match.values();
        for (int i = 0; i < list.size(); ++i) {
            list[i] = qApp->translateVariables()->MToUser(list.at(i));
        }

        VException exception(tr("Measurement file doesn't include all the required measurements."));
        exception.AddMoreInformation(
            tr("Please provide additional measurements: %1").arg(QStringList(list).join(", ")));
        throw exception;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SetToolButton set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function what handle after close dialog.
 */
template <typename Dialog, typename Func>
void MainWindow::SetToolButton(
    bool checked, Tool t, const QString& cursor, const QString& toolTip, Func closeDialogSlot)
{
    if (checked) {
        CancelTool();
        emit EnableItemMove(false);
        m_currentTool = m_lastUsedTool = t;
        auto cursorResource = cursor;
        if (qApp->devicePixelRatio() >= 2) {
            // Try to load HiDPI versions of the cursors if available
            auto cursorHidpiResource = QString(cursor).replace(".png", "@2x.png");
            if (QFileInfo::exists(cursorResource)) {
                cursorResource = cursorHidpiResource;
            }
        }
        QPixmap pixmap(cursorResource);
        QCursor cur(pixmap, 2, 2);
        m_ui->view->viewport()->setCursor(cur);
        m_helpLabel->setText(toolTip);
        m_ui->view->setShowToolOptions(false);
        m_dialogTool = QSharedPointer<Dialog>(new Dialog(pattern, 0, this));

        switch (t) {
        case Tool::ArcIntersectAxis:
            m_dialogTool->setWindowTitle("Point - Intersect Arc and Axis");
            break;
        case Tool::Midpoint: m_dialogTool->Build(t); break;
        case Tool::InternalPath:
        case Tool::AnchorPoint:
        case Tool::InsertNodes: m_dialogTool->SetPiecesList(doc->getActivePatternPieces()); break;
        default: break;
        }

        VMainGraphicsScene* scene = qobject_cast<VMainGraphicsScene*>(currentScene);
        SCASSERT(scene != nullptr)

        connect(
            scene,
            &VMainGraphicsScene::ChosenObject,
            m_dialogTool.data(),
            &DialogTool::ChosenObject);
        connect(
            scene,
            &VMainGraphicsScene::SelectedObject,
            m_dialogTool.data(),
            &DialogTool::SelectedObject);
        connect(m_dialogTool.data(), &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(m_dialogTool.data(), &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        emit m_ui->view->itemClicked(nullptr);
    } else {
        if (QToolButton* tButton = qobject_cast<QToolButton*>(this->sender())) {
            tButton->setChecked(true);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename Dialog, typename Func, typename Func2>
/**
 * @brief SetToolButtonWithApply set tool and show dialog.
 * @param checked true if tool button checked.
 * @param t tool type.
 * @param cursor path tool cursor icon.
 * @param toolTip first tooltipe.
 * @param closeDialogSlot function to handle close of dialog.
 * @param applyDialogSlot function to handle apply in dialog.
 */
void MainWindow::SetToolButtonWithApply(
    bool checked,
    Tool t,
    const QString& cursor,
    const QString& toolTip,
    Func closeDialogSlot,
    Func2 applyDialogSlot)
{
    if (checked) {
        SetToolButton<Dialog>(checked, t, cursor, toolTip, closeDialogSlot);

        connect(m_dialogTool.data(), &DialogTool::DialogApplied, this, applyDialogSlot);
    } else {
        if (QToolButton* tButton = qobject_cast<QToolButton*>(this->sender())) {
            tButton->setChecked(true);
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialog handle close dialog
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialog(int result)
{
    SCASSERT(!m_dialogTool.isNull())
    if (result == QDialog::Accepted) {
        VMainGraphicsScene* scene = qobject_cast<VMainGraphicsScene*>(currentScene);
        SCASSERT(scene != nullptr)

        QGraphicsItem* tool =
            dynamic_cast<QGraphicsItem*>(DrawTool::Create(m_dialogTool, scene, doc, pattern));
        // Do not check for nullptr! See issue #719.
        emit m_ui->view->itemClicked(tool);
    }
    handleArrowTool(true);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ClosedDialogWithApply handle close dialog that has apply button
 * @param result result working dialog.
 */
template <typename DrawTool>
void MainWindow::ClosedDialogWithApply(int result, VMainGraphicsScene* scene)
{
    SCASSERT(!m_dialogTool.isNull())
    if (result == QDialog::Accepted) {
        ApplyDialog<DrawTool>(scene);
    }
    // If before Cancel was used Apply we have an item
    DrawTool* vtool = qobject_cast<DrawTool*>(
        m_dialogTool->GetAssociatedTool());   // Don't check for nullptr here
    if (m_dialogTool->GetAssociatedTool() != nullptr) {
        SCASSERT(vtool != nullptr)
        vtool->DialogLinkDestroy();
        connect(vtool, &DrawTool::ToolTip, this, &MainWindow::ShowToolTip);
    }
    handleArrowTool(true);
    m_ui->view->itemClicked(vtool);   // Don't check for nullptr here
    // If insert not to the end of file call lite parse
    if (doc->getCursor() > 0) {
        doc->LiteParseTree(Document::LiteParse);
        if (m_historyDialog) {
            m_historyDialog->updateHistory();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ApplyDialog handle apply in dialog
 */
template <typename DrawTool>
void MainWindow::ApplyDialog(VMainGraphicsScene* scene)
{
    SCASSERT(!m_dialogTool.isNull())

    // Only create tool if not already created with apply
    if (m_dialogTool->GetAssociatedTool() == nullptr) {
        SCASSERT(scene != nullptr)

        m_dialogTool->SetAssociatedTool(DrawTool::Create(m_dialogTool, scene, doc, pattern));
    } else {   // Or update associated tool with data
        DrawTool* vtool = qobject_cast<DrawTool*>(m_dialogTool->GetAssociatedTool());
        SCASSERT(vtool != nullptr)
        vtool->FullUpdateFromGuiApply();
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ClosedDrawDialogWithApply(int result)
{
    ClosedDialogWithApply<DrawTool>(result, m_draftScene);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ApplyDrawDialog()
{
    ApplyDialog<DrawTool>(m_draftScene);
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::ClosedPiecesDialogWithApply(int result)
{
    ClosedDialogWithApply<DrawTool>(result, m_pieceScene);
    if (!pattern->DataPieces().isEmpty()) {
        m_ui->anchorPoint_ToolButton->setEnabled(true);
        m_ui->internalPath_ToolButton->setEnabled(true);
        m_ui->insertNodes_ToolButton->setEnabled(true);
        m_ui->importImage_ToolButton->setEnabled(true);
        m_ui->anchorPoint_Action->setEnabled(true);
        m_ui->internalPath_Action->setEnabled(true);
        m_ui->images_Action->setEnabled(true);
        m_ui->insertNodes_Action->setEnabled(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename DrawTool>
void MainWindow::applyPiecesDialog()
{
    ApplyDialog<DrawTool>(m_pieceScene);
}

// Points
//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleMidpointTool(bool checked)
{
    ToolSelectPointByRelease();
    // Reuse DialogAlongLine and VToolAlongLine but with different cursor
    SetToolButtonWithApply<DialogAlongLine>(
        checked,
        Tool::Midpoint,
        ":/cursor/midpoint_cursor.png",
        tr("<b>Tool::Point - Midpoint on Line</b>: Select first point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolAlongLine>,
        &MainWindow::ApplyDrawDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointAtDistanceAngleTool handler for handlePointAtDistanceAngle tool.
 * @param checked true - button checked.
 */
void MainWindow::handlePointAtDistanceAngleTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogEndLine>(
        checked,
        Tool::EndLine,
        ":/cursor/endline_cursor.png",
        tr("<b>Tool::Point - Length and Angle</b>: Select point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolEndLine>,
        &MainWindow::ApplyDrawDialog<VToolEndLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleAlongLineTool handler for point along Line tools.
 * @param checked true - button checked.
 */
void MainWindow::handleAlongLineTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogAlongLine>(
        checked,
        Tool::AlongLine,
        ":/cursor/alongline_cursor.png",
        tr("<b>Tool::Point - On Line:</b> Select first point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolAlongLine>,
        &MainWindow::ApplyDrawDialog<VToolAlongLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleNormalTool handler point on perpendicular tool.
 * @param checked true - button checked.
 */
void MainWindow::handleNormalTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogNormal>(
        checked,
        Tool::Normal,
        ":/cursor/normal_cursor.png",
        tr("<b>Tool::Point - On Perpendicular:</b> Select first point of line"),
        &MainWindow::ClosedDrawDialogWithApply<VToolNormal>,
        &MainWindow::ApplyDrawDialog<VToolNormal>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleBisectorTool handler for bisector tool.
 * @param checked true - button checked.
 */
void MainWindow::handleBisectorTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogBisector>(
        checked,
        Tool::Bisector,
        ":/cursor/bisector_cursor.png",
        tr("<b>Tool::Point - On Bisector:</b> Select first point of angle"),
        &MainWindow::ClosedDrawDialogWithApply<VToolBisector>,
        &MainWindow::ApplyDrawDialog<VToolBisector>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleShoulderPointTool handler for shoulderPoint tool.
 * @param checked true - button checked.
 */
void MainWindow::handleShoulderPointTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogShoulderPoint>(
        checked,
        Tool::ShoulderPoint,
        ":/cursor/shoulder_cursor.png",
        tr("<b>Tool::Point - Length to Line:</b> Select point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolShoulderPoint>,
        &MainWindow::ApplyDrawDialog<VToolShoulderPoint>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointOfContactTool handler for pointOfContact tool.
 * @param checked true - button checked.
 */
void MainWindow::handlePointOfContactTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogPointOfContact>(
        checked,
        Tool::PointOfContact,
        ":/cursor/point_intersect_arc_line_cursor.png",
        tr("<b>Tool::Point - Intersect Arc and Line:</b> Select first point of line"),
        &MainWindow::ClosedDrawDialogWithApply<VToolPointOfContact>,
        &MainWindow::ApplyDrawDialog<VToolPointOfContact>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleTriangleTool handler Point - Intersect Axis and Triangle.
 * @param checked true - button checked.
 */
void MainWindow::handleTriangleTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogTriangle>(
        checked,
        Tool::Triangle,
        ":/cursor/triangle_cursor.png",
        tr("<b>Tool::Point - Intersect Axis and Triangle:</b> Select first point of axis"),
        &MainWindow::ClosedDrawDialogWithApply<VToolTriangle>,
        &MainWindow::ApplyDrawDialog<VToolTriangle>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointIntersectXYTool handler for pointOfIntersection tool.
 * @param checked true - button checked.
 */
void MainWindow::handlePointIntersectXYTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<PointIntersectXYDialog>(
        checked,
        Tool::PointOfIntersection,
        ":/cursor/pointofintersect_cursor.png",
        tr("<b>Tool::Point - Intersect XY</b> Select point for X value (vertical)"),
        &MainWindow::ClosedDrawDialogWithApply<PointIntersectXYTool>,
        &MainWindow::ApplyDrawDialog<PointIntersectXYTool>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleHeightTool handler tool height.
 * @param checked true - button checked.
 */
void MainWindow::handleHeightTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogHeight>(
        checked,
        Tool::Height,
        ":/cursor/height_cursor.png",
        tr("<b>Tool::Point - Intersect Line and Perpendicular:</b> Select base point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolHeight>,
        &MainWindow::ApplyDrawDialog<VToolHeight>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleLineIntersectAxisTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLineIntersectAxis>(
        checked,
        Tool::LineIntersectAxis,
        ":/cursor/line_intersect_axis_cursor.png",
        tr("<b>Tool::Point - Intersect Line and Axis:</b> Select first point of line"),
        &MainWindow::ClosedDrawDialogWithApply<VToolLineIntersectAxis>,
        &MainWindow::ApplyDrawDialog<VToolLineIntersectAxis>);
}

// Lines
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleLineTool handler for line tool.
 * @param checked true - button checked.
 */
void MainWindow::handleLineTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLine>(
        checked,
        Tool::Line,
        ":/cursor/line_cursor.png",
        tr("<b>Tool::Line:</b>:Select first point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolLine>,
        &MainWindow::ApplyDrawDialog<VToolLine>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleLineIntersectTool handler for lineIntersect tool.
 * @param checked true - button checked.
 */
void MainWindow::handleLineIntersectTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogLineIntersect>(
        checked,
        Tool::LineIntersect,
        ":/cursor/intersect_cursor.png",
        tr("<b>Tool::Point - Intersect Lines:</b> Select first point of first line"),
        &MainWindow::ClosedDrawDialogWithApply<VToolLineIntersect>,
        &MainWindow::ApplyDrawDialog<VToolLineIntersect>);
}

// Curves
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleCurveTool handler for curve tool.
 * @param checked true - button checked.
 */
void MainWindow::handleCurveTool(bool checked)
{
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSpline>(
        checked,
        Tool::Spline,
        ":/cursor/spline_cursor.png",
        tr("<b>Tool::Curve - Interactive:</b> Select start point of curve"),
        &MainWindow::ClosedDrawDialogWithApply<VToolSpline>,
        &MainWindow::ApplyDrawDialog<VToolSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleSplineTool handler for spline tool.
 * @param checked true - button checked.
 */
void MainWindow::handleSplineTool(bool checked)
{
    ToolSelectPointByPress();
    SetToolButtonWithApply<DialogSplinePath>(
        checked,
        Tool::SplinePath,
        ":/cursor/splinepath_cursor.png",
        tr("<b>Tool::Spline - Interactive:</b> Select start point of spline"),
        &MainWindow::ClosedDrawDialogWithApply<VToolSplinePath>,
        &MainWindow::ApplyDrawDialog<VToolSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleCurveWithControlPointsTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezier>(
        checked,
        Tool::CubicBezier,
        ":/cursor/cubic_bezier_cursor.png",
        tr("<b>Tool::Curve - Fixed:</b> Select first point of curve"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCubicBezier>,
        &MainWindow::ApplyDrawDialog<VToolCubicBezier>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleSplineWithControlPointsTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogCubicBezierPath>(
        checked,
        Tool::CubicBezierPath,
        ":/cursor/cubic_bezier_path_cursor.png",
        tr("<b>Tool::Spline - Fixed:</b> Select first point of spline"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCubicBezierPath>,
        &MainWindow::ApplyDrawDialog<VToolCubicBezierPath>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointAlongCurveTool handler for point along curve tool.
 * @param checked true - button is checked
 */
void MainWindow::handlePointAlongCurveTool(bool checked)
{
    ToolSelectSpline();
    SetToolButtonWithApply<DialogCutSpline>(
        checked,
        Tool::CutSpline,
        ":/cursor/spline_cut_point_cursor.png",
        tr("<b>Tool::Point - On Curve:</b> Select first point of curve"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCutSpline>,
        &MainWindow::ApplyDrawDialog<VToolCutSpline>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointAlongSplineTool handler for point along spline tool.
 * @param checked true - button is checked
 */
void MainWindow::handlePointAlongSplineTool(bool checked)
{
    ToolSelectSplinePath();
    SetToolButtonWithApply<DialogCutSplinePath>(
        checked,
        Tool::CutSplinePath,
        ":/cursor/splinepath_cut_point_cursor.png",
        tr("<b>Tool::Point - On Spline:</b> Select spline"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCutSplinePath>,
        &MainWindow::ApplyDrawDialog<VToolCutSplinePath>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleCurveIntersectCurveTool(bool checked)
{
    ToolSelectCurve();
    SetToolButtonWithApply<DialogPointOfIntersectionCurves>(
        checked,
        Tool::PointOfIntersectionCurves,
        "://cursor/intersection_curves_cursor.png",
        tr("<b>Tool::Point - Intersect Curves:</b> Select first curve"),
        &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersectionCurves>,
        &MainWindow::ApplyDrawDialog<VToolPointOfIntersectionCurves>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleCurveIntersectAxisTool(bool checked)
{
    selectAllDraftObjectsTool();
    SetToolButtonWithApply<DialogCurveIntersectAxis>(
        checked,
        Tool::CurveIntersectAxis,
        ":/cursor/curve_intersect_axis_cursor.png",
        tr("<b>Tool::Point - Intersect Curve and Axis:</b> Select curve"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCurveIntersectAxis>,
        &MainWindow::ApplyDrawDialog<VToolCurveIntersectAxis>);
}

// Arcs
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleArcTool handler for arc tool.
 * @param checked true - button checked.
 */
void MainWindow::handleArcTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogArc>(
        checked,
        Tool::Arc,
        ":/cursor/arc_cursor.png",
        tr("<b>Tool::Arc - Radius and Angles:</b> Select point of center of arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolArc>,
        &MainWindow::ApplyDrawDialog<VToolArc>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePointAlongArcTool handler for PointAlongArc tool.
 * @param checked true - button checked.
 */
void MainWindow::handlePointAlongArcTool(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogCutArc>(
        checked,
        Tool::CutArc,
        ":/cursor/arc_cut_cursor.png",
        tr("<b>Tool::Point - On Arc:</b> Select arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCutArc>,
        &MainWindow::ApplyDrawDialog<VToolCutArc>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleArcIntersectAxisTool(bool checked)
{
    selectAllDraftObjectsTool();
    // Reuse handleCurveIntersectAxisTool but with different cursor and tool tip
    SetToolButtonWithApply<DialogCurveIntersectAxis>(
        checked,
        Tool::ArcIntersectAxis,
        ":/cursor/arc_intersect_axis_cursor.png",
        tr("<b>Tool::Point - Intersect Arc and Axis:</b> Select arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolCurveIntersectAxis>,
        &MainWindow::ApplyDrawDialog<VToolCurveIntersectAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handlePointOfIntersectionArcsTool(bool checked)
{
    ToolSelectArc();
    SetToolButtonWithApply<DialogPointOfIntersectionArcs>(
        checked,
        Tool::PointOfIntersectionArcs,
        "://cursor/point_of_intersection_arcs.png",
        tr("<b>Tool::Point - Intersect Arcs:</b> Select first an arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolPointOfIntersectionArcs>,
        &MainWindow::ApplyDrawDialog<VToolPointOfIntersectionArcs>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handlePointOfIntersectionCirclesTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<IntersectCirclesDialog>(
        checked,
        Tool::PointOfIntersectionCircles,
        "://cursor/point_of_intersection_circles.png",
        tr("<b>Tool::Point - Intersect Circles:</b> Select first circle center"),
        &MainWindow::ClosedDrawDialogWithApply<IntersectCirclesTool>,
        &MainWindow::ApplyDrawDialog<IntersectCirclesTool>);
}


//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handlePointFromCircleAndTangentTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<IntersectCircleTangentDialog>(
        checked,
        Tool::PointFromCircleAndTangent,
        "://cursor/point_from_circle_and_tangent_cursor.png",
        tr("<b>Tool::Point - Intersect Circle and Tangent:</b> Select point on tangent"),
        &MainWindow::ClosedDrawDialogWithApply<IntersectCircleTangentTool>,
        &MainWindow::ApplyDrawDialog<IntersectCircleTangentTool>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handlePointFromArcAndTangentTool(bool checked)
{
    ToolSelectPointArc();
    SetToolButtonWithApply<DialogPointFromArcAndTangent>(
        checked,
        Tool::PointFromArcAndTangent,
        "://cursor/point_from_arc_and_tangent_cursor.png",
        tr("<b>Tool::Point - Intersect Arc and Tangent:</b> Select point on tangent"),
        &MainWindow::ClosedDrawDialogWithApply<VToolPointFromArcAndTangent>,
        &MainWindow::ApplyDrawDialog<VToolPointFromArcAndTangent>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleArcWithLengthTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogArcWithLength>(
        checked,
        Tool::ArcWithLength,
        "://cursor/arc_with_length_cursor.png",
        tr("<b>Tool::Arc - Radius and Length:</b> Select point of the center of the arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolArcWithLength>,
        &MainWindow::ApplyDrawDialog<VToolArcWithLength>);
}

// Elliptical Arcs
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleEllipticalArcTool handler for EllipticalArc tool.
 * @param checked true - button checked.
 */
void MainWindow::handleEllipticalArcTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogEllipticalArc>(
        checked,
        Tool::EllipticalArc,
        ":/cursor/el_arc_cursor.png",
        tr("<b>Tool::Arc - Elliptical:</b> Select point of center of elliptical arc"),
        &MainWindow::ClosedDrawDialogWithApply<VToolEllipticalArc>,
        &MainWindow::ApplyDrawDialog<VToolEllipticalArc>);
}

// Operations
//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleGroupTool(bool checked)
{
    ToolSelectGroupObjects();
    const QString tooltip =
        tr("<b>Tool::Operations - Create Group:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to finish group creation ")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButton<AddToGroupDialog>(
        checked,
        Tool::Group,
        ":/cursor/group_cursor.png",
        tooltip,
        &MainWindow::ClosedEditGroupDialog);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedEditGroupDialog(int result)
{
    SCASSERT(m_dialogTool != nullptr)
    if (result == QDialog::Accepted) {
        QSharedPointer<AddToGroupDialog> dialog = m_dialogTool.objectCast<AddToGroupDialog>();
        SCASSERT(dialog != nullptr)

        QString gName = dialog->getName();
        QMap<quint32, quint32> gData = dialog->getGroupData();
        QDomElement group = doc->addGroupItems(gName, gData);
        if (group.isNull()) {
            QMessageBox::information(
                this,
                tr("Add Group Objects"),
                tr("Group is locked. Unlock to add objects"),
                QMessageBox::Ok,
                QMessageBox::Ok);
        }
    }
    handleArrowTool(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleRotationTool(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip =
        tr("<b>Tool::Operations - Rotation:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to confirm selection")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButtonWithApply<DialogRotation>(
        checked,
        Tool::Rotation,
        ":/cursor/rotation_cursor.png",
        tooltip,
        &MainWindow::ClosedDrawDialogWithApply<VToolRotation>,
        &MainWindow::ApplyDrawDialog<VToolRotation>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleMirrorByLineTool(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip =
        tr("<b>Tool::Operations - Mirror by Line:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to confirm selection")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButtonWithApply<DialogMirrorByLine>(
        checked,
        Tool::MirrorByLine,
        ":/cursor/mirror_by_line_cursor.png",
        tooltip,
        &MainWindow::ClosedDrawDialogWithApply<VToolMirrorByLine>,
        &MainWindow::ApplyDrawDialog<VToolMirrorByLine>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleMirrorByAxisTool(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip =
        tr("<b>Tool::Operations - Mirror by Axis:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to confirm selection")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButtonWithApply<DialogMirrorByAxis>(
        checked,
        Tool::MirrorByAxis,
        ":/cursor/mirror_by_axis_cursor.png",
        tooltip,
        &MainWindow::ClosedDrawDialogWithApply<VToolMirrorByAxis>,
        &MainWindow::ApplyDrawDialog<VToolMirrorByAxis>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleMoveTool(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip =
        tr("<b>Tool::Operations - Move:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to confirm selection")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButtonWithApply<DialogMove>(
        checked,
        Tool::Move,
        ":/cursor/move_cursor.png",
        tooltip,
        &MainWindow::ClosedDrawDialogWithApply<VToolMove>,
        &MainWindow::ApplyDrawDialog<VToolMove>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleTrueDartTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButtonWithApply<DialogTrueDarts>(
        checked,
        Tool::TrueDarts,
        "://cursor/true_darts_cursor.png",
        tr("<b>Tool::Operations - TrueDarts:</b> Select the first base line point"),
        &MainWindow::ClosedDrawDialogWithApply<VToolTrueDarts>,
        &MainWindow::ApplyDrawDialog<VToolTrueDarts>);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handlePatternPieceTool handler for pattern piece tool.
 * @param checked true - button checked.
 */
void MainWindow::handlePatternPieceTool(bool checked)
{
    selectAllDraftObjectsTool();
    SetToolButtonWithApply<PatternPieceDialog>(
        checked,
        Tool::Piece,
        ":/cursor/new_piece_cursor.png",
        tr("<b>Tool::Piece - Add New Pattern Piece:</b> Select main path of objects clockwise."),
        &MainWindow::ClosedPiecesDialogWithApply<PatternPieceTool>,
        &MainWindow::applyPiecesDialog<PatternPieceTool>);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleAnchorPointTool(bool checked)
{
    ToolSelectPointByRelease();
    SetToolButton<AnchorPointDialog>(
        checked,
        Tool::AnchorPoint,
        ":/cursor/anchor_point_cursor.png",
        tr("<b>Tool::Piece - Add Anchor Point:</b> Select anchor point"),
        &MainWindow::ClosedDialogAnchorPoint);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogAnchorPoint(int result)
{
    SCASSERT(m_dialogTool != nullptr);
    if (result == QDialog::Accepted) {
        AnchorPointTool::Create(m_dialogTool, doc, pattern);
    }
    handleArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleInternalPathTool(bool checked)
{
    selectAllDraftObjectsTool();
    SetToolButton<DialogInternalPath>(
        checked,
        Tool::InternalPath,
        ":/cursor/path_cursor.png",
        tr("<b>Tool::Piece - Internal Path:</b> Select path objects, use <b>SHIFT</b> to reverse "
           "curve direction"),
        &MainWindow::ClosedDialogInternalPath);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedDialogInternalPath(int result)
{
    SCASSERT(m_dialogTool != nullptr);
    if (result == QDialog::Accepted) {
        VToolInternalPath::Create(m_dialogTool, m_pieceScene, doc, pattern);
    }
    handleArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleInsertNodesTool(bool checked)
{
    ToolSelectOperationObjects();
    const QString tooltip =
        tr("<b>Tool::Piece - Insert Nodes:</b> Select one or more objects -"
           " Hold <b>%1</b> for multiple selection, "
           "Press <b>ENTER</b> to confirm selection")
            .arg(QCoreApplication::translate(
                strQShortcut.toUtf8().constData(), strCtrl.toUtf8().constData()));
    SetToolButton<InsertNodesDialog>(
        checked,
        Tool::InsertNodes,
        "://cursor/insert_nodes_cursor.png",
        tooltip,
        &MainWindow::ClosedInsertNodesDialog);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ClosedInsertNodesDialog(int result)
{
    SCASSERT(m_dialogTool != nullptr);
    if (result == QDialog::Accepted) {
        QSharedPointer<InsertNodesDialog> tool = m_dialogTool.objectCast<InsertNodesDialog>();
        SCASSERT(tool != nullptr)
        PatternPieceTool::insertNodes(
            tool->getNodes(), tool->getPieceId(), m_pieceScene, pattern, doc);
    }
    handleArrowTool(true);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleImportImage()
{
    qCDebug(vMainWindow, "Add Image");
    m_ui->draft_ToolBox->setCurrentWidget(m_ui->backgroundImage_Page);
    m_ui->importImage_ToolButton->setChecked(true);

    QString filename = getImageFilename();
    qCDebug(vMainWindow, "Image filename: %s", qUtf8Printable(filename));

    DraftImage image;
    QFileInfo f(filename);
    if (filename.isEmpty()) {
        m_ui->importImage_ToolButton->setChecked(false);
        return;
    }
    image.name = f.baseName();
    image.filename = filename;
    image.units = qApp->patternUnit();

    addImage(image);
}

void MainWindow::addImage(DraftImage image)
{
    static bool firstImportImage = false;
    QImageReader imageReader(image.filename);

    image.id = VContainer::getNextId();

    if (!imageReader.canRead()) {
        qCDebug(vMainWindow, "Can't read image");
        QMessageBox::critical(
            this,
            tr("Can't read image"),
            tr("Could not read the image.") + "\n" + tr("It may be corrupted..."),
            QMessageBox::Ok);
        return;
    }

    image.pixmap = QPixmap::fromImageReader(&imageReader);

    if (image.pixmap.isNull()) {
        qCDebug(vMainWindow, "Can't read image");
        QMessageBox::critical(
            this,
            tr("Can't read image"),
            tr("Could not read the image.") + "\n" + tr("It may be corrupted or empty..."),
            QMessageBox::Ok);
        return;
    }

    if (image.width == 0 || image.height == 0) {
        image.width = image.pixmap.width();
        image.height = image.pixmap.height();
    }

    ImageItem* item = new ImageItem(doc, image);
    doc->addBackgroundImage(image.id, item);
    m_draftScene->addItem(item);
    // Need error dialog

    // connect(this, &MainWindow::EnableImageSelection, item, &ImageItem::enableSelection);
    // connect(this, &MainWindow::EnableImageHover, item, &ImageItem::enableHovering);

    connect(item, &ImageItem::deleteImage, this, &MainWindow::handleDeleteImage);
    // connect(item, &ImageItem::imageSelected, this, &MainWindow::handleImageSelected);
    connect(item, &ImageItem::setStatusMessage, this, &MainWindow::setStatusMessage);

    m_ui->importImage_ToolButton->setChecked(false);

    if (!firstImportImage) {
        qCDebug(vMainWindow, "Inside first import image");
        InfoUnsavedImages(&firstImportImage);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleDeleteImage(quint32 id)
{
    qCDebug(vMainWindow, "delete Image");
    bool deleteConfirmed = true;

    if (qApp->Settings()->getConfirmItemDelete()) {
        Utils::CheckableMessageBox msgBox(qApp->getMainWindow());
        msgBox.setWindowTitle(tr("Confirm deletion"));
        msgBox.setText(tr("Do you really want to delete?"));
        msgBox.setStandardButtons(QDialogButtonBox::Yes | QDialogButtonBox::No);
        msgBox.setDefaultButton(QDialogButtonBox::No);
        msgBox.setIconPixmap(
            QApplication::style()->standardIcon(QStyle::SP_MessageBoxQuestion).pixmap(32, 32));

        int dialogResult = msgBox.exec();

        if (dialogResult != QDialog::Accepted) {
            deleteConfirmed = false;
        }

        qApp->Settings()->setConfirmItemDelete(not msgBox.isChecked());
    }

    if (deleteConfirmed) {
        ImageItem* item = doc->getBackgroundImage(id);
        if (item) {
            doc->removeBackgroundImage(id);
            item->deleteImageItem();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleImageSelected(quint32 id)
{
    ImageItem* item = doc->getBackgroundImage(id);
    if (item) {
        // May be useful in the development of the background-image feature
    }
}

//--------------------------------------------------------------------------------------------------------------------
void MainWindow::setStatusMessage(QString message) { m_helpLabel->setText(message); }

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleLockImage(bool state)
{
    qCDebug(vMainWindow, "lock Image = %s", state ? "True" : "False");
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleAlignImage() { qCDebug(vMainWindow, "align Image"); }

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::handleLockImageAspect(bool state)
{
    qCDebug(vMainWindow, "lock Image Aspect= %s", state ? "True" : "False");
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::updateImage(DraftImage image)
{
    qCDebug(vMainWindow, "Id = %d", image.id);
    qCDebug(vMainWindow, "Name = %s", qUtf8Printable(image.name));
    qCDebug(vMainWindow, "Filename = %s", qUtf8Printable(image.filename));
    qCDebug(vMainWindow, "lock Image = %s", image.locked ? "True" : "False");
    qCDebug(vMainWindow, "Anchor = %d", static_cast<int>(image.anchor));
    qCDebug(vMainWindow, "Xpos = %f", image.xPos);
    qCDebug(vMainWindow, "YPos = %f", image.yPos);
    qCDebug(vMainWindow, "Width = %f", image.width);
    qCDebug(vMainWindow, "Height = %f", image.height);
    qCDebug(vMainWindow, "lock Image Aspect = %s", image.aspectLocked ? "True" : "False");
    qCDebug(vMainWindow, "Units = %d", static_cast<int>(image.units));
    qCDebug(vMainWindow, "Rotation = %f", image.rotation);
    qCDebug(vMainWindow, "Visible = %s", image.visible ? "True" : "False");
    qCDebug(vMainWindow, "Opacity = %f", image.opacity);
    qCDebug(vMainWindow, "Order = %d\n", static_cast<int>(image.order));

    auto* item = dynamic_cast<ImageItem*>(qApp->getCurrentScene()->focusItem());
    if (item) {
        item->setImage(image);
    }
}


//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::getImageFilename()
{
    const QString filter = tr("Images") + QLatin1String(" (*.bmp *.jpg *.png *.svg *.tf);;") + "BMP"
                         + QLatin1String(" (*.bmp);;") + "JPG" + QLatin1String(" (*.jpg);;") + "PNG"
                         + QLatin1String(" (*.png);;") + "SVG" + QLatin1String(" (*.svg);;") + "TIF"
                         + QLatin1String(" (*.tf)");

    const QString path = qApp->Seamly2DSettings()->getImageFilePath();

    QDir directory(path);
    if (!directory.exists()) {
        directory.mkpath(".");
    }

    return QFileDialog::getOpenFileName(
        this, tr("Open Image File"), path, filter, nullptr, QFileDialog::DontUseNativeDialog);
}

// Pieces
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleUnionTool handler for Union tool.
 * @param checked true - button checked.
 */
void MainWindow::handleUnionTool(bool checked)
{
    selectPieceTool();
    SetToolButton<UnionDialog>(
        checked,
        Tool::Union,
        ":/cursor/union_cursor.png",
        tr("<b>Tool::Details - Union:</b> Select pattern piece"),
        &MainWindow::closeUnionDialog);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeUnionDialog actions after closing Union tool dialog.
 * @param result result of dialog working.
 */
void MainWindow::closeUnionDialog(int result)
{
    ClosedDialog<UnionTool>(result);
    doc->LiteParseTree(Document::LiteParse);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleNewLayout handler for New Layout tool.
 * @param tButton - toolbutton.
 * @param checked true - button checked.
 */
void MainWindow::handleNewLayout(bool checked)
{
    toolLayoutSettings(m_ui->layoutSettings_ToolButton, checked);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ShowTool  highlight tool.Tip show tools tooltip.
 * @param toolTip tooltip text.
 */
void MainWindow::ShowToolTip(const QString& toolTip) { m_helpLabel->setText(toolTip); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief triggers the update of the groups
 */
void MainWindow::updateGroups() { m_groupsWidget->updateGroups(); }

void MainWindow::showAllGroups() { m_groupsWidget->showAllGroups(); }

void MainWindow::hideAllGroups() { m_groupsWidget->hideAllGroups(); }

void MainWindow::lockAllGroups() { m_groupsWidget->lockAllGroups(); }

void MainWindow::unlockAllGroups() { m_groupsWidget->unlockAllGroups(); }

void MainWindow::addGroupToList() { m_groupsWidget->addGroupToList(); }
void MainWindow::deleteGroupFromList() { m_groupsWidget->deleteGroupFromList(); }

void MainWindow::editGroup() { m_groupsWidget->editGroup(); }

void MainWindow::addSelectedItemsToGroup() { qCDebug(vMainWindow, "Add Selected items to Group."); }

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle after show window.
 * @param event show event.
 */
void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    if (event->spontaneous()) {
        return;
    }

    if (m_isInitialized) {
        return;
    }
    // do your initialize stuff here

    MinimumScrollBar();

    m_isInitialized = true;   // first show windows are held
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        // retranslate designer form (single inheritance approach)
        m_ui->retranslateUi(this);
        undoAction->setText(tr("&Undo"));
        redoAction->setText(tr("&Redo"));
        m_helpLabel->setText(QObject::tr("Changes applied."));
        m_draftBlockLabel->setText(tr("Draft Block:"));

        if (m_mode == Draw::Calculation) {
            m_ui->groups_DockWidget->setWindowTitle(tr("Group Manager"));
        } else {
            m_ui->groups_DockWidget->setWindowTitle(tr("Pattern Pieces"));
        }

        UpdateWindowTitle();
        initPenToolBar();
        initBasePointComboBox();
        emit m_pieceScene->LanguageChanged();
    }
    // remember to call base class implementation
    QMainWindow::changeEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle after close window.
 * @param event close event.
 */
void MainWindow::closeEvent(QCloseEvent* event)
{
    qCDebug(vMainWindow, "Closing main window");
    if (MaybeSave()) {
        FileClosedCorrect();

        event->accept();
        QApplication::closeAllWindows();
    } else {
        qCDebug(vMainWindow, "Closing canceled.");
        event->ignore();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::customEvent(QEvent* event)
{
    if (event->type() == UNDO_EVENT) {
        qApp->getUndoStack()->undo();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::CleanLayout()
{
    qDeleteAll(scenes);
    scenes.clear();
    shadows.clear();
    papers.clear();
    m_ui->listWidget->clear();
    m_groupsWidget->clear();
    SetLayoutModeActions();
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::PrepareSceneList()
{
    for (int i = 1; i <= scenes.size(); ++i) {
        QListWidgetItem* item = new QListWidgetItem(ScenePreview(i - 1), QString::number(i));
        m_ui->listWidget->addItem(item);
    }

    if (!scenes.isEmpty()) {
        m_ui->listWidget->setCurrentRow(0);
        SetLayoutModeActions();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::exportToCSVData(const QString& fileName, const DialogExportToCSV& dialog)
{
    QxtCsvModel csv;

    csv.insertColumn(0);
    csv.insertColumn(1);
    csv.insertColumn(2);

    if (dialog.WithHeader()) {
        csv.setHeaderText(0, tr("Name"));
        csv.setHeaderText(1, tr("The calculated value"));
        csv.setHeaderText(2, tr("Formula"));
    }

    const QMap<QString, QSharedPointer<CustomVariable>> customVariables = pattern->variablesData();
    QMap<QString, QSharedPointer<CustomVariable>>::const_iterator i;
    QMap<quint32, QString> map;
    // Sorting QHash by id
    for (i = customVariables.constBegin(); i != customVariables.constEnd(); ++i) {
        QSharedPointer<CustomVariable> variable = i.value();
        map.insert(variable->getIndex(), i.key());
    }

    qint32 currentRow = -1;
    QMapIterator<quint32, QString> iMap(map);
    while (iMap.hasNext()) {
        iMap.next();
        QSharedPointer<CustomVariable> variable = customVariables.value(iMap.value());
        currentRow++;

        csv.insertRow(currentRow);
        csv.setText(currentRow, 0, variable->GetName());   // name
        csv.setText(
            currentRow, 1, qApp->LocaleToString(*variable->GetValue()));   // calculated value

        QString formula;
        try {
            formula = qApp->translateVariables()->FormulaToUser(
                variable->GetFormula(), qApp->Settings()->getOsSeparator());
        } catch (qmu::QmuParserError& error) {
            Q_UNUSED(error)
            formula = variable->GetFormula();
        }

        csv.setText(currentRow, 2, formula);   // formula
    }

    csv.toCSV(
        fileName,
        dialog.WithHeader(),
        dialog.Separator(),
        QTextCodec::codecForMib(dialog.SelectedMib()));
}

void MainWindow::handleExportToCSV()
{
    QString file = tr("untitled");
    if (!qApp->getFilePath().isEmpty()) {
        QString filePath = qApp->getFilePath();
        file = QFileInfo(filePath).baseName();
    }
    exportToCSV(file);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadIndividual()
{
    const QString filter = tr("Individual measurements") + QLatin1String(" (*.") + smisExt
                         + QLatin1String(" *.") + vitExt + QLatin1String(")");

    // Use standard path to individual measurements
    const QString dir = qApp->Seamly2DSettings()->getIndividualSizePath();

    bool usedNotExistedDir = false;
    QDir directory(dir);

    if (!directory.exists()) {
        usedNotExistedDir = directory.mkpath(".");
    }

    const QString filename = fileDialog(
        this,
        tr("Open file"),
        dir,
        filter,
        nullptr,
        QFileDialog::DontUseNativeDialog,
        QFileDialog::ExistingFile,
        QFileDialog::AcceptOpen);


    if (!filename.isEmpty()) {
        if (loadMeasurements(filename)) {
            if (!doc->MPath().isEmpty()) {
                m_watcher->removePath(AbsoluteMPath(qApp->getFilePath(), doc->MPath()));
            }

            qCInfo(vMainWindow, "Individual file %s was loaded.", qUtf8Printable(filename));

            m_ui->unloadMeasurements_Action->setEnabled(true);

            doc->SetMPath(RelativeMPath(qApp->getFilePath(), filename));
            m_watcher->addPath(filename);
            patternChangesWereSaved(false);

            m_ui->editCurrent_Action->setEnabled(true);
            m_helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);

            UpdateWindowTitle();
        }
    }

    if (usedNotExistedDir) {
        QDir directory(dir);
        directory.rmpath(".");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadMultisize()
{
    const QString filter = tr("Multisize measurements") + QLatin1String(" (*.") + smmsExt
                         + QLatin1String(" *.") + vstExt + QLatin1String(")");

    // Use standard path to multisize measurements
    QString dir = qApp->Seamly2DSettings()->getMultisizePath();
    dir = VCommonSettings::prepareMultisizeTables(dir);

    const QString filename = fileDialog(
        this,
        tr("Open file"),
        dir,
        filter,
        nullptr,
        QFileDialog::DontUseNativeDialog,
        QFileDialog::ExistingFile,
        QFileDialog::AcceptOpen);

    if (!filename.isEmpty()) {
        QString hText;
        if (!m_gradationHeights.isNull()) {
            hText = m_gradationHeights->currentText();
        }
        QString sText;
        if (!m_gradationSizes.isNull()) {
            sText = m_gradationSizes->currentText();
        }

        if (loadMeasurements(filename)) {
            if (!doc->MPath().isEmpty()) {
                m_watcher->removePath(AbsoluteMPath(qApp->getFilePath(), doc->MPath()));
            }

            qCInfo(vMainWindow, "Multisize file %s was loaded.", qUtf8Printable(filename));

            m_ui->unloadMeasurements_Action->setEnabled(true);

            doc->SetMPath(RelativeMPath(qApp->getFilePath(), filename));
            m_watcher->addPath(filename);
            patternChangesWereSaved(false);

            m_ui->editCurrent_Action->setEnabled(true);
            m_helpLabel->setText(tr("Measurements loaded"));
            doc->LiteParseTree(Document::LiteParse);

            UpdateWindowTitle();

            if (qApp->patternType() == MeasurementsType::Multisize) {
                if (!hText.isEmpty() && not m_gradationHeights.isNull()) {
                    m_gradationHeights->setCurrentText(hText);
                }

                if (!sText.isEmpty() && not m_gradationSizes.isNull()) {
                    m_gradationSizes->setCurrentText(sText);
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UnloadMeasurements()
{
    if (doc->MPath().isEmpty()) {
        m_ui->unloadMeasurements_Action->setDisabled(true);
        return;
    }

    if (doc->ListMeasurements().isEmpty()) {
        m_watcher->removePath(AbsoluteMPath(qApp->getFilePath(), doc->MPath()));
        if (qApp->patternType() == MeasurementsType::Multisize) {
            initializeStatusToolBar();
        }
        qApp->setPatternType(MeasurementsType::Unknown);
        doc->SetMPath(QString());
        emit doc->UpdatePatternLabel();
        patternChangesWereSaved(false);
        m_ui->editCurrent_Action->setEnabled(false);
        m_ui->unloadMeasurements_Action->setDisabled(true);
        m_helpLabel->setText(tr("Measurements unloaded"));

        UpdateWindowTitle();
    } else {
        qCWarning(
            vMainWindow,
            "%s",
            qUtf8Printable(
                tr("Couldn't unload measurements. Some of them are used in the pattern.")));
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ShowMeasurements()
{
    if (!doc->MPath().isEmpty()) {
        const QString absoluteMPath = AbsoluteMPath(qApp->getFilePath(), doc->MPath());

        QStringList arguments;
        if (qApp->patternType() == MeasurementsType::Multisize) {
            arguments = QStringList()
                     << absoluteMPath << "-u" << UnitsToStr(qApp->patternUnit()) << "-e"
                     << QString().setNum(static_cast<int>(
                            UnitConvertor(VContainer::height(), doc->measurementUnits(), Unit::Cm)))
                     << "-s"
                     << QString().setNum(static_cast<int>(
                            UnitConvertor(VContainer::size(), doc->measurementUnits(), Unit::Cm)));
        } else {
            arguments = QStringList() << absoluteMPath << "-u" << UnitsToStr(qApp->patternUnit());
        }

        if (isNoScaling) {
            arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
        }

        const QString seamlyme = qApp->seamlyMeFilePath();
        const QString workingDirectory = QFileInfo(seamlyme).absoluteDir().absolutePath();
        QProcess::startDetached(seamlyme, arguments, workingDirectory);
    } else {
        m_ui->editCurrent_Action->setEnabled(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::MeasurementsChanged(const QString& path)
{
    m_changes = false;
    QFileInfo checkFile(path);
    if (checkFile.exists()) {
        m_changes = true;
        m_changesAsked = false;
    } else {
        for (int i = 0; i <= 1000; i = i + 10) {
            if (checkFile.exists()) {
                m_changes = true;
                m_changesAsked = false;
                break;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }

    UpdateWindowTitle();
    m_ui->syncMeasurements_Action->setEnabled(m_changes);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SyncMeasurements()
{
    if (m_changes) {
        const QString path = AbsoluteMPath(qApp->getFilePath(), doc->MPath());
        if (updateMeasurements(
                path,
                static_cast<int>(VContainer::size()),
                static_cast<int>(VContainer::height()))) {
            if (!m_watcher->files().contains(path)) {
                m_watcher->addPath(path);
            }
            const QString msg = tr("Measurements have been synced");
            qCDebug(vMainWindow, "%s", qUtf8Printable(msg));
            m_helpLabel->setText(msg);
            VWidgetPopup::PopupMessage(this, msg);
            doc->LiteParseTree(Document::LiteParse);
            m_changes = false;
            m_changesAsked = true;
            UpdateWindowTitle();
            m_ui->syncMeasurements_Action->setEnabled(m_changes);
        } else {
            qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't sync measurements.")));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_MAC)
void MainWindow::OpenAt(QAction* where)
{
    const QString path =
        qApp->getFilePath().left(qApp->getFilePath().indexOf(where->text())) + where->text();
    if (path == qApp->getFilePath()) {
        return;
    }
    QProcess process;
    process.start(QStringLiteral("/usr/bin/open"), QStringList() << path, QIODevice::ReadOnly);
    process.waitForFinished();
}
#endif   // defined(Q_OS_MAC)

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initializeStatusToolBar initialize horizontal bar for presenting status information
 */
void MainWindow::initializeStatusToolBar()
{
    if (!m_mouseCoordinates.isNull()) {
        delete m_mouseCoordinates;
    }
    if (!m_infoToolButton.isNull()) {
        delete m_infoToolButton;
    }
    if (!m_gradationHeights.isNull()) {
        delete m_gradationHeights;
    }
    if (!m_gradationSizes.isNull()) {
        delete m_gradationSizes;
    }
    if (!m_gradationHeightsLabel.isNull()) {
        delete m_gradationHeightsLabel;
    }
    if (!m_gradationSizesLabel.isNull()) {
        delete m_gradationSizesLabel;
    }

    if (qApp->patternType() == MeasurementsType::Multisize) {
        const QStringList listHeights =
            MeasurementVariable::ListHeights(doc->GetGradationHeights(), qApp->patternUnit());
        const QStringList listSizes =
            MeasurementVariable::ListSizes(doc->GetGradationSizes(), qApp->patternUnit());

        m_gradationHeightsLabel = new QLabel(tr("Height:"), this);
        m_gradationHeights = SetGradationList(m_gradationHeightsLabel, listHeights);

        // set default height
        SetDefaultHeight();

        connect(
            m_gradationHeights.data(),
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::ChangedHeight);

        m_gradationSizesLabel = new QLabel(tr("Size:"), this);
        m_gradationSizes = SetGradationList(m_gradationSizesLabel, listSizes);

        // set default size
        SetDefaultSize();

        connect(
            m_gradationSizes.data(),
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::ChangedSize);
    }

    m_mouseCoordinates = new MouseCoordinates(qApp->patternUnit());
    m_ui->statusBar->addPermanentWidget((m_mouseCoordinates));

    m_infoToolButton = new QToolButton();
    m_infoToolButton->setDefaultAction(m_ui->documentInfo_Action);
    m_ui->statusBar->addPermanentWidget((m_infoToolButton));
}

//---------------------------------------------------------------------------------------------------------------------
QComboBox* MainWindow::SetGradationList(QLabel* label, const QStringList& list)
{
    QComboBox* comboBox = new QComboBox(this);
    comboBox->addItems(list);
    m_ui->statusBar->addPermanentWidget(label);
    m_ui->statusBar->addPermanentWidget(comboBox);

    return comboBox;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeModesToolBar()
{
    m_leftGoToStage = new QLabel(this);
    m_leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
    m_ui->mode_ToolBar->insertWidget(m_ui->pieceMode_Action, m_leftGoToStage);

    m_rightGoToStage = new QLabel(this);
    m_rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));
    m_ui->mode_ToolBar->insertWidget(m_ui->layoutMode_Action, m_rightGoToStage);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initializePointNameToolBar enable Point Name toolbar.
 */
void MainWindow::initializePointNameToolBar()
{
    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setCurrentFont(qApp->Seamly2DSettings()->getPointNameFont());
    m_ui->pointName_ToolBar->insertWidget(m_ui->showPointNames_Action, m_fontComboBox);
    m_fontComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_fontComboBox->setEnabled(true);

    connect(
        m_fontComboBox,
        static_cast<void (QFontComboBox::*)(const QFont&)>(&QFontComboBox::currentFontChanged),
        this,
        [this](QFont font) {
            qApp->Seamly2DSettings()->setPointNameFont(font);
            upDateScenes();
        });

    m_fontSizeComboBox = new QComboBox;
    m_ui->pointName_ToolBar->insertWidget(m_ui->showPointNames_Action, m_fontSizeComboBox);
    m_fontSizeComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_fontSizeComboBox->addItem("6", QVariant(static_cast<int>(6)));
    m_fontSizeComboBox->addItem("7", QVariant(static_cast<int>(7)));
    m_fontSizeComboBox->addItem("8", QVariant(static_cast<int>(8)));
    m_fontSizeComboBox->addItem("9", QVariant(static_cast<int>(9)));
    m_fontSizeComboBox->addItem("10", QVariant(static_cast<int>(10)));
    m_fontSizeComboBox->addItem("11", QVariant(static_cast<int>(11)));
    m_fontSizeComboBox->addItem("12", QVariant(static_cast<int>(12)));
    m_fontSizeComboBox->addItem("13", QVariant(static_cast<int>(13)));
    m_fontSizeComboBox->addItem("14", QVariant(static_cast<int>(14)));
    m_fontSizeComboBox->addItem("15", QVariant(static_cast<int>(15)));
    m_fontSizeComboBox->addItem("16", QVariant(static_cast<int>(16)));
    m_fontSizeComboBox->addItem("18", QVariant(static_cast<int>(18)));
    m_fontSizeComboBox->addItem("20", QVariant(static_cast<int>(20)));
    m_fontSizeComboBox->addItem("22", QVariant(static_cast<int>(22)));
    m_fontSizeComboBox->addItem("24", QVariant(static_cast<int>(24)));
    m_fontSizeComboBox->addItem("26", QVariant(static_cast<int>(26)));
    m_fontSizeComboBox->addItem("28", QVariant(static_cast<int>(28)));
    m_fontSizeComboBox->addItem("32", QVariant(static_cast<int>(32)));
    m_fontSizeComboBox->addItem("36", QVariant(static_cast<int>(36)));
    m_fontSizeComboBox->addItem("40", QVariant(static_cast<int>(40)));
    m_fontSizeComboBox->addItem("44", QVariant(static_cast<int>(44)));
    m_fontSizeComboBox->addItem("48", QVariant(static_cast<int>(48)));
    m_fontSizeComboBox->addItem("54", QVariant(static_cast<int>(54)));
    m_fontSizeComboBox->addItem("60", QVariant(static_cast<int>(60)));
    m_fontSizeComboBox->addItem("66", QVariant(static_cast<int>(66)));
    m_fontSizeComboBox->addItem("72", QVariant(static_cast<int>(72)));
    m_fontSizeComboBox->addItem("80", QVariant(static_cast<int>(80)));
    m_fontSizeComboBox->addItem("96", QVariant(static_cast<int>(96)));

    int index = m_fontSizeComboBox->findData(qApp->Seamly2DSettings()->getPointNameSize());
    if (index < 0 || index > 28) {
        index = 18;
    }
    m_fontSizeComboBox->setCurrentIndex(index);

    connect(m_fontSizeComboBox, &QComboBox::currentTextChanged, this, [this](QString text) {
        qApp->Seamly2DSettings()->setPointNameSize(text.toInt());
        upDateScenes();
    });
    m_fontSizeComboBox->setEnabled(true);

    m_basePointComboBox = new QComboBox;
    m_ui->pointName_ToolBar->addWidget(m_basePointComboBox);
    initBasePointComboBox();
    m_basePointComboBox->setEnabled(true);

    connect(
        m_basePointComboBox, &QComboBox::currentTextChanged, this, &MainWindow::basePointChanged);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initBasePointComboBox fills basePointComboBox.
 */
void MainWindow::initBasePointComboBox()
{
    m_basePointComboBox->clear();
    m_basePointComboBox->addItem(tr("Default"));
    m_basePointComboBox->addItems(
        doc->GetCurrentAlphabet());   // These items are based on the Point name language
    m_basePointComboBox->setToolTip(
        tr("Base name used for new points.\nPress enter to temporarily add it to the list."));
    m_basePointComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_basePointComboBox->setCurrentIndex(0);
    m_basePointComboBox->setEditable(true);
    m_basePointComboBox->setInsertPolicy(QComboBox::InsertAtTop);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initializeDraftToolBar enable draw toolbar.
 */
void MainWindow::initializeDraftToolBar()
{
    m_draftBlockLabel = new QLabel(tr("Draft Block:"));
    m_ui->draft_ToolBar->addWidget(m_draftBlockLabel);

    // By using Qt m_ui Designer we can't add QComboBox to toolbar
    m_draftBlockComboBox = new QComboBox;
    m_ui->draft_ToolBar->addWidget(m_draftBlockComboBox);
    m_draftBlockComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_draftBlockComboBox->setEnabled(false);

    connect(
        m_draftBlockComboBox,
        static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this,
        [this](int index) { changeDraftBlock(index); });

    connect(m_ui->renameDraft_Action, &QAction::triggered, this, [this]() {
        const QString activeDraftBlock = doc->getActiveDraftBlockName();
        const QString draftBlockName = createDraftBlockName(activeDraftBlock);
        if (draftBlockName.isEmpty()) {
            return;
        }
        RenameDraftBlock* draftBlock =
            new RenameDraftBlock(doc, draftBlockName, m_draftBlockComboBox);
        qApp->getUndoStack()->push(draftBlock);
        fullParseFile();
    });
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeToolsToolBar()
{
    /*First we will try use Standard Shortcuts from Qt, but because keypad "-" and "+" not the same
    keys like in main keypad, shortcut Ctrl+"-" or "+" from keypad will not working with standard
    shortcut (QKeySequence::ZoomIn or QKeySequence::ZoomOut). For example "+" is Qt::Key_Plus +
    Qt::KeypadModifier for keypad. Also for me don't work Qt:CTRL and work Qt::ControlModifier.*/

    QList<QKeySequence> zoomInShortcuts;
    zoomInShortcuts.append(QKeySequence(QKeySequence::ZoomIn));
    zoomInShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Plus + Qt::KeypadModifier));
    m_ui->zoomIn_Action->setShortcuts(zoomInShortcuts);
    connect(m_ui->zoomIn_Action, &QAction::triggered, m_ui->view, &VMainGraphicsView::zoomIn);

    QList<QKeySequence> zoomOutShortcuts;
    zoomOutShortcuts.append(QKeySequence(QKeySequence::ZoomOut));
    zoomOutShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Minus + Qt::KeypadModifier));
    m_ui->zoomOut_Action->setShortcuts(zoomOutShortcuts);
    connect(m_ui->zoomOut_Action, &QAction::triggered, m_ui->view, &VMainGraphicsView::zoomOut);

    QList<QKeySequence> zoom100PercentShortcuts;
    zoom100PercentShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_0));
    zoom100PercentShortcuts.append(
        QKeySequence(Qt::ControlModifier + Qt::Key_0 + Qt::KeypadModifier));
    m_ui->zoom100Percent_Action->setShortcuts(zoom100PercentShortcuts);
    connect(
        m_ui->zoom100Percent_Action,
        &QAction::triggered,
        m_ui->view,
        &VMainGraphicsView::zoom100Percent);

    QList<QKeySequence> zoomToFitShortcuts;
    zoomToFitShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Equal));
    m_ui->zoomToFit_Action->setShortcuts(zoomToFitShortcuts);
    connect(m_ui->zoomToFit_Action, &QAction::triggered, m_ui->view, &VMainGraphicsView::zoomToFit);

    QList<QKeySequence> zoomToSelectedShortcuts;
    zoomToSelectedShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Right));
    m_ui->zoomToSelected_Action->setShortcuts(zoomToSelectedShortcuts);
    connect(m_ui->zoomToSelected_Action, &QAction::triggered, this, &MainWindow::zoomToSelected);

    QList<QKeySequence> zoomToPreviousShortcuts;
    zoomToPreviousShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Left));
    m_ui->zoomToPrevious_Action->setShortcuts(zoomToPreviousShortcuts);
    connect(m_ui->zoomToPrevious_Action, &QAction::triggered, this, &MainWindow::zoomToPrevious);

    QList<QKeySequence> zoomToAreaShortcuts;
    zoomToAreaShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_A));
    m_ui->zoomToArea_Action->setShortcuts(zoomToAreaShortcuts);
    connect(m_ui->zoomToArea_Action, &QAction::toggled, this, &MainWindow::zoomToArea);

    resetPanShortcuts();
    connect(m_ui->zoomPan_Action, &QAction::toggled, this, &MainWindow::zoomPan);

    QList<QKeySequence> zoomToPointShortcuts;
    zoomToPointShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::AltModifier + Qt::Key_P));
    m_ui->zoomToPoint_Action->setShortcuts(zoomToPointShortcuts);
    connect(
        m_ui->zoomToPoint_Action, &QAction::triggered, this, &MainWindow::showZoomToPointDialog);

    m_zoomToPointComboBox = new QComboBox(m_ui->zoom_ToolBar);
    m_zoomToPointComboBox->setEnabled(false);
    m_zoomToPointComboBox->setToolTip(m_ui->zoomToPoint_Action->toolTip());
    m_ui->zoom_ToolBar->addWidget(m_zoomToPointComboBox);
    connect(m_zoomToPointComboBox, &QComboBox::currentTextChanged, this, &MainWindow::zoomToPoint);

    if (m_zoomScaleSpinBox != nullptr) {
        delete m_zoomScaleSpinBox;
    }
    m_zoomScaleSpinBox = new QDoubleSpinBox();
    m_zoomScaleSpinBox->setDecimals(1);
    m_zoomScaleSpinBox->setAlignment(Qt::AlignRight);
    m_zoomScaleSpinBox->setSingleStep(0.1);
    m_zoomScaleSpinBox->setMinimum(qFloor(VMainGraphicsView::MinScale() * 1000) / 10.0);
    m_zoomScaleSpinBox->setMaximum(qFloor(VMainGraphicsView::MaxScale() * 1000) / 10.0);
    m_zoomScaleSpinBox->setSuffix("%");
    m_zoomScaleSpinBox->setMaximumWidth(80);
    m_zoomScaleSpinBox->setKeyboardTracking(false);
    m_ui->zoom_ToolBar->insertWidget(m_ui->zoomIn_Action, m_zoomScaleSpinBox);

    zoomScaleChanged(m_ui->view->transform().m11());
    connect(
        m_zoomScaleSpinBox,
        QOverload<double>::of(&QDoubleSpinBox::valueChanged),
        this,
        [this](double d) { m_ui->view->zoomByScale(d / 100.0); });
}

void MainWindow::initializeToolBarVisibility()
{
    updateToolBarVisibility();
    connect(m_ui->tools_ToolBox_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->tools_ToolBox_ToolBar->setVisible(visible);
        qApp->Settings()->setShowToolsToolBar(visible);
    });
    connect(m_ui->points_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->points_ToolBar->setVisible(visible);
        qApp->Settings()->setShowPointToolBar(visible);
    });
    connect(m_ui->lines_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->lines_ToolBar->setVisible(visible);
        qApp->Settings()->setShowLineToolBar(visible);
    });
    connect(m_ui->curves_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->curves_ToolBar->setVisible(visible);
        qApp->Settings()->setShowCurveToolBar(visible);
    });
    connect(m_ui->arcs_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->arcs_ToolBar->setVisible(visible);
        qApp->Settings()->setShowArcToolBar(visible);
    });
    connect(m_ui->operations_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->operations_ToolBar->setVisible(visible);
        qApp->Settings()->setShowOpsToolBar(visible);
    });
    connect(m_ui->pieces_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->pieces_ToolBar->setVisible(visible);
        qApp->Settings()->setShowPieceToolBar(visible);
    });
    connect(m_ui->details_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->details_ToolBar->setVisible(visible);
        qApp->Settings()->setShowDetailsToolBar(visible);
    });
    connect(m_ui->layout_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->layout_ToolBar->setVisible(visible);
        qApp->Settings()->setShowLayoutToolBar(visible);
    });
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initPenToolBar enable default color, line wight & type toolbar.
 */
void MainWindow::initPenToolBar()
{
    if (m_penToolBar != nullptr) {
        disconnect(m_penToolBar, nullptr, this, nullptr);
        delete m_penToolBar;
    }
    m_penToolBar = new PenToolBar(tr("Pen Toolbar"), this);
    m_penToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_penToolBar->setObjectName("penToolBar");
    this->addToolBar(Qt::TopToolBarArea, m_penToolBar);

    connect(
        m_penToolBar, &PenToolBar::penChanged, this, [this](Pen pen) { doc->setDefaultPen(pen); });
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief initPropertyEditor initialize the Properties Editor.
 */
void MainWindow::initPropertyEditor()
{
    qCDebug(vMainWindow, "Initialize the Tool Property Editor.");
    if (m_toolProperties != nullptr) {
        disconnect(m_toolProperties, nullptr, this, nullptr);
        delete m_toolProperties;
    }
    m_toolProperties = new VToolOptionsPropertyBrowser(pattern, m_ui->toolProperties_DockWidget);

    connect(
        m_ui->view,
        &VMainGraphicsView::itemClicked,
        m_toolProperties,
        &VToolOptionsPropertyBrowser::itemClicked);
    connect(
        doc,
        &VPattern::FullUpdateFromFile,
        m_toolProperties,
        &VToolOptionsPropertyBrowser::updateOptions);
}

void MainWindow::basePointChanged()
{
    QString text = m_basePointComboBox->currentText();
    QString basePoint = QString();

    QRegularExpression rx(NameRegExp());
    if (rx.match(text).hasMatch() == false) {
        m_basePointComboBox->setStyleSheet("QComboBox {color: red;}");
    } else {
        m_basePointComboBox->setStyleSheet("QComboBox {color: black;}");

        if (!text.isEmpty() && text != tr("Default")) {
            basePoint = text;
        }
    }

    // we keep basePoint empty if default is selected
    doc->setDefaultBasePoint(basePoint);
}

void MainWindow::updateToolBarVisibility()
{
    setToolBarVisibility(m_ui->tools_ToolBox_ToolBar, qApp->Settings()->getShowToolsToolBar());
    setToolBarVisibility(m_ui->points_ToolBar, qApp->Settings()->getShowPointToolBar());
    setToolBarVisibility(m_ui->lines_ToolBar, qApp->Settings()->getShowLineToolBar());
    setToolBarVisibility(m_ui->curves_ToolBar, qApp->Settings()->getShowCurveToolBar());
    setToolBarVisibility(m_ui->arcs_ToolBar, qApp->Settings()->getShowArcToolBar());
    setToolBarVisibility(m_ui->operations_ToolBar, qApp->Settings()->getShowOpsToolBar());
    setToolBarVisibility(m_ui->pieces_ToolBar, qApp->Settings()->getShowPieceToolBar());
    setToolBarVisibility(m_ui->details_ToolBar, qApp->Settings()->getShowDetailsToolBar());
    setToolBarVisibility(m_ui->layout_ToolBar, qApp->Settings()->getShowLayoutToolBar());
}

void MainWindow::setToolBarVisibility(QToolBar* toolbar, bool visible)
{
    toolbar->blockSignals(true);
    toolbar->setVisible(visible);
    toolbar->blockSignals(false);
}

void MainWindow::zoomScaleChanged(qreal scale)
{
    m_zoomScaleSpinBox->blockSignals(true);
    m_zoomScaleSpinBox->setValue(qFloor(scale * 1000) / 10.0);
    m_zoomScaleSpinBox->blockSignals(false);
    qCDebug(vMainWindow, "Value %f\n", (qreal(qFloor(scale * 1000) / 10.0)));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::zoomToSelected()
{
    QGraphicsItem* item = qApp->getCurrentScene()->focusItem();
    QRectF rect;

    if (qApp->getCurrentScene() == m_draftScene) {
        if ((item != nullptr)
            && (item->type()
                == QGraphicsItem::UserType + static_cast<int>(Tool::BackgroundImage))) {
            rect = item->boundingRect();
            rect.translate(item->scenePos());
            m_ui->view->zoomToRect(rect);
        } else {
            m_ui->view->zoomToRect(doc->ActiveDrawBoundingRect());
        }
    } else if (qApp->getCurrentScene() == m_pieceScene) {
        if ((item != nullptr)
            && (item->type() == QGraphicsItem::UserType + static_cast<int>(Tool::Piece))) {
            rect = item->boundingRect();
            rect.translate(item->scenePos());
            m_ui->view->zoomToRect(rect);
        }
    }
}

void MainWindow::zoomToPrevious()
{
    VMainGraphicsScene* scene = qobject_cast<VMainGraphicsScene*>(currentScene);
    SCASSERT(scene != nullptr)

    /*Set transform for current scene*/
    scene->swapTransforms();
    m_ui->view->setTransform(scene->transform());
    zoomScaleChanged(m_ui->view->transform().m11());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::zoomToArea(bool checked)
{
    m_ui->view->zoomToAreaEnabled(checked);

    if (m_ui->zoomToArea_Action->isChecked()) {
        m_ui->zoomPan_Action->setChecked(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::zoomPan(bool checked)
{
    m_ui->view->zoomPanEnabled(checked);
    if (checked) {
        m_ui->zoomToArea_Action->setChecked(false);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief zoomToPoint show dialog for choosing a point and update the graphics view to focus on it.
 */
void MainWindow::showZoomToPointDialog()
{
    QStringList pointNames = draftPointNamesList();

    bool ok;
    QString pointName = QInputDialog::getItem(
        this,
        tr("Zoom to Point"),
        tr("Point:"),
        pointNames,
        0,
        true,
        &ok,
        Qt::WindowSystemMenuHint | Qt::WindowTitleHint);
    if (!ok || pointName.isEmpty())
        return;

    zoomToPoint(pointName);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief zoomToPoint show dialog for choosing a point and update the graphics view to focus on it.
 * @param pointName name of to zoom into.
 */
void MainWindow::zoomToPoint(const QString& pointName)
{
    const QHash<quint32, QSharedPointer<VGObject>>& objects = pattern->DataGObjects();
    QHash<quint32, QSharedPointer<VGObject>>::const_iterator i;

    for (i = objects.constBegin(); i != objects.constEnd(); ++i) {
        QSharedPointer<VGObject> object = i.value();
        const quint32 objectId = object->getIdObject();
        const QString objectName = object->name();

        if (objectName == pointName) {
            VPointF* point = (VPointF*)object.data();
            m_ui->view->zoom100Percent();
            m_ui->view->centerOn(point->toQPointF());

            // show point name if it's hidden
            // TODO: Need to make this work with operation's and dart tools
            quint32 toolId = point->getIdTool();
            const quint32 objId = point->getIdObject();
            if (objId != NULL_ID) {
                toolId = objId;
            }
            if (VAbstractTool* tool =
                    qobject_cast<VAbstractTool*>(VAbstractPattern::getTool(toolId))) {
                tool->setPointNameVisiblity(toolId, true);
            }

            // show any hiden groups containing object
            QMap<quint32, QString> groups = doc->getGroupsContainingItem(toolId, objectId, true);
            m_groupsWidget->showGroups(groups);

            // Reset combobox so same point can be selected again
            m_zoomToPointComboBox->blockSignals(true);
            m_zoomToPointComboBox->setCurrentIndex(-1);
            m_zoomToPointComboBox->blockSignals(false);

            return;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeToolButtons()
{
    connect(
        m_ui->arrowPointer_ToolButton, &QToolButton::clicked, this, &MainWindow::handleArrowTool);

    // This check helps to find missed tools
    static_assert(
        static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 54, "Check if all tools were connected.");

    connect(
        m_ui->pointAtDistanceAngle_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointAtDistanceAngleTool);
    connect(m_ui->line_ToolButton, &QToolButton::clicked, this, &MainWindow::handleLineTool);
    connect(
        m_ui->alongLine_ToolButton, &QToolButton::clicked, this, &MainWindow::handleAlongLineTool);
    connect(
        m_ui->shoulderPoint_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleShoulderPointTool);
    connect(m_ui->normal_ToolButton, &QToolButton::clicked, this, &MainWindow::handleNormalTool);
    connect(
        m_ui->bisector_ToolButton, &QToolButton::clicked, this, &MainWindow::handleBisectorTool);
    connect(
        m_ui->lineIntersect_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleLineIntersectTool);
    connect(m_ui->curve_ToolButton, &QToolButton::clicked, this, &MainWindow::handleCurveTool);
    connect(
        m_ui->curveWithCPs_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleCurveWithControlPointsTool);
    connect(m_ui->arc_ToolButton, &QToolButton::clicked, this, &MainWindow::handleArcTool);
    connect(m_ui->spline_ToolButton, &QToolButton::clicked, this, &MainWindow::handleSplineTool);
    connect(
        m_ui->splineWithCPs_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleSplineWithControlPointsTool);
    connect(
        m_ui->pointOfContact_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointOfContactTool);
    connect(
        m_ui->addPatternPiece_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePatternPieceTool);
    connect(
        m_ui->internalPath_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleInternalPathTool);
    connect(m_ui->height_ToolButton, &QToolButton::clicked, this, &MainWindow::handleHeightTool);
    connect(
        m_ui->triangle_ToolButton, &QToolButton::clicked, this, &MainWindow::handleTriangleTool);
    connect(
        m_ui->pointIntersectXY_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointIntersectXYTool);
    connect(
        m_ui->pointAlongCurve_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointAlongCurveTool);
    connect(
        m_ui->pointAlongSpline_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointAlongSplineTool);
    connect(
        m_ui->unitePieces_ToolButton, &QToolButton::clicked, this, &MainWindow::handleUnionTool);
    connect(
        m_ui->pointAlongArc_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointAlongArcTool);
    connect(
        m_ui->lineIntersectAxis_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleLineIntersectAxisTool);
    connect(
        m_ui->curveIntersectAxis_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleCurveIntersectAxisTool);
    connect(
        m_ui->arcIntersectAxis_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleArcIntersectAxisTool);
    connect(
        m_ui->layoutSettings_ToolButton, &QToolButton::clicked, this, &MainWindow::handleNewLayout);
    connect(
        m_ui->pointOfIntersectionArcs_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointOfIntersectionArcsTool);
    connect(
        m_ui->pointOfIntersectionCircles_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointOfIntersectionCirclesTool);
    connect(
        m_ui->pointOfIntersectionCurves_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleCurveIntersectCurveTool);
    connect(
        m_ui->pointFromCircleAndTangent_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointFromCircleAndTangentTool);
    connect(
        m_ui->pointFromArcAndTangent_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handlePointFromArcAndTangentTool);
    connect(
        m_ui->arcWithLength_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleArcWithLengthTool);
    connect(
        m_ui->trueDarts_ToolButton, &QToolButton::clicked, this, &MainWindow::handleTrueDartTool);
    connect(
        m_ui->exportDraftBlocks_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::exportDraftBlocksAs);
    connect(m_ui->group_ToolButton, &QToolButton::clicked, this, &MainWindow::handleGroupTool);
    connect(
        m_ui->rotation_ToolButton, &QToolButton::clicked, this, &MainWindow::handleRotationTool);
    connect(
        m_ui->mirrorByLine_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleMirrorByLineTool);
    connect(
        m_ui->mirrorByAxis_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleMirrorByAxisTool);
    connect(m_ui->move_ToolButton, &QToolButton::clicked, this, &MainWindow::handleMoveTool);
    connect(
        m_ui->midpoint_ToolButton, &QToolButton::clicked, this, &MainWindow::handleMidpointTool);
    connect(
        m_ui->exportLayout_ToolButton, &QToolButton::clicked, this, &MainWindow::exportLayoutAs);
    connect(
        m_ui->exportPiecesAs_ToolButton, &QToolButton::clicked, this, &MainWindow::exportPiecesAs);
    connect(
        m_ui->ellipticalArc_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleEllipticalArcTool);
    connect(
        m_ui->anchorPoint_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleAnchorPointTool);
    connect(
        m_ui->importImage_ToolButton, &QToolButton::clicked, this, &MainWindow::handleImportImage);
    connect(
        m_ui->insertNodes_ToolButton,
        &QToolButton::clicked,
        this,
        &MainWindow::handleInsertNodesTool);
}

void MainWindow::handlePointsMenu()
{
    qCDebug(vMainWindow, "Points Menu selected. \n");

    QMenu menu;

    QAction* action_PointAtDA =
        menu.addAction(QIcon(":/toolicon/32x32/segment.png"), tr("Length and Angle") + "\tL, A");
    QAction* action_PointAlongLine =
        menu.addAction(QIcon(":/toolicon/32x32/along_line.png"), tr("On Line") + "\tO, L");
    QAction* action_Midpoint = menu.addAction(
        QIcon(":/toolicon/32x32/midpoint.png"), tr("Midpoint") + "\t" + tr("Shift+O, Shift+L"));
    QAction* action_AlongPerpendicular =
        menu.addAction(QIcon(":/toolicon/32x32/normal.png"), tr("On Perpendicular") + "\tO, P");
    QAction* action_Bisector =
        menu.addAction(QIcon(":/toolicon/32x32/bisector.png"), tr("On Bisector") + "\tO, B");
    QAction* action_Shoulder =
        menu.addAction(QIcon(":/toolicon/32x32/shoulder.png"), tr("Length to Line") + "\tP, S");
    QAction* action_PointOfContact = menu.addAction(
        QIcon(":/toolicon/32x32/point_intersect_arc_line.png"),
        tr("Intersect Arc and Line") + "\tA, L");
    QAction* action_Triangle = menu.addAction(
        QIcon(":/toolicon/32x32/triangle.png"), tr("Intersect Axis and Triangle") + "\tX, T");
    QAction* action_PointIntersectXY = menu.addAction(
        QIcon(":/toolicon/32x32/point_intersectxy_icon.png"), tr("Intersect XY") + "\tX, Y");
    QAction* action_PerpendicularPoint = menu.addAction(
        QIcon(":/toolicon/32x32/height.png"), tr("Intersect Line and Perpendicular") + "\tL, P");
    QAction* action_PointIntersectAxis = menu.addAction(
        QIcon(":/toolicon/32x32/line_intersect_axis.png"),
        tr("Intersect Line and Axis") + "\tL, X");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Midpoint) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->midpoint_ToolButton->setChecked(true);
        handleMidpointTool(true);
    } else if (selectedAction == action_PointAtDA) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointAtDistanceAngle_ToolButton->setChecked(true);
        handlePointAtDistanceAngleTool(true);
    } else if (selectedAction == action_PointAlongLine) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->alongLine_ToolButton->setChecked(true);
        handleAlongLineTool(true);
    } else if (selectedAction == action_AlongPerpendicular) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->normal_ToolButton->setChecked(true);
        handleNormalTool(true);
    } else if (selectedAction == action_Bisector) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->bisector_ToolButton->setChecked(true);
        handleBisectorTool(true);
    } else if (selectedAction == action_Shoulder) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->shoulderPoint_ToolButton->setChecked(true);
        handleShoulderPointTool(true);
    } else if (selectedAction == action_PointOfContact) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointOfContact_ToolButton->setChecked(true);
        handlePointOfContactTool(true);
    } else if (selectedAction == action_Triangle) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->triangle_ToolButton->setChecked(true);
        handleTriangleTool(true);
    } else if (selectedAction == action_PointIntersectXY) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointIntersectXY_ToolButton->setChecked(true);
        handlePointIntersectXYTool(true);
    } else if (selectedAction == action_PerpendicularPoint) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->height_ToolButton->setChecked(true);
        handleHeightTool(true);
    } else if (selectedAction == action_PointIntersectAxis) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->lineIntersectAxis_ToolButton->setChecked(true);
        handleLineIntersectAxisTool(true);
    }
}

void MainWindow::handleLinesMenu()
{
    qCDebug(vMainWindow, "Lines Menu selected. \n");
    QMenu menu;

    QAction* action_Line =
        menu.addAction(QIcon(":/toolicon/32x32/line.png"), tr("Line") + "\t" + tr("Alt+L"));
    QAction* action_LineIntersect =
        menu.addAction(QIcon(":/toolicon/32x32/intersect.png"), tr("Intersect Lines") + "\tI, L");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Line) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->lines_Page);
        m_ui->line_ToolButton->setChecked(true);
        handleLineTool(true);
    } else if (selectedAction == action_LineIntersect) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->lines_Page);
        m_ui->lineIntersect_ToolButton->setChecked(true);
        handleLineIntersectTool(true);
    }
}

void MainWindow::handleArcsMenu()
{
    qCDebug(vMainWindow, "Arcs Menu selected. \n");
    QMenu menu;

    QAction* action_Arc = menu.addAction(
        QIcon(":/toolicon/32x32/arc.png"), tr("Radius and Angles") + "\t" + tr("Alt+A"));
    QAction* action_PointAlongArc =
        menu.addAction(QIcon(":/toolicon/32x32/arc_cut.png"), tr("Point on Arc") + "\tO, A");
    QAction* action_ArcIntersectAxis = menu.addAction(
        QIcon(":/toolicon/32x32/arc_intersect_axis.png"), tr("Intersect Arc and Axis") + "\tA, X");
    QAction* action_ArcIntersectArc = menu.addAction(
        QIcon(":/toolicon/32x32/point_of_intersection_arcs.png"), tr("Intersect Arcs") + "\tI, A");
    QAction* action_CircleIntersect = menu.addAction(
        QIcon(":/toolicon/32x32/point_of_intersection_circles.png"),
        tr("Intersect Circles") + "\t" + tr("Shift+I, Shift+C"));
    QAction* action_CircleTangent = menu.addAction(
        QIcon(":/toolicon/32x32/point_from_circle_and_tangent.png"),
        tr("Intersect Circle and Tangent") + "\tC, T");
    QAction* action_ArcTangent = menu.addAction(
        QIcon(":/toolicon/32x32/point_from_arc_and_tangent.png"),
        tr("Intersect Arc and Tangent") + "\tA, T");
    QAction* action_ArcWithLength = menu.addAction(
        QIcon(":/toolicon/32x32/arc_with_length.png"),
        tr("Radius and Length") + "\t" + tr("Alt+Shift+A"));
    QAction* action_EllipticalArc =
        menu.addAction(QIcon(":/toolicon/32x32/el_arc.png"), tr("Elliptical") + "\t" + tr("Alt+E"));

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Arc) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arc_ToolButton->setChecked(true);
        handleArcTool(true);
    } else if (selectedAction == action_PointAlongArc) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointAlongArc_ToolButton->setChecked(true);
        handlePointAlongArcTool(true);
    } else if (selectedAction == action_ArcIntersectAxis) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arcIntersectAxis_ToolButton->setChecked(true);
        handleArcIntersectAxisTool(true);
    } else if (selectedAction == action_ArcIntersectArc) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointOfIntersectionArcs_ToolButton->setChecked(true);
        handlePointOfIntersectionArcsTool(true);
    } else if (selectedAction == action_CircleIntersect) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointOfIntersectionCircles_ToolButton->setChecked(true);
        handlePointOfIntersectionCirclesTool(true);
    } else if (selectedAction == action_CircleTangent) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointFromCircleAndTangent_ToolButton->setChecked(true);
        handlePointFromCircleAndTangentTool(true);
    } else if (selectedAction == action_ArcTangent) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointFromArcAndTangent_ToolButton->setChecked(true);
        handlePointFromArcAndTangentTool(true);
    } else if (selectedAction == action_ArcWithLength) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arcWithLength_ToolButton->setChecked(true);
        handleArcWithLengthTool(true);
    } else if (selectedAction == action_EllipticalArc) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->ellipticalArc_ToolButton->setChecked(true);
        handleEllipticalArcTool(true);
    }
}

void MainWindow::handleCurvesMenu()
{
    qCDebug(vMainWindow, "Curves Menu selected. \n");
    QMenu menu;

    QAction* action_Curve = menu.addAction(
        QIcon(":/toolicon/32x32/spline.png"), tr("Curve - Interactive") + "\t" + tr("Alt+C"));
    QAction* action_Spline = menu.addAction(
        QIcon(":/toolicon/32x32/splinePath.png"), tr("Spline - Interactive") + "\t" + tr("Alt+S"));
    QAction* action_CurveWithCPs = menu.addAction(
        QIcon(":/toolicon/32x32/cubic_bezier.png"), tr("Curve - Fixed") + "\t" + tr("Alt+Shift+C"));
    QAction* action_SplineWithCPs = menu.addAction(
        QIcon(":/toolicon/32x32/cubic_bezier_path.png"),
        tr("Spline - Fixed") + "\t" + tr("Alt+Shift+S"));
    QAction* action_PointAlongCurve = menu.addAction(
        QIcon(":/toolicon/32x32/spline_cut_point.png"), tr("Point on Curve") + "\tO, C");
    QAction* action_PointAlongSpline = menu.addAction(
        QIcon(":/toolicon/32x32/splinePath_cut_point.png"), tr("Point on Spline") + "\tO, S");
    QAction* action_CurveIntersectCurve = menu.addAction(
        QIcon(":/toolicon/32x32/intersection_curves.png"), tr("Intersect Curves") + "\tI, C");
    QAction* action_CurveIntersectAxis = menu.addAction(
        QIcon(":/toolicon/32x32/curve_intersect_axis.png"),
        tr("Intersect Curve & Axis") + "\tC, X");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Curve) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curve_ToolButton->setChecked(true);
        handleCurveTool(true);
    } else if (selectedAction == action_Spline) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->spline_ToolButton->setChecked(true);
        handleSplineTool(true);
    } else if (selectedAction == action_PointAlongCurve) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointAlongCurve_ToolButton->setChecked(true);
        handlePointAlongCurveTool(true);
    } else if (selectedAction == action_PointAlongSpline) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointAlongSpline_ToolButton->setChecked(true);
        handlePointAlongSplineTool(true);
    } else if (selectedAction == action_CurveWithCPs) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curveWithCPs_ToolButton->setChecked(true);
        handleCurveWithControlPointsTool(true);
    } else if (selectedAction == action_SplineWithCPs) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->splineWithCPs_ToolButton->setChecked(true);
        handleSplineWithControlPointsTool(true);
    } else if (selectedAction == action_CurveIntersectCurve) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointOfIntersectionCurves_ToolButton->setChecked(true);
        handleCurveIntersectCurveTool(true);
    } else if (selectedAction == action_CurveIntersectAxis) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curveIntersectAxis_ToolButton->setChecked(true);
        handleCurveIntersectAxisTool(true);
    }
}

void MainWindow::handleCirclesMenu() { qCDebug(vMainWindow, "Circles Menu selected. \n"); }

void MainWindow::handleOperationsMenu()
{
    qCDebug(vMainWindow, "Operations Menu selected. \n");
    QMenu menu;

    QAction* action_Group =
        menu.addAction(QIcon(":/toolicon/32x32/group.png"), tr("Add Objects to Group") + "\tG");
    QAction* action_Rotate =
        menu.addAction(QIcon(":/toolicon/32x32/rotation.png"), tr("Rotate") + "\tR");
    QAction* action_MirrorByLine = menu.addAction(
        QIcon(":/toolicon/32x32/mirror_by_line.png"), tr("Mirror by Line") + "\tM, L");
    QAction* action_MirrorByAxis = menu.addAction(
        QIcon(":/toolicon/32x32/mirror_by_axis.png"), tr("Mirror by Axis") + "\tM, A");
    QAction* action_Move =
        menu.addAction(QIcon(":/toolicon/32x32/move.png"), tr("Move") + "\t" + tr("Alt+M"));
    QAction* action_TrueDarts =
        menu.addAction(QIcon(":/toolicon/32x32/true_darts.png"), tr("True Darts") + "\tT, D");
    QAction* action_ExportDraftBlocks =
        menu.addAction(QIcon(":/toolicon/32x32/export.png"), tr("Export Draft Blocks") + "\tE, D");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Group) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->group_ToolButton->setChecked(true);
        handleGroupTool(true);
    } else if (selectedAction == action_Rotate) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->rotation_ToolButton->setChecked(true);
        handleRotationTool(true);
    } else if (selectedAction == action_MirrorByLine) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->mirrorByLine_ToolButton->setChecked(true);
        handleMirrorByLineTool(true);
    } else if (selectedAction == action_MirrorByAxis) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->mirrorByAxis_ToolButton->setChecked(true);
        handleMirrorByAxisTool(true);
    } else if (selectedAction == action_Move) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->move_ToolButton->setChecked(true);
        handleMoveTool(true);
    } else if (selectedAction == action_TrueDarts) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->trueDarts_ToolButton->setChecked(true);
        handleTrueDartTool(true);
    } else if (selectedAction == action_ExportDraftBlocks) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        exportDraftBlocksAs();
    }
}

void MainWindow::handlePieceMenu()
{
    QMenu menu;

    QAction* action_Piece = menu.addAction(
        QIcon(":/toolicon/32x32/new_detail.png"), tr("New Pattern Piece") + "\tN, P");
    QAction* action_AnchorPoint = menu.addAction(
        QIcon(":/toolicon/32x32/anchor_point.png"), tr("Add AnchorPoint") + "\tA, P");
    QAction* action_InternalPath =
        menu.addAction(QIcon(":/toolicon/32x32/path.png"), tr("Create Internal Path") + "\tI, N");
    QAction* action_InsertNodes = menu.addAction(
        QIcon(":/toolicon/32x32/insert_nodes_icon.png"), tr("Insert Nodes in Path") + "\tI, P");

    const bool hasPatternPieces{ !pattern->DataPieces().isEmpty() };
    action_AnchorPoint->setEnabled(hasPatternPieces);
    action_InternalPath->setEnabled(hasPatternPieces);
    action_InsertNodes->setEnabled(hasPatternPieces);

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Piece) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->addPatternPiece_ToolButton->setChecked(true);
        handlePatternPieceTool(true);
    } else if (selectedAction == action_AnchorPoint) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->anchorPoint_ToolButton->setChecked(true);
        handleAnchorPointTool(true);
    } else if (selectedAction == action_InternalPath) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->internalPath_ToolButton->setChecked(true);
        handleInternalPathTool(true);
    } else if (selectedAction == action_InsertNodes) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->insertNodes_ToolButton->setChecked(true);
        handleInsertNodesTool(true);
    }
}

void MainWindow::handlePatternPiecesMenu()
{
    qCDebug(vMainWindow, "PatternPieces Menu selected. \n");
    QMenu menu;

    QAction* action_Union =
        menu.addAction(QIcon(":/toolicon/32x32/union.png"), tr("Union Tool") + "\tU");
    QAction* action_ExportPieces = menu.addAction(
        QIcon(":/toolicon/32x32/export.png"), tr("Export Pattern Pieces") + "\tE, P");
    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_Union) {
        m_ui->piece_ToolBox->setCurrentWidget(m_ui->details_Page);
        m_ui->unitePieces_ToolButton->setChecked(true);
        handleUnionTool(true);
    } else if (selectedAction == action_ExportPieces) {
        m_ui->piece_ToolBox->setCurrentWidget(m_ui->details_Page);
        exportPiecesAs();
    }
}

void MainWindow::handleLayoutMenu()
{
    qCDebug(vMainWindow, "Layout Menu selected. \n");


    QMenu menu;

    QAction* action_NewLayout = menu.addAction(
        QIcon(":/toolicon/32x32/layout_settings.png"), tr("New Print Layout") + "\tN, L");
    QAction* action_ExportLayout =
        menu.addAction(QIcon(":/toolicon/32x32/export.png"), tr("Export Layout") + "\tE, L");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_NewLayout) {
        m_ui->layout_ToolBox->setCurrentWidget(m_ui->layout_Page);
        m_ui->layoutSettings_ToolButton->setChecked(true);
        handleNewLayout(true);
    } else if (selectedAction == action_ExportLayout) {
        m_ui->layout_ToolBox->setCurrentWidget(m_ui->layout_Page);
        exportLayoutAs();
    }
}

void MainWindow::handleImagesMenu()
{
    qCDebug(vMainWindow, "Images Menu selected. \n");


    QMenu menu;

    QAction* action_ImportImage =
        menu.addAction(QIcon(":/icon/32x32/add_image.png"), tr("Import Image") + "\tAlt+ I");

    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == nullptr) {
        return;
    } else if (selectedAction == action_ImportImage) {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->backgroundImage_Page);
        m_ui->importImage_ToolButton->setChecked(true);
        handleImportImage();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief mouseMove save mouse position and show user.
 * @param scenePos position mouse.
 */
void MainWindow::MouseMove(const QPointF& scenePos)
{
    if (m_mouseCoordinates) {
        m_mouseCoordinates->updateCoordinates(scenePos);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief CancelTool cancel tool.
 */
void MainWindow::CancelTool()
{
    qCDebug(vMainWindow, "Canceling tool.");
    m_dialogTool.clear();
    qCDebug(vMainWindow, "Dialog closed.");

    currentScene->setFocus(Qt::OtherFocusReason);
    currentScene->clearSelection();
    emit m_ui->view->itemClicked(nullptr);   // Hide visualization to avoid a crash

    m_ui->zoomPan_Action->setChecked(false);   // Disable Pan Zoom
    m_ui->view->zoomPanEnabled(false);

    m_ui->zoomToArea_Action->setChecked(false);   // Disable Zoom to Area
    m_ui->view->zoomToAreaEnabled(false);

    switch (m_currentTool) {
    case Tool::Arrow:
        m_ui->arrowPointer_ToolButton->setChecked(false);
        m_ui->arrow_Action->setChecked(false);
        m_helpLabel->setText(QString(""));

        // Crash: using CRTL+Z while using line tool.
        undoAction->setEnabled(false);
        redoAction->setEnabled(false);
        VAbstractTool::m_suppressContextMenu = true;
        return;
    case Tool::BasePoint:
    case Tool::SinglePoint:
    case Tool::DoublePoint:
    case Tool::LinePoint:
    case Tool::AbstractSpline:
    case Tool::Cut:
    case Tool::LAST_ONE_DO_NOT_USE:
    case Tool::NodePoint:
    case Tool::NodeArc:
    case Tool::NodeElArc:
    case Tool::NodeSpline:
    case Tool::NodeSplinePath:
    case Tool::BackgroundImage:
        Q_UNREACHABLE();   //-V501
        // Nothing to do here because we can't create this tool from main window.
        break;
    case Tool::EndLine: m_ui->pointAtDistanceAngle_ToolButton->setChecked(false); break;
    case Tool::Line: m_ui->line_ToolButton->setChecked(false); break;
    case Tool::AlongLine: m_ui->alongLine_ToolButton->setChecked(false); break;
    case Tool::Midpoint: m_ui->midpoint_ToolButton->setChecked(false); break;
    case Tool::ShoulderPoint: m_ui->shoulderPoint_ToolButton->setChecked(false); break;
    case Tool::Normal: m_ui->normal_ToolButton->setChecked(false); break;
    case Tool::Bisector: m_ui->bisector_ToolButton->setChecked(false); break;
    case Tool::LineIntersect: m_ui->lineIntersect_ToolButton->setChecked(false); break;
    case Tool::Spline: m_ui->curve_ToolButton->setChecked(false); break;
    case Tool::CubicBezier: m_ui->curveWithCPs_ToolButton->setChecked(false); break;
    case Tool::Arc: m_ui->arc_ToolButton->setChecked(false); break;
    case Tool::ArcWithLength: m_ui->arcWithLength_ToolButton->setChecked(false); break;
    case Tool::SplinePath: m_ui->spline_ToolButton->setChecked(false); break;
    case Tool::CubicBezierPath: m_ui->splineWithCPs_ToolButton->setChecked(false); break;
    case Tool::PointOfContact: m_ui->pointOfContact_ToolButton->setChecked(false); break;
    case Tool::Piece: m_ui->addPatternPiece_ToolButton->setChecked(false); break;
    case Tool::InternalPath: m_ui->internalPath_ToolButton->setChecked(false); break;
    case Tool::Height: m_ui->height_ToolButton->setChecked(false); break;
    case Tool::Triangle: m_ui->triangle_ToolButton->setChecked(false); break;
    case Tool::PointOfIntersection: m_ui->pointIntersectXY_ToolButton->setChecked(false); break;
    case Tool::CutSpline: m_ui->pointAlongCurve_ToolButton->setChecked(false); break;
    case Tool::CutSplinePath: m_ui->pointAlongSpline_ToolButton->setChecked(false); break;
    case Tool::Union: m_ui->unitePieces_ToolButton->setChecked(false); break;
    case Tool::CutArc: m_ui->pointAlongArc_ToolButton->setChecked(false); break;
    case Tool::LineIntersectAxis: m_ui->lineIntersectAxis_ToolButton->setChecked(false); break;
    case Tool::CurveIntersectAxis: m_ui->curveIntersectAxis_ToolButton->setChecked(false); break;
    case Tool::ArcIntersectAxis: m_ui->arcIntersectAxis_ToolButton->setChecked(false); break;
    case Tool::PointOfIntersectionArcs:
        m_ui->pointOfIntersectionArcs_ToolButton->setChecked(false);
        break;
    case Tool::PointOfIntersectionCircles:
        m_ui->pointOfIntersectionCircles_ToolButton->setChecked(false);
        break;
    case Tool::PointOfIntersectionCurves:
        m_ui->pointOfIntersectionCurves_ToolButton->setChecked(false);
        break;
    case Tool::PointFromCircleAndTangent:
        m_ui->pointFromCircleAndTangent_ToolButton->setChecked(false);
        break;
    case Tool::PointFromArcAndTangent:
        m_ui->pointFromArcAndTangent_ToolButton->setChecked(false);
        break;
    case Tool::TrueDarts: m_ui->trueDarts_ToolButton->setChecked(false); break;
    case Tool::Group: m_ui->group_ToolButton->setChecked(false); break;
    case Tool::Rotation: m_ui->rotation_ToolButton->setChecked(false); break;
    case Tool::MirrorByLine: m_ui->mirrorByLine_ToolButton->setChecked(false); break;
    case Tool::MirrorByAxis: m_ui->mirrorByAxis_ToolButton->setChecked(false); break;
    case Tool::Move: m_ui->move_ToolButton->setChecked(false); break;
    case Tool::EllipticalArc: m_ui->ellipticalArc_ToolButton->setChecked(false); break;
    case Tool::AnchorPoint: m_ui->anchorPoint_ToolButton->setChecked(false); break;
    case Tool::InsertNodes: m_ui->insertNodes_ToolButton->setChecked(false); break;
    }

    // Crash: using CRTL+Z while using line tool.
    undoAction->setEnabled(qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(qApp->getUndoStack()->canRedo());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief handleArrowTool enable arrow tool.
 */
void MainWindow::handleArrowTool(bool checked)
{
    if (checked && m_currentTool != Tool::Arrow) {
        qCDebug(vMainWindow, "Arrow tool.");
        CancelTool();
        m_ui->arrowPointer_ToolButton->setChecked(true);
        m_ui->arrow_Action->setChecked(true);
        m_currentTool = Tool::Arrow;
        emit EnableItemMove(true);
        emit ItemsSelection(SelectionType::ByMouseRelease);
        VAbstractTool::m_suppressContextMenu = false;

        // Only true for rubber band selection
        emit EnableLabelSelection(true);
        emit EnablePointSelection(false);
        emit EnableLineSelection(false);
        emit EnableArcSelection(false);
        emit EnableElArcSelection(false);
        emit EnableSplineSelection(false);
        emit EnableSplinePathSelection(false);
        emit EnableNodeLabelSelection(true);
        emit EnableNodePointSelection(true);
        emit EnableImageSelection(true);
        emit enablePieceSelection(true);   // Disable when done with pattern piece visualization

        // Hovering
        emit EnableLabelHover(true);
        emit EnablePointHover(true);
        emit EnableLineHover(true);
        emit EnableArcHover(true);
        emit EnableElArcHover(true);
        emit EnableSplineHover(true);
        emit EnableSplinePathHover(true);
        emit EnableNodeLabelHover(true);
        emit EnableNodePointHover(true);
        emit EnableImageHover(true);
        emit enablePieceHover(true);

        m_ui->view->allowRubberBand(true);

        m_ui->view->viewport()->unsetCursor();
        m_helpLabel->setText("");
        m_ui->view->setShowToolOptions(true);
        qCDebug(vMainWindow, "Enabled arrow tool.");
    } else {
        m_ui->view->viewport()->setCursor(QCursor(Qt::ArrowCursor));
        m_ui->arrowPointer_ToolButton->setChecked(true);
        m_ui->arrow_Action->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyPressEvent handle key press events.
 * @param event key event.
 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Escape: handleArrowTool(true); break;
    case Qt::Key_Return:
    case Qt::Key_Enter: EndVisualization(); break;
    case Qt::Key_Space:
        if (qApp->Seamly2DSettings()->isPanActiveSpaceKey()) {
            m_ui->zoomPan_Action->setChecked(true);
        }
        break;
    default: break;
    }
    QMainWindow::keyPressEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief keyReleaseEvent handle key press events.
 * @param event key event.
 */
void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Space:
        if (qApp->Seamly2DSettings()->isPanActiveSpaceKey()) {
            m_ui->zoomPan_Action->setChecked(false);
        }
    default: break;
    }
    QMainWindow::keyReleaseEvent(event);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveCurrentScene save scene options before set another.
 */
void MainWindow::SaveCurrentScene()
{
    if (m_mode == Draw::Calculation || m_mode == Draw::Modeling) {
        VMainGraphicsScene* scene = qobject_cast<VMainGraphicsScene*>(currentScene);
        SCASSERT(scene != nullptr)

        /*Save transform*/
        scene->setCurrentTransform(m_ui->view->transform());
        /*Save scroll bars value for previous scene.*/
        QScrollBar* horScrollBar = m_ui->view->horizontalScrollBar();
        scene->setHorScrollBar(horScrollBar->value());
        QScrollBar* verScrollBar = m_ui->view->verticalScrollBar();
        scene->setVerScrollBar(verScrollBar->value());
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief RestoreCurrentScene restore scene options after change.
 */
void MainWindow::RestoreCurrentScene()
{
    VMainGraphicsScene* scene = qobject_cast<VMainGraphicsScene*>(currentScene);
    SCASSERT(scene != nullptr)

    /*Set transform for current scene*/
    m_ui->view->setTransform(scene->transform());
    zoomScaleChanged(m_ui->view->transform().m11());
    /*Set value for current scene scroll bar.*/
    QScrollBar* horScrollBar = m_ui->view->horizontalScrollBar();
    horScrollBar->setValue(scene->getHorScrollBar());
    QScrollBar* verScrollBar = m_ui->view->verticalScrollBar();
    verScrollBar->setValue(scene->getVerScrollBar());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showDraftMode show draw scene.
 * @param checked true - button checked.
 */
void MainWindow::showDraftMode(bool checked)
{
    if (checked) {
        m_ui->toolbox_StackedWidget->setCurrentIndex(0);
        qCDebug(vMainWindow, "Show draft scene");
        handleArrowTool(true);

        m_leftGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_left_to_right_arrow.png"));
        m_rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        m_ui->showDraftMode->setChecked(true);
        m_ui->pieceMode_Action->setChecked(false);
        m_ui->layoutMode_Action->setChecked(false);

        SaveCurrentScene();

        currentScene = m_draftScene;
        m_ui->view->setScene(currentScene);
        RestoreCurrentScene();

        m_mode = Draw::Calculation;
        m_draftBlockComboBox->setCurrentIndex(m_currentBlockIndex);   // restore current draft Block
        m_drawMode = true;

        setToolsEnabled(true);
        setWidgetsEnabled(true);

        m_draftScene->enablePiecesMode(qApp->Seamly2DSettings()->getShowControlPoints());
        m_draftScene->setOriginsVisible(qApp->Settings()->getShowAxisOrigin());

        updateViewToolbar();

        // m_ui->toggleAnchorPoints_Action->setChecked(qApp->Settings()->getShowAnchorPoints());
        // m_draftScene->setOriginsVisible(qApp->Settings()->getShowAnchorPoints());

        m_ui->useToolColor_Action->setChecked(qApp->Settings()->getUseToolColor());

        m_ui->draft_ToolBox->setCurrentIndex(m_currentToolBoxIndex);

        if (qApp->patternType() == MeasurementsType::Multisize) {
            m_gradationHeightsLabel->setVisible(true);
            m_gradationHeights->setVisible(true);
            m_gradationSizesLabel->setVisible(true);
            m_gradationSizes->setVisible(true);
        }
        m_ui->groups_DockWidget->setWidget(m_groupsWidget);
        m_ui->groups_DockWidget->setWindowTitle(tr("Group Manager"));
    } else {
        m_ui->showDraftMode->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showPieceMode show Piece scene.
 * @param checked true - button checked.
 */
void MainWindow::showPieceMode(bool checked)
{
    if (checked) {
        m_ui->toolbox_StackedWidget->setCurrentIndex(1);
        handleArrowTool(true);

        if (m_drawMode) {
            m_currentBlockIndex =
                m_draftBlockComboBox->currentIndex();   // Save current draftf Block.
            m_drawMode = false;
        }
        m_draftBlockComboBox->setCurrentIndex(
            m_draftBlockComboBox->count() - 1);   // Need to get data about all Blocks.

        m_leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        m_rightGoToStage->setPixmap(QPixmap("://icon/24x24/left_to_right_arrow.png"));

        m_ui->showDraftMode->setChecked(false);
        m_ui->pieceMode_Action->setChecked(true);
        m_ui->layoutMode_Action->setChecked(false);

        if (!qApp->getOpeningPattern()) {
            if (pattern->DataPieces().isEmpty()) {
                QMessageBox::information(
                    this,
                    tr("Piece mode"),
                    tr("You can't use Piece m_mode yet. "
                       "Please, create at least one pattern piece."),
                    QMessageBox::Ok,
                    QMessageBox::Ok);
                showDraftMode(true);
                return;
            }
        }

        m_patternPiecesWidget->updateList();

        qCDebug(vMainWindow, "Show piece scene");
        SaveCurrentScene();

        currentScene = m_pieceScene;
        emit m_ui->view->itemClicked(nullptr);
        m_ui->view->setScene(currentScene);
        RestoreCurrentScene();

        if (m_mode == Draw::Calculation) {
            m_currentToolBoxIndex = m_ui->piece_ToolBox->currentIndex();
        }
        m_mode = Draw::Modeling;
        setToolsEnabled(true);
        setWidgetsEnabled(true);

        m_pieceScene->setOriginsVisible(qApp->Settings()->getShowAxisOrigin());

        updateViewToolbar();

        m_ui->piece_ToolBox->setCurrentIndex(m_ui->piece_ToolBox->indexOf(m_ui->details_Page));

        if (qApp->patternType() == MeasurementsType::Multisize) {
            m_gradationHeightsLabel->setVisible(true);
            m_gradationHeights->setVisible(true);
            m_gradationSizesLabel->setVisible(true);
            m_gradationSizes->setVisible(true);
        }
        m_ui->groups_DockWidget->setWidget(m_patternPiecesWidget);
        m_ui->groups_DockWidget->setWindowTitle(tr("Pattern Pieces"));

        m_helpLabel->setText("");
    } else {
        m_ui->pieceMode_Action->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showLayoutMode show layout scene.
 * @param checked true - button checked.
 */
void MainWindow::showLayoutMode(bool checked)
{
    if (checked) {
        m_ui->toolbox_StackedWidget->setCurrentIndex(2);
        handleArrowTool(true);

        if (m_drawMode) {
            m_currentBlockIndex =
                m_draftBlockComboBox->currentIndex();   // save current drfat Block
            m_drawMode = false;
        }
        m_draftBlockComboBox->setCurrentIndex(
            m_draftBlockComboBox->count() - 1);   // Need to get data about all draft Blocks

        m_leftGoToStage->setPixmap(QPixmap("://icon/24x24/right_to_left_arrow.png"));
        m_rightGoToStage->setPixmap(QPixmap("://icon/24x24/fast_forward_right_to_left_arrow.png"));

        m_ui->showDraftMode->setChecked(false);
        m_ui->pieceMode_Action->setChecked(false);
        m_ui->layoutMode_Action->setChecked(true);

        QHash<quint32, VPiece> pieces;
        if (!qApp->getOpeningPattern()) {
            const QHash<quint32, VPiece>& allPieces = pattern->DataPieces();
            if (allPieces.isEmpty()) {
                QMessageBox::information(
                    this,
                    tr("Layout mode"),
                    tr("You can't use Layout m_mode yet. "
                       "Please, create at least one pattern piece."),
                    QMessageBox::Ok,
                    QMessageBox::Ok);
                showDraftMode(true);
                return;
            } else {
                QHash<quint32, VPiece>::const_iterator i = allPieces.constBegin();
                while (i != allPieces.constEnd()) {
                    if (i.value().isInLayout()) {
                        pieces.insert(i.key(), i.value());
                    }
                    ++i;
                }

                if (pieces.isEmpty()) {
                    QMessageBox::information(
                        this,
                        tr("Layout mode"),
                        tr("You can't use Layout m_mode yet. Please, "
                           "include at least one pattern piece in layout."),
                        QMessageBox::Ok,
                        QMessageBox::Ok);
                    m_mode == Draw::Calculation ? showDraftMode(true) : showPieceMode(true);
                    return;
                }
            }
        }

        m_draftBlockComboBox->setCurrentIndex(-1);   // Hide pattern pieces

        qCDebug(vMainWindow, "Show layout scene");

        SaveCurrentScene();

        try {
            pieceList = preparePiecesForLayout(pieces);
        }

        catch (VException& exception) {
            pieceList.clear();
            QMessageBox::warning(
                this,
                tr("Layout mode"),
                tr("You can't use Layout m_mode yet.") + QLatin1String(" \n")
                    + exception.ErrorMessage(),
                QMessageBox::Ok,
                QMessageBox::Ok);
            m_mode == Draw::Calculation ? showDraftMode(true) : showPieceMode(true);
            return;
        }

        currentScene = tempSceneLayout;
        emit m_ui->view->itemClicked(nullptr);
        m_ui->view->setScene(currentScene);

        if (m_mode == Draw::Calculation) {
            m_currentToolBoxIndex = m_ui->layout_ToolBox->currentIndex();
        }
        m_mode = Draw::Layout;
        setToolsEnabled(true);
        setWidgetsEnabled(true);
        m_ui->layout_ToolBox->setCurrentIndex(m_ui->layout_ToolBox->indexOf(m_ui->layout_Page));

        m_mouseCoordinates->updateCoordinates(QPointF());

        if (qApp->patternType() == MeasurementsType::Multisize) {
            m_gradationHeightsLabel->setVisible(false);
            m_gradationHeights->setVisible(false);
            m_gradationSizesLabel->setVisible(false);
            m_gradationSizes->setVisible(false);
        }

        showLayoutPages(m_ui->listWidget->currentRow());

        if (scenes.isEmpty()) {
            m_ui->layoutSettings_ToolButton->click();
        }

        m_helpLabel->setText("");
    } else {
        m_ui->layoutMode_Action->setChecked(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SaveAs save as pattern file.
 * @return true for successes saving.
 */
bool MainWindow::SaveAs()
{
    if (m_patternReadOnly) {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setText(tr("Can not save file."));
        messageBox.setInformativeText(tr("Pattern is read only."));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
        return false;
    }
    QString filters(tr("Pattern files") + QLatin1String("(*.") + sm2dExt + QLatin1String(")"));
    QString filePath = qApp->getFilePath();
    QString dir;
    QString fileName;
    bool usedNotExistedDir = false;
    if (filePath.isEmpty()) {
        dir = qApp->Seamly2DSettings()->getPatternPath();
        fileName = tr("pattern");
    } else {
        dir = QFileInfo(filePath).path();
        fileName = QFileInfo(filePath).baseName();
    }

    auto RemoveTempDir = [usedNotExistedDir, dir]() {
        if (usedNotExistedDir) {
            QDir directory(dir);
            directory.rmpath(".");
        }
    };

    QDir directory(dir);
    if (!directory.exists()) {
        directory.mkpath(".");
    }

    fileName = fileDialog(
        this,
        tr("Save as"),
        dir + QLatin1String("/") + fileName + QLatin1String(".") + sm2dExt,
        filters,
        nullptr,
        QFileDialog::DontUseNativeDialog,
        QFileDialog::AnyFile,
        QFileDialog::AcceptSave);

    if (fileName.isEmpty()) {
        RemoveTempDir();
        return false;
    }

    QFileInfo fileInfo(fileName);
    if (fileInfo.suffix().isEmpty() && fileInfo.suffix() != sm2dExt) {
        fileName += QLatin1String(".") + sm2dExt;
    }

    if (fileInfo.exists() && fileName != filePath) {
        // Temporarily try to m_lock the file before saving
        // Also help to rewrite current read-only pattern
        VLockGuard<char> m_lock(fileName);
        if (!m_lock.IsLocked()) {
            qCWarning(
                vMainWindow,
                "%s",
                qUtf8Printable(
                    tr("Failed to lock. File with this name is opened in another window.")));
            RemoveTempDir();
            return false;
        }
    }

    // Need for restoring previous state in case of failure
    const bool wasModified =
        doc->IsModified();   // Need because SetReadOnly() will change internal state
    QString error;
    const bool result = SavePattern(fileName, error);
    if (result == false) {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setInformativeText(tr("Could not save file"));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setDetailedText(error);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();

        // Restoring previous state
        doc->SetModified(wasModified);

        RemoveTempDir();
        return result;
    }

    QFile::remove(qApp->getFilePath() + autosavePrefix);
    m_curFileFormatVersion = VPatternConverter::PatternMaxVer;
    m_curFileFormatVersionStr = VPatternConverter::PatternMaxVerStr;

    if (fileName != filePath) {
        VlpCreateLock(m_lock, fileName);
        if (!m_lock->IsLocked()) {
            qCWarning(
                vMainWindow,
                "%s",
                qUtf8Printable(tr("Failed to lock. This file already opened in another window. "
                                  "Expect collisions when running 2 copies of the program.")));
            RemoveTempDir();
            return false;
        }
    }

    RemoveTempDir();
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Save save pattern file.
 * @return true for successes saving.
 */
bool MainWindow::Save()
{
    if (m_patternReadOnly) {
        QMessageBox messageBox(this);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setText(tr("Can not save file."));
        messageBox.setInformativeText(tr("Pattern is read only."));
        messageBox.setDefaultButton(QMessageBox::Ok);
        messageBox.setStandardButtons(QMessageBox::Ok);
        messageBox.exec();
        return false;
    }
    if (qApp->getFilePath().isEmpty()) {
        return SaveAs();
    } else {
        if (m_curFileFormatVersion < VPatternConverter::PatternMaxVer
            && not ContinueFormatRewrite(
                m_curFileFormatVersionStr, VPatternConverter::PatternMaxVerStr)) {
            return false;
        }

#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup++;   // turn checking on
#endif                                 /*Q_OS_WIN32*/
        const bool isFileWritable = QFileInfo(qApp->getFilePath()).isWritable();

        if (!isFileWritable) {
            QMessageBox messageBox(this);
            messageBox.setIcon(QMessageBox::Question);
            messageBox.setText(tr("The document has no write permissions."));
            messageBox.setInformativeText("Do you want to change the permissions?");
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            messageBox.setDefaultButton(QMessageBox::Yes);

            if (messageBox.exec() == QMessageBox::Yes) {
                bool changed = QFile::setPermissions(
                    qApp->getFilePath(),
                    QFileInfo(qApp->getFilePath()).permissions() | QFileDevice::WriteUser);
#ifdef Q_OS_WIN32
                qt_ntfs_permission_lookup--;   // turn it off again
#endif                                         /*Q_OS_WIN32*/

                if (!changed) {
                    QMessageBox messageBox(this);
                    messageBox.setIcon(QMessageBox::Warning);
                    messageBox.setText(
                        tr("Cannot set permissions for %1 to writable.").arg(qApp->getFilePath()));
                    messageBox.setInformativeText(tr("Could not save the file."));
                    messageBox.setDefaultButton(QMessageBox::Ok);
                    messageBox.setStandardButtons(QMessageBox::Ok);
                    messageBox.exec();
                    return false;
                }
            } else {
                return false;
            }
        }

        QString error;
        bool result = SavePattern(qApp->getFilePath(), error);
        if (result) {
            QFile::remove(qApp->getFilePath() + autosavePrefix);
            m_curFileFormatVersion = VPatternConverter::PatternMaxVer;
            m_curFileFormatVersionStr = VPatternConverter::PatternMaxVerStr;
        } else {
            QMessageBox messageBox(this);
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.setText(tr("Could not save the file"));
            messageBox.setDefaultButton(QMessageBox::Ok);
            messageBox.setDetailedText(error);
            messageBox.setStandardButtons(QMessageBox::Ok);
            messageBox.exec();
        }
        return result;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Open ask user select pattern file.
 */
void MainWindow::Open()
{
    qCDebug(vMainWindow, "Opening new file.");

    const QString filter = tr("Pattern files") + QLatin1String(" (*.") + valExt
                         + QLatin1String(" *.") + sm2dExt + QLatin1String(")");

    // Get list last open files
    const QStringList files = qApp->Seamly2DSettings()->GetRecentFileList();
    QString dir;
    if (files.isEmpty()) {
        dir = QDir::homePath();
    } else {
        // Absolute path to last open file
        dir = QFileInfo(files.first()).absolutePath();
    }
    qCDebug(vMainWindow, "Run QFileDialog::getOpenFileName: dir = %s.", qUtf8Printable(dir));

    const QString filename = fileDialog(
        this,
        tr("Open file"),
        dir,
        filter,
        nullptr,
        QFileDialog::DontUseNativeDialog,
        QFileDialog::ExistingFile,
        QFileDialog::AcceptOpen);

    if (filename.isEmpty()) {
        return;
    }
    LoadPattern(filename);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Clear reset to default window.
 */
void MainWindow::Clear()
{
    qCDebug(vMainWindow, "Resetting main window.");
    m_lock.reset();
    qCDebug(vMainWindow, "Unlocked pattern file.");
    showDraftMode(true);
    qCDebug(vMainWindow, "Returned to Draft mode.");
    setCurrentFile(QString());
    pattern->Clear();
    qCDebug(vMainWindow, "Clearing pattern.");
    if (!qApp->getFilePath().isEmpty() && not doc->MPath().isEmpty()) {
        m_watcher->removePath(AbsoluteMPath(qApp->getFilePath(), doc->MPath()));
    }
    doc->clear();
    qCDebug(vMainWindow, "Clearing scenes.");
    m_draftScene->clear();
    m_pieceScene->clear();
    handleArrowTool(true);
    m_draftBlockComboBox->clear();
    m_ui->showDraftMode->setEnabled(false);
    m_ui->pieceMode_Action->setEnabled(false);
    m_ui->layoutMode_Action->setEnabled(false);
    m_ui->newDraft_Action->setEnabled(false);
    m_ui->renameDraft_Action->setEnabled(false);
    m_ui->save_Action->setEnabled(false);
    m_ui->saveAs_Action->setEnabled(false);
    m_ui->patternPreferences_Action->setEnabled(false);

    // disable zoom actions until a pattern is loaded
    m_zoomScaleSpinBox->setEnabled(false);
    m_ui->zoomIn_Action->setEnabled(false);
    m_ui->zoomOut_Action->setEnabled(false);
    m_ui->zoomToFit_Action->setEnabled(false);
    m_ui->zoomToSelected_Action->setEnabled(false);
    m_ui->zoom100Percent_Action->setEnabled(false);
    m_ui->zoomToPrevious_Action->setEnabled(false);
    m_ui->zoomToArea_Action->setEnabled(false);
    m_ui->zoomPan_Action->setEnabled(false);
    m_ui->zoomToPoint_Action->setEnabled(false);

    // disable group actions
    m_ui->groups_DockWidget->setEnabled(false);

    // disable history menu actions
    m_ui->history_Action->setEnabled(false);
    m_ui->table_Action->setEnabled(false);

    m_ui->lastTool_Action->setEnabled(false);
    m_ui->increaseSize_Action->setEnabled(false);
    m_ui->decreaseSize_Action->setEnabled(false);
    m_ui->useToolColor_Action->setEnabled(false);
    m_ui->showPointNames_Action->setEnabled(false);
    m_ui->toggleWireframe_Action->setEnabled(false);
    m_ui->toggleControlPoints_Action->setEnabled(false);
    m_ui->toggleAxisOrigin_Action->setEnabled(false);
    m_ui->toggleSeamAllowances_Action->setEnabled(false);
    m_ui->toggleGrainLines_Action->setEnabled(false);
    m_ui->toggleLabels_Action->setEnabled(false);
    // m_ui->toggleAnchorPoints_Action->setEnabled(false);


    // disable measurements menu actions
    m_ui->loadIndividual_Action->setEnabled(false);
    m_ui->loadMultisize_Action->setEnabled(false);
    m_ui->unloadMeasurements_Action->setEnabled(false);
    m_ui->editCurrent_Action->setEnabled(false);

    setToolsEnabled(false);

    qApp->setPatternUnit(Unit::Cm);
    qApp->setPatternType(MeasurementsType::Unknown);

#ifndef QT_NO_CURSOR
    QGuiApplication::restoreOverrideCursor();
#endif
    CleanLayout();
    pieceList.clear();   // don't move to CleanLayout()
    doc->clearBackgroundImageMap();
    qApp->getUndoStack()->clear();
    m_toolProperties->clearPropertyBrowser();
    m_toolProperties->itemClicked(nullptr);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::FileClosedCorrect()
{
    WriteSettings();

    // File was closed correct.
    QStringList restoreFiles = qApp->Seamly2DSettings()->GetRestoreFileList();
    restoreFiles.removeAll(qApp->getFilePath());
    qApp->Seamly2DSettings()->SetRestoreFileList(restoreFiles);

    // Remove autosave file
    QFile autofile(qApp->getFilePath() + autosavePrefix);
    if (autofile.exists()) {
        autofile.remove();
    }
    qCDebug(vMainWindow, "File %s closed correct.", qUtf8Printable(qApp->getFilePath()));
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::fullParseFile()
{
    qCDebug(vMainWindow, "Full parsing file");

    m_toolProperties->clearPropertyBrowser();
    try {
        setGuiEnabled(true);
        doc->Parse(Document::FullParse);
    }

    catch (const VExceptionUndo& exception) {
        Q_UNUSED(exception)

        /* If user want undo last operation before undo we need finish broken redo operation. For
         * those we post event myself. Later in method customEvent call undo.*/
        QApplication::postEvent(this, new UndoEvent());
        return;
    }

    catch (const VExceptionObjectError& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error parsing file.")),   //-V807
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    catch (const VExceptionConversionError& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error can't convert value.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    catch (const VExceptionEmptyParameter& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error empty parameter.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    catch (const VExceptionWrongId& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error wrong id.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    catch (VException& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error parsing file.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    } catch (const std::bad_alloc&) {
        qCCritical(vMainWindow, "%s", qUtf8Printable(tr("Error parsing file (std::bad_alloc).")));

        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    QString draftBlock;
    if (m_draftBlockComboBox->currentIndex() != -1) {
        draftBlock = m_draftBlockComboBox->itemText(m_draftBlockComboBox->currentIndex());
    }
    m_draftBlockComboBox->blockSignals(true);
    m_draftBlockComboBox->clear();

    QStringList draftBlockNames = doc->getPatternPieces();
    draftBlockNames.sort();
    m_draftBlockComboBox->addItems(draftBlockNames);

    if (!m_drawMode) {
        m_draftBlockComboBox->setCurrentIndex(m_draftBlockComboBox->count() - 1);
    } else {
        const qint32 index = m_draftBlockComboBox->findText(draftBlock);
        if (index != -1) {
            m_draftBlockComboBox->setCurrentIndex(index);
        }
    }
    m_draftBlockComboBox->blockSignals(false);
    m_ui->patternPreferences_Action->setEnabled(true);

    changeDraftBlockGlobally(draftBlock);

    setToolsEnabled(m_draftBlockComboBox->count() > 0);
    m_patternPiecesWidget->updateList();

    VMainGraphicsView::NewSceneRect(m_draftScene, qApp->getSceneView());
    VMainGraphicsView::NewSceneRect(m_pieceScene, qApp->getSceneView());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::changeDraftBlockGlobally(const QString& draftBlock)
{
    const qint32 index = m_draftBlockComboBox->findText(draftBlock);
    try {
        if (index != -1) {   // -1 for not found
            changeDraftBlock(index, false);
            m_draftBlockComboBox->blockSignals(true);
            m_draftBlockComboBox->setCurrentIndex(index);
            m_draftBlockComboBox->blockSignals(false);
        } else {
            changeDraftBlock(0, false);
        }
    }

    catch (VExceptionBadId& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Bad id.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }

    catch (const VExceptionEmptyParameter& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Error empty parameter.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        setGuiEnabled(false);

        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::setGuiEnabled(bool enabled)
{
    if (m_guiEnabled != enabled) {
        if (enabled == false) {
            handleArrowTool(true);
            qApp->getUndoStack()->clear();
        }
        setWidgetsEnabled(enabled);

        m_guiEnabled = enabled;

        setToolsEnabled(enabled);
        m_ui->statusBar->setEnabled(enabled);
#ifndef QT_NO_CURSOR
        QGuiApplication::setOverrideCursor(Qt::ArrowCursor);
#endif
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setWidgetsEnabled enable action button.
 * @param enable enable value.
 */
void MainWindow::setWidgetsEnabled(bool enable)
{
    const bool draftStage = (m_mode == Draw::Calculation);
    const bool pieceStage = (m_mode == Draw::Modeling);
    const bool designStage = (draftStage || pieceStage);
    const bool layoutStage = (m_mode == Draw::Layout);

    m_draftBlockComboBox->setEnabled(enable && draftStage);
    m_ui->arrow_Action->setEnabled(enable && designStage);

    // enable file menu actions
    m_ui->save_Action->setEnabled(isWindowModified() && enable && not m_patternReadOnly);
    m_ui->saveAs_Action->setEnabled(enable);
    m_ui->patternPreferences_Action->setEnabled(enable && designStage);

    // enable edit  menu actions
    undoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canUndo());
    redoAction->setEnabled(enable && designStage && qApp->getUndoStack()->canRedo());

    // enable view menu actions
    m_ui->showDraftMode->setEnabled(enable);
    m_ui->pieceMode_Action->setEnabled(enable);
    m_ui->layoutMode_Action->setEnabled(enable);
    m_zoomScaleSpinBox->setEnabled(enable);
    m_ui->zoomIn_Action->setEnabled(enable);
    m_ui->zoomOut_Action->setEnabled(enable);
    m_ui->zoomToFit_Action->setEnabled(enable);
    m_ui->zoomToSelected_Action->setEnabled(enable);
    m_ui->zoom100Percent_Action->setEnabled(enable);
    m_ui->zoomToPrevious_Action->setEnabled(enable);
    m_ui->zoomToArea_Action->setEnabled(enable);
    m_ui->zoomPan_Action->setEnabled(enable);
    m_ui->zoomToPoint_Action->setEnabled(enable && draftStage);
    m_zoomToPointComboBox->setEnabled(enable && draftStage);

    m_ui->increaseSize_Action->setEnabled(enable);
    m_ui->decreaseSize_Action->setEnabled(enable);
    m_ui->useToolColor_Action->setEnabled(enable && draftStage);
    m_ui->showPointNames_Action->setEnabled(enable);
    m_ui->toggleWireframe_Action->setEnabled(enable);
    m_ui->toggleControlPoints_Action->setEnabled(enable && draftStage);
    m_ui->toggleAxisOrigin_Action->setEnabled(enable);
    m_ui->toggleSeamAllowances_Action->setEnabled(enable && pieceStage);
    m_ui->toggleGrainLines_Action->setEnabled(enable && pieceStage);
    m_ui->toggleLabels_Action->setEnabled(enable && pieceStage);
    // m_ui->toggleAnchorPoints_Action->setEnabled(enable && draftStage);

    // enable group actions
    m_groupsWidget->setAddGroupEnabled(enable && draftStage);

    // enable tool menu actions
    m_ui->newDraft_Action->setEnabled(enable && draftStage);
    m_ui->renameDraft_Action->setEnabled(enable && draftStage);

    // enable measurement menu actions
    m_ui->loadIndividual_Action->setEnabled(enable && designStage);
    m_ui->loadMultisize_Action->setEnabled(enable && designStage);
    m_ui->unloadMeasurements_Action->setEnabled(enable && designStage);
    m_ui->table_Action->setEnabled(enable && draftStage);

    // enable history menu actions
    m_ui->history_Action->setEnabled(enable && draftStage);

    // enable utilities menu actions
    m_ui->calculator_Action->setEnabled(enable);
    m_ui->decimalChart_Action->setEnabled(enable);

    // enable help menu
    m_ui->shortcuts_Action->setEnabled(enable);

    // enable dock widget actions
    m_ui->groups_DockWidget->setEnabled(enable && designStage);
    m_ui->toolProperties_DockWidget->setEnabled(enable && draftStage);
    m_ui->layoutPages_DockWidget->setEnabled(enable && layoutStage);
    actionDockWidgetToolOptions->setEnabled(enable && designStage);
    actionDockWidgetGroups->setEnabled(enable && designStage);
    actionDockWidgetLayouts->setEnabled(enable && layoutStage);

    // Now we don't want allow user call context menu
    m_draftScene->setToolsDisabled(!enable, doc->getActiveDraftBlockName());
    m_ui->view->setEnabled(enable);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateHeightsList(const QStringList& list)
{
    QString value;
    if (m_gradationHeights->currentIndex() != -1) {
        value = m_gradationHeights->currentText();
    }

    m_gradationHeights->blockSignals(true);
    m_gradationHeights->clear();
    m_gradationHeights->addItems(list);
    m_gradationHeights->blockSignals(false);

    int index = m_gradationHeights->findText(value);
    if (index != -1) {
        m_gradationHeights->setCurrentIndex(index);
    } else {
        ChangedHeight(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateSizesList(const QStringList& list)
{
    QString value;
    if (m_gradationSizes->currentIndex() != -1) {
        value = m_gradationSizes->currentText();
    }

    m_gradationSizes->blockSignals(true);
    m_gradationSizes->clear();
    m_gradationSizes->addItems(list);
    m_gradationSizes->blockSignals(false);

    int index = m_gradationSizes->findText(value);
    if (index != -1) {
        m_gradationSizes->setCurrentIndex(index);
    } else {
        ChangedSize(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief NewPattern create new empty pattern.
 */
void MainWindow::New()
{
    if (m_draftBlockComboBox->count() == 0) {
        // Creating a new pattern design requires creating a new draft Block
        qCDebug(vMainWindow, "New Draft Block.");
        QString draftBlockName = tr("Draft Block %1").arg(m_draftBlockComboBox->count() + 1);
        qCDebug(vMainWindow, "Generated Draft Block name: %s", qUtf8Printable(draftBlockName));

        qCDebug(vMainWindow, "First Draft Block");
        DialogNewPattern newPattern{ draftBlockName, this };
        if (newPattern.exec() == QDialog::Accepted) {
            draftBlockName = newPattern.name();
            qApp->setPatternUnit(newPattern.PatternUnit());
            qCDebug(vMainWindow, "Draft Block name: %s", qUtf8Printable(draftBlockName));
        } else {
            qCDebug(vMainWindow, "Creating new Draft Block was canceled.");
            return;
        }

        // Set scene size to size scene view
        VMainGraphicsView::NewSceneRect(m_draftScene, m_ui->view);
        VMainGraphicsView::NewSceneRect(m_pieceScene, m_ui->view);

        addDraftBlock(draftBlockName);

        m_mouseCoordinates = new MouseCoordinates(qApp->patternUnit());
        m_ui->statusBar->addPermanentWidget((m_mouseCoordinates));

        m_curFileFormatVersion = VPatternConverter::PatternMaxVer;
        m_curFileFormatVersionStr = VPatternConverter::PatternMaxVerStr;
    } else {
        startNewSeamly2D();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief haveChange enable action save if we have unsaved change.
 */
void MainWindow::patternChangesWereSaved(bool saved)
{
    if (m_guiEnabled) {
        const bool state = doc->IsModified() || !saved;
        setWindowModified(state);
        not m_patternReadOnly ? m_ui->save_Action->setEnabled(state)
                              : m_ui->save_Action->setEnabled(false);
        isLayoutStale = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedSize change new size value.
 * @param index index of the selected item.
 */
void MainWindow::ChangedSize(int index)
{
    const int size = static_cast<int>(VContainer::size());
    if (updateMeasurements(
            AbsoluteMPath(qApp->getFilePath(), doc->MPath()),
            m_gradationSizes.data()->itemText(index).toInt(),
            static_cast<int>(VContainer::height()))) {
        doc->LiteParseTree(Document::LiteParse);
        emit m_pieceScene->DimensionsChanged();
    } else {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = m_gradationSizes->findText(QString().setNum(size));
        if (index != -1) {
            m_gradationSizes->setCurrentIndex(index);
        } else {
            qCWarning(vMainWindow, "Couldn't restore size value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ChangedGrowth change new height value.
 * @param index index of the selected item.
 */
void MainWindow::ChangedHeight(int index)
{
    const int height = static_cast<int>(VContainer::height());
    if (updateMeasurements(
            AbsoluteMPath(qApp->getFilePath(), doc->MPath()),
            static_cast<int>(VContainer::size()),
            m_gradationHeights.data()->itemText(index).toInt())) {
        doc->LiteParseTree(Document::LiteParse);
        emit m_pieceScene->DimensionsChanged();
    } else {
        qCWarning(vMainWindow, "%s", qUtf8Printable(tr("Couldn't update measurements.")));

        const qint32 index = m_gradationHeights->findText(QString().setNum(height));
        if (index != -1) {
            m_gradationHeights->setCurrentIndex(index);
        } else {
            qCWarning(vMainWindow, "Couldn't restore height value.");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultHeight()
{
    const QString defHeight = QString().setNum(doc->GetDefCustomHeight());
    int index = m_gradationHeights->findText(defHeight);
    if (index != -1) {
        m_gradationHeights->setCurrentIndex(index);
    } else {
        index = m_gradationHeights->findText(QString().setNum(VContainer::height()));
        if (index != -1) {
            m_gradationHeights->setCurrentIndex(index);
        }
    }
    VContainer::setHeight(m_gradationHeights->currentText().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultSize()
{
    const QString defSize = QString().setNum(doc->GetDefCustomSize());
    int index = m_gradationSizes->findText(defSize);
    if (index != -1) {
        m_gradationSizes->setCurrentIndex(index);
    } else {
        index = m_gradationSizes->findText(QString().setNum(VContainer::size()));
        if (index != -1) {
            m_gradationSizes->setCurrentIndex(index);
        }
    }
    VContainer::setSize(m_gradationSizes->currentText().toInt());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setToolsEnabled enable button.
 * @param enable enable value.
 */
void MainWindow::setToolsEnabled(bool enable)
{
    bool draftTools = false;
    bool pieceTools = false;
    bool layoutTools = false;

    switch (m_mode) {
    case Draw::Calculation: draftTools = enable; break;
    case Draw::Modeling: pieceTools = enable; break;
    case Draw::Layout: layoutTools = enable; break;
    default: break;
    }

    // This check helps to find missed tools
    static_assert(static_cast<int>(Tool::LAST_ONE_DO_NOT_USE) == 54, "Not all tools were handled.");

    // Toolbox Drafting Tools
    // Points
    m_ui->pointAtDistanceAngle_ToolButton->setEnabled(draftTools);
    m_ui->alongLine_ToolButton->setEnabled(draftTools);
    m_ui->normal_ToolButton->setEnabled(draftTools);
    m_ui->bisector_ToolButton->setEnabled(draftTools);
    m_ui->shoulderPoint_ToolButton->setEnabled(draftTools);
    m_ui->pointOfContact_ToolButton->setEnabled(draftTools);
    m_ui->triangle_ToolButton->setEnabled(draftTools);
    m_ui->pointIntersectXY_ToolButton->setEnabled(draftTools);
    m_ui->height_ToolButton->setEnabled(draftTools);
    m_ui->lineIntersectAxis_ToolButton->setEnabled(draftTools);
    m_ui->midpoint_ToolButton->setEnabled(draftTools);

    // Lines
    m_ui->line_ToolButton->setEnabled(draftTools);
    m_ui->lineIntersect_ToolButton->setEnabled(draftTools);

    // Curves
    m_ui->curve_ToolButton->setEnabled(draftTools);
    m_ui->spline_ToolButton->setEnabled(draftTools);
    m_ui->curveWithCPs_ToolButton->setEnabled(draftTools);
    m_ui->splineWithCPs_ToolButton->setEnabled(draftTools);
    m_ui->pointAlongCurve_ToolButton->setEnabled(draftTools);
    m_ui->pointAlongSpline_ToolButton->setEnabled(draftTools);
    m_ui->pointOfIntersectionCurves_ToolButton->setEnabled(draftTools);
    m_ui->curveIntersectAxis_ToolButton->setEnabled(draftTools);

    // Arcs
    m_ui->arc_ToolButton->setEnabled(draftTools);
    m_ui->pointAlongArc_ToolButton->setEnabled(draftTools);
    m_ui->arcIntersectAxis_ToolButton->setEnabled(draftTools);
    m_ui->pointOfIntersectionArcs_ToolButton->setEnabled(draftTools);
    m_ui->pointOfIntersectionCircles_ToolButton->setEnabled(draftTools);
    m_ui->pointFromCircleAndTangent_ToolButton->setEnabled(draftTools);
    m_ui->pointFromArcAndTangent_ToolButton->setEnabled(draftTools);
    m_ui->arcWithLength_ToolButton->setEnabled(draftTools);
    m_ui->ellipticalArc_ToolButton->setEnabled(draftTools);

    // Operations
    m_ui->group_ToolButton->setEnabled(draftTools);
    m_ui->rotation_ToolButton->setEnabled(draftTools);
    m_ui->mirrorByLine_ToolButton->setEnabled(draftTools);
    m_ui->mirrorByAxis_ToolButton->setEnabled(draftTools);
    m_ui->move_ToolButton->setEnabled(draftTools);
    m_ui->trueDarts_ToolButton->setEnabled(draftTools);
    m_ui->exportDraftBlocks_ToolButton->setEnabled(draftTools);

    // Piece
    m_ui->addPatternPiece_ToolButton->setEnabled(draftTools);
    const bool hasPatternPieces{ !pattern->DataPieces().isEmpty() };
    m_ui->anchorPoint_ToolButton->setEnabled(draftTools && hasPatternPieces);
    m_ui->internalPath_ToolButton->setEnabled(draftTools && hasPatternPieces);
    m_ui->insertNodes_ToolButton->setEnabled(draftTools && hasPatternPieces);

    // Images
    m_ui->importImage_ToolButton->setEnabled(draftTools);

    // Details
    m_ui->unitePieces_ToolButton->setEnabled(pieceTools);
    m_ui->exportPiecesAs_ToolButton->setEnabled(pieceTools);

    // Layout
    m_ui->layoutSettings_ToolButton->setEnabled(layoutTools);

    // enable Toolbox Toolbar actions
    m_ui->arrow_Action->setEnabled(enable);
    m_ui->points_Action->setEnabled(draftTools);
    m_ui->lines_Action->setEnabled(draftTools);
    m_ui->arcs_Action->setEnabled(draftTools);
    m_ui->curves_Action->setEnabled(draftTools);
    m_ui->modifications_Action->setEnabled(draftTools);
    m_ui->pieces_Action->setEnabled(draftTools);
    m_ui->details_Action->setEnabled(pieceTools);
    m_ui->layout_Action->setEnabled(layoutTools);
    m_ui->images_Action->setEnabled(layoutTools);

    // Menu Actions
    // Points
    m_ui->midpoint_Action->setEnabled(draftTools);
    m_ui->pointAtDistanceAngle_Action->setEnabled(draftTools);
    m_ui->pointAlongLine_Action->setEnabled(draftTools);
    m_ui->pointAlongPerpendicular_Action->setEnabled(draftTools);
    m_ui->bisector_Action->setEnabled(draftTools);
    m_ui->pointOnShoulder_Action->setEnabled(draftTools);
    m_ui->pointOfContact_Action->setEnabled(draftTools);
    m_ui->triangle_Action->setEnabled(draftTools);
    m_ui->pointIntersectXY_Action->setEnabled(draftTools);
    m_ui->perpendicularPoint_Action->setEnabled(draftTools);
    m_ui->pointIntersectAxis_Action->setEnabled(draftTools);

    // Lines
    m_ui->lineTool_Action->setEnabled(draftTools);
    m_ui->lineIntersect_Action->setEnabled(draftTools);

    // Curves
    m_ui->curve_Action->setEnabled(draftTools);
    m_ui->spline_Action->setEnabled(draftTools);
    m_ui->curveWithCPs_Action->setEnabled(draftTools);
    m_ui->splineWithCPs_Action->setEnabled(draftTools);
    m_ui->pointAlongCurve_Action->setEnabled(draftTools);
    m_ui->pointAlongSpline_Action->setEnabled(draftTools);
    m_ui->curveIntersectCurve_Action->setEnabled(draftTools);
    m_ui->splineIntersectAxis_Action->setEnabled(draftTools);

    // Arcs
    m_ui->arcTool_Action->setEnabled(draftTools);
    m_ui->pointAlongArc_Action->setEnabled(draftTools);
    m_ui->arcIntersectAxis_Action->setEnabled(draftTools);
    m_ui->arcIntersectArc_Action->setEnabled(draftTools);
    m_ui->circleIntersect_Action->setEnabled(draftTools);
    m_ui->circleTangent_Action->setEnabled(draftTools);
    m_ui->arcTangent_Action->setEnabled(draftTools);
    ;
    m_ui->arcWithLength_Action->setEnabled(draftTools);
    m_ui->ellipticalArc_Action->setEnabled(draftTools);

    // Operations
    m_ui->group_Action->setEnabled(draftTools);
    m_ui->rotation_Action->setEnabled(draftTools);
    m_ui->mirrorByLine_Action->setEnabled(draftTools);
    m_ui->mirrorByAxis_Action->setEnabled(draftTools);
    m_ui->move_Action->setEnabled(draftTools);
    m_ui->trueDarts_Action->setEnabled(draftTools);
    m_ui->exportDraftBlocks_Action->setEnabled(draftTools);

    // Piece
    m_ui->addPiece_Action->setEnabled(draftTools);
    m_ui->anchorPoint_Action->setEnabled(draftTools && hasPatternPieces);
    m_ui->images_Action->setEnabled(draftTools);
    m_ui->internalPath_Action->setEnabled(draftTools && hasPatternPieces);
    m_ui->insertNodes_Action->setEnabled(draftTools && hasPatternPieces);

    // Images
    m_ui->importImage_Action->setEnabled(draftTools);

    // Details
    m_ui->union_Action->setEnabled(pieceTools);
    m_ui->exportPieces_Action->setEnabled(pieceTools);

    // Layout
    m_ui->newPrintLayout_Action->setEnabled(layoutTools);
    m_ui->exportLayout_Action->setEnabled(layoutTools);
    m_ui->lastTool_Action->setEnabled(draftTools);

    m_ui->arrowPointer_ToolButton->setEnabled(draftTools || pieceTools);
    m_ui->arrowPointer_ToolButton->setChecked(draftTools || pieceTools);
    m_ui->arrow_Action->setChecked(draftTools || pieceTools);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::SetLayoutModeActions()
{
    const bool enabled = not scenes.isEmpty();

    m_ui->exportLayout_ToolButton->setEnabled(enabled);
    m_ui->exportAs_Action->setEnabled(enabled);
    m_ui->printPreview_Action->setEnabled(enabled);
    m_ui->printPreviewTiled_Action->setEnabled(enabled);
    m_ui->print_Action->setEnabled(enabled);
    m_ui->printTiled_Action->setEnabled(enabled);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MinimumScrollBar set scroll bar to minimum.
 */
void MainWindow::MinimumScrollBar()
{
    QScrollBar* horScrollBar = m_ui->view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar* verScrollBar = m_ui->view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SavePattern save pattern file.
 * @param fileName pattern file name.
 * @return true if all is good.
 */
bool MainWindow::SavePattern(const QString& fileName, QString& error)
{
    qCDebug(vMainWindow, "Saving pattern file %s.", qUtf8Printable(fileName));
    QFileInfo tempInfo(fileName);

    const QString filename = AbsoluteMPath(qApp->getFilePath(), doc->MPath());
    if (!filename.isEmpty() && qApp->getFilePath() != fileName) {
        doc->SetMPath(RelativeMPath(fileName, filename));
    }

    const bool result = doc->SaveDocument(fileName, error);
    if (result) {
        if (tempInfo.suffix() != QLatin1String("autosave")) {
            setCurrentFile(fileName);
            m_helpLabel->setText(tr("File saved"));
            qCDebug(vMainWindow, "File %s saved.", qUtf8Printable(fileName));
            patternChangesWereSaved(result);
        }
    } else {
        doc->SetMPath(filename);
        emit doc->UpdatePatternLabel();
        qCWarning(
            vMainWindow,
            "Could not save file %s. %s.",
            qUtf8Printable(fileName),
            qUtf8Printable(error));
    }
    return result;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AutoSavePattern start safe saving.
 */
void MainWindow::AutoSavePattern()
{
    if (m_patternReadOnly) {
        return;
    }
    qCDebug(vMainWindow, "Autosaving pattern.");

    if (qApp->getFilePath().isEmpty() == false && this->isWindowModified() == true) {
        QString autofile = qApp->getFilePath() + autosavePrefix;
        QString error;
        SavePattern(autofile, error);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief setCurrentFile the function is called to reset the state of a few variables when a file
 * is loaded or saved, or when the user starts editing a new file (in which case fileName is empty).
 * @param fileName file name.
 */
void MainWindow::setCurrentFile(const QString& fileName)
{
    qCDebug(vMainWindow, "Set current name to \"%s\"", qUtf8Printable(fileName));
    qApp->setFilePath(fileName);
    doc->SetPatternWasChanged(true);
    emit doc->UpdatePatternLabel();
    qApp->getUndoStack()->setClean();

    if (!qApp->getFilePath().isEmpty() && Application2D::isGUIMode()) {
        qCDebug(vMainWindow, "Updating recent file list.");
        VSettings* settings = qApp->Seamly2DSettings();
        QStringList files = settings->GetRecentFileList();
        files.removeAll(fileName);
        files.prepend(fileName);
        while (files.size() > MaxRecentFiles) {
            files.removeLast();
        }

        settings->SetRecentFileList(files);
        UpdateRecentFileActions();

        qCDebug(vMainWindow, "Updating restore file list.");
        QStringList restoreFiles = settings->GetRestoreFileList();
        restoreFiles.removeAll(fileName);
        restoreFiles.prepend(fileName);
        settings->SetRestoreFileList(restoreFiles);
    }

    UpdateWindowTitle();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ReadSettings read setting for app.
 */
void MainWindow::ReadSettings()
{
    qCDebug(vMainWindow, "Reading settings.");
    const VSettings* settings = qApp->Seamly2DSettings();
    restoreGeometry(settings->GetGeometry());
    restoreState(settings->GetWindowState());
    restoreState(settings->GetToolbarsState(), APP_VERSION);

    // Scene antialiasing
    const bool graphOutputValue = settings->GetGraphicalOutput();
    m_ui->view->setRenderHint(QPainter::Antialiasing, graphOutputValue);
    m_ui->view->setRenderHint(QPainter::SmoothPixmapTransform, graphOutputValue);

    // Stack limit
    qApp->getUndoStack()->setUndoLimit(settings->GetUndoCount());

    // Text under tool button icon
    ToolBarStyles();

    m_isToolOptionsDockVisible = m_ui->toolProperties_DockWidget->isVisible();
    m_isGroupsDockVisible = m_ui->groups_DockWidget->isVisible();
    m_isLayoutsDockVisible = m_ui->layoutPages_DockWidget->isVisible();
    m_isToolboxDockVisible = m_ui->toolbox_DockWidget->isVisible();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief WriteSettings save setting for app.
 */
void MainWindow::WriteSettings()
{
    showDraftMode(true);

    VSettings* setings = qApp->Seamly2DSettings();
    setings->SetGeometry(saveGeometry());
    setings->SetWindowState(saveState());
    setings->SetToolbarsState(saveState(APP_VERSION));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief MaybeSave The function is called to save pending changes.
 * @return returns true in all cases, except when the user clicks Cancel.
 */
bool MainWindow::MaybeSave()
{
    if (this->isWindowModified() && m_guiEnabled) {
        QMessageBox messageBox{ tr("Unsaved changes"),
                                tr("The pattern has been modified.\n"
                                   "Do you want to save your changes?"),
                                QMessageBox::Warning,
                                QMessageBox::Yes,
                                QMessageBox::No,
                                QMessageBox::Cancel,
                                this,
                                Qt::Sheet };

        messageBox.setDefaultButton(QMessageBox::Yes);
        messageBox.setEscapeButton(QMessageBox::Cancel);

        messageBox.setButtonText(
            QMessageBox::Yes,
            qApp->getFilePath().isEmpty() || m_patternReadOnly ? tr("Save...") : tr("Save"));
        messageBox.setButtonText(QMessageBox::No, tr("Don't Save"));

        messageBox.setWindowModality(Qt::ApplicationModal);
        messageBox.setWindowFlags(
            windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMaximizeButtonHint
            & ~Qt::WindowMinimizeButtonHint);
        const auto ret = static_cast<QMessageBox::StandardButton>(messageBox.exec());

        switch (ret) {
        case QMessageBox::Yes:
            if (m_patternReadOnly) {
                return SaveAs();
            } else {
                return Save();
            }
        case QMessageBox::No: return true;
        case QMessageBox::Cancel: return false;
        default: break;
        }
    }
    return true;
}
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief InfoUnsavedImages is called when the user imports his first image.
 */
void MainWindow::InfoUnsavedImages(bool* firstImportImage)
{
    if (m_guiEnabled) {
        QMessageBox messageBox{
            QMessageBox::Information,
            tr("Images will not be saved"),
            tr("Please note that the images can not be saved and that they are not affected "
               "by the undo and redo functions in the current version of the software.\n\n"
               "You may want to take a screenshot of the image properties dialog before closing "
               "the software "
               "to be able to recreate identically the image when opening the software again."),
            QMessageBox::NoButton,
            this,
            Qt::Sheet
        };

        messageBox.setWindowModality(Qt::ApplicationModal);
        messageBox.setWindowFlags(
            windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMaximizeButtonHint
            & ~Qt::WindowMinimizeButtonHint);

        messageBox.exec();
        *firstImportImage = true;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateRecentFileActions()
{
    qCDebug(vMainWindow, "Updating recent file actions.");
    const QStringList files = qApp->Seamly2DSettings()->GetRecentFileList();
    const int numRecentFiles = qMin(files.size(), static_cast<int>(MaxRecentFiles));

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = QString("&%1. %2").arg(i + 1).arg(strippedName(files.at(i)));
        m_recentFileActs[i]->setText(text);
        m_recentFileActs[i]->setData(files.at(i));
        m_recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j) {
        m_recentFileActs[j]->setVisible(false);
    }

    m_separatorAct->setVisible(numRecentFiles > 0);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::createMenus()
{
    // Add last 5 most recent projects to file menu.
    for (int i = 0; i < MaxRecentFiles; ++i) {
        m_ui->file_Menu->insertAction(m_ui->exit_Action, m_recentFileActs[i]);
    }
    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);
    m_ui->file_Menu->insertAction(m_ui->exit_Action, m_separatorAct);
    UpdateRecentFileActions();

    // Add Undo/Redo actions to edit menu.
    QList<QKeySequence> undoShortcuts;
    undoShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Z));
    undoShortcuts.append(QKeySequence(Qt::AltModifier + Qt::Key_Backspace));

    undoAction = qApp->getUndoStack()->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(undoShortcuts);
    undoAction->setIcon(QIcon::fromTheme("edit-undo"));
    connect(
        undoAction,
        &QAction::triggered,
        m_toolProperties,
        &VToolOptionsPropertyBrowser::refreshOptions);
    m_ui->edit_Menu->addAction(undoAction);
    m_ui->edit_Toolbar->addAction(undoAction);

    QList<QKeySequence> redoShortcuts;
    redoShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::Key_Y));
    redoShortcuts.append(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Z));
    redoShortcuts.append(QKeySequence(Qt::AltModifier + Qt::ShiftModifier + Qt::Key_Backspace));

    redoAction = qApp->getUndoStack()->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(redoShortcuts);
    redoAction->setIcon(QIcon::fromTheme("edit-redo"));
    connect(
        redoAction,
        &QAction::triggered,
        m_toolProperties,
        &VToolOptionsPropertyBrowser::refreshOptions);
    m_ui->edit_Menu->addAction(redoAction);
    m_ui->edit_Toolbar->addAction(redoAction);

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);
    m_ui->edit_Menu->addAction(m_separatorAct);

    AddDocks();

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);
    m_ui->view_Menu->addAction(m_separatorAct);

    QMenu* menu = new QMenu(tr("Toolbars"));
    m_ui->view_Menu->addMenu(menu);

    menu->addAction(m_ui->file_ToolBar->toggleViewAction());
    connect(m_ui->file_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->file_ToolBar->setVisible(visible);
    });
    menu->addAction(m_ui->edit_Toolbar->toggleViewAction());
    connect(m_ui->edit_Toolbar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->edit_Toolbar->setVisible(visible);
    });
    menu->addAction(m_ui->view_ToolBar->toggleViewAction());
    connect(m_ui->view_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->view_ToolBar->setVisible(visible);
    });

    menu->addAction(m_ui->mode_ToolBar->toggleViewAction());
    connect(m_ui->mode_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->mode_ToolBar->setVisible(visible);
    });
    menu->addAction(m_ui->draft_ToolBar->toggleViewAction());
    connect(m_ui->draft_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->draft_ToolBar->setVisible(visible);
    });

    menu->addAction(m_ui->zoom_ToolBar->toggleViewAction());
    connect(m_ui->zoom_ToolBar, &QToolBar::visibilityChanged, this, [this](bool visible) {
        m_ui->zoom_ToolBar->setVisible(visible);
    });
    menu->addAction(m_ui->tools_ToolBox_ToolBar->toggleViewAction());
    menu->addAction(m_ui->points_ToolBar->toggleViewAction());
    menu->addAction(m_ui->lines_ToolBar->toggleViewAction());
    menu->addAction(m_ui->curves_ToolBar->toggleViewAction());
    menu->addAction(m_ui->arcs_ToolBar->toggleViewAction());
    menu->addAction(m_ui->operations_ToolBar->toggleViewAction());
    menu->addAction(m_ui->pieces_ToolBar->toggleViewAction());
    menu->addAction(m_ui->details_ToolBar->toggleViewAction());
    menu->addAction(m_ui->layout_ToolBar->toggleViewAction());
    menu->addAction(m_ui->pointName_ToolBar->toggleViewAction());
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::LastUsedTool()
{
    if (m_currentTool == m_lastUsedTool) {
        return;
    }

    switch (m_lastUsedTool) {
    case Tool::Arrow:
        m_ui->arrowPointer_ToolButton->setChecked(true);
        m_ui->arrow_Action->setChecked(true);
        handleArrowTool(true);
        break;
    case Tool::BasePoint:
    case Tool::SinglePoint:
    case Tool::DoublePoint:
    case Tool::LinePoint:
    case Tool::AbstractSpline:
    case Tool::Cut:
    case Tool::LAST_ONE_DO_NOT_USE:
    case Tool::NodePoint:
    case Tool::NodeArc:
    case Tool::NodeElArc:
    case Tool::NodeSpline:
    case Tool::NodeSplinePath:
    case Tool::BackgroundImage:
        Q_UNREACHABLE();   //-V501
        // Nothing to do here because we can't create this tool from main window.
        break;
    case Tool::EndLine:
        m_ui->pointAtDistanceAngle_ToolButton->setChecked(true);
        handlePointAtDistanceAngleTool(true);
        break;
    case Tool::Line:
        m_ui->line_ToolButton->setChecked(true);
        handleLineTool(true);
        break;
    case Tool::AlongLine:
        m_ui->alongLine_ToolButton->setChecked(true);
        handleAlongLineTool(true);
        break;
    case Tool::Midpoint:
        m_ui->midpoint_ToolButton->setChecked(true);
        handleMidpointTool(true);
        break;
    case Tool::ShoulderPoint:
        m_ui->shoulderPoint_ToolButton->setChecked(true);
        handleShoulderPointTool(true);
        break;
    case Tool::Normal:
        m_ui->normal_ToolButton->setChecked(true);
        handleNormalTool(true);
        break;
    case Tool::Bisector:
        m_ui->bisector_ToolButton->setChecked(true);
        handleBisectorTool(true);
        break;
    case Tool::LineIntersect:
        m_ui->lineIntersect_ToolButton->setChecked(true);
        handleLineIntersectTool(true);
        break;
    case Tool::Spline:
        m_ui->curve_ToolButton->setChecked(true);
        handleCurveTool(true);
        break;
    case Tool::CubicBezier:
        m_ui->curveWithCPs_ToolButton->setChecked(true);
        handleCurveWithControlPointsTool(true);
        break;
    case Tool::Arc:
        m_ui->arc_ToolButton->setChecked(true);
        handleArcTool(true);
        break;
    case Tool::SplinePath:
        m_ui->spline_ToolButton->setChecked(true);
        handleSplineTool(true);
        break;
    case Tool::CubicBezierPath:
        m_ui->splineWithCPs_ToolButton->setChecked(true);
        handleSplineWithControlPointsTool(true);
        break;
    case Tool::PointOfContact:
        m_ui->pointOfContact_ToolButton->setChecked(true);
        handlePointOfContactTool(true);
        break;
    case Tool::Piece:
        m_ui->addPatternPiece_ToolButton->setChecked(true);
        handlePatternPieceTool(true);
        break;
    case Tool::InternalPath:
        m_ui->internalPath_ToolButton->setChecked(true);
        handleInternalPathTool(true);
        break;
    case Tool::Height:
        m_ui->height_ToolButton->setChecked(true);
        handleHeightTool(true);
        break;
    case Tool::Triangle:
        m_ui->triangle_ToolButton->setChecked(true);
        handleTriangleTool(true);
        break;
    case Tool::PointOfIntersection:
        m_ui->pointIntersectXY_ToolButton->setChecked(true);
        handlePointIntersectXYTool(true);
        break;
    case Tool::PointOfIntersectionArcs:
        m_ui->pointOfIntersectionArcs_ToolButton->setChecked(true);
        handlePointOfIntersectionArcsTool(true);
        break;
    case Tool::CutSpline:
        m_ui->pointAlongCurve_ToolButton->setChecked(true);
        handlePointAlongCurveTool(true);
        break;
    case Tool::CutSplinePath:
        m_ui->pointAlongSpline_ToolButton->setChecked(true);
        handlePointAlongSplineTool(true);
        break;
    case Tool::Union:
        m_ui->unitePieces_ToolButton->setChecked(true);
        handleUnionTool(true);
        break;
    case Tool::CutArc:
        m_ui->pointAlongArc_ToolButton->setChecked(true);
        handlePointAlongArcTool(true);
        break;
    case Tool::LineIntersectAxis:
        m_ui->lineIntersectAxis_ToolButton->setChecked(true);
        handleLineIntersectAxisTool(true);
        break;
    case Tool::CurveIntersectAxis:
        m_ui->curveIntersectAxis_ToolButton->setChecked(true);
        handleCurveIntersectAxisTool(true);
        break;
    case Tool::ArcIntersectAxis:
        m_ui->arcIntersectAxis_ToolButton->setChecked(true);
        handleArcIntersectAxisTool(true);
        break;
    case Tool::PointOfIntersectionCircles:
        m_ui->pointOfIntersectionCircles_ToolButton->setChecked(true);
        handlePointOfIntersectionCirclesTool(true);
        break;
    case Tool::PointOfIntersectionCurves:
        m_ui->pointOfIntersectionCurves_ToolButton->setChecked(true);
        handleCurveIntersectCurveTool(true);
        break;
    case Tool::PointFromCircleAndTangent:
        m_ui->pointFromCircleAndTangent_ToolButton->setChecked(true);
        handlePointFromCircleAndTangentTool(true);
        break;
    case Tool::PointFromArcAndTangent:
        m_ui->pointFromArcAndTangent_ToolButton->setChecked(true);
        handlePointFromArcAndTangentTool(true);
        break;
    case Tool::ArcWithLength:
        m_ui->arcWithLength_ToolButton->setChecked(true);
        handleArcWithLengthTool(true);
        break;
    case Tool::TrueDarts:
        m_ui->trueDarts_ToolButton->setChecked(true);
        handleTrueDartTool(true);
        break;
    case Tool::Group:
        m_ui->group_ToolButton->setChecked(true);
        handleGroupTool(true);
        break;
    case Tool::Rotation:
        m_ui->rotation_ToolButton->setChecked(true);
        handleRotationTool(true);
        break;
    case Tool::MirrorByLine:
        m_ui->mirrorByLine_ToolButton->setChecked(true);
        handleMirrorByLineTool(true);
        break;
    case Tool::MirrorByAxis:
        m_ui->mirrorByAxis_ToolButton->setChecked(true);
        handleMirrorByAxisTool(true);
        break;
    case Tool::Move:
        m_ui->move_ToolButton->setChecked(true);
        handleMoveTool(true);
        break;
    case Tool::EllipticalArc:
        m_ui->ellipticalArc_ToolButton->setChecked(true);
        handleEllipticalArcTool(true);
        break;
    case Tool::AnchorPoint:
        m_ui->anchorPoint_ToolButton->setChecked(true);
        handleAnchorPointTool(true);
        break;
    case Tool::InsertNodes:
        m_ui->insertNodes_ToolButton->setChecked(true);
        handleInsertNodesTool(true);
        break;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::AddDocks()
{
    // Add dock
    actionDockWidgetToolOptions = m_ui->toolProperties_DockWidget->toggleViewAction();
    m_ui->view_Menu->addAction(actionDockWidgetToolOptions);
    connect(
        m_ui->toolProperties_DockWidget,
        &QDockWidget::visibilityChanged,
        this,
        [this](bool visible) { m_isToolOptionsDockVisible = visible; });

    actionDockWidgetGroups = m_ui->groups_DockWidget->toggleViewAction();
    m_ui->view_Menu->addAction(actionDockWidgetGroups);
    connect(m_ui->groups_DockWidget, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        m_isGroupsDockVisible = visible;
    });

    actionDockWidgetLayouts = m_ui->layoutPages_DockWidget->toggleViewAction();
    m_ui->view_Menu->addAction(actionDockWidgetLayouts);
    connect(
        m_ui->layoutPages_DockWidget, &QDockWidget::visibilityChanged, this, [this](bool visible) {
            m_isLayoutsDockVisible = visible;
        });

    actionDockWidgetToolbox = m_ui->toolbox_DockWidget->toggleViewAction();
    m_ui->view_Menu->addAction(actionDockWidgetToolbox);
    connect(m_ui->toolbox_DockWidget, &QDockWidget::visibilityChanged, this, [this](bool visible) {
        m_isToolboxDockVisible = visible;
    });

    tabifyDockWidget(m_ui->groups_DockWidget, m_ui->toolProperties_DockWidget);
    splitDockWidget(m_ui->toolProperties_DockWidget, m_ui->layoutPages_DockWidget, Qt::Vertical);
}
//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeDocksContain()
{
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::West);
    setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::East);

    initPropertyEditor();

    qCDebug(vMainWindow, "Initialize Groups manager.");
    m_groupsWidget = new GroupsWidget(pattern, doc, this);
    m_ui->groups_DockWidget->setWidget(m_groupsWidget);
    connect(doc, &VAbstractPattern::updateGroups, this, &MainWindow::updateGroups);

    m_patternPiecesWidget = new PiecesWidget(pattern, doc, this);
    connect(doc, &VPattern::FullUpdateFromFile, m_patternPiecesWidget, &PiecesWidget::updateList);
    connect(doc, &VPattern::UpdateInLayoutList, m_patternPiecesWidget, &PiecesWidget::togglePiece);
    connect(doc, &VPattern::showPiece, m_patternPiecesWidget, &PiecesWidget::selectPiece);
    connect(
        m_patternPiecesWidget,
        &PiecesWidget::Highlight,
        m_pieceScene,
        &VMainGraphicsScene::HighlightItem);
    m_patternPiecesWidget->setVisible(false);

    m_ui->toolbox_StackedWidget->setCurrentIndex(0);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::startNewSeamly2D(const QString& fileName) const
{
    if (this->isWindowModified() || qApp->getFilePath().isEmpty() == false) {
        Application2D::startNewSeamly2D(fileName);
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::createActions()
{
    m_ui->setupUi(this);

    // Files menu
    connect(m_ui->actionNew, &QAction::triggered, this, &MainWindow::New);
    connect(m_ui->actionOpen, &QAction::triggered, this, &MainWindow::Open);
    connect(m_ui->save_Action, &QAction::triggered, this, &MainWindow::Save);
    connect(m_ui->saveAs_Action, &QAction::triggered, this, &MainWindow::SaveAs);
    connect(m_ui->closePattern_Action, &QAction::triggered, this, [this]() {
        if (MaybeSave()) {
            FileClosedCorrect();
            Clear();
        }
    });

    connect(m_ui->exportAs_Action, &QAction::triggered, this, &MainWindow::exportLayoutAs);
    connect(m_ui->printPreview_Action, &QAction::triggered, this, &MainWindow::PrintPreviewOrigin);
    connect(
        m_ui->printPreviewTiled_Action, &QAction::triggered, this, &MainWindow::PrintPreviewTiled);
    connect(m_ui->print_Action, &QAction::triggered, this, &MainWindow::PrintOrigin);
    connect(m_ui->printTiled_Action, &QAction::triggered, this, &MainWindow::PrintTiled);

    connect(m_ui->appPreferences_Action, &QAction::triggered, this, &MainWindow::Preferences);
    connect(m_ui->patternPreferences_Action, &QAction::triggered, this, [this]() {
        DialogPatternProperties proper(doc, pattern, this);
        connect(&proper, &DialogPatternProperties::UpdateGradation, this, [this]() {
            UpdateHeightsList(
                MeasurementVariable::ListHeights(doc->GetGradationHeights(), qApp->patternUnit()));
            UpdateSizesList(
                MeasurementVariable::ListSizes(doc->GetGradationSizes(), qApp->patternUnit()));
        });
        proper.exec();
    });
    m_ui->patternPreferences_Action->setEnabled(false);

    // Actions for recent files loaded by a main window application.
    for (int i = 0; i < MaxRecentFiles; ++i) {
        QAction* action = new QAction(this);
        action->setVisible(false);
        m_recentFileActs[i] = action;
        connect(m_recentFileActs[i], &QAction::triggered, this, [this]() {
            if (QAction* action = qobject_cast<QAction*>(sender())) {
                const QString filePath = action->data().toString();
                if (!filePath.isEmpty()) {
                    LoadPattern(filePath);
                }
            }
        });
    }

    connect(m_ui->documentInfo_Action, &QAction::triggered, this, [this]() {
        ShowInfoDialog* infoDialog = new ShowInfoDialog(doc, this);
        infoDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        infoDialog->adjustSize();
        infoDialog->show();
    });

    connect(m_ui->exit_Action, &QAction::triggered, this, &MainWindow::close);

    // Edit Menu
    connect(m_ui->labelTemplateEditor_Action, &QAction::triggered, this, [this]() {
        EditLabelTemplateDialog editor(doc);
        editor.exec();
    });

    // View menu
    connect(m_ui->showDraftMode, &QAction::triggered, this, &MainWindow::showDraftMode);
    connect(m_ui->pieceMode_Action, &QAction::triggered, this, &MainWindow::showPieceMode);
    connect(m_ui->layoutMode_Action, &QAction::triggered, this, &MainWindow::showLayoutMode);

    connect(m_ui->toggleWireframe_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setWireframe(checked);
        emit m_ui->view->itemClicked(nullptr);
        upDateScenes();
    });

    connect(m_ui->toggleControlPoints_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setShowControlPoints(checked);
        emit m_ui->view->itemClicked(nullptr);
        m_draftScene->enablePiecesMode(checked);
    });

    connect(m_ui->toggleAxisOrigin_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setShowAxisOrigin(checked);
        m_draftScene->setOriginsVisible(checked);
        m_pieceScene->setOriginsVisible(checked);
    });

    connect(m_ui->toggleSeamAllowances_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setShowSeamAllowances(checked);
        emit m_ui->view->itemClicked(nullptr);
        refreshSeamAllowances();
    });

    connect(m_ui->toggleGrainLines_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setShowGrainlines(checked);
        emit m_ui->view->itemClicked(nullptr);
        refreshGrainLines();
    });

    connect(m_ui->toggleLabels_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setShowLabels(checked);
        emit m_ui->view->itemClicked(nullptr);
        refreshLabels();
    });
    /**
        connect(m_ui->toggleAnchorPoints_Action, &QAction::triggered, this, [this](bool checked)
        {
            qApp->Seamly2DSettings()->setShowAnchorPoints(checked);
        });
    **/
    connect(m_ui->increaseSize_Action, &QAction::triggered, this, [this]() {
        int index = qMin(m_fontSizeComboBox->currentIndex() + 1, m_fontSizeComboBox->count() - 1);
        m_fontSizeComboBox->setCurrentIndex(index);
        qApp->Seamly2DSettings()->setPointNameSize(m_fontSizeComboBox->currentText().toInt());
        upDateScenes();
    });

    connect(m_ui->decreaseSize_Action, &QAction::triggered, this, [this]() {
        const int index = qMax(m_fontSizeComboBox->currentIndex() - 1, 0);
        m_fontSizeComboBox->setCurrentIndex(index);
        qApp->Seamly2DSettings()->setPointNameSize(m_fontSizeComboBox->currentText().toInt());
        upDateScenes();
    });

    connect(m_ui->showPointNames_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setHidePointNames(checked);
        upDateScenes();
    });

    connect(m_ui->useToolColor_Action, &QAction::triggered, this, [this](bool checked) {
        qApp->Seamly2DSettings()->setUseToolColor(checked);
        upDateScenes();
    });

    // Tools menu
    connect(m_ui->newDraft_Action, &QAction::triggered, this, [this]() {
        qCDebug(vMainWindow, "New Draft Block.");
        QString draftBlockName = tr("Draft Block %1").arg(m_draftBlockComboBox->count() + 1);
        qCDebug(vMainWindow, "Generated Draft Block name: %s", qUtf8Printable(draftBlockName));

        qCDebug(vMainWindow, "Draft Block count %d", m_draftBlockComboBox->count());
        draftBlockName = createDraftBlockName(draftBlockName);
        qCDebug(vMainWindow, "Draft Block name: %s", qUtf8Printable(draftBlockName));
        if (draftBlockName.isEmpty()) {
            qCDebug(vMainWindow, "Draft Block name is empty.");
            return;
        }

        addDraftBlock(draftBlockName);
    });

    // Tools->Point submenu actions
    connect(m_ui->midpoint_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->midpoint_ToolButton->setChecked(true);
        handleMidpointTool(true);
    });
    connect(m_ui->pointAtDistanceAngle_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointAtDistanceAngle_ToolButton->setChecked(true);
        handlePointAtDistanceAngleTool(true);
    });
    connect(m_ui->pointAlongLine_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->alongLine_ToolButton->setChecked(true);
        handleAlongLineTool(true);
    });
    connect(m_ui->pointAlongPerpendicular_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->normal_ToolButton->setChecked(true);
        handleNormalTool(true);
    });
    connect(m_ui->bisector_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->bisector_ToolButton->setChecked(true);
        handleBisectorTool(true);
    });
    connect(m_ui->pointOnShoulder_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->shoulderPoint_ToolButton->setChecked(true);
        handleShoulderPointTool(true);
    });
    connect(m_ui->pointOfContact_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointOfContact_ToolButton->setChecked(true);
        handlePointOfContactTool(true);
    });
    connect(m_ui->triangle_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->triangle_ToolButton->setChecked(true);
        handleTriangleTool(true);
    });
    connect(m_ui->pointIntersectXY_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->pointIntersectXY_ToolButton->setChecked(true);
        handlePointIntersectXYTool(true);
    });
    connect(m_ui->perpendicularPoint_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->height_ToolButton->setChecked(true);
        handleHeightTool(true);
    });
    connect(m_ui->pointIntersectAxis_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->points_Page);
        m_ui->lineIntersectAxis_ToolButton->setChecked(true);
        handleLineIntersectAxisTool(true);
    });

    // Tools->Line submenu actions
    connect(m_ui->lineTool_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->lines_Page);
        m_ui->line_ToolButton->setChecked(true);
        handleLineTool(true);
    });
    connect(m_ui->lineIntersect_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->lines_Page);
        m_ui->lineIntersect_ToolButton->setChecked(true);
        handleLineIntersectTool(true);
    });

    // Tools->Curve submenu actions
    connect(m_ui->curve_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curve_ToolButton->setChecked(true);
        handleCurveTool(true);
    });
    connect(m_ui->spline_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->spline_ToolButton->setChecked(true);
        handleSplineTool(true);
    });
    connect(m_ui->curveWithCPs_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curveWithCPs_ToolButton->setChecked(true);
        handleCurveWithControlPointsTool(true);
    });
    connect(m_ui->splineWithCPs_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->splineWithCPs_ToolButton->setChecked(true);
        handleSplineWithControlPointsTool(true);
    });
    connect(m_ui->pointAlongCurve_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointAlongCurve_ToolButton->setChecked(true);
        handlePointAlongCurveTool(true);
    });
    connect(m_ui->pointAlongSpline_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointAlongSpline_ToolButton->setChecked(true);
        handlePointAlongSplineTool(true);
    });
    connect(m_ui->curveIntersectCurve_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->pointOfIntersectionCurves_ToolButton->setChecked(true);
        handleCurveIntersectCurveTool(true);
    });
    connect(m_ui->splineIntersectAxis_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->curves_Page);
        m_ui->curveIntersectAxis_ToolButton->setChecked(true);
        handleCurveIntersectAxisTool(true);
    });

    // Tools->Arc submenu actions
    connect(m_ui->arcTool_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arc_ToolButton->setChecked(true);
        handleArcTool(true);
    });

    connect(m_ui->pointAlongArc_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointAlongArc_ToolButton->setChecked(true);
        handlePointAlongArcTool(true);
    });

    connect(m_ui->arcIntersectAxis_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arcIntersectAxis_ToolButton->setChecked(true);
        handleArcIntersectAxisTool(true);
    });

    connect(m_ui->arcIntersectArc_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointOfIntersectionArcs_ToolButton->setChecked(true);
        handlePointOfIntersectionArcsTool(true);
    });

    connect(m_ui->circleIntersect_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointOfIntersectionCircles_ToolButton->setChecked(true);
        handlePointOfIntersectionCirclesTool(true);
    });

    connect(m_ui->circleTangent_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointFromCircleAndTangent_ToolButton->setChecked(true);
        handlePointFromCircleAndTangentTool(true);
    });

    connect(m_ui->arcTangent_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->pointFromArcAndTangent_ToolButton->setChecked(true);
        handlePointFromArcAndTangentTool(true);
    });

    connect(m_ui->arcWithLength_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->arcWithLength_ToolButton->setChecked(true);
        handleArcWithLengthTool(true);
    });

    connect(m_ui->ellipticalArc_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->arcs_Page);
        m_ui->ellipticalArc_ToolButton->setChecked(true);
        handleEllipticalArcTool(true);
    });

    // Tools->Operations submenu actions
    connect(m_ui->group_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->group_ToolButton->setChecked(true);
        handleGroupTool(true);
    });

    connect(m_ui->rotation_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->rotation_ToolButton->setChecked(true);
        handleRotationTool(true);
    });

    connect(m_ui->mirrorByLine_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->mirrorByLine_ToolButton->setChecked(true);
        handleMirrorByLineTool(true);
    });
    connect(m_ui->mirrorByAxis_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->mirrorByAxis_ToolButton->setChecked(true);
        handleMirrorByAxisTool(true);
    });

    connect(m_ui->move_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->move_ToolButton->setChecked(true);
        handleMoveTool(true);
    });

    connect(m_ui->trueDarts_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        m_ui->trueDarts_ToolButton->setChecked(true);
        handleTrueDartTool(true);
    });

    connect(m_ui->exportDraftBlocks_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->operations_Page);
        exportDraftBlocksAs();
    });

    // Tools->Details submenu actions
    connect(m_ui->union_Action, &QAction::triggered, this, [this] {
        m_ui->piece_ToolBox->setCurrentWidget(m_ui->details_Page);
        m_ui->unitePieces_ToolButton->setChecked(true);
        handleUnionTool(true);
    });

    connect(m_ui->exportPieces_Action, &QAction::triggered, this, [this] {
        m_ui->piece_ToolBox->setCurrentWidget(m_ui->details_Page);
        exportPiecesAs();
    });

    // Tools->Piece submenu actions
    connect(m_ui->addPiece_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->addPatternPiece_ToolButton->setChecked(true);
        handlePatternPieceTool(true);
    });
    connect(m_ui->anchorPoint_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->anchorPoint_ToolButton->setChecked(true);
        handleAnchorPointTool(true);
    });
    connect(m_ui->internalPath_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->internalPath_ToolButton->setChecked(true);
        handleInternalPathTool(true);
    });
    connect(m_ui->insertNodes_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->piece_Page);
        m_ui->insertNodes_ToolButton->setChecked(true);
        handleInsertNodesTool(true);
    });

    // Tools-> Images submenu actions

    connect(m_ui->importImage_Action, &QAction::triggered, this, [this] {
        m_ui->draft_ToolBox->setCurrentWidget(m_ui->backgroundImage_Page);
        m_ui->importImage_ToolButton->setChecked(true);
        handleImportImage();
    });
    // connect(ui->deleteImage_Action, &QAction::triggered, this, &MainWindow::handleDeleteImage);
    // connect(ui->lockImage_Action, &QAction::triggered, this, &MainWindow::handleLockImage);
    // connect(ui->alignImage_Action, &QAction::triggered, this, &MainWindow::handleAlignImage);

    // Tools->Layout submenu actions
    connect(m_ui->newPrintLayout_Action, &QAction::triggered, this, [this] {
        m_ui->layout_ToolBox->setCurrentWidget(m_ui->layout_Page);
        m_ui->layoutSettings_ToolButton->setChecked(true);
        handleNewLayout(true);
    });

    connect(m_ui->exportLayout_Action, &QAction::triggered, this, [this] {
        m_ui->layout_ToolBox->setCurrentWidget(m_ui->layout_Page);
        exportLayoutAs();
    });

    connect(m_ui->lastTool_Action, &QAction::triggered, this, &MainWindow::LastUsedTool);

    // Measurements menu
    connect(m_ui->openSeamlyMe_Action, &QAction::triggered, this, [this]() {
        const QString seamlyme = qApp->seamlyMeFilePath();
        const QString workingDirectory = QFileInfo(seamlyme).absoluteDir().absolutePath();

        QStringList arguments;
        if (isNoScaling) {
            arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
        }

        QProcess::startDetached(seamlyme, arguments, workingDirectory);
    });

    connect(m_ui->editCurrent_Action, &QAction::triggered, this, &MainWindow::ShowMeasurements);
    connect(
        m_ui->unloadMeasurements_Action,
        &QAction::triggered,
        this,
        &MainWindow::UnloadMeasurements);
    connect(m_ui->loadIndividual_Action, &QAction::triggered, this, &MainWindow::LoadIndividual);
    connect(m_ui->loadMultisize_Action, &QAction::triggered, this, &MainWindow::LoadMultisize);
    connect(
        m_ui->syncMeasurements_Action, &QAction::triggered, this, &MainWindow::SyncMeasurements);
    connect(m_ui->table_Action, &QAction::triggered, this, [this](bool checked) {
        if (checked) {
            m_dialogTable = new DialogVariables(pattern, doc, this);
            connect(
                m_dialogTable.data(),
                &DialogVariables::updateProperties,
                m_toolProperties,
                &VToolOptionsPropertyBrowser::refreshOptions);
            connect(m_dialogTable.data(), &DialogVariables::DialogClosed, this, [this]() {
                m_ui->table_Action->setChecked(false);
                if (m_dialogTable != nullptr) {
                    delete m_dialogTable;
                }
            });
            m_dialogTable->show();
        } else {
            m_ui->table_Action->setChecked(true);
            m_dialogTable->activateWindow();
        }
    });
    connect(
        m_ui->exportVariablesToCSV_Action,
        &QAction::triggered,
        this,
        &MainWindow::handleExportToCSV);

    // History menu
    connect(m_ui->history_Action, &QAction::triggered, this, [this](bool checked) {
        if (checked) {
            m_historyDialog = new HistoryDialog(pattern, doc, this);
            connect(
                this,
                &MainWindow::RefreshHistory,
                m_historyDialog.data(),
                &HistoryDialog::updateHistory);
            connect(m_historyDialog.data(), &HistoryDialog::DialogClosed, this, [this]() {
                m_ui->history_Action->setChecked(false);
                if (m_historyDialog != nullptr) {
                    delete m_historyDialog;
                }
            });
            m_historyDialog->show();
        } else {
            m_ui->history_Action->setChecked(true);
            m_historyDialog->activateWindow();
        }
    });

    // Utilities menu
    connect(m_ui->calculator_Action, &QAction::triggered, this, [this]() {
        CalculatorDialog* calcDialog = new CalculatorDialog(this);
        calcDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        calcDialog->setWindowTitle(tr("Calculator"));
        calcDialog->adjustSize();
        calcDialog->show();
    });

    connect(m_ui->decimalChart_Action, &QAction::triggered, this, [this]() {
        DecimalChartDialog* decimalchartDialog = new DecimalChartDialog(this);
        decimalchartDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        decimalchartDialog->show();
    });

    // Help menu
    connect(m_ui->shortcuts_Action, &QAction::triggered, this, [this]() {
        ShortcutsDialog* shortcutsDialog = new ShortcutsDialog(this);
        shortcutsDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        shortcutsDialog->show();
    });
    connect(m_ui->wiki_Action, &QAction::triggered, this, []() {
        qCDebug(vMainWindow, "Showing online help");
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://wiki.seamly.net/wiki/Main_Page")));
    });

    connect(m_ui->forum_Action, &QAction::triggered, this, []() {
        qCDebug(vMainWindow, "Opening forum");
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://forum.seamly.io/")));
    });

    connect(m_ui->reportBug_Action, &QAction::triggered, this, []() {
        qCDebug(vMainWindow, "Reporting bug");
        QDesktopServices::openUrl(
            QUrl(QStringLiteral("https://github.com/FashionFreedom/Seamly2D/issues/"
                                "new?&labels=bug&template=bug_report.md&title=BUG%3A")));
    });

    connect(m_ui->aboutQt_Action, &QAction::triggered, this, [this]() {
        QMessageBox::aboutQt(this, tr("About Qt"));
    });

    connect(m_ui->aboutSeamly2D_Action, &QAction::triggered, this, [this]() {
        DialogAboutApp* aboutDialog = new DialogAboutApp(this);
        aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
        aboutDialog->show();
    });

    // Toolbox toolbar
    connect(m_ui->arrow_Action, &QAction::triggered, this, &MainWindow::handleArrowTool);
    connect(m_ui->points_Action, &QAction::triggered, this, &MainWindow::handlePointsMenu);
    connect(m_ui->lines_Action, &QAction::triggered, this, &MainWindow::handleLinesMenu);
    connect(m_ui->arcs_Action, &QAction::triggered, this, &MainWindow::handleArcsMenu);
    connect(m_ui->curves_Action, &QAction::triggered, this, &MainWindow::handleCurvesMenu);
    connect(
        m_ui->modifications_Action, &QAction::triggered, this, &MainWindow::handleOperationsMenu);
    connect(m_ui->details_Action, &QAction::triggered, this, &MainWindow::handlePatternPiecesMenu);
    connect(m_ui->pieces_Action, &QAction::triggered, this, &MainWindow::handlePieceMenu);
    connect(m_ui->layout_Action, &QAction::triggered, this, &MainWindow::handleLayoutMenu);
    connect(m_ui->images_Action, &QAction::triggered, this, &MainWindow::handleImagesMenu);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::initializeAutoSave()
{
    // Autosaving file each 1 minutes
    delete m_autoSaveTimer;
    m_autoSaveTimer = nullptr;

    m_autoSaveTimer = new QTimer(this);
    m_autoSaveTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(m_autoSaveTimer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
    m_autoSaveTimer->stop();

    if (qApp->Seamly2DSettings()->GetAutosaveState()) {
        const qint32 autoTime = qApp->Seamly2DSettings()->getAutosaveInterval();
        m_autoSaveTimer->start(autoTime * 60000);
        qCInfo(vMainWindow, "Autosaving every %d minutes.", autoTime);
    }
    qApp->setAutoSaveTimer(m_autoSaveTimer);
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::createDraftBlockName(const QString& text)
{
    QInputDialog dialog{ this };
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setLabelText(tr("Name:"));
    dialog.setWindowTitle(tr("Draft block."));
    dialog.setWindowFlags(
        windowFlags() & ~Qt::WindowContextHelpButtonHint & ~Qt::WindowMaximizeButtonHint
        & ~Qt::WindowMinimizeButtonHint);
    dialog.resize(300, 100);
    dialog.setTextValue(text);
    QString draftBlockName;
    while (true) {
        const bool result = dialog.exec();
        draftBlockName = dialog.textValue().trimmed();
        if (!result || draftBlockName.isEmpty()) {
            return QString();
        }
        if (m_draftBlockComboBox->findText(draftBlockName) == -1) {
            break;   // exit dialog
        }
        // repeat show dialog
        QMessageBox messageBox;
        messageBox.setWindowTitle(tr("Name Exists"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Retry);
        messageBox.setText(
            tr("The action can't be completed because the Draft Block name already exists."));
        int boxResult = messageBox.exec();

        switch (boxResult) {
        case QMessageBox::Retry: break;               // Repeat Dialog
        case QMessageBox::Cancel: return QString();   // Exit Dialog
        default: break;                               // should never be reached
        }
    }
    return draftBlockName;
}

//---------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    CancelTool();
    MainWindow::CleanLayout();

    delete doc;
    delete m_ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief LoadPattern open pattern file.
 * @param fileName name of file.
 */
bool MainWindow::LoadPattern(const QString& fileName, const QString& customMeasureFile)
{
    qCInfo(vMainWindow, "Loading new file %s.", qUtf8Printable(fileName));

    // We have unsaved changes or load more then one file per time
    if (startNewSeamly2D(fileName)) {
        return false;
    }

    if (fileName.isEmpty()) {
        qCWarning(vMainWindow, "New loaded filename is empty.");
        Clear();
        return false;
    }

    try {
        // Here comes undocumented Seamly2D's feature.
        // Because app bundle in Mac OS X doesn't allow setup association for SeamlyMe we must do
        // this through Seamly2D
        MeasurementDoc measurements(pattern);
        measurements.setSize(VContainer::rsize());
        measurements.setHeight(VContainer::rheight());
        measurements.setXMLContent(fileName);

        if (measurements.Type() == MeasurementsType::Multisize
            || measurements.Type() == MeasurementsType::Individual) {
            const QString seamlyme = qApp->seamlyMeFilePath();
            const QString workingDirectory = QFileInfo(seamlyme).absoluteDir().absolutePath();

            QStringList arguments = QStringList() << fileName;
            if (isNoScaling) {
                arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
            }

            QProcess::startDetached(seamlyme, arguments, workingDirectory);
            qApp->exit(V_EX_OK);
            return false;   // stop continue processing
        }
    }

    catch (VException& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("File exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));

        Clear();
        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    qCDebug(vMainWindow, "locking file");
    VlpCreateLock(m_lock, fileName);

    if (m_lock->IsLocked()) {
        qCInfo(vMainWindow, "Pattern file %s was locked.", qUtf8Printable(fileName));
    } else {
        if (!IgnoreLocking(m_lock->GetLockError(), fileName)) {
            return false;
        }
    }

    // On this stage scene empty. Fit scene size to view size
    VMainGraphicsView::NewSceneRect(m_draftScene, m_ui->view);
    VMainGraphicsView::NewSceneRect(m_pieceScene, m_ui->view);

    qApp->setOpeningPattern();   // Begin opening file
    try {
        VPatternConverter converter(fileName);
        m_curFileFormatVersion = converter.GetCurrentFormatVarsion();
        m_curFileFormatVersionStr = converter.GetVersionStr();
        doc->setXMLContent(converter.Convert());
        if (!customMeasureFile.isEmpty()) {
            doc->SetMPath(RelativeMPath(fileName, customMeasureFile));
        }
        qApp->setPatternUnit(doc->measurementUnits());
        const QString path = AbsoluteMPath(fileName, doc->MPath());

        if (!path.isEmpty()) {
            // Check if exist
            const QString newPath = checkPathToMeasurements(fileName, path);
            if (newPath.isEmpty()) {
                qApp->setOpeningPattern();   // End opening file
                Clear();
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(tr("The measurements file '%1' could not be found.").arg(path)));
                if (!Application2D::isGUIMode()) {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            }

            if (!loadMeasurements(newPath)) {
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(
                        tr("The measurements file '%1' could not be found.").arg(newPath)));
                qApp->setOpeningPattern();   // End opening file
                Clear();
                if (!Application2D::isGUIMode()) {
                    qApp->exit(V_EX_NOINPUT);
                }
                return false;
            } else {
                m_ui->unloadMeasurements_Action->setEnabled(true);
                m_watcher->addPath(path);
                m_ui->editCurrent_Action->setEnabled(true);
            }
        }

        if (qApp->patternType() == MeasurementsType::Unknown) {   // Show toolbar only if was not
                                                                  // uploaded any measurements.

            initializeStatusToolBar();
        }
    }

    catch (VException& exception) {
        qCCritical(
            vMainWindow,
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("File exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));

        qApp->setOpeningPattern();   // End opening file
        Clear();
        if (!Application2D::isGUIMode()) {
            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }

    fullParseFile();

    if (m_guiEnabled) {   // No errors occurred
        m_patternReadOnly = doc->isReadOnly();
        setWidgetsEnabled(true);
        setCurrentFile(fileName);
        m_helpLabel->setText(tr("File loaded"));
        qCDebug(vMainWindow, "%s", qUtf8Printable(m_helpLabel->text()));

        // Fit scene size to best size for first show
        zoomFirstShow();
        updateZoomToPointComboBox(draftPointNamesList());

        showDraftMode(true);

        qApp->setOpeningPattern();   // End opening file
        return true;
    } else {
        qApp->setOpeningPattern();   // End opening file
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList MainWindow::GetUnlokedRestoreFileList() const
{
    QStringList restoreFiles;
    // Take all files that need to be restored
    QStringList files = qApp->Seamly2DSettings()->GetRestoreFileList();
    if (!files.isEmpty()) {
        for (int i = 0; i < files.size(); ++i) {
            // Seeking file that really needs reopen
            VLockGuard<char> m_lock(files.at(i));
            if (m_lock.IsLocked()) {
                restoreFiles.append(files.at(i));
            }
        }

        // Clearing list after filtering
        for (int i = 0; i < restoreFiles.size(); ++i) {
            files.removeAll(restoreFiles.at(i));
        }

        // Clear all files that do not exist.
        QStringList filtered;
        for (int i = 0; i < files.size(); ++i) {
            if (QFileInfo::exists(files.at(i))) {
                filtered.append(files.at(i));
            }
        }

        qApp->Seamly2DSettings()->SetRestoreFileList(filtered);
    }
    return restoreFiles;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolBarStyles()
{
    ToolBarStyle(m_ui->draft_ToolBar);
    ToolBarStyle(m_ui->mode_ToolBar);
    ToolBarStyle(m_ui->edit_Toolbar);
    ToolBarStyle(m_ui->zoom_ToolBar);
    ToolBarStyle(m_ui->file_ToolBar);

    m_fontComboBox->setCurrentFont(qApp->Seamly2DSettings()->getPointNameFont());
    int index = m_fontSizeComboBox->findData(qApp->Seamly2DSettings()->getPointNameSize());
    m_fontSizeComboBox->setCurrentIndex(index);
}

void MainWindow::resetOrigins()
{
    m_draftScene->initializeOrigins();
    m_draftScene->setOriginsVisible(true);
    m_pieceScene->initializeOrigins();
    m_pieceScene->setOriginsVisible(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::showLayoutPages(int index)
{
    if (index < 0 || index >= scenes.size()) {
        m_ui->view->setScene(tempSceneLayout);
    } else {
        m_ui->view->setScene(scenes.at(index));
    }

    m_ui->view->fitInView(m_ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::Preferences()
{
    // Calling constructor of the dialog take some time. Because of this user have time to call the
    // dialog twice.
    static QPointer<DialogPreferences> guard;   // Prevent any second run
    if (guard.isNull()) {
        QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        auto dialog{ std::make_unique<DialogPreferences>(this) };
        guard = dialog.get();
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::WindowsLocale);   // Must be first
        connect(
            dialog.get(), &DialogPreferences::updateProperties, this, &MainWindow::ToolBarStyles);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::updateToolBarVisibility);
        connect(
            dialog.get(), &DialogPreferences::updateProperties, this, &MainWindow::refreshLabels);
        connect(
            dialog.get(), &DialogPreferences::updateProperties, this, &MainWindow::resetOrigins);
        connect(
            dialog.get(), &DialogPreferences::updateProperties, this, &MainWindow::upDateScenes);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::updateViewToolbar);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::resetPanShortcuts);
        connect(dialog.get(), &DialogPreferences::updateProperties, this, [this]() {
            emit doc->FullUpdateFromFile();
        });
        // connect(dialog.data(), &DialogPreferences::updateProperties,
        //        m_toolProperties, &VToolOptionsPropertyBrowser::refreshOptions);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::initPropertyEditor);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            this,
            &MainWindow::initBasePointComboBox);

        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            m_ui->view,
            &VMainGraphicsView::resetScrollBars);
        connect(
            dialog.get(),
            &DialogPreferences::updateProperties,
            m_ui->view,
            &VMainGraphicsView::resetScrollAnimations);

        QGuiApplication::restoreOverrideCursor();

        if (guard->exec() == QDialog::Accepted) {
            initializeAutoSave();
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
#if defined(Q_OS_MAC)
void MainWindow::CreateMeasurements()
{
    const QString seamlyme = qApp->seamlyMeFilePath();
    const QString workingDirectory = QFileInfo(seamlyme).absoluteDir().absolutePath();

    QStringList arguments;
    if (isNoScaling) {
        arguments.append(QLatin1String("--") + LONG_OPTION_NO_HDPI_SCALING);
    }

    QProcess::startDetached(seamlyme, arguments, workingDirectory);
}
#endif

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::exportLayoutAs()
{
    m_ui->exportLayout_ToolButton->setChecked(true);

    if (isLayoutStale) {
        if (ContinueIfLayoutStale() == QMessageBox::No) {
            m_ui->exportLayout_ToolButton->setChecked(false);
            return;
        }
    }

    try {
        ExportLayoutDialog dialog(scenes.size(), Draw::Layout, FileName(), this);

        if (dialog.exec() == QDialog::Rejected) {
            m_ui->exportLayout_ToolButton->setChecked(false);
            return;
        }

        ExportData(QVector<VLayoutPiece>(), dialog);
    }

    catch (const VException& exception) {
        m_ui->exportLayout_ToolButton->setChecked(false);
        qCritical(
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Export exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        return;
    }
    m_ui->exportLayout_ToolButton->setChecked(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::exportPiecesAs()
{
    m_helpLabel->setText(QString(""));

    m_ui->arrowPointer_ToolButton->setChecked(false);
    m_ui->arrow_Action->setChecked(false);
    m_ui->exportPiecesAs_ToolButton->setChecked(true);

    const QHash<quint32, VPiece>& allPieces = pattern->DataPieces();
    QHash<quint32, VPiece>::const_iterator i = allPieces.constBegin();
    QHash<quint32, VPiece> piecesInLayout;
    while (i != allPieces.constEnd()) {
        if (i.value().isInLayout()) {
            piecesInLayout.insert(i.key(), i.value());
        }
        ++i;
    }

    if (piecesInLayout.isEmpty()) {
        QMessageBox::information(
            this,
            tr("Layout mode"),
            tr("You don't have any pieces to export. Please, "
               "include at least one piece in layout."),
            QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }

    QVector<VLayoutPiece> pieceList;
    try {
        pieceList = preparePiecesForLayout(piecesInLayout);
    }

    catch (VException& exception) {
        QMessageBox::warning(
            this,
            tr("Export pieces"),
            tr("Can't export pieces.") + QLatin1String(" \n") + exception.ErrorMessage(),
            QMessageBox::Ok,
            QMessageBox::Ok);
        return;
    }

    try {
        ExportLayoutDialog dialog(1, Draw::Modeling, FileName(), this);
        dialog.setWindowTitle("Export Pattern Pieces");

        if (dialog.exec() == QDialog::Rejected) {
            m_ui->exportPiecesAs_ToolButton->setChecked(false);
            return;
        }

        ExportData(pieceList, dialog);
    }

    catch (const VException& exception) {
        m_ui->exportPiecesAs_ToolButton->setChecked(false);
        qCritical(
            "%s\n\n%s\n\n%s",
            qUtf8Printable(tr("Export exception.")),
            qUtf8Printable(exception.ErrorMessage()),
            qUtf8Printable(exception.DetailedInformation()));
        return;
    }

    m_ui->arrowPointer_ToolButton->setChecked(true);
    m_ui->arrow_Action->setChecked(true);
    m_ui->exportPiecesAs_ToolButton->setChecked(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::exportDraftBlocksAs()
{
    m_helpLabel->setText(QString(""));

    // select export tool button
    m_ui->arrowPointer_ToolButton->setChecked(false);
    m_ui->arrow_Action->setChecked(false);
    m_ui->exportDraftBlocks_ToolButton->setChecked(true);

    // Get view info so we can restore after export
    int vScrollBar = m_ui->view->verticalScrollBar()->value();
    int hScrollBar = m_ui->view->horizontalScrollBar()->value();
    QTransform viewTransform = m_ui->view->transform();

    // Include all items in draft scene
    m_ui->view->zoomToFit();
    m_ui->view->repaint();
    m_ui->view->zoom100Percent();

    // Enable all draft Blocks in the scene
    const QList<QGraphicsItem*> items = m_draftScene->items();
    for (auto* item : items) {
        item->setEnabled(true);
    }
    m_ui->view->repaint();

    m_draftScene->setOriginsVisible(false);

    // Open a file dialog to save export
    ExportLayoutDialog dialog(1, Draw::Calculation, FileName(), this);
    dialog.setWindowTitle("Export Draft Blocks");

    if (dialog.exec() == QDialog::Accepted) {
        const QString filename = QString("%1/%2%3").arg(
            dialog.path(),                                              // 1
            dialog.fileName(),                                          // 2
            ExportLayoutDialog::exportFormatSuffix(dialog.format()));   // 3

        QRectF rect;
        rect = m_draftScene->itemsBoundingRect();
        m_draftScene->update(rect);
        QGraphicsRectItem* paper = new QGraphicsRectItem(rect);
        QMarginsF margins = QMarginsF(0, 0, 0, 0);

        switch (dialog.format()) {
        case LayoutExportFormat::SVG: {
            exportSVG(filename, paper, m_draftScene);
            break;
        }
        case LayoutExportFormat::PNG: {
            exportPNG(filename, m_draftScene);
            break;
        }
        case LayoutExportFormat::JPG: {
            exportJPG(filename, m_draftScene);
            break;
        }
        case LayoutExportFormat::BMP: {
            exportBMP(filename, m_draftScene);
            break;
        }
        case LayoutExportFormat::TIF: {
            exportTIF(filename, m_draftScene);
            break;
        }
        case LayoutExportFormat::PPM: {
            exportPPM(filename, m_draftScene);
            break;
        }
        case LayoutExportFormat::PDF: {
            exportPDF(filename, paper, m_draftScene, true, margins);
            break;
        }
        case LayoutExportFormat::PDFTiled:
        case LayoutExportFormat::OBJ:
        case LayoutExportFormat::PS: {
            exportPS(filename, paper, m_draftScene, true, margins);
            break;
        }
        case LayoutExportFormat::EPS: {
            exportEPS(filename, paper, m_draftScene, true, margins);
            break;
        }
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
        case LayoutExportFormat::DXF_AC1027_ASTM:
        default: break;
        }
    }

    // Disable draft Blocks in the scenee except current block
    doc->changeActiveDraftBlock(doc->getActiveDraftBlockName(), Document::FullParse);

    m_draftScene->setOriginsVisible(qApp->Settings()->getShowAxisOrigin());

    // Restore scale, scrollbars, current active draft block
    m_ui->view->setTransform(viewTransform);
    VMainGraphicsView::NewSceneRect(m_ui->view->scene(), m_ui->view);
    zoomScaleChanged(m_ui->view->transform().m11());

    m_ui->view->verticalScrollBar()->setValue(vScrollBar);
    m_ui->view->horizontalScrollBar()->setValue(hScrollBar);

    // reset tool buttons
    m_ui->arrowPointer_ToolButton->setChecked(true);
    m_ui->arrow_Action->setChecked(true);
    m_ui->exportDraftBlocks_ToolButton->setChecked(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ReopenFilesAfterCrash(QStringList& args)
{
    const QStringList files = GetUnlokedRestoreFileList();
    if (!files.isEmpty()) {
        qCDebug(vMainWindow, "Reopen files after crash.");

        QStringList restoreFiles;
        for (int i = 0; i < files.size(); ++i) {
            QFile file(files.at(i) + autosavePrefix);
            if (file.exists()) {
                restoreFiles.append(files.at(i));
            }
        }

        if (!restoreFiles.isEmpty()) {
            QMessageBox::StandardButton reply;
            const QString mes = tr("Seamly2D didn't shut down correctly. Do you want reopen files "
                                   "(%1) you had open?")
                                    .arg(restoreFiles.size());
            reply = QMessageBox::question(
                this,
                tr("Reopen files."),
                mes,
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes);
            if (reply == QMessageBox::Yes) {
                qCDebug(vMainWindow, "User said Yes.");

                for (int i = 0; i < restoreFiles.size(); ++i) {
                    QString error;
                    if (VDomDocument::SafeCopy(
                            restoreFiles.at(i) + autosavePrefix, restoreFiles.at(i), error)) {
                        QFile autoFile(restoreFiles.at(i) + autosavePrefix);
                        autoFile.remove();
                        LoadPattern(restoreFiles.at(i));
                        args.removeAll(
                            restoreFiles.at(i));   // Do not open file twice after we restore him.
                    } else {
                        qCWarning(
                            vMainWindow,
                            "Could not copy %s%s to %s %s",
                            qUtf8Printable(restoreFiles.at(i)),
                            qUtf8Printable(autosavePrefix),
                            qUtf8Printable(restoreFiles.at(i)),
                            qUtf8Printable(error));
                    }
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::checkPathToMeasurements(const QString& patternPath, const QString& path)
{
    if (path.isEmpty()) {
        return path;
    }

    QFileInfo table(path);
    if (table.exists() == false) {
        if (!Application2D::isGUIMode()) {
            return QString();   // console mode doesn't support fixing path to a measurement file
        } else {
            const QString text = tr("The measurements file <br/><br/> <b>%1</b> <br/><br/> could "
                                    "not be found. Do you "
                                    "want to update the file location?")
                                     .arg(path);
            QMessageBox::StandardButton result = QMessageBox::question(
                this,
                tr("Loading measurements file"),
                text,
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes);
            if (result == QMessageBox::No) {
                return QString();
            } else {
                MeasurementsType patternType;
                if ((table.suffix() == vstExt) || (table.suffix() == smmsExt)) {
                    patternType = MeasurementsType::Multisize;
                } else if ((table.suffix() == vitExt) || (table.suffix() == smisExt)) {
                    patternType = MeasurementsType::Individual;
                } else {
                    patternType = MeasurementsType::Unknown;
                }

                QString filename;
                if (patternType == MeasurementsType::Multisize) {
                    const QString filter = tr("Multisize measurements") + QLatin1String(" (*.")
                                         + smmsExt + QLatin1String(" *.") + vstExt
                                         + QLatin1String(")");
                    // Use standard path to multisize measurements
                    QString dir = qApp->Seamly2DSettings()->getMultisizePath();
                    dir = VCommonSettings::prepareMultisizeTables(dir);
                    filename = fileDialog(
                        this,
                        tr("Open file"),
                        dir,
                        filter,
                        nullptr,
                        QFileDialog::DontUseNativeDialog,
                        QFileDialog::ExistingFile,
                        QFileDialog::AcceptOpen);

                } else if (patternType == MeasurementsType::Individual) {
                    const QString filter = tr("Individual measurements") + QLatin1String(" (*.")
                                         + smisExt + QLatin1String(" *.") + vitExt
                                         + QLatin1String(")");
                    // Use standard path to individual measurements
                    const QString dir = qApp->Seamly2DSettings()->getIndividualSizePath();

                    bool usedNotExistedDir = false;

                    QDir directory(dir);
                    if (!directory.exists()) {
                        usedNotExistedDir = directory.mkpath(".");
                    }

                    filename = fileDialog(
                        this,
                        tr("Open file"),
                        dir,
                        filter,
                        nullptr,
                        QFileDialog::DontUseNativeDialog,
                        QFileDialog::ExistingFile,
                        QFileDialog::AcceptOpen);

                    if (usedNotExistedDir) {
                        QDir directory(dir);
                        directory.rmpath(".");
                    }
                } else {
                    const QString filter = tr("Individual measurements") + QLatin1String(" (*.")
                                         + smisExt + QLatin1String(" *.") + vitExt
                                         + QLatin1String(");;") + tr("Multisize measurements")
                                         + QLatin1String(" (*.") + smmsExt + QLatin1String(" *.")
                                         + vstExt + QLatin1String(")");

                    // Use standard path to individual measurements
                    const QString dir = qApp->Seamly2DSettings()->getIndividualSizePath();
                    VCommonSettings::prepareMultisizeTables(
                        VCommonSettings::getDefaultMultisizePath());

                    bool usedNotExistedDir = false;
                    QDir directory(path);
                    if (!directory.exists()) {
                        usedNotExistedDir = directory.mkpath(".");
                    }

                    filename = fileDialog(
                        this,
                        tr("Open file"),
                        dir,
                        filter,
                        nullptr,
                        QFileDialog::DontUseNativeDialog,
                        QFileDialog::ExistingFile,
                        QFileDialog::AcceptOpen);

                    if (usedNotExistedDir) {
                        QDir directory(dir);
                        directory.rmpath(".");
                    }
                }

                if (filename.isEmpty()) {
                    return filename;
                } else {
                    MeasurementDoc measurements{ pattern };
                    measurements.setSize(VContainer::rsize());
                    measurements.setHeight(VContainer::rheight());
                    measurements.setXMLContent(filename);

                    patternType = measurements.Type();

                    if (patternType == MeasurementsType::Unknown) {
                        VException exception(tr("Measurement file has unknown format."));
                        throw exception;
                    }

                    if (patternType == MeasurementsType::Multisize) {
                        MultiSizeConverter converter(filename);
                        QString filename = converter.Convert();
                        if (filename.contains(QLatin1String(".") + vstExt)) {
                            filename.replace(
                                QLatin1String(".") + vstExt, QLatin1String(".") + smmsExt);
                            QString error;
                            const bool result = measurements.SaveDocument(filename, error);
                            if (result) {
                                UpdateWindowTitle();
                            }
                        }
                        measurements.setXMLContent(filename);   // Read again after conversion
                    } else {
                        IndividualSizeConverter converter(filename);
                        QString filename = converter.Convert();
                        if (filename.contains(QLatin1String(".") + vitExt)) {
                            filename.replace(
                                QLatin1String(".") + vitExt, QLatin1String(".") + smisExt);
                            QString error;
                            const bool result = measurements.SaveDocument(filename, error);
                            if (result) {
                                UpdateWindowTitle();
                            }
                        }
                        measurements.setXMLContent(filename);   // Read again after conversion
                    }

                    if (!measurements.eachKnownNameIsValid()) {
                        VException exception(
                            tr("Measurement file contains invalid known measurement(s)."));
                        throw exception;
                    }

                    checkRequiredMeasurements(measurements);

                    qApp->setPatternType(patternType);

                    doc->SetMPath(RelativeMPath(patternPath, filename));
                    patternChangesWereSaved(false);
                    return filename;
                }
            }
        }
    }
    return path;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::changeDraftBlock(int index, bool zoomBestFit)
{
    if (index != -1) {
        doc->changeActiveDraftBlock(m_draftBlockComboBox->itemText(index));
        doc->setCurrentData();
        emit RefreshHistory();
        if (m_drawMode) {
            handleArrowTool(true);
            if (zoomBestFit) {
                zoomToSelected();
            }
        }
        m_toolProperties->itemClicked(nullptr);   // hide options for tool in previous pattern piece
        m_groupsWidget->updateGroups();
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::EndVisualization(bool click)
{
    if (!m_dialogTool.isNull()) {
        m_dialogTool->ShowDialog(click);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief zoomFirstShow Fit scene size to best size for first show
 */
void MainWindow::zoomFirstShow()
{
    /* If don't call zoomToFit() twice, after first scaling or moving pattern piece, scene change
     * coordinate and whole pattern will be moved. Looks very ugly. It is best solution that i have
     * now.
     */
    if (!pattern->DataPieces().isEmpty()) {
        showPieceMode(true);
        m_ui->view->zoomToFit();
    }
    if (!m_ui->showDraftMode->isChecked()) {
        showDraftMode(true);
    }
    zoomToSelected();

    VMainGraphicsView::NewSceneRect(m_draftScene, m_ui->view);
    VMainGraphicsView::NewSceneRect(m_pieceScene, m_ui->view);

    if (!pattern->DataPieces().isEmpty()) {
        showPieceMode(true);
        m_ui->view->zoomToFit();
    }

    if (!m_ui->showDraftMode->isChecked()) {
        showDraftMode(true);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::DoExport(const VCommandLinePtr& expParams)
{
    const QHash<quint32, VPiece>& pieces = pattern->DataPieces();
    if (!qApp->getOpeningPattern()) {
        if (pieces.isEmpty()) {
            qCCritical(vMainWindow, "%s", qUtf8Printable(tr("You can't export empty scene.")));
            qApp->exit(V_EX_DATAERR);
            return;
        }
    }
    pieceList = preparePiecesForLayout(pieces);

    const bool exportOnlyPieces = expParams->exportOnlyPieces();
    if (exportOnlyPieces) {
        try {
            ExportLayoutDialog dialog(1, Draw::Modeling, expParams->OptBaseName(), this);
            dialog.setDestinationPath(expParams->OptDestinationPath());
            dialog.selectFormat(static_cast<LayoutExportFormat>(expParams->OptExportType()));
            dialog.setBinaryDXFFormat(expParams->IsBinaryDXF());
            dialog.setTextAsPaths(expParams->isTextAsPaths());

            ExportData(pieceList, dialog);
        }

        catch (const VException& exception) {
            qCCritical(
                vMainWindow,
                "%s\n\n%s",
                qUtf8Printable(tr("Export exception.")),
                qUtf8Printable(exception.ErrorMessage()));
            qApp->exit(V_EX_DATAERR);
            return;
        }
    } else {
        auto settings = expParams->DefaultGenerator();
        settings->SetTestAsPaths(expParams->isTextAsPaths());

        if (LayoutSettings(*settings.get())) {
            try {
                ExportLayoutDialog dialog(
                    scenes.size(), Draw::Layout, expParams->OptBaseName(), this);
                dialog.setDestinationPath(expParams->OptDestinationPath());
                dialog.selectFormat(static_cast<LayoutExportFormat>(expParams->OptExportType()));
                dialog.setBinaryDXFFormat(expParams->IsBinaryDXF());

                ExportData(pieceList, dialog);
            }

            catch (const VException& exception) {
                qCCritical(
                    vMainWindow,
                    "%s\n\n%s",
                    qUtf8Printable(tr("Export exception.")),
                    qUtf8Printable(exception.ErrorMessage()));
                qApp->exit(V_EX_DATAERR);
                return;
            }
        } else {
            return;
        }
    }

    qApp->exit(V_EX_OK);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::setSize(const QString& text)
{
    if (!Application2D::isGUIMode()) {
        if (this->isWindowModified() || not qApp->getFilePath().isEmpty()) {
            if (qApp->patternType() == MeasurementsType::Multisize) {
                const int size = static_cast<int>(
                    UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = m_gradationSizes->findText(QString().setNum(size));
                if (index != -1) {
                    m_gradationSizes->setCurrentIndex(index);
                } else {
                    qCCritical(
                        vMainWindow,
                        "%s",
                        qUtf8Printable(
                            tr("Not supported size value '%1' for this pattern file.").arg(text)));
                    return false;
                }
            } else {
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(
                        tr("Couldn't set size. Need a file with multisize measurements.")));
                return false;
            }
        } else {
            qCCritical(
                vMainWindow, "%s", qUtf8Printable(tr("Couldn't set size. File wasn't opened.")));
            return false;
        }
    } else {
        qCWarning(
            vMainWindow,
            "%s",
            qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::setHeight(const QString& text)
{
    if (!Application2D::isGUIMode()) {
        if (this->isWindowModified() || not qApp->getFilePath().isEmpty()) {
            if (qApp->patternType() == MeasurementsType::Multisize) {
                const int height = static_cast<int>(
                    UnitConvertor(text.toInt(), Unit::Cm, *pattern->GetPatternUnit()));
                const qint32 index = m_gradationHeights->findText(QString().setNum(height));
                if (index != -1) {
                    m_gradationHeights->setCurrentIndex(index);
                } else {
                    qCCritical(
                        vMainWindow,
                        "%s",
                        qUtf8Printable(tr("Not supported height value '%1' for this pattern file.")
                                           .arg(text)));
                    return false;
                }
            } else {
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(
                        tr("Couldn't set height. Need a file with multisize measurements.")));
                return false;
            }
        } else {
            qCCritical(
                vMainWindow, "%s", qUtf8Printable(tr("Couldn't set height. File wasn't opened.")));
            return false;
        }
    } else {
        qCWarning(
            vMainWindow,
            "%s",
            qUtf8Printable(tr("The method %1 does nothing in GUI mode").arg(Q_FUNC_INFO)));
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::processCommandLine()
{
    const VCommandLinePtr cmd = qApp->commandLine();
    auto args = cmd->OptInputFileNames();

    isNoScaling = cmd->IsNoScalingEnabled();

    if (Application2D::isGUIMode()) {
        ReopenFilesAfterCrash(args);
    } else {
        if (args.size() != 1) {
            qCritical() << tr("Please, provide one input file.");
            qApp->exit(V_EX_NOINPUT);
            return;
        }
    }

    for (int i = 0, sz = args.size(); i < sz; ++i) {
        const bool loaded = LoadPattern(args.at(static_cast<int>(i)), cmd->OptMeasurePath());

        if (!loaded && not Application2D::isGUIMode()) {
            return;   // process only one input file
        }

        bool hSetted = true;
        bool sSetted = true;
        if (loaded && (cmd->IsTestModeEnabled() || cmd->IsExportEnabled())) {
            if (cmd->IsSetGradationSize()) {
                sSetted = setSize(cmd->OptGradationSize());
            }

            if (cmd->IsSetGradationHeight()) {
                hSetted = setHeight(cmd->OptGradationHeight());
            }
        }

        if (!cmd->IsTestModeEnabled()) {
            if (cmd->IsExportEnabled()) {
                if (loaded && hSetted && sSetted) {
                    DoExport(cmd);
                    return;   // process only one input file
                } else {
                    qApp->exit(V_EX_DATAERR);
                    return;
                }
                break;
            }
        }
    }

    if (!Application2D::isGUIMode()) {
        qApp->exit(V_EX_OK);   // close program after processing in console mode
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetPatternFileName()
{
    QString shownName = tr("untitled.sm2d");
    if (!qApp->getFilePath().isEmpty()) {
        shownName = qApp->getFilePath();
    }
    shownName += QLatin1String("[*]");
    return shownName;
}

//---------------------------------------------------------------------------------------------------------------------
QString MainWindow::GetMeasurementFileName()
{
    if (doc->MPath().isEmpty()) {
        return "";
    } else {
        QString shownName(" - [");
        shownName += strippedName(AbsoluteMPath(qApp->getFilePath(), doc->MPath()));

        if (m_changes) {
            shownName += QLatin1String("*");
        }

        shownName += QLatin1String("]");
        return shownName;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::UpdateWindowTitle()
{
    bool isFileWritable = true;
    if (!qApp->getFilePath().isEmpty()) {
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup++;   // turn checking on
#endif                                 /*Q_OS_WIN32*/
        isFileWritable = QFileInfo(qApp->getFilePath()).isWritable();
#ifdef Q_OS_WIN32
        qt_ntfs_permission_lookup--;   // turn it off again
#endif                                 /*Q_OS_WIN32*/
    }

    if (!m_patternReadOnly && isFileWritable) {
        setWindowTitle(
            VER_INTERNALNAME_STR + QString(" - ") + GetPatternFileName()
            + GetMeasurementFileName());
    } else {
        setWindowTitle(
            VER_INTERNALNAME_STR + QString(" - ") + GetPatternFileName() + GetMeasurementFileName()
            + QString(" - ") + tr("read only"));
    }
    setWindowFilePath(qApp->getFilePath());

#if defined(Q_OS_MAC)
    static QIcon fileIcon = QIcon(
        QCoreApplication::applicationDirPath() + QLatin1String("/../Resources/Seamly2D.icns"));
    QIcon icon;
    if (!qApp->getFilePath().isEmpty()) {
        if (!isWindowModified()) {
            icon = fileIcon;
        } else {
            static QIcon darkIcon;

            if (darkIcon.isNull()) {
                darkIcon = QIcon(darkenPixmap(fileIcon.pixmap(16, 16)));
            }
            icon = darkIcon;
        }
    }
    setWindowIcon(icon);
#endif   // defined(Q_OS_MAC)
}

void MainWindow::upDateScenes()
{
    if (m_draftScene) {
        m_draftScene->update();
    }

    if (m_pieceScene) {
        m_pieceScene->update();
    }
}

void MainWindow::updateViewToolbar()
{
    m_ui->toggleWireframe_Action->setChecked(qApp->Settings()->isWireframe());
    m_ui->toggleControlPoints_Action->setChecked(qApp->Settings()->getShowControlPoints());
    m_ui->toggleAxisOrigin_Action->setChecked(qApp->Settings()->getShowAxisOrigin());
    m_ui->toggleGrainLines_Action->setChecked(qApp->Settings()->showGrainlines());
    m_ui->toggleSeamAllowances_Action->setChecked(qApp->Settings()->showSeamAllowances());
    m_ui->toggleLabels_Action->setChecked(qApp->Settings()->showLabels());
}

void MainWindow::resetPanShortcuts()
{
    QList<QKeySequence> zoomPanShortcuts;
    zoomPanShortcuts = m_ui->zoomPan_Action->shortcuts();
    zoomPanShortcuts.removeAll(QKeySequence(Qt::Key_Space));
    if (!qApp->Seamly2DSettings()->isPanActiveSpaceKey()) {
        zoomPanShortcuts.append(QKeySequence(Qt::Key_Space));
    }
    m_ui->zoomPan_Action->setShortcuts(zoomPanShortcuts);
}

//---------------------------------------------------------------------------------------------------------------------
bool MainWindow::IgnoreLocking(int error, const QString& path)
{
    QMessageBox::StandardButton answer = QMessageBox::Abort;
    if (Application2D::isGUIMode()) {
        switch (error) {
        case QLockFile::LockFailedError:
            answer = QMessageBox::warning(
                this,
                tr("locking file"),
                tr("This file already opened in another window. Ignore if you want "
                   "to continue (not recommended, can cause a data corruption)."),
                QMessageBox::Abort | QMessageBox::Ignore,
                QMessageBox::Abort);
            break;
        case QLockFile::PermissionError:
            answer = QMessageBox::question(
                this,
                tr("locking file"),
                tr("The lock file could not be created, for lack of permissions. "
                   "Ignore if you want to continue (not recommended, can cause "
                   "a data corruption)."),
                QMessageBox::Abort | QMessageBox::Ignore,
                QMessageBox::Abort);
            break;
        case QLockFile::UnknownError:
            answer = QMessageBox::question(
                this,
                tr("locking file"),
                tr("Unknown error happened, for instance a full partition prevented "
                   "writing out the lock file. Ignore if you want to continue (not "
                   "recommended, can cause a data corruption)."),
                QMessageBox::Abort | QMessageBox::Ignore,
                QMessageBox::Abort);
            break;
        default: answer = QMessageBox::Abort; break;
        }
    }

    if (answer == QMessageBox::Abort) {
        qCDebug(vMainWindow, "Failed to lock %s", qUtf8Printable(path));
        qCDebug(vMainWindow, "Error type: %d", error);
        Clear();
        if (!Application2D::isGUIMode()) {
            switch (error) {
            case QLockFile::LockFailedError:
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(tr("This file already opened in another window.")));
                break;
            case QLockFile::PermissionError:
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(
                        tr("The lock file could not be created, for lack of permissions.")));
                break;
            case QLockFile::UnknownError:
                qCCritical(
                    vMainWindow,
                    "%s",
                    qUtf8Printable(
                        tr("Unknown error happened, for instance a full partition prevented "
                           "writing out the lock file.")));
                break;
            default: break;
            }

            qApp->exit(V_EX_NOINPUT);
        }
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief draftPointNamesList gets the list of points in draft mode.
 */
QStringList MainWindow::draftPointNamesList()
{
    QStringList pointNames;
    for (QHash<quint32, QSharedPointer<VGObject>>::const_iterator item =
             pattern->DataGObjects().begin();
         item != pattern->DataGObjects().end();
         ++item) {
        if (item.value()->getType() == GOType::Point && !pointNames.contains(item.value()->name()))
            pointNames << item.value()->name();
    }
    pointNames.sort();
    pointNames.removeDuplicates();
    return pointNames;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief updateZoomToPointComboBox updates the list of points included in the toolbar combobox.
 */
void MainWindow::updateZoomToPointComboBox(QStringList namesList)
{
    m_zoomToPointComboBox->blockSignals(
        true);   // prevent this UI update from zooming to the first point
    m_zoomToPointComboBox->clear();
    m_zoomToPointComboBox->addItems(namesList);
    m_zoomToPointComboBox->blockSignals(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPoint() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);
    emit EnableImageHover(false);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByRelease() const
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMouseRelease);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointByPress() const
{
    ToolSelectPoint();
    emit ItemsSelection(SelectionType::ByMousePress);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSpline() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(false);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectSplinePath() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(false);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(true);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectArc() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectPointArc() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(false);
    emit EnableSplineHover(false);
    emit EnableSplinePathHover(false);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectCurve() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(false);
    emit EnablePointHover(false);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::selectAllDraftObjectsTool() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(false);
    emit EnablePointSelection(false);
    emit EnableLineSelection(false);
    emit EnableArcSelection(false);
    emit EnableElArcSelection(false);
    emit EnableSplineSelection(false);
    emit EnableSplinePathSelection(false);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectOperationObjects() const
{
    // Only true for rubber band selection
    emit EnableLabelSelection(true);
    emit EnablePointSelection(true);
    emit EnableLineSelection(false);
    emit EnableArcSelection(true);
    emit EnableElArcSelection(true);
    emit EnableSplineSelection(true);
    emit EnableSplinePathSelection(true);
    emit EnableImageSelection(false);

    // Hovering
    emit EnableLabelHover(true);
    emit EnablePointHover(true);
    emit EnableLineHover(false);
    emit EnableArcHover(true);
    emit EnableElArcHover(true);
    emit EnableSplineHover(true);
    emit EnableSplinePathHover(true);
    emit EnableImageHover(false);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::ToolSelectGroupObjects() const
{
    ToolSelectOperationObjects();
    // Only true for rubber band selection
    emit EnableLineSelection(true);

    // Hovering
    emit EnableLineHover(true);
}

//---------------------------------------------------------------------------------------------------------------------
void MainWindow::selectPieceTool() const
{
    // Only true for rubber band selection
    emit EnableNodeLabelSelection(false);
    emit EnableNodePointSelection(false);
    emit enablePieceSelection(true);   // Disable when done with pattern piece visualization.

    // Hovering
    emit EnableNodeLabelHover(true);
    emit EnableNodePointHover(true);
    emit enablePieceHover(true);

    emit ItemsSelection(SelectionType::ByMouseRelease);

    m_ui->view->allowRubberBand(false);
}
