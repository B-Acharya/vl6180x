#include <Wire.h>
#define VL6180X_ADDRESS 0x29
void setup() {
  // initialization of vl6180 setting gpio to 1 
  Wire.begin();                // join i2c bus (address optional for master)
  Serial.begin(115200);          // start serial communication at 9600bps
  Wire.beginTransmission(43); //GPIO expander address 
  Wire.write(14);   //GPDR to set pin direction
  Wire.write(0x01);  //GPIO_0 set as output
  Wire.endTransmission();
  Wire.beginTransmission(43); //GPIO expander address 
  Wire.write(12);   //GPDR to set pin state
  Wire.write(0x01);  //GPIO_0 set as 1
  Wire.endTransmission();
  delay(100);
  // Mandatory : private registers
  if (ReadByte(0x016) == 1){
    WriteByte(0x0207, 0x01);
    WriteByte(0x0208, 0x01);
    WriteByte(0x0096, 0x00);
    WriteByte(0x0097, 0xfd);
    WriteByte(0x00e3, 0x01);
    WriteByte(0x00e4, 0x03);
    WriteByte(0x00e5, 0x02);
    WriteByte(0x00e6, 0x01);
    WriteByte(0x00e7, 0x03);
    WriteByte(0x00f5, 0x02);
    WriteByte(0x00d9, 0x05);
    WriteByte(0x00db, 0xce);
    WriteByte(0x00dc, 0x03);
    WriteByte(0x00dd, 0xf8);
    WriteByte(0x009f, 0x00);
    WriteByte(0x00a3, 0x3c);
    WriteByte(0x00b7, 0x00);
    WriteByte(0x00bb, 0x3c);
    WriteByte(0x00b2, 0x09);
    WriteByte(0x00ca, 0x09);
    WriteByte(0x0198, 0x01);
    WriteByte(0x01b0, 0x17);
    WriteByte(0x01ad, 0x00);
    WriteByte(0x00ff, 0x05);
    WriteByte(0x0100, 0x05);
    WriteByte(0x0199, 0x05);
    WriteByte(0x01a6, 0x1b);
    WriteByte(0x01ac, 0x3e);
    WriteByte(0x01a7, 0x1f);
    WriteByte(0x0030, 0x00);
    // Recommended : Public registers - See data sheet for more detail
    WriteByte(0x0011, 0x10); // Enables polling for ‘New Sample ready’
                             // when measurement completes
    WriteByte(0x010a, 0x30); // Set the averaging sample period
                             // (compromise between lower noise and
                             // increased execution time)
    WriteByte(0x003f, 0x46); // Sets the light and dark gain (upper
                             // nibble). Dark gain should not be
                             // changed.
    WriteByte(0x0031, 0xFF); // sets the # of range measurements after
                             // which auto calibration of system is
                             // performed
    WriteByte(0x0041, 0x63); // Set ALS integration time to 100ms
  
    WriteByte(0x002e, 0x01); // perform a single temperature calibration
                             // of the ranging sensor
    //Optional: Public registers - See data sheet for more detail
    WriteByte(0x001b, 0x09); // Set default ranging inter-measurement
                             // period to 100ms
    WriteByte(0x003e, 0x31); // Set default ALS inter-measurement period
                             // to 500ms
    WriteByte(0x0014, 0x24); // Configures interrupt on ‘New Sample
                             // Ready threshold event’  
  }  
  Serial.println("Set-up complete");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Loop starts");
  WriteByte(0x018,0x01);        // VL6180X_Start_Range();
  // VL6180X_Poll_Range();
  while ((ReadByte(0x04f) & 0x07) != 4)
    delay(1);
  int range = ReadByte(0x062);  // VL6180X_Read_Range();
  Serial.println(range);
  WriteByte(0x015,0x07);        // VL6180X_Clear_Interrupts();
  delay(100);
  Serial.println("Loop ends");
}


void WriteByte (uint16_t subAddress, uint8_t data) {
  Wire.beginTransmission(VL6180X_ADDRESS);
  Wire.write((subAddress >> 8) & 0xFF);
  Wire.write(subAddress & 0xFF);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t ReadByte (uint16_t subAddress) {
  Wire.beginTransmission(VL6180X_ADDRESS);
  Wire.write((subAddress >> 8) & 0xFF);
  Wire.write(subAddress & 0xFF);
  Wire.endTransmission(false);
  Wire.requestFrom(VL6180X_ADDRESS, 1);
  return Wire.read();
}
