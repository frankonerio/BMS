#include "bms_gui/mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->setWindowTitle("Battery Management System");
    commNode = new Ros2Comm();
    ui->setupUi(this);
    signalMapper = new QSignalMapper(this);
    
    
    // Use a loop to set the progress bar values to 0
    for (int i = 1; i <= 9; ++i) {
        QString progressBarName = QString("progressBar_cell%1").arg(i);
        QProgressBar* progressBar = findChild<QProgressBar*>(progressBarName);
        if (progressBar) {
            progressBar->setValue(0);
        }
    }

    connect(commNode, SIGNAL(cell1_voltage(int)), this, SLOT(on_progressBar_cell1_valueChanged(int)));
    connect(commNode, SIGNAL(cell2_voltage(int)), this, SLOT(on_progressBar_cell2_valueChanged(int)));
    connect(commNode, SIGNAL(cell3_voltage(int)), this, SLOT(on_progressBar_cell3_valueChanged(int)));
    connect(commNode, SIGNAL(cell4_voltage(int)), this, SLOT(on_progressBar_cell4_valueChanged(int)));
    connect(commNode, SIGNAL(cell5_voltage(int)), this, SLOT(on_progressBar_cell5_valueChanged(int)));
    connect(commNode, SIGNAL(cell6_voltage(int)), this, SLOT(on_progressBar_cell6_valueChanged(int)));

}

void MainWindow::on_progressBar_cell1_valueChanged(int value)
{   int val = voltageToPercentage(value);
    //std::cout<<value<<std::endl;
    ui->progressBar_cell1->setValue(val);
    //std::cout<<val<<std::endl;
}

void MainWindow::on_progressBar_cell2_valueChanged(int value)
{
    //ui-> cell2_voltage->clear();
    ui->progressBar_cell2->setValue(10);
    
}


void MainWindow::on_progressBar_cell3_valueChanged(int value)
{
    ui->progressBar_cell3->setValue(value);
}


void MainWindow::on_progressBar_cell4_valueChanged(int value)
{
    ui->progressBar_cell4->setValue(value);
}


void MainWindow::on_progressBar_cell5_valueChanged(int value)
{
    ui->progressBar_cell5->setValue(value);
}


void MainWindow::on_progressBar_cell6_valueChanged(int value)
{
    ui->progressBar_cell6->setValue(value);
}

int MainWindow::voltageToPercentage(int voltage) {
    if (voltage > FULL_VOLTAGE) {
        return 100;
    }
    if (voltage < EMPTY_VOLTAGE) {
        return 0;
    }
    float percentage = ((float)(voltage - EMPTY_VOLTAGE) / (FULL_VOLTAGE - EMPTY_VOLTAGE)) * 100;
    // Round to the nearest whole number
    return (int)round(percentage);
}

MainWindow::~MainWindow()
{

    delete ui;
}

