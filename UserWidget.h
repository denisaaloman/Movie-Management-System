#pragma once
#include <qtoolbar.h>
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
#include "WatchListWindow.h"

class UserWidget : public QWidget
{
	Q_OBJECT
private:
	Service& service;
    AdminWidget* admin;
    BarChartScreen* barchartscreen;
	QPushButton* filterByGenre, * deleteMovieFromWatchListButton,
		* nextMovie, * addMovieToWatchList, * giveLike, * exportWatchlist;
	QLineEdit* moviegenrefilter, * titleinput, * genreinput, * indexOfMoveiInWatchList,
		* yearinput;
    WatchListWindow* watchlistwindow;
public:
    UserWidget(Service& service, QWidget* parent = nullptr, BarChartScreen* barchartscreen=nullptr, AdminWidget* admin=nullptr, WatchListWindow* watchlistwindow=nullptr);
    ~UserWidget();
    QHBoxLayout* createGenreGroupFilter();
    QVBoxLayout* createDeleteMovieFromWatchListLayout();
    QHBoxLayout* createMovieTitleLayout();
    QHBoxLayout* createMovieGenreLayout();
    QHBoxLayout* createMovieYearLayout();
    QHBoxLayout* createButtonsLayout();
    QVBoxLayout* createWatchListInformation(QHBoxLayout* movieGenreGroupFilter, QVBoxLayout* deleteMovieFromWatchlistLayout);
    void connectSignalsAndSlots();
public slots:
    void filterMoviesByGenreHandler();
    void deleteMovieFromWatchlistByTitleHandler();
    void addMovieToWatchListHandler();
    void nextMovieHandler();
    void exportWatchlistHandler();
    void likeMovieAndDeleteIt();
signals:
    void watchlistupdated();




};