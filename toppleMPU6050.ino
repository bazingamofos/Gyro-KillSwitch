
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int MPU_addr=0x68; 
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, Gyz;
int minVal=265;
int maxVal=402;
const int LED=7;
const int killSwitch=2;
int switchVal=0;
// int RELAY=12;
double x;
double y;
double z;
void setup()
{ 
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
   display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  Wire.begin();
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600); 
  pinMode(LED, OUTPUT);
  pinMode(killSwitch, INPUT);
}

void loop()
{
  switchVal=digitalRead(killSwitch);
  Serial.println(switchVal);
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);

  x= RAD_TO_DEG*(atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG*(atan2(-xAng, -zAng)+PI); 
  z= RAD_TO_DEG*(atan2(-yAng, -xAng)+PI);
  Serial.print("AngleX="); 
  Serial.println(x);
  Serial.print("AngleY=");
  Serial.println(y); 
  Serial.println("---------------------"); 
// // Display Text
//   display.setTextSize(1.5);
//   display.setTextColor(WHITE);
//   display.setCursor(0, 0);
//   display.println("AngleX: ");
//   display.display();

//   display.setTextSize(2);
//   display.setTextColor(WHITE);
//   display.setCursor(10, 0);
//   display.println();
//   display.println(x);
//   display.display();

  if((x>40&&x<320)||(y>40&&y<320))
  {
    digitalWrite(LED, HIGH);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("About to activate");
    display.println("kill switch");
    display.display();
  }
  else
  {
    digitalWrite(LED,LOW);
    display.clearDisplay();
    display.display();
  }

  if((x>50&&x<310) ||(y>50&&y<310))
  {
    delay(5000);
    //digitalWrite(RELAY,HIGH);
    digitalWrite(LED, HIGH);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Kill switch activated");
    display.display();
    exit(0);
    Serial.println("hi");
  } 
  else
  {
    //digitalWrite(RELAY, LOW);
    digitalWrite(LED,LOW);
    display.clearDisplay();
    display.display();
  }

    delay(400);
}
