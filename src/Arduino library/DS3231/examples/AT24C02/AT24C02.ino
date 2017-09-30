//Demo for DS3231 RTC module
//by Open-Smart Team and Catalex Team
//Store: http://www.aliexpress.com/store/1199788
//      http://dx.com
//Demo Function: To write and read the AT24C02 on the module 

#include <Wire.h>
int i = 0;
int val; 
void Writebyte(byte DeviceAddress, byte DataAddress, byte Data)
{
  int rData = Data;
  Wire.beginTransmission(DeviceAddress);
  Wire.write(DataAddress);
  Wire.write(rData);
  Wire.endTransmission();
  delay(10);
}

byte Readbyte(int DeviceAddress, byte DataAddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(DeviceAddress);
  Wire.write(DataAddress);
  Wire.endTransmission();
  Wire.requestFrom(DeviceAddress, 1);
  delay(10);
  if (Wire.available())
  {
    rdata = Wire.read();
  }
  delay(10);
  return rdata;
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  for(int i=0;i<16;i++)
    {
     Writebyte(0x57, i, i+1);
    }
}

void loop()
{
  Serial.println("Reading:");
  for(i=0;i<16;i++)
    {     
      val = Readbyte(0x57, i); //access an address from the memory
      Serial.print(val); 
      Serial.print(" ");
  }
  Serial.println("");
  delay(2000);   

}
