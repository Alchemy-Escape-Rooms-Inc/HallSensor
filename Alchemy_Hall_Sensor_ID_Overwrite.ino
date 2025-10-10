#include <Wire.h>

static const uint8_t OLD_ADDR = 0x00;
static const uint8_t NEW_ADDR = 0x67;
static const int SDA_PIN = 15;
static const int SCL_PIN = 18;

static const uint8_t CUSTOMER_ACCESS_REG = 0x35;
static const uint32_t CUSTOMER_ACCESS_CODE = 0x2C413534;
static const uint8_t ADDRESS_REG = 0x02;
static const uint8_t EEPROM_CTRL_REG = 0x0B;  // EEPROM control

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

void setup(){
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n=== ALS31300 Address Programmer (Fixed Bit Positions) ===");
  
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  
  Wire.beginTransmission(OLD_ADDR);
  if(Wire.endTransmission() != 0){
    Serial.println("ERROR: No device found");
    return;
  }
  Serial.println("✓ Device found at 0x00");
  
  // Enter Customer Access Mode
  Serial.println("Entering Customer Access Mode...");
  if(!write32(OLD_ADDR, CUSTOMER_ACCESS_REG, CUSTOMER_ACCESS_CODE)){
    Serial.println("ERROR: Failed customer access!");
    return;
  }
  delay(50);
  
  // Read register 0x02
  uint32_t regValue;
  if(!read32(OLD_ADDR, ADDRESS_REG, regValue)){
    Serial.println("ERROR: Failed to read!");
    return;
  }
  Serial.print("Current register 0x02: 0x");
  Serial.println(regValue, HEX);
  
  // Update address - slaveAddress is bits 10-16 (7 bits starting at bit 10)
  uint32_t addressMask = 0x7F << 10;  // Bits 10-16
  regValue = (regValue & ~addressMask) | ((NEW_ADDR & 0x7F) << 10);
  
  Serial.print("New register value: 0x");
  Serial.println(regValue, HEX);
  
  // Write new address
  if(!write32(OLD_ADDR, ADDRESS_REG, regValue)){
    Serial.println("ERROR: Write failed!");
    return;
  }
  delay(50);
  
  // Try EEPROM write command (might be needed to make it permanent)
  Serial.println("Attempting EEPROM burn...");
  write32(OLD_ADDR, EEPROM_CTRL_REG, 0x00000002);  // Enable EEPROM write
  delay(10);
  write32(OLD_ADDR, EEPROM_CTRL_REG, 0x00000006);  // Burn to EEPROM
  delay(200);  // EEPROM write time
  write32(OLD_ADDR, EEPROM_CTRL_REG, 0x00000000);  // Lock
  
  Serial.println("\n✓✓✓ SUCCESS ✓✓✓");
  Serial.println("*** PHYSICALLY UNPLUG THE SENSOR (JST CONNECTOR) ***");
  Serial.println("Wait 3 seconds, then reconnect.");
  Serial.print("Should appear at 0x");
  Serial.println(NEW_ADDR, HEX);
}

void loop(){}