#include "mainwindow.h"
#include "settings.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>
#include <iostream>

void MainWindow::initialize() {
  realtime = new Realtime;
  aspectRatioWidget = new AspectRatioWidget(this);
  aspectRatioWidget->setAspectWidget(realtime, 9.f / 16.f);
  QHBoxLayout *hLayout = new QHBoxLayout;   // horizontal alignment
  QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
  vLayout->setAlignment(Qt::AlignTop);
  hLayout->addLayout(vLayout);
  hLayout->addWidget(aspectRatioWidget, 1);
  this->setLayout(hLayout);
}

void MainWindow::finish() {
  realtime->finish();
  delete (realtime);
}
