#ifndef SRC_BMS_GUI__ROS2COMM_H
#define SRC_BMS_GUI__ROS2COMM_H

#include <QObject>
#include <QThread>
#include <QTreeWidget>
#include <QMap>
#include <QString>
#include <QList>

#include <memory>
#include <random>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include "stdio.h"
#include <modbus/modbus.h>
#include <errno.h>
#include <unistd.h>
#include <rclcpp/rclcpp.hpp>

#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/battery_state.hpp"


//typedef QMap<QString, qint32> map;
//typedef QList<QString> str_list;
//typedef std::map<std::string, int> std_map;
//typedef std::pair<std::string, int> pair;

using std::placeholders::_1;

class Ros2Comm : public QThread
{
    Q_OBJECT
public:
    Ros2Comm();

    sensor_msgs::msg::BatteryState battery_state;
    int update_battery_state();
    bool read_registers;
    bool write_registers;


protected:
	void run();
private:
 	std::shared_ptr<rclcpp::Node> node;

    //battery state publisher
    rclcpp::Publisher<sensor_msgs::msg::BatteryState>::SharedPtr battery_state_publisher;

};

#endif // SRC_BMS_GUI__ROS2COMM_H
