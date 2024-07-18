#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_progressBar_cell1_valueChanged(int value)
{

}


void MainWindow::on_progressBar_cell2_valueChanged(int value)
{

}


void MainWindow::on_progressBar_cell3_valueChanged(int value)
{

}


void MainWindow::on_progressBar_cell4_valueChanged(int value)
{

}


void MainWindow::on_progressBar_cell5_valueChanged(int value)
{

}


void MainWindow::on_progressBar_cell6_valueChanged(int value)
{

}

