/******************************************************************************
 *   @file   mainwindow.h
 **  @author Douglas S Caskey
 **  @date   14 Jul, 2023
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
 *************************************************************************/

/************************************************************************
 **
 **  @file   mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../vmisc/vlockguard.h"
#include "core/vcmdexport.h"
#include "mainwindowsnogui.h"

#include <QFileSystemWatcher>
#include <QMap>
#include <QPointer>
#include <QSharedPointer>

#include <memory>

namespace Ui {
class MainWindow;
}

class VToolOptionsPropertyBrowser;
class MeasurementDoc;
class QFileSystemWatcher;
class QLabel;
class DialogVariables;
class DialogTool;
class HistoryDialog;
class CalculatorDialog;
class DecimalChartDialog;
class ShowInfoDialog;
class ShortcutsDialog;
class GroupsWidget;
class PiecesWidget;
class DraftToolBox;
class PieceToolBox;
class LayoutToolBox;
class QToolButton;
class QDoubleSpinBox;
class QFontComboBox;

struct DraftImage;
class ImageItem;

class MouseCoordinates;
class PenToolBar;

/**
 * @brief The MainWindow class main windows.
 */
class MainWindow : public MainWindowsNoGUI
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

    bool LoadPattern(const QString& fileName, const QString& customMeasureFile = QString());

public slots:
    void processCommandLine();
    void penChanged(Pen pen);
    void basePointChanged();

    void ShowToolTip(const QString& toolTip) override;
    void updateGroups() override;
    void zoomToSelected() override;
    void showAllGroups();
    void hideAllGroups();
    void lockAllGroups();
    void unlockAllGroups();
    void addGroupToList();
    void deleteGroupFromList();
    void editGroup();
    void addSelectedItemsToGroup();

signals:
    void RefreshHistory();
    void EnableItemMove(bool move);
    void ItemsSelection(SelectionType type) const;

    void EnableLabelSelection(bool enable) const;
    void EnablePointSelection(bool enable) const;
    void EnableLineSelection(bool enable) const;
    void EnableArcSelection(bool enable) const;
    void EnableElArcSelection(bool enable) const;
    void EnableSplineSelection(bool enable) const;
    void EnableSplinePathSelection(bool enable) const;
    void EnableNodeLabelSelection(bool enable) const;
    void EnableNodePointSelection(bool enable) const;
    void EnableImageSelection(bool enable) const;
    void enablePieceSelection(bool enable) const;
    void EnableLabelHover(bool enable) const;
    void EnablePointHover(bool enable) const;
    void EnableLineHover(bool enable) const;
    void EnableArcHover(bool enable) const;
    void EnableElArcHover(bool enable) const;
    void EnableSplineHover(bool enable) const;
    void EnableSplinePathHover(bool enable) const;
    void EnableNodeLabelHover(bool enable) const;
    void EnableNodePointHover(bool enable) const;
    void EnableImageHover(bool enable) const;
    void enablePieceHover(bool enable) const;

    void signalZoomToAreaActive(bool enable) const;
    void signalZoomPanActive(bool enable) const;

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    void customEvent(QEvent* event) override;
    void CleanLayout() override;
    void PrepareSceneList() override;
    void exportToCSVData(const QString& fileName, const DialogExportToCSV& dialog) final;
    void handleExportToCSV();

private slots:
    void zoomScaleChanged(qreal scale);
    void MouseMove(const QPointF& scenePos);
    void Clear();
    void patternChangesWereSaved(bool saved);
    void LastUsedTool();
    void fullParseFile();
    void setGuiEnabled(bool enabled);
    void changeDraftBlockGlobally(const QString& patternPiece);
    void ToolBarStyles();
    void resetOrigins();
    void showLayoutPages(int index);
    void Preferences();
#if defined(Q_OS_MAC)
    void CreateMeasurements();
