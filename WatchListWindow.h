#pragma once

#include <QWidget>
#include <QTableView>
#include "MovieTableModel.h"

class WatchListWindow : public QWidget {
    Q_OBJECT

private:
    QTableView* tableView;
    MovieTableModel* model;

public:
    WatchListWindow(QWidget* parent = nullptr);
    void setMovies(const std::vector<Movie>& movies);
    void addMovieToWatchlist(const Movie& movie);
    void removeMovieFromWatchlist(int index);
    void likeMovie(int index);
public slots:
    void reloadMovies(const std::vector<Movie>& updatedMovies) {
        model->setMovies(updatedMovies);
    }
};

