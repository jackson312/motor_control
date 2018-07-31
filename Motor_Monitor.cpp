//
// Created by jackson on 7/27/18.
//

#include "Motor_Monitor.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>

bool Motor_Monitor::open_file(string &filename)
{
    input_file_name = filename;
    infile = new ifstream ( input_file_name.c_str() );

    return infile->is_open();
}

bool Motor_Monitor::read_input()
{
    string s;

    // Rewind file
    infile->seekg(0);

    // Initial setup
    INITIAL_V_DELTA = raw_voltage;
    MAX_ADJUSTED_RAW_V = static_cast<uint8_t>(MAX_P_VALUE) - INITIAL_V_DELTA;
    RAW_V_TO_REAL_V_DIVISOR = MAX_VOLTS/MAX_ADJUSTED_RAW_V;

#if 0
    cout << "First values are time: " << time << " encoder_value: "
         << encoder_value << " raw_voltage: " << (uint16_t) raw_voltage << endl;
#endif

    previous_encoder_value = encoder_value;
    previous_raw_voltage_f = (float)raw_voltage;

    cout << "# Time      Read Pos      Calc Pos" << endl;

    while ( *infile )
    {
        if (!getline(*infile, s))
        {
            break;
        }

        parse_line(time, encoder_value, raw_voltage, s);
#if 0
        cout << "Values time: " << time << " encoder_value: " << encoder_value
             << " raw_voltage: " << (uint16_t) raw_voltage << endl;
#endif


        if ( !calculate_voltage() )
        {
            cout << "Error threshold exceeded at: " << setprecision(4) << time
                 << endl;
            break;
        }
    }

    return true;
}

void Motor_Monitor::parse_line (float &time, int64_t &encoder_value,
                                uint8_t &raw_voltage, string &input_line)
{
    uint16_t tmp_raw_v; // g++ I/O does not seem to handle 8 bit values

    istringstream ss ( input_line );
    // Gets Time
    ss >> skipws >> time;
    // Gets Encoder Value
    ss >> skipws >> encoder_value;
    // Gets Raw voltage
    ss >> skipws >> tmp_raw_v;
    raw_voltage = static_cast<uint8_t>(tmp_raw_v);
}

bool Motor_Monitor::calculate_voltage ()
{
    float calculated_raw_voltage_f;
    int64_t calculated_raw_voltage;
    int64_t delta_value = 0;
    float calc_voltage_change;
    float real_voltage;
    int64_t pet_delta;

    delta_value = encoder_value - previous_encoder_value;

    calc_voltage_change = ((float)delta_value * VOLTAGE_DELTA_PER_ENCODE_DELTA);

    calculated_raw_voltage_f = previous_raw_voltage_f + calc_voltage_change;
    calculated_raw_voltage = static_cast<int64_t>(round(calculated_raw_voltage_f));

    real_voltage = fabs((trunc(calculated_raw_voltage_f) - INITIAL_V_DELTA) *  RAW_V_TO_REAL_V_DIVISOR);

#if 0
    cout << "Calc Raw Voltage: " <<  setprecision(5) << calculated_raw_voltage_f
         << "/" << calculated_raw_voltage << "  delta_value: " << delta_value
         << "  real voltage: " << setprecision(3) << real_voltage << endl << endl;
#endif

    // Output with calculated position
    //cout << left << setw(12) << time << setw(9) << calculated_raw_voltage << endl;

    // Output with read in position
    //cout << left << setw(12) << time << setw(9) << static_cast<uint16_t>(raw_voltage) << endl;

    // Output showing deltas
    cout << left << setw(12) << time << setw(9) << static_cast<uint16_t>(raw_voltage) << calculated_raw_voltage <<endl;

    previous_raw_voltage_f = calculated_raw_voltage_f;
    previous_encoder_value = encoder_value;

    pet_delta = abs(calculated_raw_voltage - static_cast<int64_t>(raw_voltage));
    largest_pet_delta ( pet_delta );

    if ( (pet_delta > ERROR_THRESHOLD) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Motor_Monitor::calibrate_motor_monitor ()
{
    uint64_t i = 0;
    float calibrated_value = 0.0;
    string s;

    while ( *infile )
    {
        if (!getline(*infile, s))
        {
            break;
        }

        parse_line(time, encoder_value, raw_voltage, s);

        if ( time <= LAST_CALIBRATION_TIME)
        {
            i++;
            calibrated_value += raw_voltage;
        }
        else
        {
            break;
        }
    }

    calibrated_value = calibrated_value / static_cast<float>(i);

    //cout << calibrated_value << "!!!!!!" << endl;

    INITIAL_V_DELTA = static_cast<uint8_t>(round(calibrated_value));
}

inline void Motor_Monitor::largest_pet_delta ( int64_t curr_delta )
{
    if (curr_delta > largest_delta)
    {
        largest_delta = curr_delta;
    }
}

void Motor_Monitor::print_largest_delta ()
{
    cout << "# The largest delta was: " << largest_delta << endl;
}

Motor_Monitor::~Motor_Monitor()
{
    delete infile;
}