#include "WatchListWindow.h"
#include <QVBoxLayout>

WatchListWindow::WatchListWindow(QWidget* parent)
    : QWidget(parent), tableView(new QTableView(this)), model(new MovieTableModel(this)) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tableView);
    tableView->setModel(model);
    this->setLayout(layout);
    this->setWindowTitle("Watch List");
}

void WatchListWindow::setMovies(const std::vector<Movie>& movies) {
    model->setMovies(movies);
}
void WatchListWindow::addMovieToWatchlist(const Movie& movie) {
    model->addMovie(movie);
}

void WatchListWindow::removeMovieFromWatchlist(int index) {
    model->removeMovie(index);
}

void WatchListWindow::likeMovie(int index) {
    model->incrementLikes(index);
}