#endif
    void exportLayoutAs();
    void exportPiecesAs();
    void exportDraftBlocksAs();

    void handleArrowTool(bool checked);
    void handlePointAtDistanceAngleTool(bool checked);
    void handleAlongLineTool(bool checked);
    void handleMidpointTool(bool checked);
    void handleShoulderPointTool(bool checked);
    void handleNormalTool(bool checked);
    void handleBisectorTool(bool checked);
    void handleHeightTool(bool checked);
    void handleTriangleTool(bool checked);
    void handleLineIntersectAxisTool(bool checked);
    void handlePointOfContactTool(bool checked);
    void handlePointIntersectXYTool(bool checked);

    void handleLineTool(bool checked);
    void handleLineIntersectTool(bool checked);

    void handleCurveTool(bool checked);
    void handleSplineTool(bool checked);
    void handleCurveWithControlPointsTool(bool checked);
    void handleSplineWithControlPointsTool(bool checked);
    void handlePointAlongCurveTool(bool checked);
    void handlePointAlongSplineTool(bool checked);
    void handleCurveIntersectCurveTool(bool checked);
    void handleCurveIntersectAxisTool(bool checked);

    void handleArcTool(bool checked);
    void handlePointAlongArcTool(bool checked);
    void handlePointFromArcAndTangentTool(bool checked);
    void handleArcWithLengthTool(bool checked);
    void handleArcIntersectAxisTool(bool checked);
    void handlePointOfIntersectionArcsTool(bool checked);
    void handlePointOfIntersectionCirclesTool(bool checked);
    void handlePointFromCircleAndTangentTool(bool checked);

    void handleEllipticalArcTool(bool checked);

    void handleGroupTool(bool checked);
    void handleRotationTool(bool checked);
    void handleMirrorByLineTool(bool checked);
    void handleMirrorByAxisTool(bool checked);
    void handleMoveTool(bool checked);
    void handleTrueDartTool(bool checked);

    void handleInternalPathTool(bool checked);
    void handleAnchorPointTool(bool checked);
    void handleInsertNodesTool(bool checked);

    void handleImportImage();
    void addImage(DraftImage image);

    void handleDeleteImage(quint32 id);
    void handleImageSelected(quint32 id);
    void handleLockImage(bool state);
    void handleAlignImage();
    void updateImage(DraftImage image);
    void handleLockImageAspect(bool state);
    void setStatusMessage(QString message);
    QString getImageFilename();


    void handlePatternPieceTool(bool checked);
    void handleUnionTool(bool checked);

    void handleNewLayout(bool checked);

    void showDraftMode(bool checked);
    void showPieceMode(bool checked);
    void showLayoutMode(bool checked);

    void New();
    bool SaveAs();
    bool Save();
    void Open();

    void closeUnionDialog(int result);
    void ClosedEditGroupDialog(int result);
    void ClosedDialogInternalPath(int result);
    void ClosedDialogAnchorPoint(int result);
    void ClosedInsertNodesDialog(int result);

    void zoomToPrevious();
    void zoomToArea(bool checked);
    void zoomPan(bool checked);

    void zoomToPoint(const QString& pointName);
    void showZoomToPointDialog();

    void LoadIndividual();
    void LoadMultisize();
    void UnloadMeasurements();
    void ShowMeasurements();
    void MeasurementsChanged(const QString& path);
    void SyncMeasurements();
#if defined(Q_OS_MAC)
    void OpenAt(QAction* where);
#endif   // defined(Q_OS_MAC)

    void ChangedSize(int index);
    void ChangedHeight(int index);

