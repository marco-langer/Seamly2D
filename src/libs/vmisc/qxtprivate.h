#ifndef QXTPRIVATE_H
#define QXTPRIVATE_H

#include <QtGlobal>

/****************************************************************************
** This file is derived from code bearing the following notice:
** The sole author of this file, Adam Higerd, has explicitly disclaimed all
** copyright interest and protection for the content within. This file has
** been placed in the public domain according to United States copyright
** statute and case law. In jurisdictions where this public domain dedication
** is not legally recognized, anyone who receives a copy of this file is
** permitted to use, modify, duplicate, and redistribute this file, in whole
** or in part, with no restrictions or conditions. In these jurisdictions,
** this file shall be copyright (C) 2006-2008 by Adam Higerd.
****************************************************************************/

#define QXT_DECLARE_PRIVATE(PUB) \
    friend class PUB##Private;   \
    QxtPrivateInterface<PUB, PUB##Private> qxt_d;
#define QXT_DECLARE_PUBLIC(PUB) friend class PUB;
#define QXT_INIT_PRIVATE(PUB)   qxt_d.setPublic(this);
#define QXT_D(PUB)              PUB##Private& d = qxt_d()
#define QXT_P(PUB)              PUB& p = qxt_p()

template <typename PUB>
class QxtPrivate
{
public:
    QxtPrivate()
        : qxt_p_ptr(nullptr)
    {
    }
    virtual ~QxtPrivate() {}
    inline void QXT_setPublic(PUB* pub) { qxt_p_ptr = pub; }

protected:
    inline PUB& qxt_p() { return *qxt_p_ptr; }
    inline const PUB& qxt_p() const { return *qxt_p_ptr; }
    inline PUB* qxt_ptr() { return qxt_p_ptr; }
    inline const PUB* qxt_ptr() const { return qxt_p_ptr; }

private:
    Q_DISABLE_COPY(QxtPrivate)
    PUB* qxt_p_ptr;
};

template <typename PUB, typename PVT>
class QxtPrivateInterface
{
    friend class QxtPrivate<PUB>;

public:
    QxtPrivateInterface()
        : pvt(new PVT)
    {
    }
    ~QxtPrivateInterface() { delete pvt; }

    inline void setPublic(PUB* pub) { pvt->QXT_setPublic(pub); }
    inline PVT& operator()() { return *static_cast<PVT*>(pvt); }
    inline const PVT& operator()() const { return *static_cast<PVT*>(pvt); }
    inline PVT* operator->() { return static_cast<PVT*>(pvt); }
    inline const PVT* operator->() const { return static_cast<PVT*>(pvt); }

private:
    Q_DISABLE_COPY(QxtPrivateInterface)
    QxtPrivate<PUB>* pvt;
};

#endif   // QXTPRIVATE_H
