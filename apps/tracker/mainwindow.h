#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
#include <QtGui/QMainWindow>

namespace Ui {
    class MainWindow;
}

class QFileSystemModel;

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *m_model;
    QModelIndex m_prevRoot;

    void setRootPath(const QString& rootPath);

private slots:
    void viewAsIcons();
    void viewAsList();
    void viewAsTree();
    void viewAsColumns();
    void goBack();
    void goForward();
    void doubleClicked(QModelIndex);
};

#endif // MAINWINDOW_H
