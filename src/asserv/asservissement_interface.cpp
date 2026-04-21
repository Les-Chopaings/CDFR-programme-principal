#include "asserv/asservissement_interface.h"

//***********************************************
// Start auto generation CMD_FONCTION
// Last generation 2025-05-22 22:04:06: python3 autoGen.py
// DO NOT EDIT
void asservissement_interface::get_version(uint16_t &part1, uint16_t &part2, uint16_t &part3, uint16_t &part4){
    uint8_t data[8];
    int length = 8;
    I2cReceiveData(1, data, length);
    DataUnpacker unpacker(data, length);
    part1 = (uint16_t)unpacker.popUint16();
    part2 = (uint16_t)unpacker.popUint16();
    part3 = (uint16_t)unpacker.popUint16();
    part4 = (uint16_t)unpacker.popUint16();
}

uint16_t asservissement_interface::get_log_size(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(2, data, length);
    DataUnpacker unpacker(data, length);
    uint16_t retPara = (uint16_t)unpacker.popUint16();
    return retPara;
}

void asservissement_interface::get_log(char* data, int length){
    I2cReceiveData(3, (uint8_t*)data, length);
}

void asservissement_interface::set_led_1(bool status){
    LOG_DEBUG("set_led_1 : ",status);
    DataPacker packer;
    packer.addUint16((int16_t)status);
    I2cSendData(10, packer.getData(), packer.getSize());
}

void asservissement_interface::set_led_2(bool status){
    LOG_DEBUG("set_led_2 : ",status);
    DataPacker packer;
    packer.addUint16((int16_t)status);
    I2cSendData(11, packer.getData(), packer.getSize());
}

void asservissement_interface::get_coordinates(int16_t &x, int16_t &y, int16_t &theta){
    #ifdef EMULATE
        run_emulate();
        x = emulate_x;
        y = emulate_y;
        theta = emulate_theta;
        return;
    #endif
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(20, data, length);
    DataUnpacker unpacker(data, length);
    x = (int16_t)unpacker.popUint16();
    y = (int16_t)unpacker.popUint16();
    theta = (int16_t)unpacker.popUint16();
}

void asservissement_interface::set_coordinates(int16_t x, int16_t y, int16_t theta){
    LOG_DEBUG("set_coordinates : ",x," ",y," ",theta);
    #ifdef EMULATE
        emulate_x = x;
        emulate_y = y;
        emulate_theta = theta;
        return;
    #endif
    DataPacker packer;
    packer.addUint16((int16_t)x);
    packer.addUint16((int16_t)y);
    packer.addUint16((int16_t)theta);
    I2cSendData(21, packer.getData(), packer.getSize());
}

void asservissement_interface::stop(){
    #ifdef EMULATE
        emulate_nextValid = false;
        enumate_actionQueue.clear();
    #endif
    LOG_DEBUG("stop");
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(30, data, length);
}

void asservissement_interface::pause(){
    LOG_DEBUG("pause");
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(31, data, length);
}

void asservissement_interface::resume(){
    LOG_DEBUG("resume");
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(32, data, length);
}

void asservissement_interface::go_to_point(int16_t x, int16_t y, Rotation rotation, Direction direction){
    LOG_DEBUG("go_to_point : ",x," ",y," ",(int)rotation," ",(int)direction);
    #ifdef EMULATE
        enumate_actionQueue.push_back(emulate_action(x, y));
        run_emulate();
        return;
    #endif
    DataPacker packer;
    packer.addUint16((int16_t)x);
    packer.addUint16((int16_t)y);
    packer.addUint16((int16_t)rotation);
    packer.addUint16((int16_t)direction);
    I2cSendData(33, packer.getData(), packer.getSize());
}

void asservissement_interface::go_to_point(int16_t x, int16_t y, int16_t theta, Rotation rotationFirst, Direction direction, Rotation rotationSecond){
    LOG_DEBUG("go_to_point : ",x," ",y," ",theta," ",(int)rotationFirst," ",(int)direction," ",(int)rotationSecond);
    #ifdef EMULATE
        enumate_actionQueue.push_back(emulate_action(x, y));
        run_emulate();
        return;
    #endif
    DataPacker packer;
    packer.addUint16((int16_t)x);
    packer.addUint16((int16_t)y);
    packer.addUint16((int16_t)theta);
    packer.addUint16((int16_t)rotationFirst);
    packer.addUint16((int16_t)direction);
    packer.addUint16((int16_t)rotationSecond);
    I2cSendData(34, packer.getData(), packer.getSize());
}

