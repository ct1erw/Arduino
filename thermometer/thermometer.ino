#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP9808.h>

// Code from:
// http://www.instructables.com/id/Clock-with-termometer-using-Arduino-i2c-16x2-lcd-D/?lang=pt
// https://bitbucket.org/fmalpartida/new-liquidcrystal

#define LCD_I2C_ADDR  0x20  // PCF8754

// Create the LCD object
LiquidCrystal_I2C lcd(0x20,8,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
// LCD como duas linhas de 8 char ... mas est~o na mesma linha

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

#define THERM_ICO_CUR  1
#define THERM_ICO_MIN  2

byte thermo_ico_cur[8] = //icon for thermometer
{
    B01110,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte thermo_ico_min[8] = //icon for thermometer
{
    B01110,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B11111,
    B01110
};

void setup()
{
  // init lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(THERM_ICO_CUR,thermo_ico_cur);    // icon for current temperature
  lcd.createChar(THERM_ICO_MIN,thermo_ico_min); // icon for max temperature
  
  // init serial
  Serial.begin(9600);
  Serial.println("MCP9808 demo");
  
  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
}

int count = 0;
float max = 0;
float min = 200;

void loop()
{
  // Read the temperature and convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;

  static float avg = c; 
  
  // Record max temperature
  if(c > max) {
    max = c;
  }
  // Record min temperature
  if(c < min) {
    min = c;
  }
  // Avergage
  avg = (avg + c)/2;
  
  // Send to LCD
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.write(THERM_ICO_CUR); // write thermo_ico_cur
  lcd.print(c);
  lcd.print((char)223); //degree sign
  lcd.print("C");
  
  lcd.setCursor(0,1);
  lcd.write(THERM_ICO_MIN); // write thermo_ico_min  
  lcd.print(min);
  lcd.print((char)223); //degree sign
  lcd.print("C");
  //lcd.print(max);
  
  // Send to serial port
  //  Serial.print("Temp: "); Serial.print(c); Serial.print("*C\t"); 
  //  Serial.print(f); Serial.println("*F");
  Serial.print(millis());  // miliseconds since boot 
  Serial.print(",");
  Serial.print(c);  // temperature in Celsius 
  Serial.print(",");
  Serial.print(min);  // max temperature in Celsius 
  Serial.print(",");
  Serial.print(max);  // min temperature in Celsius  
  Serial.print(",");
  Serial.print(avg);  // avg temperature in Celsius  
  Serial.println(",0,0"); // dummy data
  
  delay(1000);
}
