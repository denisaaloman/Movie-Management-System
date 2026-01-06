#pragma once
#include "Exceptions.h"
#include "CSVWatchList.h"
#include "FileRepository.h"
#include "HTMLWatchList.h"
#include <ctime>
#include "Repository.h"
#include "MovieValidator.h"
#include "Action.h"
#include "WatchList.h"
#include "FileWatchList.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <string>
#include <vector>
#include <initializer_list>
#include<memory>


class Service
{
private:
	Repository* repository;
	bool adminCredentials;
	FileWatchList* watchList;
	std::vector<int> indexesOfMoviesThatTheUserLikes;
	int currentIndexForMovieToBeDisplayed = 0;
	std::vector<Movie> moviesFilteredByGenre;
	std::vector<std::shared_ptr<Action>> action_undo;
	std::vector<std::shared_ptr<Action>> action_redo;

public:
	Service();
	~Service();
	void openTrailerOfMovieInBrowser(int);
	bool compareStrings(const std::string&, const std::string&) const;
	void addMovie(Movie);
	void removeMovieByPosition(int);
	void updateMovie(int, Movie);
	std::vector<Movie> getMovies() const;
	int getMoviesCount() const;
	Movie getMovieByPosition(int) const;
	void setAdminCredentials(bool);
	bool getAdminCredentials();
	std::vector<Movie> getMoviesByGenre(const std::string&);
	std::vector<Movie> getMoviesFromWatchList() const;
	void removeWatchedMovieFromWatchList(int,bool);
	void addMovieToWatchList(Movie);
	void increaseLikesCountOfMovie(Movie&);
	void setTypeOfWatchList(FileWatchList*);
	void setTypeOfRepository(Repository*);
	void displayPlaylist();
	void checkIfMovieIsAlreadyInThePlaylist(const Movie&);
	void clearFile(const std::string&);
	void incrementIndexOfCurrentMovieToBeDisplayed();
	void openTrailerOfCurrentMovieInBrowser() const;
	void addCurrentMovieToWatchList();
	Movie getCurrentMovieToBeDisplayed();
	bool undo();
	bool redo();
};