void asservissement_interface::consigne_angulaire(int16_t angle, Rotation rotation){
    LOG_DEBUG("consigne_angulaire : ",angle," ",(int)rotation);
    DataPacker packer;
    packer.addUint16((int16_t)angle);
    packer.addUint16((int16_t)rotation);
    I2cSendData(35, packer.getData(), packer.getSize());
}

void asservissement_interface::consigne_angulaire(int16_t x, int16_t y, Rotation rotation, Direction direction){
    LOG_DEBUG("consigne_angulaire : ",x," ",y," ",(int)rotation," ",(int)direction);
    DataPacker packer;
    packer.addUint16((int16_t)x);
    packer.addUint16((int16_t)y);
    packer.addUint16((int16_t)rotation);
    packer.addUint16((int16_t)direction);
    I2cSendData(36, packer.getData(), packer.getSize());
}

void asservissement_interface::set_linear_max_speed(int16_t max_speed, int16_t max_acceleration, int16_t max_deceleration){
    LOG_DEBUG("set_linear_max_speed : ",max_speed," ",max_acceleration," ",max_deceleration);
    DataPacker packer;
    packer.addUint16((int16_t)max_speed);
    packer.addUint16((int16_t)max_acceleration);
    packer.addUint16((int16_t)max_deceleration);
    I2cSendData(37, packer.getData(), packer.getSize());
}

void asservissement_interface::set_angular_max_speed(int16_t max_speed, int16_t max_acceleration, int16_t max_deceleration){
    LOG_DEBUG("set_angular_max_speed : ",max_speed," ",max_acceleration," ",max_deceleration);
    DataPacker packer;
    packer.addUint16((int16_t)max_speed);
    packer.addUint16((int16_t)max_acceleration);
    packer.addUint16((int16_t)max_deceleration);
    I2cSendData(38, packer.getData(), packer.getSize());
}

int16_t asservissement_interface::get_braking_distance(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(50, data, length);
    DataUnpacker unpacker(data, length);
    int16_t retPara = (int16_t)unpacker.popUint16();
    return retPara;
}

int16_t asservissement_interface::get_command_buffer_size(){
    #ifdef EMULATE
        run_emulate();
        return enumate_actionQueue.size();
    #endif
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(51, data, length);
    DataUnpacker unpacker(data, length);
    int16_t retPara = (int16_t)unpacker.popUint16();
    return retPara;
}

Direction asservissement_interface::get_direction_side(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(52, data, length);
    DataUnpacker unpacker(data, length);
    Direction retPara = (Direction)unpacker.popUint16();
    return retPara;
}

Rotation asservissement_interface::get_rotation_side(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(53, data, length);
    DataUnpacker unpacker(data, length);
    Rotation retPara = (Rotation)unpacker.popUint16();
    return retPara;
}

void asservissement_interface::get_current_target(int16_t &x, int16_t &y, int16_t &theta){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(54, data, length);
    DataUnpacker unpacker(data, length);
    x = (int16_t)unpacker.popUint16();
    y = (int16_t)unpacker.popUint16();
    theta = (int16_t)unpacker.popUint16();
}

bool asservissement_interface::get_moving_is_done(){
    #ifdef EMULATE
        run_emulate();
        return enumate_actionQueue.size()==0;
    #endif
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(55, data, length);
    DataUnpacker unpacker(data, length);
    bool retPara = (bool)unpacker.popUint16();
    return retPara;
}

bool asservissement_interface::get_running_is_done(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(56, data, length);
    DataUnpacker unpacker(data, length);
    bool retPara = (bool)unpacker.popUint16();
    return retPara;
}

bool asservissement_interface::get_turning_is_done(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(57, data, length);
    DataUnpacker unpacker(data, length);
    bool retPara = (bool)unpacker.popUint16();
    return retPara;
}

int16_t asservissement_interface::get_linear_error(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(58, data, length);
    DataUnpacker unpacker(data, length);
    int16_t retPara = (int16_t)unpacker.popUint16();
    return retPara;
}

int16_t asservissement_interface::get_angular_error(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(59, data, length);
    DataUnpacker unpacker(data, length);
    int16_t retPara = (int16_t)unpacker.popUint16();
    return retPara;
}

