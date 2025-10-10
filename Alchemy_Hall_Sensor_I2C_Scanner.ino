#include <Wire.h>
static const int SDA_PIN = 15;
static const int SCL_PIN = 18;

void setup(){
  Serial.begin(115200); delay(300);
  Serial.println("\n=== I2C BRUTE-FORCE SCANNER ===");
  Serial.println("Scanning 0x00 to 0x7F...\n");
  
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);
  
  int found = 0;
  
  // Scan EVERY address including 0x00
  for(uint8_t addr = 0x00; addr <= 0x7F; addr++){
    Wire.beginTransmission(addr);
    uint8_t error = Wire.endTransmission();
    
    if(error == 0){
      Serial.print("Device found at 0x");
      if(addr < 16) Serial.print("0");
      Serial.print(addr, HEX);
      Serial.print(" (decimal ");
      Serial.print(addr);
      Serial.println(")");
      found++;
    }
    delay(5); // Small delay for stability
  }
  
  if(found == 0){
    Serial.println("\nNO devices found!");
    Serial.println("Check:");
    Serial.println("  - Wiring (SDA=15, SCL=18)");
    Serial.println("  - Power to sensor");
    Serial.println("  - JST connector firmly seated");
  } else {
    Serial.print("\nTotal devices found: ");
    Serial.println(found);
  }
}

void loop(){}