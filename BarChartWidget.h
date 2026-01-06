#pragma once
#include <QWidget>
#include <vector>
#include <QString>

class BarChartWidget : public QWidget {
    Q_OBJECT

public:
    BarChartWidget(QWidget* parent = nullptr);
    void setData(const std::vector<std::pair<QString, int>>& movieLikes);
    void updateHeight();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<std::pair<QString, int>> data;
};

