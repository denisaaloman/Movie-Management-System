#include "AdminWidget.h"
#include<iostream>
#include<qlineedit.h>
#include<qline.h>
#include <qtoolbar.h>
#include<qlabel.h>
#include<qboxlayout.h>
#include<qobject.h>
#include<qpushbutton.h>
#include<qmessagebox.h>
#include<qtablewidget.h>
#include<qstring.h>
#include<qstringlist.h>
#include "BarChart.h"
#include<qshortcut.h>

QHBoxLayout* AdminWidget::createButtons()
{
	QHBoxLayout* buttons = new QHBoxLayout();
	this->addButton = new QPushButton("Add Movie", this);
	this->deleteButton = new QPushButton("Delete Movie", this);
	this->updateButton = new QPushButton("Update Movie", this);
	this->undobutton = new QPushButton("Undo Action", this);
	this->redobutton = new QPushButton("Redo Action", this);
	buttons->addWidget(this->addButton);
	buttons->addWidget(this->deleteButton);
	buttons->addWidget(this->updateButton);
	buttons->addWidget(this->undobutton);
	buttons->addWidget(this->redobutton);

	return buttons;
}
QHBoxLayout* AdminWidget::createMovieTitle() {
	QHBoxLayout* movieTitle = new QHBoxLayout();
	QLabel* title= new QLabel("Title: ", this);
	this->titleInput = new QLineEdit(this);
	movieTitle->addWidget(title);
	movieTitle->addWidget(this->titleInput);
	return movieTitle;
}

QHBoxLayout* AdminWidget::createMovieGenre() {
	QHBoxLayout* genre = new QHBoxLayout();
	QLabel* moviegenre = new QLabel("Genre: ", this);
	this->genreInput = new QLineEdit(this);
	genre->addWidget(moviegenre);
	genre->addWidget(this->genreInput);
	return genre;
}
QHBoxLayout* AdminWidget::createMovieYear() {
	QHBoxLayout* year = new QHBoxLayout();
	QLabel* movieyear = new QLabel("Year: ", this);
	this->yearInput = new QLineEdit(this);
	year->addWidget(movieyear);
	year->addWidget(this->yearInput);
	return year;
}
QHBoxLayout* AdminWidget::createMovieLikes() {
	QHBoxLayout* likes = new QHBoxLayout();
	QLabel* movielikes = new QLabel("Number of Likes: ", this);
	this->likesInput = new QLineEdit(this);
	likes->addWidget(movielikes);
	likes->addWidget(this->likesInput);
	return likes;
}

QHBoxLayout* AdminWidget::createMovieTrailer() {
	QHBoxLayout* trailer = new QHBoxLayout();
	QLabel* movietrailer = new QLabel("Trailer: ", this);
	this->trailerInput= new QLineEdit(this);
	trailer->addWidget(movietrailer);
	trailer->addWidget(this->trailerInput);
	return trailer;
}

AdminWidget::AdminWidget(Service& service, QWidget* parent, BarChartScreen* barchartscreen) : QWidget(parent), service(service), barchartscreen(barchartscreen) {
	
	
	QHBoxLayout* adminLayout = new QHBoxLayout;
	QVBoxLayout* movieinfo = new QVBoxLayout;

	QHBoxLayout* buttons = this->createButtons();
	QHBoxLayout* movietitle = this->createMovieTitle();
	QHBoxLayout* moviegenre = this->createMovieGenre();
	QHBoxLayout* movieyear = this->createMovieYear();
	QHBoxLayout* movieLikes = this->createMovieLikes();
	QHBoxLayout* movieTrailer = this->createMovieTrailer();

	movieinfo->setSpacing(2);
	movieinfo->addLayout(movietitle);
	movieinfo->addLayout(moviegenre);
	movieinfo->addLayout(movieyear);
	movieinfo->addLayout(movieLikes);
	movieinfo->addLayout(movieTrailer);

	movieinfo->addLayout(buttons);
	this->createTableWidget();
	adminLayout->addWidget(this->movieTable);
	adminLayout->addLayout(movieinfo);

	this->setLayout(adminLayout);

	QObject::connect(this->addButton, &QPushButton::clicked, this, &AdminWidget::addMovie);
	QObject::connect(this->movieTable, &QTableWidget::itemSelectionChanged, this, &AdminWidget::displayCurrentSelectedMovie);
	QObject::connect(this->deleteButton, &QPushButton::clicked, this, &AdminWidget::deleteMovie);
	QObject::connect(this->updateButton, &QPushButton::clicked, this, &AdminWidget::updateMovie);
	QObject::connect(this->undobutton, &QPushButton::clicked, this, &AdminWidget::undoAction);
	QObject::connect(this->redobutton, &QPushButton::clicked, this, &AdminWidget::redoAction);

	QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
	connect(undoShortcut, &QShortcut::activated, this, &AdminWidget::undoAction);
	QShortcut* redoshortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
	connect(redoshortcut, &QShortcut::activated, this, &AdminWidget::redoAction);


}
AdminWidget::~AdminWidget(){}

void AdminWidget::createTableWidget() {
	movieTable = new QTableWidget();
	QStringList header = { "Title","Genre","Year of Release","Number of Likes","Trailer" };
	movieTable->setColumnCount(5);
	movieTable->setColumnWidth(0, 100);
	movieTable->setHorizontalHeaderLabels(header);
	for (const auto& movie : this->service.getMovies()) {
		this->addMovieToTable(movie);
	}
	
}

