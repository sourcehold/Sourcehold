/*
 * Asset manager tool, generates header files
 * to make asset management easier
 */

#include <QtCore>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QMenu>

#include <iostream>
#include <fstream>
#include <cinttypes>

#include <MainWindow.hh>

int main(int argc, char *argv[]) {
    QApplication app (argc, argv);

    MainWindow win;
    win.show();

    return app.exec();
}
