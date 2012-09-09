/****************************************************************************
 * This file is part of Vibe.
 *
 * Copyright (c) 2012 Pier Luigi Fiorini
 *
 * Author(s):
 *    Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:BSD$
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the Hawaii Project nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Pier Luigi Fiorini BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $END_LICENSE$
 */

#include <QApplication>
#include <QMainWindow>
#include <QStringListModel>

#include <VibeWidgets/VCategorizedView>
#include <VibeWidgets/VCategoryDrawer>
#include <VibeWidgets/VCategorizedSortFilterProxyModel>

class MyModel : public QStringListModel
{
public:
    MyModel() {
        m_icons << "system-file-manager" << "spotify" << "steam" << "blender";
    }

    virtual QVariant data(const QModelIndex &index, int role) const {
        switch (role) {
            case VCategorizedSortFilterProxyModel::CategoryDisplayRole: {
                return QString::number(index.row() / 10);
            }
            case VCategorizedSortFilterProxyModel::CategorySortRole: {
                return index.row() / 10;
            }
            case Qt::DecorationRole:
                return QIcon::fromTheme(m_icons[index.row() % 4]);
            default:
                break;
        }

        return QStringListModel::data(index, role);
    }

private:
    QStringList m_icons;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->setMinimumSize(640, 480);

    VCategorizedView *view = new VCategorizedView();
    view->setCategoryDrawer(new VCategoryDrawer());
    view->setViewMode(QListView::IconMode);

    MyModel *model = new MyModel();
    model->insertRows(0, 100);
    for (int i = 0; i < 100; ++i)
        model->setData(model->index(i, 0), QString::number(i), Qt::DisplayRole);

    VCategorizedSortFilterProxyModel *proxyModel = new VCategorizedSortFilterProxyModel();
    proxyModel->setCategorizedModel(true);
    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);

    mainWindow->setCentralWidget(view);

    mainWindow->show();

    return app.exec();
}
