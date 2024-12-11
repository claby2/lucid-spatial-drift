#include "mainwindow.h"
#include "settings.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>
#include <iostream>
#include <QLCDNumber>

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

  QLCDNumber *lcd = new QLCDNumber(this);
  lcd->setDigitCount(6);
  lcd->display(0);
  lcd->setGeometry(50, 50, 200, 50);

  realtime->connectScoreWidget(lcd);

  realtime->settingsChanged(); // THIS IS IMPORTANT
}

void MainWindow::finish() {
  realtime->finish();
  delete (realtime);
}
