/***************************************************************************
 **  @file   vlayoutgenerator.h
 **  @author Douglas S Caskey
 **  @date   Dec 27, 2022
 **
 **  @copyright
 **  Copyright (C) 2017 - 2022 Seamly, LLC
 **  https://github.com/fashionfreedom/seamly2d
 **
 **  @brief
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
 **  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

/************************************************************************
 **
 **  @file   vlayoutgenerator.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   2 1, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VLAYOUTGENERATOR_H
#define VLAYOUTGENERATOR_H

#include <QList>
#include <QMargins>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QVector>
#include <QtGlobal>
#include <atomic>
#include <memory>
#include <qcompilerdetection.h>

#include "vbank.h"
#include "vlayoutdef.h"

class QMarginsF;
class QGraphicsItem;
class VLayoutPaper;

class VLayoutGenerator : public QObject
{
    Q_OBJECT
public:
    explicit VLayoutGenerator(QObject* parent = nullptr);
    virtual ~VLayoutGenerator() override;

    void setPieces(const QVector<VLayoutPiece>& details);
    void SetLayoutWidth(qreal width);
    void SetCaseType(Cases caseType);
    int PieceCount();

    qreal GetPaperHeight() const;
    void SetPaperHeight(qreal value);

    qreal GetPaperWidth() const;
    void SetPaperWidth(qreal value);

    bool IsUsePrinterFields() const;
    QMarginsF GetPrinterFields() const;
    void SetPrinterFields(bool usePrinterFields, const QMarginsF& value);

    quint32 GetShift() const;
    void SetShift(quint32 shift);

    void Generate();

    LayoutErrors State() const;

    [[nodiscard]] QList<QGraphicsItem*> GetPapersItems() const;
    [[nodiscard]] QList<QList<QGraphicsItem*>> getAllPieceItems() const;

    QVector<QVector<VLayoutPiece>> getAllPieces() const;

    bool GetRotate() const;
    void SetRotate(bool value);

    int GetRotationIncrease() const;
    void SetRotationIncrease(int value);

    bool GetAutoCrop() const;
    void SetAutoCrop(bool value);

    bool IsSaveLength() const;
    void SetSaveLength(bool value);

    bool IsUnitePages() const;
    void SetUnitePages(bool value);

    quint8 GetMultiplier() const;
    void SetMultiplier(const quint8& value);

    bool IsStripOptimization() const;
    void SetStripOptimization(bool value);

    bool IsTestAsPaths() const;
    void SetTestAsPaths(bool value);

signals:
    void Start();
    void Arranged(int count);
    void Error(const LayoutErrors& state);
    void Finished();

public slots:
    void Abort();

private:
    Q_DISABLE_COPY(VLayoutGenerator)
    QVector<VLayoutPaper> papers;
    VBank* bank;
    qreal paperHeight;
    qreal paperWidth;
    QMarginsF margins;
    bool usePrinterFields;
    std::atomic_bool stopGeneration;
    LayoutErrors state;
    quint32 shift;
    bool rotate;
    int rotationIncrease;
    bool autoCrop;
    bool saveLength;
    bool unitePages;
    bool stripOptimizationEnabled;
    quint8 multiplier;
    bool stripOptimization;
    bool textAsPaths;

    int PageHeight() const;
    int PageWidth() const;

    void GatherPages();
    void UnitePages();
    void unitePieces(int j, QList<QList<VLayoutPiece>>& pieces, qreal length, int i);
    void UnitePapers(int j, QList<qreal>& papersLength, qreal length);
    QList<VLayoutPiece> movePieces(qreal length, const QVector<VLayoutPiece>& pieces);
};

#endif   // VLAYOUTGENERATOR_H
