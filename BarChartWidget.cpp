#include "BarChartWidget.h"
#include <QPainter>
#include <QPaintEvent>

BarChartWidget::BarChartWidget(QWidget* parent)
    : QWidget(parent) {
    this->setMinimumHeight(250);
}

void BarChartWidget::setData(const std::vector<std::pair<QString, int>>& movieLikes) {
    this->data = movieLikes;
    updateHeight();
    update();  
}
void BarChartWidget::updateHeight() {
    int barHeight = 30;
    int spacing = 20;
    int topMargin = 30;
    int totalHeight = topMargin + data.size() * (barHeight + spacing) + 30; 
    setMinimumHeight(totalHeight);
    setMaximumHeight(totalHeight);
}


void BarChartWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int leftMargin = 300;
    int topMargin = 30;
    int barHeight = 30;
    int spacing = 20;
    int chartWidth = width() - leftMargin - 30;
    
    // Determine max number of likes
    int maxLikes = 1;
    for (const auto& pair : data)
        maxLikes = std::max(maxLikes, pair.second);

    int y = topMargin;

    for (const auto& pairs : data) {

        int barWidth = (pairs.second * chartWidth) / maxLikes; //likes to be proportional to bar
        painter.drawText(10, y + barHeight / 2 + 5, pairs.first); //title as y-axis

        // Draw bar
        QRect barRect(leftMargin, y, barWidth, barHeight);
        painter.setBrush(Qt::blue);
        painter.setPen(Qt::black);
        painter.drawRect(barRect);

        painter.drawText(leftMargin + barWidth + 5, y + barHeight / 2 + 5, QString::number(pairs.second)); //likes at the end of bar
        y += barHeight + spacing;
    }

   
    painter.drawLine(leftMargin, topMargin - 10, leftMargin, y);
}

