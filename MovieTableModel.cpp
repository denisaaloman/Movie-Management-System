#include "MovieTableModel.h"

MovieTableModel::MovieTableModel(QObject* parent)
    : QAbstractTableModel(parent) {
}

void MovieTableModel::setMovies(const std::vector<Movie>& newMovies) {
    beginResetModel();
    movies = newMovies;
    endResetModel();
}
void MovieTableModel::addMovie(const Movie& movie) {
    beginInsertRows(QModelIndex(), movies.size(), movies.size());
    movies.push_back(movie);
    endInsertRows();
}

void MovieTableModel::removeMovie(int index) {
    if (index < 0 || index >= movies.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    movies.erase(movies.begin() + index);
    endRemoveRows();
}

void MovieTableModel::incrementLikes(int index) {
    if (index < 0 || index >= movies.size()) return;
    movies[index].setLikesCount(movies[index].getLikesCount() + 1);
    emit dataChanged(this->index(index, 3), this->index(index, 3)); // column 3 = Likes
}


int MovieTableModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(movies.size());
}

int MovieTableModel::columnCount(const QModelIndex&) const {
    return 4;  // title, genre, year, likes
}

bool MovieTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    int row = index.row();
    Movie& movie = movies[row]; 

    switch (index.column()) {
    case 0:
        movie.setTitle(value.toString().toStdString());
        break;
    case 1:
        movie.setGenre(value.toString().toStdString());
        break;
    case 2:
        movie.setYearOfRelease(value.toInt());
        break;
    case 3:
        movie.setLikesCount(value.toInt());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, { role });
    return true;
}



QVariant MovieTableModel::data(const QModelIndex& index, int role) const {
    if (role == Qt::DisplayRole) {
        const Movie& movie = movies[index.row()];
        switch (index.column()) {
        case 0: return QString::fromStdString(movie.getTitle());
        case 1: return QString::fromStdString(movie.getGenre());
        case 2: return movie.getYearOfRelease();
        case 3: return movie.getLikesCount();
        default: return QVariant();
        }
    }
    return QVariant();
}

QVariant MovieTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "Title";
        case 1: return "Genre";
        case 2: return "Year";
        case 3: return "Likes";
        default: return QVariant();
        }
    }
    return QVariant();
}
