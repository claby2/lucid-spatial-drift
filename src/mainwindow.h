#pragma once

#include "realtime.h"
#include "utils/aspectratiowidget/aspectratiowidget.hpp"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>

class MainWindow : public QWidget {
  Q_OBJECT

public:
  void initialize();
  void finish();

private:
  Realtime *realtime;
  AspectRatioWidget *aspectRatioWidget;
};
