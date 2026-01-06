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
#include "BarChart.h"


class AdminWidget : public QWidget {
	Q_OBJECT
private:
	Service& service;
	BarChartScreen* barchartscreen;
	
public:
	
	AdminWidget(Service& service,QWidget* parent = nullptr, BarChartScreen* barchartscreen=nullptr);
	~AdminWidget();
	
	QPushButton* addButton, * deleteButton, * updateButton, * undobutton, *redobutton;
	QLineEdit* titleInput, *genreInput, *yearInput, *likesInput, *trailerInput;
	
	QTableWidget* movieTable;
	void createTableWidget();

	QHBoxLayout* createButtons();
	QHBoxLayout* createMovieTitle();
	QHBoxLayout* createMovieGenre();
	QHBoxLayout* createMovieYear();
	QHBoxLayout* createMovieLikes();
	QHBoxLayout* createMovieTrailer();
	
public slots:
	void addMovie();
	void addMovieToTable(const Movie& movie) const;
	void deleteMovie();
	void updateMovie();
	void displayCurrentSelectedMovie();
	void refreshMovieTable();
	void undoAction();
	void redoAction();


};