/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_25;
    QLabel *label_28;
    QLabel *label_29;
    QLabel *label_26;
    QLabel *label_30;
    QLabel *label_27;
    QLabel *label_32;
    QLabel *battery_Voltage;
    QLabel *label_2;
    QLabel *label_34;
    QLabel *soh;
    QLabel *label_40;
    QLabel *cells_count;
    QLabel *battery_temperature;
    QLabel *label_36;
    QLabel *label_23;
    QLabel *label_15;
    QLabel *label_5;
    QLabel *label_17;
    QLabel *label_19;
    QLabel *label_21;
    QLabel *label_3;
    QLabel *label_13;
    QLabel *UV_status;
    QLabel *OV_status;
    QLabel *SC_status;
    QLabel *OC_status;
    QLabel *OT_status;
    QLabel *fault_status;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QProgressBar *progressBar_cell1;
    QLCDNumber *V_cell_1;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QProgressBar *progressBar_cell2;
    QLCDNumber *V_cell_2;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_3;
    QProgressBar *progressBar_cell3;
    QLCDNumber *V_cell_3;
    QWidget *layoutWidget3;
    QHBoxLayout *horizontalLayout_4;
    QProgressBar *progressBar_cell4;
    QLCDNumber *V_cell_4;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout_5;
    QProgressBar *progressBar_cell5;
    QLCDNumber *V_cell_5;
    QWidget *layoutWidget5;
    QHBoxLayout *horizontalLayout_6;
    QProgressBar *progressBar_cell6;
    QLCDNumber *V_cell_6;
    QWidget *layoutWidget6;
    QHBoxLayout *horizontalLayout_7;
    QProgressBar *progressBar_cell7;
    QLCDNumber *V_cell_7;
    QWidget *layoutWidget7;
    QHBoxLayout *horizontalLayout_8;
    QProgressBar *progressBar_cell8;
    QLCDNumber *V_cell_8;
    QWidget *layoutWidget8;
    QHBoxLayout *horizontalLayout_9;
    QProgressBar *progressBar_cell9;
    QLCDNumber *V_cell_9;
    QWidget *layoutWidget9;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_42;
    QLCDNumber *lcd_current;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_38;
    QLCDNumber *lcd_soc;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1299, 846);
        MainWindow->setMinimumSize(QSize(1128, 700));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 405, 30, 50));
        label->setMinimumSize(QSize(30, 50));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);
        label_25 = new QLabel(centralwidget);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(281, 349, 90, 50));
        label_25->setMinimumSize(QSize(90, 50));
        label_25->setFont(font);
        label_25->setTextFormat(Qt::PlainText);
        label_25->setAlignment(Qt::AlignCenter);
        label_28 = new QLabel(centralwidget);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(10, 349, 200, 50));
        label_28->setMinimumSize(QSize(200, 50));
        label_28->setFont(font);
        label_28->setTextFormat(Qt::PlainText);
        label_28->setAlignment(Qt::AlignCenter);
        label_29 = new QLabel(centralwidget);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(383, 349, 200, 50));
        label_29->setMinimumSize(QSize(200, 50));
        label_29->setFont(font);
        label_29->setTextFormat(Qt::PlainText);
        label_29->setAlignment(Qt::AlignCenter);
        label_26 = new QLabel(centralwidget);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(654, 349, 90, 50));
        label_26->setMinimumSize(QSize(90, 50));
        label_26->setFont(font);
        label_26->setTextFormat(Qt::PlainText);
        label_26->setAlignment(Qt::AlignCenter);
        label_30 = new QLabel(centralwidget);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(780, 349, 200, 50));
        label_30->setMinimumSize(QSize(200, 50));
        label_30->setFont(font);
        label_30->setTextFormat(Qt::PlainText);
        label_30->setAlignment(Qt::AlignCenter);
        label_27 = new QLabel(centralwidget);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(1027, 349, 90, 50));
        label_27->setMinimumSize(QSize(90, 50));
        label_27->setFont(font);
        label_27->setTextFormat(Qt::PlainText);
        label_27->setAlignment(Qt::AlignCenter);
        label_32 = new QLabel(centralwidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(23, 83, 200, 50));
        label_32->setMinimumSize(QSize(200, 50));
        label_32->setFont(font);
        label_32->setTextFormat(Qt::PlainText);
        label_32->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        battery_Voltage = new QLabel(centralwidget);
        battery_Voltage->setObjectName(QString::fromUtf8("battery_Voltage"));
        battery_Voltage->setGeometry(QRect(229, 83, 90, 50));
        battery_Voltage->setMinimumSize(QSize(90, 50));
        battery_Voltage->setFont(font);
        battery_Voltage->setTextFormat(Qt::PlainText);
        battery_Voltage->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 660, 1061, 21));
        label_34 = new QLabel(centralwidget);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(23, 141, 200, 50));
        label_34->setMinimumSize(QSize(200, 50));
        label_34->setFont(font);
        label_34->setTextFormat(Qt::PlainText);
        label_34->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        soh = new QLabel(centralwidget);
        soh->setObjectName(QString::fromUtf8("soh"));
        soh->setGeometry(QRect(531, 141, 90, 50));
        soh->setMinimumSize(QSize(90, 50));
        soh->setFont(font);
        soh->setTextFormat(Qt::PlainText);
        soh->setAlignment(Qt::AlignCenter);
        label_40 = new QLabel(centralwidget);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(325, 141, 200, 50));
        label_40->setMinimumSize(QSize(200, 50));
        label_40->setFont(font);
        label_40->setTextFormat(Qt::PlainText);
        label_40->setAlignment(Qt::AlignCenter);
        cells_count = new QLabel(centralwidget);
        cells_count->setObjectName(QString::fromUtf8("cells_count"));
        cells_count->setGeometry(QRect(229, 199, 90, 50));
        cells_count->setMinimumSize(QSize(90, 50));
        cells_count->setFont(font);
        cells_count->setTextFormat(Qt::PlainText);
        cells_count->setAlignment(Qt::AlignCenter);
        battery_temperature = new QLabel(centralwidget);
        battery_temperature->setObjectName(QString::fromUtf8("battery_temperature"));
        battery_temperature->setGeometry(QRect(229, 141, 90, 50));
        battery_temperature->setMinimumSize(QSize(90, 50));
        battery_temperature->setFont(font);
        battery_temperature->setTextFormat(Qt::PlainText);
        battery_temperature->setAlignment(Qt::AlignCenter);
        label_36 = new QLabel(centralwidget);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(23, 199, 200, 50));
        label_36->setMinimumSize(QSize(200, 50));
        label_36->setFont(font);
        label_36->setTextFormat(Qt::PlainText);
        label_36->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_23 = new QLabel(centralwidget);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(756, 565, 30, 50));
        label_23->setMinimumSize(QSize(30, 50));
        label_23->setFont(font);
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(383, 485, 30, 50));
        label_15->setMinimumSize(QSize(30, 50));
        label_15->setFont(font);
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 565, 30, 50));
        label_5->setMinimumSize(QSize(30, 50));
        label_5->setFont(font);
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(383, 565, 30, 50));
        label_17->setMinimumSize(QSize(30, 50));
        label_17->setFont(font);
        label_19 = new QLabel(centralwidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(756, 405, 30, 50));
        label_19->setMinimumSize(QSize(30, 50));
        label_19->setFont(font);
        label_21 = new QLabel(centralwidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(756, 485, 30, 50));
        label_21->setMinimumSize(QSize(30, 50));
        label_21->setFont(font);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 485, 30, 50));
        label_3->setMinimumSize(QSize(30, 50));
        label_3->setFont(font);
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(383, 405, 30, 50));
        label_13->setMinimumSize(QSize(30, 50));
        label_13->setFont(font);
        UV_status = new QLabel(centralwidget);
        UV_status->setObjectName(QString::fromUtf8("UV_status"));
        UV_status->setGeometry(QRect(631, 83, 150, 50));
        UV_status->setMinimumSize(QSize(120, 50));
        UV_status->setFont(font);
        UV_status->setTextFormat(Qt::PlainText);
        UV_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        OV_status = new QLabel(centralwidget);
        OV_status->setObjectName(QString::fromUtf8("OV_status"));
        OV_status->setGeometry(QRect(631, 141, 150, 50));
        OV_status->setMinimumSize(QSize(150, 50));
        OV_status->setFont(font);
        OV_status->setTextFormat(Qt::PlainText);
        OV_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        SC_status = new QLabel(centralwidget);
        SC_status->setObjectName(QString::fromUtf8("SC_status"));
        SC_status->setGeometry(QRect(631, 199, 150, 50));
        SC_status->setMinimumSize(QSize(150, 50));
        SC_status->setFont(font);
        SC_status->setTextFormat(Qt::PlainText);
        SC_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        OC_status = new QLabel(centralwidget);
        OC_status->setObjectName(QString::fromUtf8("OC_status"));
        OC_status->setGeometry(QRect(837, 83, 150, 50));
        OC_status->setMinimumSize(QSize(150, 50));
        OC_status->setFont(font);
        OC_status->setTextFormat(Qt::PlainText);
        OC_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        OT_status = new QLabel(centralwidget);
        OT_status->setObjectName(QString::fromUtf8("OT_status"));
        OT_status->setGeometry(QRect(837, 141, 150, 50));
        OT_status->setMinimumSize(QSize(150, 50));
        OT_status->setFont(font);
        OT_status->setTextFormat(Qt::PlainText);
        OT_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        fault_status = new QLabel(centralwidget);
        fault_status->setObjectName(QString::fromUtf8("fault_status"));
        fault_status->setGeometry(QRect(837, 199, 150, 50));
        fault_status->setMinimumSize(QSize(150, 50));
        fault_status->setFont(font);
        fault_status->setTextFormat(Qt::PlainText);
        fault_status->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(46, 405, 322, 52));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        progressBar_cell1 = new QProgressBar(layoutWidget);
        progressBar_cell1->setObjectName(QString::fromUtf8("progressBar_cell1"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(progressBar_cell1->sizePolicy().hasHeightForWidth());
        progressBar_cell1->setSizePolicy(sizePolicy);
        progressBar_cell1->setMinimumSize(QSize(250, 50));
        progressBar_cell1->setValue(24);

        horizontalLayout->addWidget(progressBar_cell1);

        V_cell_1 = new QLCDNumber(layoutWidget);
        V_cell_1->setObjectName(QString::fromUtf8("V_cell_1"));
        V_cell_1->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_1->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout->addWidget(V_cell_1);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(46, 485, 322, 52));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        progressBar_cell2 = new QProgressBar(layoutWidget1);
        progressBar_cell2->setObjectName(QString::fromUtf8("progressBar_cell2"));
        sizePolicy.setHeightForWidth(progressBar_cell2->sizePolicy().hasHeightForWidth());
        progressBar_cell2->setSizePolicy(sizePolicy);
        progressBar_cell2->setMinimumSize(QSize(250, 50));
        progressBar_cell2->setValue(24);

        horizontalLayout_2->addWidget(progressBar_cell2);

        V_cell_2 = new QLCDNumber(layoutWidget1);
        V_cell_2->setObjectName(QString::fromUtf8("V_cell_2"));
        V_cell_2->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_2->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_2->addWidget(V_cell_2);

        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(46, 565, 322, 52));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        progressBar_cell3 = new QProgressBar(layoutWidget2);
        progressBar_cell3->setObjectName(QString::fromUtf8("progressBar_cell3"));
        sizePolicy.setHeightForWidth(progressBar_cell3->sizePolicy().hasHeightForWidth());
        progressBar_cell3->setSizePolicy(sizePolicy);
        progressBar_cell3->setMinimumSize(QSize(250, 50));
        progressBar_cell3->setValue(24);

        horizontalLayout_3->addWidget(progressBar_cell3);

        V_cell_3 = new QLCDNumber(layoutWidget2);
        V_cell_3->setObjectName(QString::fromUtf8("V_cell_3"));
        V_cell_3->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_3->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_3->addWidget(V_cell_3);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(419, 405, 322, 52));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget3);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        progressBar_cell4 = new QProgressBar(layoutWidget3);
        progressBar_cell4->setObjectName(QString::fromUtf8("progressBar_cell4"));
        sizePolicy.setHeightForWidth(progressBar_cell4->sizePolicy().hasHeightForWidth());
        progressBar_cell4->setSizePolicy(sizePolicy);
        progressBar_cell4->setMinimumSize(QSize(250, 50));
        progressBar_cell4->setValue(24);

        horizontalLayout_4->addWidget(progressBar_cell4);

        V_cell_4 = new QLCDNumber(layoutWidget3);
        V_cell_4->setObjectName(QString::fromUtf8("V_cell_4"));
        V_cell_4->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_4->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_4->addWidget(V_cell_4);

        layoutWidget4 = new QWidget(centralwidget);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(419, 485, 322, 52));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget4);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        progressBar_cell5 = new QProgressBar(layoutWidget4);
        progressBar_cell5->setObjectName(QString::fromUtf8("progressBar_cell5"));
        sizePolicy.setHeightForWidth(progressBar_cell5->sizePolicy().hasHeightForWidth());
        progressBar_cell5->setSizePolicy(sizePolicy);
        progressBar_cell5->setMinimumSize(QSize(250, 50));
        progressBar_cell5->setValue(24);

        horizontalLayout_5->addWidget(progressBar_cell5);

        V_cell_5 = new QLCDNumber(layoutWidget4);
        V_cell_5->setObjectName(QString::fromUtf8("V_cell_5"));
        V_cell_5->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_5->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_5->addWidget(V_cell_5);

        layoutWidget5 = new QWidget(centralwidget);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(419, 565, 322, 52));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget5);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        progressBar_cell6 = new QProgressBar(layoutWidget5);
        progressBar_cell6->setObjectName(QString::fromUtf8("progressBar_cell6"));
        sizePolicy.setHeightForWidth(progressBar_cell6->sizePolicy().hasHeightForWidth());
        progressBar_cell6->setSizePolicy(sizePolicy);
        progressBar_cell6->setMinimumSize(QSize(250, 50));
        progressBar_cell6->setValue(24);

        horizontalLayout_6->addWidget(progressBar_cell6);

        V_cell_6 = new QLCDNumber(layoutWidget5);
        V_cell_6->setObjectName(QString::fromUtf8("V_cell_6"));
        V_cell_6->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_6->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_6->addWidget(V_cell_6);

        layoutWidget6 = new QWidget(centralwidget);
        layoutWidget6->setObjectName(QString::fromUtf8("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(792, 405, 322, 52));
        horizontalLayout_7 = new QHBoxLayout(layoutWidget6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
        progressBar_cell7 = new QProgressBar(layoutWidget6);
        progressBar_cell7->setObjectName(QString::fromUtf8("progressBar_cell7"));
        sizePolicy.setHeightForWidth(progressBar_cell7->sizePolicy().hasHeightForWidth());
        progressBar_cell7->setSizePolicy(sizePolicy);
        progressBar_cell7->setMinimumSize(QSize(250, 50));
        progressBar_cell7->setValue(24);

        horizontalLayout_7->addWidget(progressBar_cell7);

        V_cell_7 = new QLCDNumber(layoutWidget6);
        V_cell_7->setObjectName(QString::fromUtf8("V_cell_7"));
        V_cell_7->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_7->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_7->addWidget(V_cell_7);

        layoutWidget7 = new QWidget(centralwidget);
        layoutWidget7->setObjectName(QString::fromUtf8("layoutWidget7"));
        layoutWidget7->setGeometry(QRect(792, 480, 322, 52));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget7);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        progressBar_cell8 = new QProgressBar(layoutWidget7);
        progressBar_cell8->setObjectName(QString::fromUtf8("progressBar_cell8"));
        sizePolicy.setHeightForWidth(progressBar_cell8->sizePolicy().hasHeightForWidth());
        progressBar_cell8->setSizePolicy(sizePolicy);
        progressBar_cell8->setMinimumSize(QSize(250, 50));
        progressBar_cell8->setValue(24);

        horizontalLayout_8->addWidget(progressBar_cell8);

        V_cell_8 = new QLCDNumber(layoutWidget7);
        V_cell_8->setObjectName(QString::fromUtf8("V_cell_8"));
        V_cell_8->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_8->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_8->addWidget(V_cell_8);

        layoutWidget8 = new QWidget(centralwidget);
        layoutWidget8->setObjectName(QString::fromUtf8("layoutWidget8"));
        layoutWidget8->setGeometry(QRect(792, 565, 322, 52));
        horizontalLayout_9 = new QHBoxLayout(layoutWidget8);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(0, 0, 0, 0);
        progressBar_cell9 = new QProgressBar(layoutWidget8);
        progressBar_cell9->setObjectName(QString::fromUtf8("progressBar_cell9"));
        sizePolicy.setHeightForWidth(progressBar_cell9->sizePolicy().hasHeightForWidth());
        progressBar_cell9->setSizePolicy(sizePolicy);
        progressBar_cell9->setMinimumSize(QSize(250, 50));
        progressBar_cell9->setValue(24);

        horizontalLayout_9->addWidget(progressBar_cell9);

        V_cell_9 = new QLCDNumber(layoutWidget8);
        V_cell_9->setObjectName(QString::fromUtf8("V_cell_9"));
        QFont font1;
        font1.setBold(false);
        V_cell_9->setFont(font1);
        V_cell_9->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));
        V_cell_9->setSegmentStyle(QLCDNumber::Flat);

        horizontalLayout_9->addWidget(V_cell_9);

        layoutWidget9 = new QWidget(centralwidget);
        layoutWidget9->setObjectName(QString::fromUtf8("layoutWidget9"));
        layoutWidget9->setGeometry(QRect(325, 199, 272, 52));
        horizontalLayout_10 = new QHBoxLayout(layoutWidget9);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_42 = new QLabel(layoutWidget9);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setMinimumSize(QSize(200, 50));
        label_42->setFont(font);
        label_42->setTextFormat(Qt::PlainText);
        label_42->setAlignment(Qt::AlignCenter);

        horizontalLayout_10->addWidget(label_42);

        lcd_current = new QLCDNumber(layoutWidget9);
        lcd_current->setObjectName(QString::fromUtf8("lcd_current"));
        lcd_current->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"    color: rgb(38, 162, 105);    \n"
