#ifndef SRC_BMS_GUI_MAINWINDOW_H
#define SRC_BMS_GUI_MAINWINDOW_H

#include <QMainWindow>
#include "bms_gui/ros2comm.h"
#include <iostream>
#include <QList>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QRadioButton>
#include <QSignalMapper>
#include <QCheckBox>
#include <QIcon>
#include <QPixmap>
#include <stdio.h>

#define Read_battery_status             1
#define Configure_battery_parameters    2
#define Write_multiple_coils            3
#define FULL_VOLTAGE 4200
#define EMPTY_VOLTAGE 3000

extern uint16_t tab_dest[64];

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    int voltageToPercentage(int voltage);
    
private:
    Ui::MainWindow *ui;
    Ros2Comm *commNode;
    QSignalMapper *signalMapper;
    

private slots:
	void on_progressBar_cell1_valueChanged(int value);

	void on_progressBar_cell2_valueChanged(int value);

	void on_progressBar_cell3_valueChanged(int value);

	void on_progressBar_cell4_valueChanged(int value);

	void on_progressBar_cell5_valueChanged(int value);

	void on_progressBar_cell6_valueChanged(int value);

	void lcd_cell_1_voltage_update(int value);

	void lcd_cell_2_voltage_update(int value);

	void lcd_cell_3_voltage_update(int value);

	void lcd_cell_4_voltage_update(int value);

	void lcd_cell_5_voltage_update(int value);

	void lcd_cell_6_voltage_update(int value);

	void lcd_cell_7_voltage_update(int value);

	void lcd_cell_8_voltage_update(int value);

	void lcd_cell_9_voltage_update(int value);
	
	void lcd_soc_update(int value);

};
#endif // SRC_BMS_GUI_MAINWINDOW_H
