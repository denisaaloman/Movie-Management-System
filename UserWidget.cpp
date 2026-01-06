#include"UserWidget.h"
#include <qtoolbar.h>
#include<qlineedit.h>
#include<qlabel.h>
#include<qboxlayout.h>
#include<qpushbutton.h>
#include<qmessagebox.h>
#include<qwidget.h>
#include<qtablewidget.h>
#include<vector>
#include "Service.h"
#include "Movie.h"
#include "FileWatchList.h"
#include "CSVWatchList.h"
#include "HTMLWatchList.h"
#include "BarChart.h"
#include "AdminWidget.h"


QHBoxLayout* UserWidget::createGenreGroupFilter() {
	QHBoxLayout* movieGenreGroupFilter = new QHBoxLayout();
	QLabel* moviegenreFilter = new QLabel("Filter movies by genre", this);
	this->moviegenrefilter = new QLineEdit(this);
	this->filterByGenre = new QPushButton("Fetch Data", this);
	movieGenreGroupFilter->addWidget(moviegenreFilter);
	movieGenreGroupFilter->addWidget(this->moviegenrefilter);
	movieGenreGroupFilter->addWidget(filterByGenre);
	return movieGenreGroupFilter;

}
QVBoxLayout* UserWidget::createDeleteMovieFromWatchListLayout() {
	auto* deleteMovieFromWatchlistLayout = new QVBoxLayout;
	auto* deleteMovieFromWatchlist = new QLabel("Enter the index from the watchlist of the movie you want to delete",
		this);
	this->indexOfMoveiInWatchList = new QLineEdit(this);

	this->deleteMovieFromWatchListButton = new QPushButton("Remove from watchlist", this);
	this->giveLike = new QPushButton("Give Like and Delete Movie From Watchlist", this);

	deleteMovieFromWatchlistLayout->addWidget(deleteMovieFromWatchlist);
	deleteMovieFromWatchlistLayout->addWidget(this->indexOfMoveiInWatchList);

	auto* layoutButtons = new QHBoxLayout;
	layoutButtons->addWidget(giveLike);
	layoutButtons->addWidget(deleteMovieFromWatchListButton);

	deleteMovieFromWatchlistLayout->addLayout(layoutButtons);

	return deleteMovieFromWatchlistLayout;
}
QHBoxLayout* UserWidget::createMovieTitleLayout() {
	auto* movieTitle = new QLabel("Title of Movie", this);
	this->titleinput = new QLineEdit(this);
	auto* movieTitleGroup = new QHBoxLayout(this);
	movieTitleGroup->addWidget(movieTitle);
	movieTitleGroup->addWidget(this->titleinput);
	return movieTitleGroup;
}

QHBoxLayout* UserWidget::createMovieGenreLayout() {
	auto* movieGenre = new QLabel("Genre of Movie", this);
	this->genreinput = new QLineEdit(this);
	auto* movieGenreGroup = new QHBoxLayout;
	movieGenreGroup->addWidget(movieGenre);
	movieGenreGroup->addWidget(this->genreinput);

	return movieGenreGroup;
}

QHBoxLayout* UserWidget::createMovieYearLayout() {
	auto* movieYear = new QLabel("Year of Release", this);
	this->yearinput = new QLineEdit(this);
	auto* movieYearGroup = new QHBoxLayout;
	movieYearGroup->addWidget(movieYear);
	movieYearGroup->addWidget(yearinput);

	return movieYearGroup;
}
QHBoxLayout* UserWidget::createButtonsLayout() {
	auto* buttons = new QHBoxLayout;
	this->addMovieToWatchList = new QPushButton("Add movie to your watchlist", this);
	this->nextMovie = new QPushButton("Show next movie", this);
	this->exportWatchlist = new QPushButton("Export your watchlist", this);
	buttons->addWidget(addMovieToWatchList);
	buttons->addWidget(nextMovie);
	buttons->addWidget(exportWatchlist);

	return buttons;
}
QVBoxLayout* UserWidget::createWatchListInformation(QHBoxLayout* movieGenreGroupFilter,
	QVBoxLayout* deleteMovieFromWatchlistLayout) {
	auto* movieInformation = new QVBoxLayout;
	auto* buttons = createButtonsLayout();
	auto* movieTitleGroup = createMovieTitleLayout();
	auto* movieGenreGroup = createMovieGenreLayout();
	auto* movieYearGroup = createMovieYearLayout();

	movieInformation->setSpacing(2);
	movieInformation->addLayout(movieGenreGroupFilter);
	movieInformation->addLayout(movieTitleGroup);
	movieInformation->addLayout(movieGenreGroup);
	movieInformation->addLayout(movieYearGroup);
	movieInformation->addLayout(buttons);
	movieInformation->addLayout(deleteMovieFromWatchlistLayout);

	return movieInformation;
}
UserWidget::UserWidget(Service& service, QWidget* parent, BarChartScreen* barchartscreen, AdminWidget* admin, WatchListWindow* watchlistwindow) : service{ service }, QWidget(parent), barchartscreen(barchartscreen), admin(admin), watchlistwindow{ watchlistwindow } {
	
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	auto* moviegenregroupfilter = this->createGenreGroupFilter();
	mainLayout->addLayout(moviegenregroupfilter);

	auto* deletemoviefromwatchlistlayout = this->createDeleteMovieFromWatchListLayout();
	auto* watchListInfo = this->createWatchListInformation(moviegenregroupfilter, deletemoviefromwatchlistlayout);
	mainLayout->addLayout(watchListInfo);
	
	
	this->setLayout(mainLayout);
	//this->setMinimumSize(600, 400);
	this->connectSignalsAndSlots();

}