private:
    Q_DISABLE_COPY(MainWindow)
    /** @brief ui keeps information about user interface */
    Ui::MainWindow* m_ui;

    QFileSystemWatcher* m_watcher{ new QFileSystemWatcher(this) };

    /** @brief tool current tool */
    Tool m_currentTool{ Tool::Arrow };

    /** @brief tool last used tool */
    Tool m_lastUsedTool{ Tool::Arrow };

    /** @brief draftScene draft block scene. */
    VMainGraphicsScene* m_draftScene{ nullptr };

    /** @brief pieceScene pattern piece scene. */
    VMainGraphicsScene* m_pieceScene{ nullptr };

    /** @brief mouseCoordinates pointer to label who show mouse coordinate. */
    QPointer<MouseCoordinates> m_mouseCoordinates;

    QPointer<QToolButton> m_infoToolButton;

    /** @brief helpLabel help show tooltip. */
    QLabel* m_helpLabel{ nullptr };

    /** @brief isInitialized true after first show window. */
    bool m_isInitialized{ false };

    /** @brief m_changes true if measurement file was changed. */
    bool m_changes{ false };
    bool m_changesAsked{ true };

    bool m_patternReadOnly{ false };

    QPointer<DialogVariables> m_dialogTable;
    QSharedPointer<DialogTool> m_dialogTool;
    QPointer<HistoryDialog> m_historyDialog;

    QFontComboBox* m_fontComboBox{ nullptr };
    QComboBox* m_fontSizeComboBox{ nullptr };
    QComboBox* m_basePointComboBox{ nullptr };
    QComboBox* m_draftBlockComboBox{
        nullptr
    }; /** @brief draftBlockComboBox stores names of draft blocks.*/
    QLabel* m_draftBlockLabel{ nullptr };
    Draw m_mode{ Draw::Calculation };  /** @brief mode stores current draw mode. */
    qint32 m_currentBlockIndex{ 0 };   /** @brief currentBlockIndex  current selected draft block.*/
    qint32 m_currentToolBoxIndex{ 0 }; /** @brief currentToolBoxIndex  current set of tools. */
    bool m_isToolOptionsDockVisible{ true };
    bool m_isGroupsDockVisible{ true };
    bool m_isLayoutsDockVisible{ false };
    bool m_isToolboxDockVisible{ true };
    bool m_drawMode{ true }; /** @brief drawMode true if draft scene active. */

    enum
    {
        MaxRecentFiles = 5
    };
    QAction* m_recentFileActs[MaxRecentFiles];
    QAction* m_separatorAct{ nullptr };

    QLabel* m_leftGoToStage{ nullptr };
    QLabel* m_rightGoToStage{ nullptr };
    QTimer* m_autoSaveTimer{ nullptr };
    bool m_guiEnabled{ true };
    QPointer<QComboBox> m_gradationHeights;
    QPointer<QComboBox> m_gradationSizes;
    QPointer<QLabel> m_gradationHeightsLabel;
    QPointer<QLabel> m_gradationSizesLabel;
    VToolOptionsPropertyBrowser* m_toolProperties{ nullptr };
    GroupsWidget* m_groupsWidget{ nullptr };
    PiecesWidget* m_patternPiecesWidget{ nullptr };
    std::shared_ptr<VLockGuard<char>> m_lock;

    QDoubleSpinBox* m_zoomScaleSpinBox{ nullptr };
    PenToolBar* m_penToolBar{ nullptr };   //!< for selecting the current pen
    QComboBox* m_zoomToPointComboBox{ nullptr };

    void SetDefaultHeight();
    void SetDefaultSize();

    void initializeStatusToolBar();
    void initializeModesToolBar();
    void initializeDraftToolBar();
    void initializePointNameToolBar();
    void initializeToolsToolBar();
    void initializeToolBarVisibility();
    void initPenToolBar();
    void initPropertyEditor();
    void initBasePointComboBox();

    void updateToolBarVisibility();
    void setToolBarVisibility(QToolBar* toolbar, bool visible);
    void initializeToolButtons();

    void handlePointsMenu();
    void handleLinesMenu();
    void handleArcsMenu();
    void handleCurvesMenu();
    void handleCirclesMenu();
    void handleOperationsMenu();
    void handlePatternPiecesMenu();
    void handlePieceMenu();
    void handleLayoutMenu();
    void handleImagesMenu();

    void CancelTool();

    void setWidgetsEnabled(bool enable);
    void setToolsEnabled(bool enable);
    void SetLayoutModeActions();

    void SaveCurrentScene();
    void RestoreCurrentScene();
    void MinimumScrollBar();

    template <typename Dialog, typename Func>
    void SetToolButton(
        bool checked, Tool t, const QString& cursor, const QString& toolTip, Func closeDialogSlot);
    template <typename Dialog, typename Func, typename Func2>
    void SetToolButtonWithApply(
        bool checked,
        Tool t,
        const QString& cursor,
        const QString& toolTip,
        Func closeDialogSlot,
        Func2 applyDialogSlot);
    template <typename DrawTool>
    void ClosedDialog(int result);

    template <typename DrawTool>
    void ClosedDialogWithApply(int result, VMainGraphicsScene* scene);
    template <typename DrawTool>
    void ApplyDialog(VMainGraphicsScene* scene);
    template <typename DrawTool>
    void ClosedDrawDialogWithApply(int result);
    template <typename DrawTool>
    void ApplyDrawDialog();
    template <typename DrawTool>
    void ClosedPiecesDialogWithApply(int result);
    template <typename DrawTool>
    void applyPiecesDialog();

    bool SavePattern(const QString& fileName, QString& error);
    void AutoSavePattern();
    void setCurrentFile(const QString& fileName);

    void ReadSettings();
    void WriteSettings();

    bool MaybeSave();
    void InfoUnsavedImages(bool* firstImportImage);
    void UpdateRecentFileActions();

    void createMenus();
    void createActions();
    void initializeAutoSave();
    QString PatternPieceName(const QString& text);
    QString CheckPathToMeasurements(const QString& patternPath, const QString& path);
    QComboBox* SetGradationList(QLabel* label, const QStringList& list);
    QString createDraftBlockName(const QString& text);
    QString checkPathToMeasurements(const QString& patternPath, const QString& path);
    void changeDraftBlock(int index, bool zoomBestFit = true);
    /**
     * @brief EndVisualization try show dialog after and working with tool visualization.
     */
    void EndVisualization(bool click = false);
    void zoomFirstShow();
    void UpdateHeightsList(const QStringList& list);
    void UpdateSizesList(const QStringList& list);

    void AddDocks();
    void initializeDocksContain();
    bool startNewSeamly2D(const QString& fileName = QString()) const;
    void FileClosedCorrect();
    QStringList GetUnlokedRestoreFileList() const;

    void addDraftBlock(const QString& blockName);
    QPointF draftBlockStartPosition() const;

    void initializeScenes();

    QSharedPointer<MeasurementDoc> openMeasurementFile(const QString& fileName);
    bool loadMeasurements(const QString& fileName);
    bool updateMeasurements(const QString& fileName, int size, int height);
    void checkRequiredMeasurements(const MeasurementDoc* m);

    void ReopenFilesAfterCrash(QStringList& args);
    void DoExport(const VCommandLinePtr& expParams);

    bool setSize(const QString& text);
    bool setHeight(const QString& text);

    QString GetPatternFileName();
    QString GetMeasurementFileName();

    void UpdateWindowTitle();
    void upDateScenes();
    void updateViewToolbar();
    void resetPanShortcuts();

    QStringList draftPointNamesList();

    void updateZoomToPointComboBox(QStringList namesList);

    bool IgnoreLocking(int error, const QString& path);

    void ToolSelectPoint() const;
    void ToolSelectPointByPress() const;
    void ToolSelectPointByRelease() const;
    void ToolSelectSpline() const;
    void ToolSelectSplinePath() const;
    void ToolSelectArc() const;
    void ToolSelectPointArc() const;
    void ToolSelectCurve() const;
    void selectAllDraftObjectsTool() const;
    void ToolSelectOperationObjects() const;
    void ToolSelectGroupObjects() const;
    void selectPieceTool() const;
};

#endif   // MAINWINDOW_H
