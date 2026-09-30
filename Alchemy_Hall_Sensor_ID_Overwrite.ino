#include <Wire.h>

static const uint8_t NEW_ADDR = 0x64;
static const int SDA_PIN = 15;
static const int SCL_PIN = 18;

static const uint8_t CUSTOMER_ACCESS_REG = 0x35;
static const uint32_t CUSTOMER_ACCESS_CODE = 0x2C413534;
static const uint8_t ADDRESS_REG = 0x02;
static const uint8_t EEPROM_CTRL_REG = 0x0B;

bool write32(uint8_t addr, uint8_t reg, uint32_t value){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write((value >> 24) & 0xFF);
  Wire.write((value >> 16) & 0xFF);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  return Wire.endTransmission() == 0;
}

bool read32(uint8_t addr, uint8_t reg, uint32_t &value){
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if(Wire.endTransmission(false) != 0) return false;

  if(Wire.requestFrom((int)addr, 4) != 4) return false;

  value = ((uint32_t)Wire.read() << 24) |
          ((uint32_t)Wire.read() << 16) |
          ((uint32_t)Wire.read() << 8) |
          Wire.read();

  return true;
}

uint8_t findSensor(){
  for(uint8_t addr = 0x60; addr <= 0x67; addr++){
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) return addr;
  }
  // Also check address 0x00 (factory default when ADR pins tied to VCC)
  Wire.beginTransmission(0x00);
  if(Wire.endTransmission() == 0) return 0x00;
  return 0xFF; // not found
}

void setup(){
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n=== ALS31300 Address Programmer ===");
  Serial.print("Target address: 0x");
  Serial.println(NEW_ADDR, HEX);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  // Find sensor
  uint8_t sensorAddr = findSensor();
  if(sensorAddr == 0xFF){
    Serial.println("ERROR: No sensor found!");
    return;
  }
  Serial.print("Found sensor at 0x");
  Serial.println(sensorAddr, HEX);

  if(sensorAddr == NEW_ADDR){
    Serial.println("Sensor already at target address. No programming needed.");
    return;
  }

  // Enter Customer Access Mode
  Serial.println("Entering Customer Access Mode...");
  if(!write32(sensorAddr, CUSTOMER_ACCESS_REG, CUSTOMER_ACCESS_CODE)){
    Serial.println("ERROR: Failed customer access!");
    return;
  }
  delay(50);

  // Read register 0x02
  uint32_t regValue;
  if(!read32(sensorAddr, ADDRESS_REG, regValue)){
    Serial.println("ERROR: Failed to read register 0x02!");
    return;
  }
  Serial.print("Current register 0x02: 0x");
  Serial.println(regValue, HEX);
  Serial.print("Current EEPROM address (bits 10-16): 0x");
  Serial.println((regValue >> 10) & 0x7F, HEX);

  // Build clean register value matching working sensors:
  // bits 0-4 customerEeprom=1, bit5 intLatch=1, bit6-8 XYZ=1, bit9 i2cThresh=1
  // bits 10-16 slaveAddress, bit17 disableSlaveAdc=0, rest=0
  regValue = 0x000003E1 | ((NEW_ADDR & 0x7F) << 10);

  Serial.print("New register value: 0x");
  Serial.println(regValue, HEX);

  // Write new address
  if(!write32(sensorAddr, ADDRESS_REG, regValue)){
    Serial.println("ERROR: Write failed!");
    return;
  }
  delay(50);

  // Burn to EEPROM to make permanent
  Serial.println("Burning to EEPROM...");
  write32(sensorAddr, EEPROM_CTRL_REG, 0x00000002);  // Enable EEPROM write
  delay(10);
  write32(sensorAddr, EEPROM_CTRL_REG, 0x00000006);  // Burn to EEPROM
  delay(200);  // EEPROM write time
  write32(sensorAddr, EEPROM_CTRL_REG, 0x00000000);  // Lock

  Serial.println("\n*** SUCCESS ***");
  Serial.println("*** PHYSICALLY UNPLUG THE SENSOR (JST CONNECTOR) ***");
  Serial.println("Wait 3 seconds, then reconnect.");
  Serial.print("Should appear at 0x");
  Serial.println(NEW_ADDR, HEX);
}

void loop(){}