void asservissement_interface::get_current(int16_t &currentRight, int16_t &currentLeft){
    uint8_t data[4];
    int length = 4;
    I2cReceiveData(60, data, length);
    DataUnpacker unpacker(data, length);
    currentRight = (int16_t)unpacker.popUint16();
    currentLeft = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_speed(int16_t &speedRight, int16_t &speedLeft){
    uint8_t data[4];
    int length = 4;
    I2cReceiveData(61, data, length);
    DataUnpacker unpacker(data, length);
    speedRight = (int16_t)unpacker.popUint16();
    speedLeft = (int16_t)unpacker.popUint16();
}

void asservissement_interface::set_motor_state(bool motorEnable){
    LOG_DEBUG("set_motor_state : ",motorEnable);
    DataPacker packer;
    packer.addUint16((int16_t)motorEnable);
    I2cSendData(90, packer.getData(), packer.getSize());
}

void asservissement_interface::set_brake_state(bool brakeEnable){
    LOG_DEBUG("set_brake_state : ",brakeEnable);
    DataPacker packer;
    packer.addUint16((int16_t)brakeEnable);
    I2cSendData(91, packer.getData(), packer.getSize());
}

void asservissement_interface::set_max_torque(int16_t max_torque){
    LOG_DEBUG("set_max_torque : ",max_torque);
    DataPacker packer;
    packer.addUint16((int16_t)max_torque);
    I2cSendData(100, packer.getData(), packer.getSize());
}

void asservissement_interface::set_linear_position_control(int16_t max_speed_for, int16_t max_speed_back, int16_t max_acceleration_for, int16_t max_acceleration_back, int16_t max_deceleration_for, int16_t max_deceleration_back){
    DataPacker packer;
    packer.addUint16((int16_t)max_speed_for);
    packer.addUint16((int16_t)max_speed_back);
    packer.addUint16((int16_t)max_acceleration_for);
    packer.addUint16((int16_t)max_acceleration_back);
    packer.addUint16((int16_t)max_deceleration_for);
    packer.addUint16((int16_t)max_deceleration_back);
    I2cSendData(101, packer.getData(), packer.getSize());
}

void asservissement_interface::set_angular_position_control(int16_t max_speed_clock, int16_t max_speed_anti, int16_t max_acceleration_clock, int16_t max_acceleration_anti, int16_t max_deceleration_clock, int16_t max_deceleration_anti){
    DataPacker packer;
    packer.addUint16((int16_t)max_speed_clock);
    packer.addUint16((int16_t)max_speed_anti);
    packer.addUint16((int16_t)max_acceleration_clock);
    packer.addUint16((int16_t)max_acceleration_anti);
    packer.addUint16((int16_t)max_deceleration_clock);
    packer.addUint16((int16_t)max_deceleration_anti);
    I2cSendData(102, packer.getData(), packer.getSize());
}

void asservissement_interface::set_pid_linear_static(int16_t p, int16_t i, int16_t d){
    DataPacker packer;
    packer.addUint16((int16_t)p);
    packer.addUint16((int16_t)i);
    packer.addUint16((int16_t)d);
    I2cSendData(103, packer.getData(), packer.getSize());
}

void asservissement_interface::set_pid_linear_dynamic(int16_t p, int16_t i, int16_t d){
    DataPacker packer;
    packer.addUint16((int16_t)p);
    packer.addUint16((int16_t)i);
    packer.addUint16((int16_t)d);
    I2cSendData(104, packer.getData(), packer.getSize());
}

void asservissement_interface::set_pid_angular_static(int16_t p, int16_t i, int16_t d){
    DataPacker packer;
    packer.addUint16((int16_t)p);
    packer.addUint16((int16_t)i);
    packer.addUint16((int16_t)d);
    I2cSendData(105, packer.getData(), packer.getSize());
}

void asservissement_interface::set_pid_angular_dynamic(int16_t p, int16_t i, int16_t d){
    DataPacker packer;
    packer.addUint16((int16_t)p);
    packer.addUint16((int16_t)i);
    packer.addUint16((int16_t)d);
    I2cSendData(106, packer.getData(), packer.getSize());
}

void asservissement_interface::set_odometry_metric(int16_t sizeWheelLeft, int16_t sizeWheelRight, int16_t spaceInterWheel){
    DataPacker packer;
    packer.addUint16((int16_t)sizeWheelLeft);
    packer.addUint16((int16_t)sizeWheelRight);
    packer.addUint16((int16_t)spaceInterWheel);
    I2cSendData(107, packer.getData(), packer.getSize());
}

int16_t asservissement_interface::get_max_torque(){
    uint8_t data[2];
    int length = 2;
    I2cReceiveData(130, data, length);
    DataUnpacker unpacker(data, length);
    int16_t retPara = (int16_t)unpacker.popUint16();
    return retPara;
}

void asservissement_interface::get_linear_position_control(int16_t &max_speed_for, int16_t &max_speed_back, int16_t &max_acceleration_for, int16_t &max_acceleration_back, int16_t &max_deceleration_for, int16_t &max_deceleration_back){
    uint8_t data[12];
    int length = 12;
    I2cReceiveData(131, data, length);
    DataUnpacker unpacker(data, length);
    max_speed_for = (int16_t)unpacker.popUint16();
    max_speed_back = (int16_t)unpacker.popUint16();
    max_acceleration_for = (int16_t)unpacker.popUint16();
    max_acceleration_back = (int16_t)unpacker.popUint16();
    max_deceleration_for = (int16_t)unpacker.popUint16();
    max_deceleration_back = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_angular_position_control(int16_t &max_speed_clock, int16_t &max_speed_anti, int16_t &max_acceleration_clock, int16_t &max_acceleration_anti, int16_t &max_deceleration_clock, int16_t &max_deceleration_anti){
    uint8_t data[12];
    int length = 12;
    I2cReceiveData(132, data, length);
    DataUnpacker unpacker(data, length);
    max_speed_clock = (int16_t)unpacker.popUint16();
    max_speed_anti = (int16_t)unpacker.popUint16();
    max_acceleration_clock = (int16_t)unpacker.popUint16();
    max_acceleration_anti = (int16_t)unpacker.popUint16();
    max_deceleration_clock = (int16_t)unpacker.popUint16();
    max_deceleration_anti = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_pid_linear_static(int16_t &p, int16_t &i, int16_t &d){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(133, data, length);
    DataUnpacker unpacker(data, length);
    p = (int16_t)unpacker.popUint16();
    i = (int16_t)unpacker.popUint16();
    d = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_pid_linear_dynamic(int16_t &p, int16_t &i, int16_t &d){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(134, data, length);
    DataUnpacker unpacker(data, length);
    p = (int16_t)unpacker.popUint16();
    i = (int16_t)unpacker.popUint16();
    d = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_pid_angular_static(int16_t &p, int16_t &i, int16_t &d){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(135, data, length);
    DataUnpacker unpacker(data, length);
    p = (int16_t)unpacker.popUint16();
    i = (int16_t)unpacker.popUint16();
    d = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_pid_angular_dynamic(int16_t &p, int16_t &i, int16_t &d){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(136, data, length);
    DataUnpacker unpacker(data, length);
    p = (int16_t)unpacker.popUint16();
    i = (int16_t)unpacker.popUint16();
    d = (int16_t)unpacker.popUint16();
}

void asservissement_interface::get_odometry_metric(int16_t &sizeWheelLeft, int16_t &sizeWheelRight, int16_t &spaceInterWheel){
    uint8_t data[6];
    int length = 6;
    I2cReceiveData(137, data, length);
    DataUnpacker unpacker(data, length);
    sizeWheelLeft = (int16_t)unpacker.popUint16();
    sizeWheelRight = (int16_t)unpacker.popUint16();
    spaceInterWheel = (int16_t)unpacker.popUint16();
}

void asservissement_interface::start_calibration(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(160, data, length);
}

void asservissement_interface::end_calibration(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(161, data, length);
}

void asservissement_interface::get_status_calibration(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(162, data, length);
}

void asservissement_interface::get_all_status(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(200, data, length);
}

void asservissement_interface::get_all_error(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(201, data, length);
}

void asservissement_interface::get_all_parameter(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(202, data, length);
}

void asservissement_interface::set_all_parameter(){
    uint8_t* data = nullptr;
    int length = 0;
    I2cSendData(203, data, length);
}

#ifdef EMULATE
void asservissement_interface::run_emulate(void){
    auto distance2p = [](int x1, int y1, int x2, int y2) -> int {
        return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    };

    if(emulate_nextTimePop < millis()){
        if(emulate_nextValid == true){
            emulate_action action = enumate_actionQueue.front();
            enumate_actionQueue.pop_front();
            emulate_x = action.m_x;
            emulate_y = action.m_y;
            emulate_nextValid = false;
        }
        if(!enumate_actionQueue.empty()){
            emulate_action action = enumate_actionQueue.front();
            int time = distance2p(emulate_x, emulate_y, action.m_x, action.m_y)*5;
            LOG_DEBUG("Deplacement Time : ",time);
            emulate_nextTimePop = millis() + time;
            emulate_nextValid = true;
        }
    }
}
#endif

// End auto generation CMD_FONCTION
//***********************************************