"    background-color: rgb(255, 255, 255);\n"
"}"));

        horizontalLayout_10->addWidget(lcd_current);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(325, 83, 272, 52));
        horizontalLayout_11 = new QHBoxLayout(widget);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        horizontalLayout_11->setContentsMargins(0, 0, 0, 0);
        label_38 = new QLabel(widget);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setMinimumSize(QSize(200, 50));
        label_38->setFont(font);
        label_38->setTextFormat(Qt::PlainText);
        label_38->setAlignment(Qt::AlignCenter);

        horizontalLayout_11->addWidget(label_38);

        lcd_soc = new QLCDNumber(widget);
        lcd_soc->setObjectName(QString::fromUtf8("lcd_soc"));
        lcd_soc->setStyleSheet(QString::fromUtf8("QLCDNumber{\n"
"	border-color: rgb(38, 162, 105);\n"
"    color: rgb(38, 162, 105);\n"
"	font: 700 14pt \"Ubuntu\";\n"
"    background-color: rgb(255, 255, 255);\n"
"}"));

        horizontalLayout_11->addWidget(lcd_soc);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1299, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "01", nullptr));
        label_25->setText(QCoreApplication::translate("MainWindow", "[mV]", nullptr));
        label_28->setText(QCoreApplication::translate("MainWindow", "Cell Voltage", nullptr));
        label_29->setText(QCoreApplication::translate("MainWindow", "Cell Voltage", nullptr));
        label_26->setText(QCoreApplication::translate("MainWindow", "[mV]", nullptr));
        label_30->setText(QCoreApplication::translate("MainWindow", "Cell Voltage", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "[mV]", nullptr));
        label_32->setText(QCoreApplication::translate("MainWindow", "Battery Voltage (V)", nullptr));
        battery_Voltage->setText(QCoreApplication::translate("MainWindow", "0000", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_34->setText(QCoreApplication::translate("MainWindow", "Cell Temperature(\302\260C)", nullptr));
        soh->setText(QCoreApplication::translate("MainWindow", "0000", nullptr));
        label_40->setText(QCoreApplication::translate("MainWindow", "SOH(%)", nullptr));
        cells_count->setText(QCoreApplication::translate("MainWindow", "0000", nullptr));
        battery_temperature->setText(QCoreApplication::translate("MainWindow", "0000", nullptr));
        label_36->setText(QCoreApplication::translate("MainWindow", "Cells Count", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "09", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "05", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "03", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "06", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "07", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "08", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "02", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "04", nullptr));
        UV_status->setText(QCoreApplication::translate("MainWindow", "Under Voltage", nullptr));
        OV_status->setText(QCoreApplication::translate("MainWindow", "Over Voltage", nullptr));
        SC_status->setText(QCoreApplication::translate("MainWindow", "Shor Circuit", nullptr));
        OC_status->setText(QCoreApplication::translate("MainWindow", "Over Current", nullptr));
        OT_status->setText(QCoreApplication::translate("MainWindow", "Over Temp", nullptr));
        fault_status->setText(QCoreApplication::translate("MainWindow", "Fault Recovery", nullptr));
        label_42->setText(QCoreApplication::translate("MainWindow", "Current(Amps)", nullptr));
        label_38->setText(QCoreApplication::translate("MainWindow", "SoC (%)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
