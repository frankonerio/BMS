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
    
	ui->battery_Voltage->clear();
	//ui->battery_Voltage->setStyleSheet("background-color: cornsilk;");
	ui->battery_temperature->clear();
	ui->cells_count->clear();
	ui->soh->clear();
	//ui->current->clear();
	
	
	
	ui->UV_status->setStyleSheet("background-color:  cornsilk;");
	ui->OV_status->setStyleSheet("background-color:  cornsilk;");
	ui->SC_status->setStyleSheet("background-color:  cornsilk;");
	ui->OC_status->setStyleSheet("background-color:  cornsilk;");
	ui->OT_status->setStyleSheet("background-color:  cornsilk;");
	ui->fault_status->setStyleSheet("background-color:  cornsilk;");

	

	connect(commNode, SIGNAL(cell1_voltage(int)), this, SLOT(on_progressBar_cell1_valueChanged(int)));
	connect(commNode, SIGNAL(cell2_voltage(int)), this, SLOT(on_progressBar_cell2_valueChanged(int)));
	connect(commNode, SIGNAL(cell3_voltage(int)), this, SLOT(on_progressBar_cell3_valueChanged(int)));
	connect(commNode, SIGNAL(cell4_voltage(int)), this, SLOT(on_progressBar_cell4_valueChanged(int)));
	connect(commNode, SIGNAL(cell5_voltage(int)), this, SLOT(on_progressBar_cell5_valueChanged(int)));
	connect(commNode, SIGNAL(cell6_voltage(int)), this, SLOT(on_progressBar_cell6_valueChanged(int)));
	
	connect(commNode, SIGNAL(lcd_cell_1(int)), this, SLOT(lcd_cell_1_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_2(int)), this, SLOT(lcd_cell_2_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_3(int)), this, SLOT(lcd_cell_3_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_4(int)), this, SLOT(lcd_cell_4_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_5(int)), this, SLOT(lcd_cell_5_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_6(int)), this, SLOT(lcd_cell_6_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_7(int)), this, SLOT(lcd_cell_7_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_8(int)), this, SLOT(lcd_cell_8_voltage_update(int)));
	connect(commNode, SIGNAL(lcd_cell_9(int)), this, SLOT(lcd_cell_9_voltage_update(int)));

	connect(commNode, SIGNAL(lcd_soc(int)), this, SLOT(lcd_soc_update(int)));
	

}

void MainWindow::on_progressBar_cell1_valueChanged(int value)
{   
    ui->progressBar_cell1->setValue(value);
   
}

void MainWindow::on_progressBar_cell2_valueChanged(int value)
{
    ui->progressBar_cell2->setValue(value);
    
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


void MainWindow::lcd_cell_1_voltage_update(int value)
{
    ui->V_cell_1->display(value);
}


void MainWindow::lcd_cell_2_voltage_update(int value)
{
    ui->V_cell_2->display(value);
}

void MainWindow::lcd_cell_3_voltage_update(int value)
{
    ui->V_cell_3->display(value);
}

void MainWindow::lcd_cell_4_voltage_update(int value)
{
    ui->V_cell_4->display(value);
}

void MainWindow::lcd_cell_5_voltage_update(int value)
{
    ui->V_cell_5->display(value);
}

void MainWindow::lcd_cell_6_voltage_update(int value)
{
    ui->V_cell_6->display(value);
}

void MainWindow::lcd_cell_7_voltage_update(int value)
{
    ui->V_cell_7->display(value);
}

void MainWindow::lcd_cell_8_voltage_update(int value)
{
    ui->V_cell_8->display(value);
}

void MainWindow::lcd_cell_9_voltage_update(int value)
{
    ui->V_cell_9->display(value);
}

void MainWindow::lcd_soc_update(int value)
{
    ui->lcd_soc->display(value);
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

