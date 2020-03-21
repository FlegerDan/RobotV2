/*
Arduino Library for SyRen/Sabertooth Packet Serial
Copyright (c) 2012-2013 Dimension Engineering LLC
http://www.dimensionengineering.com/arduino

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE
USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
* Ported to mbed by Litan Virgil, 2017
* I added some features that I found useful.
* Also I removed "const" methods
*/

#ifndef _SABERTOOTH_H_
#define _SABERTOOTH_H_

#include "mbed.h"

#define START_CHARACTER 0xAA

class Sabertooth {

public:
  /*!
  Initializes a new instance of the Sabertooth class.
  The driver address and the TX serial pin used is set to the value given, and the baudrate
  \param tx The TX pin used.
  \param address The driver address.
  \param baudrate The baurate of the serial port.
  */
    Sabertooth(PinName tx, uint8_t address, int baudrate);
    //Sabertooth();
  /*
  Initializes the comunication eith the driver
  */
    void initializeComunication(void);
    /*!
    Sets the power of motor 1.
    \param power The power, between -127 and 127.
    */
    void motor(int power);
    /*!
    Sets the power of the specified motor.
    \param motor The motor number, 1 or 2.
    \param power The power, between -127 and 127.
    */
    void motor(uint8_t motor, int power);
    /*!
    Sets the driving power.
    \param power The power, between -127 and 127.
    */
    void drive(int power);
    /*!
    Sets the turning power.
    \param power The power, between -127 and 127.
    */
    void turn(int power);
    /*!
    Stops.
    */
    void stop();
    /*!
    sets the speed of each motor
    \param aPower The power for motor a
    \param bPower The power of motor b
    */
    void go(int aPower, int bPower);
    /*!
    sets the drive and the turn power
        the driver must receive both the drive and turn commands in order to
        apply them to the motors
    \param drivePower The power for driving
    \param turnPower The power for turning
    */
    void goDT(int drivePower, int turnPower);
    /*!
    sets the speed of each motor for a limitet amount of time
    \param aPower The power for motor a
    \param bPower The power of motor b
    \param secs The amount of time after which the motors Stops
    */
    void go(int aSpeed, int bSpeed, float secs);
    /*!
    sets the drive and the turn power for a limited amount of time
        the driver must receive both the drive and turn commands in order to
        apply them to the motors
    \param drivePower The power for driving
    \param turnPower The power for turning
    \param secs The amount of time after which the motors Stops
    */
    void goDT(int drivePower, int turnPower, float secs);
    /*!
    Sets the minimum voltage.
    \param value The voltage. The units of this value are driver-specific and are specified in the Packet Serial chapter of the driver's user manual.
    */
    void setMinVoltage(uint8_t value);
    /*!
    Sets the maximum voltage.
    Maximum voltage is stored in EEPROM, so changes persist between power cycles.
    \param value The voltage. The units of this value are driver-specific and are specified in the Packet Serial chapter of the driver's user manual.
    */
    void setMaxVoltage(uint8_t value);
    /*!
    Sets the deadband.
    Deadband is stored in EEPROM, so changes persist between power cycles.
    \param value The deadband value.
                 Motor powers in the range [-deadband, deadband] will be considered in the deadband, and will
                 not prevent the driver from entering nor cause the driver to leave an idle brake state.
                 0 resets to the default, which is 3.
    */
    void setDeadband(uint8_t value);
    /*!
    Sets the ramping.
    Ramping is stored in EEPROM, so changes persist between power cycles.
    \param value The ramping value. Consult the user manual for possible values.
    */
    void setRamping(uint8_t value);
    /*!
    Sets the serial timeout.
    \param milliseconds The maximum time in milliseconds between packets. If this time is exceeded,
                        the driver will stop the motors. This value is rounded up to the nearest 100 milliseconds.
                        This library assumes the command value is in units of 100 milliseconds. This is true for
                        most drivers, but not all. Check the packet serial chapter of the driver's user manual
                        to make sure.
    */
    void setTimeout(int milliseconds);
    /*!
    Sets the baud rate.
    Baud rate is stored in EEPROM, so changes persist between power cycles.
    \param baudRate The baud rate. This can be 2400, 9600, 19200, 38400, or on some drivers 115200.
    */
    void setBaudRate(int baudRate);
    uint32_t setTestRunPeriod(uint32_t);
    uint32_t setTestRunA(uint32_t);
    uint32_t setTestRunB(uint32_t);
    uint32_t runTest(uint32_t none = 0);
    uint8_t _address;
    PinName pin;
private:
  /*!
  Sends a packet serial command to the motor driver.
  \param command The number of the command.
  \param value   The command's value.
  */
    void command(uint8_t command, uint8_t value);
    void throttleCommand(uint8_t command, int power);
    Serial _sabertoothserial;
    Timeout timeOff;
    uint32_t  testRunPeriod;
    uint32_t  testRunA;
    uint32_t  testRunB;
    int       lastDrive;
    int       lastTurn;
};

#endif
