#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "Movie.h"  

class MovieTableModel : public QAbstractTableModel {
    Q_OBJECT

private:
    std::vector<Movie> movies;

public:
    MovieTableModel(QObject* parent = nullptr);

    void setMovies(const std::vector<Movie>& movies);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void addMovie(const Movie& movie);
    void removeMovie(int);
    void incrementLikes(int);
};
