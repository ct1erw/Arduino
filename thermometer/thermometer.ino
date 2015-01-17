#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP9808.h>

#define LCD_I2C_ADDR  0x20  // PCF8754

// Create the LCD object
LiquidCrystal_I2C lcd(0x20,8,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
// LCD como duas linhas de 8 char ... mas est~o na mesma linha

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

void setup()
{
  // init lcd
  lcd.init();
  lcd.backlight();

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
  lcd.print("T=");
  lcd.print(c);
  lcd.print("C");
  
  lcd.setCursor(1,1);  
  lcd.print(min);
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