void AdminWidget::addMovieToTable(const Movie& movie) const{
	int rowCount = this->movieTable->rowCount();
	this->movieTable->insertRow(rowCount);
	movieTable->setItem(rowCount, 0, new QTableWidgetItem(QString::fromStdString(movie.getTitle())));
	movieTable->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(movie.getGenre())));
	movieTable->setItem(rowCount, 2, new QTableWidgetItem(QString::number(movie.getYearOfRelease())));
	movieTable->setItem(rowCount, 3, new QTableWidgetItem(QString::number(movie.getLikesCount())));
	movieTable->setItem(rowCount, 4, new QTableWidgetItem(QString::fromStdString(movie.getTrailer())));



}
void AdminWidget::addMovie() {
	std::string title = this->titleInput->text().toStdString();
	std::string genre = this->genreInput->text().toStdString();
	int year = this->yearInput->text().toInt();
	int likes = this->likesInput->text().toInt();
	std::string trailer = this->trailerInput->text().toStdString();
	try {
		MovieValidator::validateMovieIdentifiers(title, genre, std::to_string(year), trailer, std::to_string(likes));
		Movie movie = Movie(title, genre, trailer, year, likes);
		this->service.addMovie(movie);
		this->addMovieToTable(movie);
		this->titleInput->clear();
		this->genreInput->clear();
		this->yearInput->clear();
		this->likesInput->clear();
		this->trailerInput->clear();
		this->barchartscreen->refreshChart();
		

	}
	catch (MovieExceptions& exception) {
		QMessageBox::critical(this, "Error", "Invalid Fields");
	}
	catch (DuplicateMovieException& exception) {
		QMessageBox::critical(this, "Error", exception.what());
	}


}
void AdminWidget::deleteMovie() {
	int selectedRow = this->movieTable->currentRow();
	try {
		this->service.removeMovieByPosition(selectedRow);
		this->barchartscreen->refreshChart();
	}
	catch (InvalidPositionException& exception) {
		QMessageBox::critical(this, "Error", exception.what());
	}
	this->movieTable->removeRow(selectedRow);

}
void AdminWidget::updateMovie() {

	int selectedRow = this->movieTable->currentRow();
	if (selectedRow == -1) {
		return;
	}
	std::string title = this->titleInput->text().toStdString();
	std::string genre = this->genreInput->text().toStdString();
	int year = this->yearInput->text().toInt();
	int likes = this->likesInput->text().toInt();
	std::string trailer = this->trailerInput->text().toStdString();
	
	try {
		MovieValidator::validateMovieIdentifiers(title, genre, std::to_string(year), trailer, std::to_string(likes));
		Movie movie = Movie(title, genre, trailer, year, likes);
		this->service.updateMovie(selectedRow, movie);
		this->movieTable->setItem(selectedRow, 0, new QTableWidgetItem(QString::fromStdString(title)));
		this->movieTable->setItem(selectedRow, 1, new QTableWidgetItem(QString::fromStdString(genre)));
		this->movieTable->setItem(selectedRow, 2, new QTableWidgetItem(QString::number(year)));
		this->movieTable->setItem(selectedRow, 3, new QTableWidgetItem(QString::number(likes)));
		this->movieTable->setItem(selectedRow, 4, new QTableWidgetItem(QString::fromStdString(trailer)));
		this->barchartscreen->refreshChart();
	}
	catch (MovieExceptions& exception) {
		QMessageBox::critical(this, "Error", "Invalid Fields");
	}
	catch (InvalidPositionException& exception) {
		QMessageBox::critical(this, "Error", exception.what());
	}
	catch (DuplicateMovieException& exception) {
		QMessageBox::critical(this, "Error", exception.what());
	}
}
void AdminWidget::displayCurrentSelectedMovie(){

	int selectedRow = this->movieTable->currentRow();
	if (selectedRow == -1) {
		return;
	}

	std::string title = this->movieTable->item(selectedRow, 0)->text().toStdString();
	std::string genre = this->movieTable->item(selectedRow, 1)->text().toStdString();
	int year = this->movieTable->item(selectedRow, 2)->text().toInt();
	int likes = this->movieTable->item(selectedRow, 3)->text().toInt();
	std::string trailer = this->movieTable->item(selectedRow, 4)->text().toStdString();

	this->titleInput->setText(QString::fromStdString(title));
	this->genreInput->setText(QString::fromStdString(genre));
	this->yearInput->setText(QString::number(year));
	this->likesInput->setText(QString::number(likes));
	this->trailerInput->setText(QString::fromStdString(trailer));


}
void AdminWidget::refreshMovieTable() {
	this->movieTable->setRowCount(0); //clearing the table
	for (const auto& movie : this->service.getMovies()) {
		this->addMovieToTable(movie);
	}
}
void AdminWidget::undoAction() {
	if (this->service.undo() == false) {
		QMessageBox::critical(this, "Error", "No more undos available");
	}
	this->refreshMovieTable();
}
void AdminWidget::redoAction() {
	if (this->service.redo() == false) {
		QMessageBox::critical(this, "Error", "No more redos available");

	}
	this->refreshMovieTable();
}




