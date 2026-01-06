#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Service.h"
#include "FileRepository.h"
#include "Exceptions.h"
#include "WatchListWindow.h"
#include "UserWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Service service;

public:
    MainWindow(Service& service,QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindowClass ui;
    WatchListWindow* watchListWindow;


    
    void openWatchListWindow();
    
};
