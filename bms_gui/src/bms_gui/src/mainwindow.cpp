#include "bms_gui/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->setWindowTitle("Battery Management System");
    commNode = new Ros2Comm();
    ui->setupUi(this);
    signalMapper = new QSignalMapper(this);

}


MainWindow::~MainWindow()
{

    delete ui;
}
