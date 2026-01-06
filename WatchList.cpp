#include "WatchList.h"
#include "Exceptions.h"

void WatchList::addMovie(const Movie& movie)
{
    this->movies.push_back(movie);
}

bool WatchList::isEmpty()
{
    return this->movies.size() == 0;
}

void WatchList::checkIfMovieIsAlreadyInThePlaylist(const Movie& movie)
{
    for (auto movieFromPlaylist : this->movies)
        if (movieFromPlaylist.getTitle() == movie.getTitle())
            throw DuplicateMovieException();
}

void WatchList::removeMovieFromPlaylist(int positionOfMovie, bool likedmovie)
{
    std::vector<Movie>::iterator iteratorPointingToMovieThatWillBeErased = this->movies.begin();
    std::advance(iteratorPointingToMovieThatWillBeErased, positionOfMovie); //moves iterator with positionOfMovie steps 
    
    if (likedmovie == true) {
        this->movies[positionOfMovie].setLikesCount(this->movies[positionOfMovie].getLikesCount() + 1);
    }
    if (iteratorPointingToMovieThatWillBeErased != this->movies.end())
        this->movies.erase(iteratorPointingToMovieThatWillBeErased);
}
Movie WatchList::getMovieByPosition(int pos){
    return this->movies[pos];
}