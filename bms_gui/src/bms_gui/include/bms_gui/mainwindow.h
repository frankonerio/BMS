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

private:
    Ui::MainWindow *ui;
    Ros2Comm *commNode;
    QSignalMapper *signalMapper;

private slots:
   
};
#endif // SRC_BMS_GUI_MAINWINDOW_H
