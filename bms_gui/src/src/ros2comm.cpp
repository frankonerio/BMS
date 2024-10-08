
#include "bms_gui/ros2comm.h"

modbus_t *ctx;
int rc_read = 0;
int rc_write = 0;
int i;
int rc = 0;
uint16_t tab_dest[64];
uint16_t tab_src[10] = {1};

int16_t data[4] = {9, 9, 9, 9};

Ros2Comm::Ros2Comm()
{
    int argc = 0;
    char **argv = NULL;

    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("ros2comm");

    battery_state_publisher = node->create_publisher<sensor_msgs::msg::BatteryState>("/battery_state", 10);

    read_registers = true;
    write_registers = false;

    this->start();
}
void Ros2Comm::run()
{
    rclcpp::WallRate loop_rate(0.25);
    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);
        update_battery_state();
        battery_state_publisher->publish(battery_info);
        loop_rate.sleep();
    }
    rclcpp::shutdown();
}

int Ros2Comm::update_battery_state()
{

   ctx = modbus_new_tcp("192.168.137.14", 502);

    rc = modbus_set_slave(ctx, 0x01);
    if (rc == -1)
    {
        fprintf(stderr, "Invalid slave ID\n");
        modbus_free(ctx);
        return -1;
    }

    // try connection
    while (modbus_connect(ctx) == -1)
    {

        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    
    }

    // printf("connection succeeded\n");
    if (read_registers == true)
    {

        rc = modbus_read_registers(ctx, 0, 24, tab_dest);
        if (rc == -1)
        {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
            return -1;
        }

        for (i = 0; i < rc; i++)
        {
            printf("reg[%d]=%d (0x%X)\n", i, tab_dest[i], tab_dest[i]);
            
        } 

        cell1_voltage(tab_dest[0]);
        cell2_voltage(tab_dest[1]);
        cell3_voltage(tab_dest[2]);
        cell4_voltage(tab_dest[3]);
        cell5_voltage(tab_dest[4]);
        cell6_voltage(tab_dest[8]); 
        
        lcd_soc(tab_dest[21]);
        
       
        lcd_cell_1(tab_dest[9]);
	lcd_cell_2(tab_dest[10]);
	lcd_cell_3(tab_dest[11]);
	lcd_cell_4(tab_dest[12]);
	lcd_cell_5(tab_dest[13]);
	lcd_cell_6(tab_dest[17]);
	lcd_cell_7(tab_dest[14]);
	lcd_cell_8(tab_dest[15]);
	lcd_cell_9(tab_dest[16]);
     }

    else
    {
      
    }

   /*  rc = modbus_write_registers(ctx, 0, 2, (uint16_t *)data);
    data[0] = data[0] + 1;
    data[1] = data[1] + 2; */

    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}
