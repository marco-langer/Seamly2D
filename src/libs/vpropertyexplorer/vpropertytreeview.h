/************************************************************************
 **
 **  @file   vpropertytreeview.h
 **  @author hedgeware <internal(at)hedgeware.net>
 **  @date
 **
 **  @brief
 **  @copyright
 **  All rights reserved. This program and the accompanying materials
 **  are made available under the terms of the GNU Lesser General Public License
 **  (LGPL) version 2.1 which accompanies this distribution, and is available at
 **  http://www.gnu.org/licenses/lgpl-2.1.html
 **
 **  This library is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 **  Lesser General Public License for more details.
 **
 *************************************************************************/

#ifndef VPROPERTYTREEVIEW_H
#define VPROPERTYTREEVIEW_H

#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QTreeView>
#include <QtGlobal>
#include <qcompilerdetection.h>

namespace VPE {

class VPropertyDelegate;
class VPropertyModel;
class VPropertyTreeViewPrivate;

class VPropertyTreeView : public QTreeView
{
    Q_OBJECT
public:
    //! Default constructor
    explicit VPropertyTreeView(QWidget* parent = nullptr);

    //! The destructor, taking a model and setting it to the tree view
    //! \param model The model to set as model for this tree view
    explicit VPropertyTreeView(VPropertyModel* model, QWidget* parent = nullptr);

    //! Destructor
    virtual ~VPropertyTreeView() Q_DECL_OVERRIDE;

    //! Sets the height for each row. Set this to 0 in order to let the standard delegate decide
    void setRowHeight(int height = 0, bool add_to_standard = false);


protected:
    //! This method is called by the constructors to initialize the view
    virtual void init();

    //! protected constructor
    VPropertyTreeView(VPropertyTreeViewPrivate* d, bool init_, QWidget* parent = nullptr);

    //! The protected data
    VPropertyTreeViewPrivate* d_ptr;

private:
    Q_DISABLE_COPY(VPropertyTreeView)
};

}   // namespace VPE

#endif   // VPROPERTYTREEVIEWEEVIEW_H
