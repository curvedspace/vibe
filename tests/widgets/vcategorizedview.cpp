
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
