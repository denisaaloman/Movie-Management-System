#pragma once
#include <QWidget>
#include "Service.h"
#include "BarChartWidget.h"

class BarChartScreen : public QWidget {
    Q_OBJECT

public:
    explicit BarChartScreen(Service& service, QWidget* parent = nullptr);
    void refreshChart();

private:
    BarChartWidget* chartWidget;
    Service& service;
};
