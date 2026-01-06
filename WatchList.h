#pragma once
#include "Movie.h"

class WatchList
{
protected:
	std::vector<Movie> movies;

public:
	WatchList() {};
	virtual ~WatchList() {}
	void addMovie(const Movie&);
	void removeMovieFromPlaylist(int,bool);
	bool isEmpty();
	void checkIfMovieIsAlreadyInThePlaylist(const Movie&);
	Movie getMovieByPosition(int pos);
	std::vector<Movie> getMovies() const { return this->movies; }
};