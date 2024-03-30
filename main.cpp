#include "qtimagestitching.h"
#include <QMetaType>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
    QApplication a(argc, argv);
    QtImageStitching w;
    w.show();
    return a.exec();
}
