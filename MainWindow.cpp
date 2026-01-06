#include "MainWindow.h"
#include "AdminWidget.h"
#include "UserWidget.h"
#include "Service.h"
#include<iostream>
#include <qtoolbar.h>
#include<qlabel.h>
#include<qboxlayout.h>
#include<qpushbutton.h>
#include<qmessagebox.h>
#include<qstackedlayout.h>
#include "BarChart.h"


MainWindow::MainWindow(Service& service,QWidget *parent)
    : service{ service },QMainWindow(parent)
{
    try {
        this->service.setTypeOfRepository(new FileRepository{ "movies.txt" });
    }
    catch (FileException& exception) {
        std::cout << exception.what() << std::endl;
    }
    this->resize(1000, 300);
    this->watchListWindow = new WatchListWindow();

    QWidget* central = new QWidget(this); //cetral widget and the set on it, do not set directly on main window
    this->setCentralWidget(central);
    QWidget* selectionpage = new QWidget();
    QVBoxLayout* selectionLayout = new QVBoxLayout(selectionpage);
    QLabel* label = new QLabel("Welcome to Movie Management App!\n You must first choose the type of the watch list: ");
    label->setAlignment(Qt::AlignCenter);
    //layout->addWidget(label);
   
   
    QPushButton* csvbutton = new QPushButton("CSV",this);
    QPushButton* htmlbutton = new QPushButton("HTML",this);
    selectionLayout->addWidget(label);
    selectionLayout->addWidget(csvbutton);
    selectionLayout->addWidget(htmlbutton);
    selectionpage->setLayout(selectionLayout);
   
    
    QToolBar* toolbar = new QToolBar(this);
    addToolBar(toolbar);
    QAction* AdminAction = new QAction("Admin Mode", this);
    QAction* UserAction = new QAction("User Mode", this);
    QAction* ChartAction = new QAction("Bar Chart", this);
    toolbar->addAction(AdminAction);
    toolbar->addAction(UserAction);
    toolbar->addAction(ChartAction);
    AdminAction->setEnabled(false);
    UserAction->setEnabled(false);
    ChartAction->setEnabled(false);

    this->setWindowTitle("Movie App");

    QStackedLayout* stackedLayout = new QStackedLayout();
    auto* chartwidget = new BarChartScreen(this->service);

    auto* adminwidget = new AdminWidget(this->service, this, chartwidget);
    auto* userwidget = new UserWidget(this->service, this, chartwidget, adminwidget, this->watchListWindow);
    
    stackedLayout->addWidget(selectionpage);
    stackedLayout->addWidget(adminwidget);
    stackedLayout->addWidget(userwidget);
    stackedLayout->addWidget(chartwidget);
    central->setLayout(stackedLayout);
  
    connect(csvbutton, &QPushButton::clicked, this, [this,AdminAction, UserAction, ChartAction]() {
       
        this->service.setTypeOfWatchList(new CSVWatchList("CSVWatchList.csv"));
        QMessageBox::information(this, "Just an info...","Watch List set to CSV! You may close this window and choose the credentials");
        AdminAction->setEnabled(true);
        UserAction->setEnabled(true);
        ChartAction->setEnabled(true);
        this->openWatchListWindow();
        
        });
    connect(htmlbutton, &QPushButton::clicked, this, [this, AdminAction, UserAction, ChartAction]() {
        this->service.setTypeOfWatchList(new HTMLWatchList("HTMLWatchList.html"));
        QMessageBox::information(this, "Just an info...", "Watch List set to HTML! You may close this window and choose the credentials");
        AdminAction->setEnabled(true);
        UserAction->setEnabled(true);
        ChartAction->setEnabled(true);
        this->openWatchListWindow();
        
        });

   
    connect(userwidget, &UserWidget::watchlistupdated, this->watchListWindow, [this]() {
        this->watchListWindow->setMovies(this->service.getMoviesFromWatchList());
        });



    
    connect(AdminAction, &QAction::triggered, this, [this,stackedLayout]() {

        this->resize(900, 600);
        stackedLayout->setCurrentIndex(1);

        });
    connect(UserAction, &QAction::triggered, this, [this, stackedLayout]() {
        this->resize(900, 600);
        stackedLayout->setCurrentIndex(2);


        });
    connect(ChartAction, &QAction::triggered, this, [this, stackedLayout]() {
        //this->resize(900, 600);
        stackedLayout->setCurrentIndex(3);
        });
   
  


    
}

MainWindow::~MainWindow()
{}

void MainWindow::openWatchListWindow() {
    this->watchListWindow->setMovies(this->service.getMoviesFromWatchList());
    this->watchListWindow->show();
}

