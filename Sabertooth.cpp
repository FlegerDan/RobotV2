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

#include "Sabertooth.h"
#include "ArduinoMath.h"//compatibility with arduino math methods

extern Serial debugPort;

uint32_t Sabertooth::setTestRunPeriod(uint32_t period){
  testRunPeriod = period;
  return (period);
}

uint32_t Sabertooth::setTestRunA(uint32_t aSpeed){
  if (aSpeed > 300){
    testRunA = 0;
  } else if (aSpeed > 255){
    testRunA = 255;
  }
  else{
    testRunA = aSpeed;
  }
  return (testRunA);
}

uint32_t Sabertooth::setTestRunB(uint32_t bSpeed){
  if (bSpeed > 300){
    testRunB = 0;
  } else if (bSpeed > 255){
    testRunB = 255;
  }
  else{
    testRunB = bSpeed;
  }
  return (testRunB);
}

uint32_t Sabertooth::runTest(uint32_t none){
  go(testRunA, testRunB, testRunPeriod / 1000);
  return (0);
}


Sabertooth::Sabertooth(PinName Tx, uint8_t address, int baudrate) : _sabertoothserial(Tx, NC) {
    _address = address;
    _sabertoothserial.baud(baudrate);
    lastDrive = 0;
    lastTurn = 0;
}

// Sabertooth::Sabertooth() : _sabertoothserial(NC, NC){
//   //_sabertoothserial = s;
// }

void Sabertooth::initializeComunication(void){
    wait(1.5); // wait for sabertooth to initialized
    _sabertoothserial.putc(START_CHARACTER);
    wait(0.5);
}

void Sabertooth::go(int aSpeed, int bSpeed){
  motor(1, aSpeed);
  motor(2, bSpeed);
}

void Sabertooth::stop(){
  go(0, 0);
}

void Sabertooth::goDT(int drivePower, int turnPower){
  // drive(drivePower);
  // turn(turnPower);
//  if ((lastDrive != drivePower)  || (lastTurn != turnPower)){
    //debugPort.printf("[Saber] drive: %d\tturn: %d\n\r", drivePower, turnPower);
    lastDrive = drivePower;
    lastTurn = turnPower;
    drive(drivePower);
    turn(turnPower);
//  }
}

void Sabertooth::go(int aSpeed, int bSpeed, float secs){
  go(aSpeed, bSpeed);
  timeOff.attach(callback(this,&Sabertooth::stop), secs);   // time to off
}

void Sabertooth::goDT(int drivePower, int turnPower, float secs){
  goDT(drivePower, turnPower);
  timeOff.attach(callback(this,&Sabertooth::stop), secs);   // time to off
}


void Sabertooth::command(uint8_t command, uint8_t value)  {
    _sabertoothserial.putc(_address);
    _sabertoothserial.putc(command);
    _sabertoothserial.putc(value);
    _sabertoothserial.putc((_address+command+value) & 0x7f); //checksum
}

void Sabertooth::throttleCommand(uint8_t command, int power)
{
  power = ArduinoMath::constrain(power, -126, 126);
  this->command(command, (uint8_t)abs(power));
}

void Sabertooth::motor(int power)
{
  motor(1, power);
}

void Sabertooth::motor(uint8_t motor, int power)
{
  if (motor < 1 || motor > 2) { return; }
  throttleCommand((motor == 2 ? 4 : 0) + (power < 0 ? 1 : 0), power);
}

void Sabertooth::drive(int power)
{
  throttleCommand(power < 0 ? 9 : 8, power);
}

void Sabertooth::turn(int power)
{
  throttleCommand(power < 0 ? 11 : 10, power);
}

void Sabertooth::setMinVoltage(uint8_t value)
{
  command(2, (uint8_t)ArduinoMath::min(value, 120));
}

void Sabertooth::setMaxVoltage(uint8_t value)
{
  command(3, (uint8_t)ArduinoMath::min(value, 127));
}

void Sabertooth::setDeadband(uint8_t value)
{
  command(17, (uint8_t)ArduinoMath::min(value, 127));
}

void Sabertooth::setRamping(uint8_t value)
{
  command(16, (uint8_t)ArduinoMath::constrain(value, 0, 80));
}

void Sabertooth::setTimeout(int milliseconds)
{
  command(14, (uint8_t)((ArduinoMath::constrain(milliseconds, 0, 12700) + 99) / 100));
}


void Sabertooth::setBaudRate(int baudRate)
{
  uint8_t value;
  switch (baudRate)
  {
    case 2400:           value = 1; break;
    case 9600:  default: value = 2; baudRate = 9600; break;
    case 19200:          value = 3; break;
    case 38400:          value = 4; break;
    case 115200:         value = 5; break;
  }
  command(15, value);
  _sabertoothserial.baud(baudRate);
  // Sabertooth takes about 200 ms after setting the baud rate to
  // respond to commands again (it restarts).
  // So, this 500 ms wait should deal with this.
  wait(0.5);
}
