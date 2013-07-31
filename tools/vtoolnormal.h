#ifndef VTOOLNORMAL_H
#define VTOOLNORMAL_H

#include <QSharedPointer>

#include "vtoollinepoint.h"
#include "../dialogs/dialognormal.h"

class VToolNormal : public VToolLinePoint
{
public:
                                 VToolNormal(VDomDocument *doc, VContainer *data, const qint64 &id,
                                             const QString &typeLine, const QString &formula,
                                             const qint32 &angle, const qint64 &firstPointId,
                                             const qint64 &secondPointId, Tool::Enum typeCreation,
                                             QGraphicsItem * parent = 0);
    static QPointF               FindPoint(const QPointF &firstPoint, const QPointF &secondPoint,
                                           const qreal &length, const qint32 &angle = 0);
public slots:
    virtual void                 FullUpdateFromFile();
    virtual void                 FullUpdateFromGui(int result);
protected:
    virtual void                 contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    virtual void                 AddToFile();
private:
    qint64                       secondPointId;
    QSharedPointer<DialogNormal> dialogNormal;
};

#endif // VTOOLNORMAL_H
