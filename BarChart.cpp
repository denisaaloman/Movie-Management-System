#include "BarChart.h"
#include <QVBoxLayout>
#include <QScrollArea>

BarChartScreen::BarChartScreen(Service& service, QWidget* parent)
    : QWidget(parent), service(service) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    chartWidget = new BarChartWidget(this);

    std::vector<std::pair<QString, int>> chartData;
    for (const auto& movie : service.getMovies()) {
        chartData.emplace_back(QString::fromStdString(movie.getTitle()), movie.getLikesCount());
    }
    chartWidget->setData(chartData);

    // creating scroll area and putting chartWidget inside it
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidget(chartWidget);
    scrollArea->setWidgetResizable(true);

    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

void BarChartScreen::refreshChart() {
    std::vector<std::pair<QString, int>> chartData;
    for (const auto& movie : service.getMovies()) {
        chartData.emplace_back(QString::fromStdString(movie.getTitle()), movie.getLikesCount());
    }
    chartWidget->setData(chartData);
}
