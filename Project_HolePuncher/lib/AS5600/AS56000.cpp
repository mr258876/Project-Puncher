#include "AS5600.h"

AS5600::AS5600(uint8_t address)
{
  i2cAddress = address;
  i2c_wire = &Wire;
}

AS5600::AS5600(TwoWire *i2c_port, uint8_t address)
{
  i2cAddress = address;
  i2c_wire = i2c_port;
}

uint8_t AS5600::getBurnCount()
{
  return readByte(REG_ZMCO);
}
 
uint16_t AS5600::getAngle()
{
  return readTwoBytes(REG_ANGLE);
}
 
uint16_t AS5600::getRawAngle()
{ 
  return readTwoBytes(REG_RAW_ANGLE);
}

uint16_t AS5600::getStartPos()
{
  return readTwoBytes(REG_ZPOS);
}

void AS5600::setStartPos(uint16_t value)
{
  writeTwoBytes(REG_ZPOS, value);
}

uint16_t AS5600::getEndPos()
{
  return readTwoBytes(REG_MPOS);
}

void AS5600::setEndPos(uint16_t value)
{
  writeTwoBytes(REG_MPOS, value);
}

uint16_t AS5600::getMaxAngle()
{
  return readTwoBytes(REG_MANG);
}

void AS5600::setMaxAngle(uint16_t value)
{
  writeTwoBytes(REG_MANG, value);
}

AS5600_Config AS5600::getConfig()
{
  uint16_t value = readTwoBytes(REG_CONF);
  
  uint8_t low = value & 0x00FF;
  uint8_t high = value >> 8;

  AS5600_Config config = {
    // low 8 bits
    uint8_t(low & 0b00000011),      // powerMode 2 bits
    uint8_t((low & 0b00001100) >> 2), // hysteresis 2 bits
    uint8_t((low & 0b00110000) >> 4), // outputStage 4 bits
    uint8_t((low & 0b11000000) >> 6), // pwmFrequency 2 bits
    // high 6bits
    uint8_t(high & 0b00000011),     // slowFilter 2 bits 
    uint8_t((high & 0b00011100) >> 2),// fast filter threshold 3 bits
    (bool)uint8_t((high & 0b00100000) >> 5)// watchdog 1 bit
  };
  return config;
}

void AS5600::setConfig(AS5600_Config config)
{
  uint8_t low = config.powerMode | \
                config.hysteresis << 2 | \
                config.outputStage << 4 | \
                config.pwmFrequency << 6;

  uint8_t high = config.slowFilter | \
                 config.fastFilterThreshold << 2 | \
                 config.watchdog << 5;
  
  uint16_t value = high << 8 | low;
  writeTwoBytes(REG_CONF, value);
}

AS5600_Status AS5600::getStatus()
{
  uint8_t value = readByte(REG_STATUS);
  // Serial.println((value >> 3));
  // AS5600_Status status = {
  //   (value & B00100000), // 3rd bit is magnet detect
  //   (value & B00010000), // 4th bit is magnet low (too weak)
  //   (value & B00001000) // 5th bit is magnet high (too strong)
  // };
  AS5600_Status status = {
    bool(value & (1 << 3)), // bit3 is magnet high (too strong)
    bool(value & (1 << 4)), // bit4 bit is magnet low (too weak)
    bool(value & (1 << 5))  // bit5 is magnet detected
  };

  return status;
}

uint8_t AS5600::getAutomaticGainControl()
{
  return readByte(REG_AGC);
}

uint16_t AS5600::getMagnitude()
{
  return readTwoBytes(REG_MAGNITUDE);
}

/* HERE BE DRAGONS! */
void AS5600::burnAngle()
{
  writeByte(REG_BURN, BURN_ANGLE);
}

/* HERE BE DRAGONS! */
void AS5600::burnSettings()
{
  writeByte(REG_BURN, BURN_SETTINGS);
}

uint8_t AS5600::readByte(uint8_t addr)
{

  i2c_wire->beginTransmission(i2cAddress);
  i2c_wire->write(addr);
  i2c_wire->endTransmission();
  i2c_wire->requestFrom(i2cAddress, uint8_t(2));
  while(i2c_wire->available() < 1);
  return i2c_wire->read();
}

uint16_t AS5600::readTwoBytes(uint8_t addr)
{
  uint16_t retVal;

  i2c_wire->beginTransmission(i2cAddress);
  i2c_wire->write(addr);
  i2c_wire->endTransmission();
  i2c_wire->requestFrom(i2cAddress, uint8_t(2));
  
  while(i2c_wire->available() < 2);
  retVal = (uint16_t) i2c_wire->read() << 8;
  retVal = retVal | (uint16_t) i2c_wire->read();
  return retVal;
}

void AS5600::writeByte(uint8_t addr, uint8_t value)
{
  i2c_wire->beginTransmission(i2cAddress);
  i2c_wire->write(addr);
  i2c_wire->write(value);
  i2c_wire->endTransmission(); 
}

void AS5600::writeTwoBytes(uint8_t addr, uint16_t value)
{

  uint8_t high = value >> 8;
  uint8_t low = (uint8_t) value & 0x00FF;

  i2c_wire->beginTransmission(i2cAddress);
  i2c_wire->write(addr);
  i2c_wire->write(high);
  i2c_wire->write(low);
  i2c_wire->endTransmission();
  
}