void UserWidget::filterMoviesByGenreHandler(){
	this->titleinput->setText("");
	this->genreinput->setText("");
	this->yearinput->setText("");

	auto v = this->service.getMoviesByGenre(this->moviegenrefilter->text().toStdString());
	if (!v.empty()) {
		
		Movie movieToBeDisplayed = this->service.getCurrentMovieToBeDisplayed();
		this->titleinput->setText(QString::fromStdString(movieToBeDisplayed.getTitle()));
		this->genreinput->setText(QString::fromStdString(movieToBeDisplayed.getGenre()));
		this->yearinput->setText(QString::fromStdString(std::to_string(movieToBeDisplayed.getYearOfRelease())));
	}
	else
		QMessageBox::information(this, "Info", "There are no movies with the given genre!");
}
void UserWidget::deleteMovieFromWatchlistByTitleHandler() {

	int indexOfMovieToBeDeleted = this->indexOfMoveiInWatchList->text().toInt();

	if (indexOfMovieToBeDeleted == 0 && this->indexOfMoveiInWatchList->text().toStdString() != "0") {
		QMessageBox::information(this, "Info", "Introduce a valid position!");
		return;
	}

	try {
		this->service.removeWatchedMovieFromWatchList(indexOfMovieToBeDeleted, false);
		emit watchlistupdated();
		QMessageBox::information(this, "Info", "Movie deleted from watchlist!");
	}
	catch (InvalidPositionException& e) {
		QMessageBox::information(this, "Info", "Invalid position");
		return;
	}
}
void UserWidget::addMovieToWatchListHandler() {
	if (this->titleinput->text().isEmpty()) {

		QMessageBox::information(this, "Info", "You must introduce a genre first!");
		return;
	}

	try {
		this->service.addCurrentMovieToWatchList();
		emit watchlistupdated();
		QMessageBox::information(this, "Info", "Movie added to watchlist!");
	}
	catch (DuplicateMovieException& e) {
		QMessageBox::information(this, "Info", e.what());
		return;
	}
}

void UserWidget::nextMovieHandler() {
	if (this->titleinput->text().isEmpty()) {
		QMessageBox::information(this, "Info", "You must introduce a genre first!");
		return;
	}

	this->service.incrementIndexOfCurrentMovieToBeDisplayed();

	Movie movieToBeDisplayed = this->service.getCurrentMovieToBeDisplayed();
	this->titleinput->setText(QString::fromStdString(movieToBeDisplayed.getTitle()));
	this->genreinput->setText(QString::fromStdString(movieToBeDisplayed.getGenre()));
	this->yearinput->setText(QString::fromStdString(std::to_string(movieToBeDisplayed.getYearOfRelease())));
}
void UserWidget::exportWatchlistHandler() {
	try {
		this->service.displayPlaylist();
	}
	catch (FileException& caughtException) {
		QMessageBox::information(this, "Info", caughtException.what());
		return;
	}
}

void UserWidget::likeMovieAndDeleteIt(){
	int indexOfMovieToBeDeleted = this->indexOfMoveiInWatchList->text().toInt();

	if (indexOfMovieToBeDeleted == 0 && this->indexOfMoveiInWatchList->text().toStdString() != "0") {
		QMessageBox::information(this, "Info", "Introduce a valid position!");
		return;
	}

	try {
		this->service.removeWatchedMovieFromWatchList(indexOfMovieToBeDeleted, true);
		QMessageBox::information(this, "Info", "Movie deleted from watchlist!");
		this->barchartscreen->refreshChart();
		this->admin->refreshMovieTable();
		emit watchlistupdated();
	}
	catch (InvalidPositionException& e) {
		QMessageBox::information(this, "Info", "Invalid position");
		return;
	}
}
UserWidget::~UserWidget(){}
void UserWidget::connectSignalsAndSlots() {
	QObject::connect(this->addMovieToWatchList, &QPushButton::clicked, this, &UserWidget::addMovieToWatchListHandler);
	QObject::connect(this->nextMovie, &QPushButton::clicked, this, &UserWidget::nextMovieHandler);
	QObject::connect(this->exportWatchlist, &QPushButton::clicked, this, &UserWidget::exportWatchlistHandler);
	QObject::connect(this->filterByGenre, &QPushButton::clicked, this, &UserWidget::filterMoviesByGenreHandler);
	QObject::connect(this->deleteMovieFromWatchListButton, &QPushButton::clicked, this,&UserWidget::deleteMovieFromWatchlistByTitleHandler);
	QObject::connect(this->giveLike, &QPushButton::clicked, this, &UserWidget::likeMovieAndDeleteIt);

	}
