
#include <QApplication>

#include "previewwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    PreviewWidget *preview = new PreviewWidget();
    preview->show();

    app.exec();
    return 0;
}
