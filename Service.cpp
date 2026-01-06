#include "Service.h"

Service::Service()
{
    this->repository = nullptr;
    this->adminCredentials = false;
    this->watchList = nullptr;
    this->action_undo = std::vector<std::shared_ptr<Action>>();
    this->action_redo = std::vector<std::shared_ptr<Action>>();
}

Service::~Service()
{
    delete this->watchList;
    delete this->repository;
}

bool Service::compareStrings(const std::string& firstComparedString, const std::string& secondComparedString) const
{
    if (firstComparedString.size() != secondComparedString.size())
        return false;
    for (int index = 0; firstComparedString[index]; ++index)
        if (tolower(firstComparedString[index]) != tolower(secondComparedString[index]))
            return false;
    return true;
}

void Service::setAdminCredentials(bool adminCredentials)
{
    this->adminCredentials = adminCredentials;
}

bool Service::getAdminCredentials()
{
    return this->adminCredentials;
}

void Service::addMovie(Movie movie)
{
    // If the movie is already in the repository, we don't add it
    if (this->repository->validatePosition(this->repository->returnPosition(movie)))
        throw DuplicateMovieException();
    std::shared_ptr<AddAction>action(new AddAction(this->repository, movie));
    this->action_undo.push_back(std::move(action));
    this->action_redo.clear();
    this->repository->addMovie(movie);
}

void Service::removeMovieByPosition(int positionOfMovie)
{
    if (!this->repository->validatePosition(positionOfMovie))
        throw InvalidPositionException();
    Movie mov = this->repository->getMovies()[positionOfMovie];
    std::shared_ptr<RemoveAction>action(new RemoveAction(this->repository, mov));
    this->action_undo.push_back(std::move(action));
    this->action_redo.clear();
    this->repository->removeMovieByPosition(positionOfMovie);
}

void Service::updateMovie(int position, Movie movie)
{
    if (!this->repository->validatePosition(position))
        throw InvalidPositionException();

    if (this->repository->returnPosition(movie) != Positions::INDEX_NOT_FOUND)
        throw DuplicateMovieException();
    Movie oldmovie = this->repository->getMovieByPosition(position);
    std::shared_ptr<UpdateAction>action(new UpdateAction(this->repository, movie,oldmovie,position));
    this->action_undo.push_back(std::move(action));
    this->action_redo.clear();

    this->repository->updateMovie(position, movie);
}

std::vector<Movie> Service::getMovies() const
{
    return this->repository->getMovies();
}

int Service::getMoviesCount() const
{
    return this->repository->getMoviesCount();
}

Movie Service::getMovieByPosition(int position) const
{
    if (!this->repository->validatePosition(position))
        throw InvalidPositionException();
    return this->repository->getMovieByPosition(position);
}


std::vector<Movie> Service::getMoviesByGenre(const std::string& genreOfMovie) {
    this->moviesFilteredByGenre.clear();
    this->currentIndexForMovieToBeDisplayed = 0;
    for (const auto& movie : this->repository->getMovies()) {
        std::string genreOfCurrentMovie = movie.getGenre();
        if (genreOfMovie.empty() || (this->compareStrings(genreOfCurrentMovie, genreOfMovie)))
            this->moviesFilteredByGenre.push_back(movie);
    }
    return this->moviesFilteredByGenre;
}
std::vector<Movie> Service::getMoviesFromWatchList() const
{
    return this->watchList->getMovies();
}

void Service::openTrailerOfMovieInBrowser(int positionOfMovieInTheDynamicArray)
{
    Movie movieWhoseTrailerWillBeDisplayedInTheBrowser = this->getMovieByPosition(positionOfMovieInTheDynamicArray);
    movieWhoseTrailerWillBeDisplayedInTheBrowser.openTrailerInBrowser();
}

void Service::addMovieToWatchList(Movie movieToAddToWatchList)
{
    this->watchList->addMovie(movieToAddToWatchList);
}

void Service::increaseLikesCountOfMovie(Movie& movieForWhichTheLikesCountWillBeIncreased)
{
    int positionOfMovieThatTheUserHasJustLiked = this->repository->returnPosition(movieForWhichTheLikesCountWillBeIncreased);
    this->repository->modifyLikesCountOfMovieByPosition(positionOfMovieThatTheUserHasJustLiked, 1);
}

void Service::setTypeOfWatchList(FileWatchList* watchList)
{
    this->watchList = watchList;
}


void Service::displayPlaylist()
{
    this->watchList->openInApp();
}

void Service::checkIfMovieIsAlreadyInThePlaylist(const Movie& movie)
{
    this->watchList->checkIfMovieIsAlreadyInThePlaylist(movie);
}

void Service::setTypeOfRepository(Repository* repository)
{
    this->repository = repository;
    this->repository->loadMoviesIntoRepository();
}

void Service::clearFile(const std::string& fileName)
{
    std::ofstream file(fileName);
    file.close();
}
void Service::incrementIndexOfCurrentMovieToBeDisplayed() {
    this->currentIndexForMovieToBeDisplayed++;
}
void Service::openTrailerOfCurrentMovieInBrowser() const {
    Movie currentMovie = this->moviesFilteredByGenre[this->currentIndexForMovieToBeDisplayed];
    currentMovie.openTrailerInBrowser();
}

void Service::removeWatchedMovieFromWatchList(int positionOfMovie, bool likedMovie) {
    Movie currentMovieInWatchList = this->watchList->getMovieByPosition(positionOfMovie);
    if (likedMovie)
        for (auto movie : this->repository->getMovies())
            if (movie == currentMovieInWatchList) {
                this->increaseLikesCountOfMovie(movie);
                break;
            }
    this->watchList->removeMovieFromPlaylist(positionOfMovie, likedMovie);
}

void Service::addCurrentMovieToWatchList() {
    this->addMovieToWatchList(this->moviesFilteredByGenre[this->currentIndexForMovieToBeDisplayed]);
}

Movie Service::getCurrentMovieToBeDisplayed() {
    if (this->currentIndexForMovieToBeDisplayed == this->moviesFilteredByGenre.size())
        this->currentIndexForMovieToBeDisplayed = 0;
    this->openTrailerOfCurrentMovieInBrowser();
    return this->moviesFilteredByGenre[this->currentIndexForMovieToBeDisplayed];
}



bool Service::undo() {
    if (this->action_undo.size() == 0) {
        return false;
    }
    this->action_undo.back()->executeundo();

    this->action_redo.push_back(std::move(this->action_undo.back()));
    this->action_undo.pop_back();
    return true;
}
bool Service::redo() {
    if (this->action_redo.size() == 0) {
        return false;
    }
    this->action_redo.back()->executeredo();
    this->action_undo.push_back(std::move(this->action_redo.back()));
    this->action_redo.pop_back();

    return true;
}