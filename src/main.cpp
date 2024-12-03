#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QSettings>
#include <iostream>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  a.setOverrideCursor(Qt::BlankCursor);

  QCoreApplication::setApplicationName("Lucid Spatial Drift");
  QCoreApplication::setOrganizationName("CS 1230");
  QCoreApplication::setApplicationVersion(QT_VERSION_STR);

  QSurfaceFormat fmt;
  fmt.setVersion(4, 1);
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(fmt);

  MainWindow w;
  w.initialize();
  w.resize(QGuiApplication::primaryScreen()->availableSize());
  w.show();

  int return_val = a.exec();
  w.finish();
  return return_val;
}
