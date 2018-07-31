//
// Created by jackson on 7/27/18.
//

#ifndef MOTOR_MONITOR_H
#define MOTOR_MONITOR_H

#include <cstdint>
#include <string>
#include <fstream>

using namespace std;

class Motor_Monitor
{

public:

    bool open_file ( string &filename );

    bool read_input ();
    void calibrate_motor_monitor ();
    void print_largest_delta ();
    ~Motor_Monitor ();

private:

    void parse_line (float &time, int64_t &encoder_value, uint8_t &raw_voltage, string &input_line);

    bool calculate_voltage ();

    inline void largest_pet_delta ( int64_t curr_delta );


    string input_file_name;

    // Some Constants
    const float NUM_ENCODER_TICKS_PER_REV = 2048;
    const float NUM_REVS_TO_MAX_P_VALUE = 30.0;
    const float MAX_VOLTS = 5.0;
    float MAX_P_VALUE = 255.0;
    const float VOLTAGE_DELTA_PER_ENCODE_DELTA = MAX_P_VALUE / (NUM_REVS_TO_MAX_P_VALUE * NUM_ENCODER_TICKS_PER_REV);
    const float LAST_CALIBRATION_TIME = 0.5;

    const uint64_t ERROR_THRESHOLD = 5;

    uint8_t INITIAL_V_DELTA = 0;

    float time = 0.0;
    uint8_t raw_voltage = 0;
    int64_t encoder_value = 0;
    float previous_raw_voltage_f = 0.0;
    int64_t previous_encoder_value = 0;
    ifstream *infile = nullptr; // input file stream
    int64_t largest_delta = 0;
    uint8_t MAX_ADJUSTED_RAW_V = 0;

    float RAW_V_TO_REAL_V_DIVISOR = 0.0;
};

#endif // MOTOR_MONITOR_H
