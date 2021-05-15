int maxTEMP = 22; // default temperatur (°C) when cooler is switched on (Relais 1 on) at boot
int minFLOW = 30; // min Water Flow (l/hour) befor warning (Relais 2 on) - YF- S401 = Q60 / YF- S201 = Q7.5 / YF- S402 = Q73
int maxSetTEMP = 25; // max Set Temperatur
int minSetTEMP = 15; // min Set Temperatur

// Pinout OLED SDA = A4 | SCL = A5
const int buttonPin1 = 8, buttonPin2 = 9; // Digital imput for Button 1 and 2 (default D8 and D9)
const int flowSensorPin = 2; // Digital imput flow Sensor Pin (default D2
const int tempSensorPin = 3; // Digital imput Tempatur Sensor Pin (default D3)
const int relais1 = 4; // Relais 1 Pinout (default D4)
const int relais2 = 5; // Relais 2 Pinout (default D5)
const int relais3 = 6; // Relais 3 Pinout (default D6)
const int relais4 = 7; // Relais 4 Pinout (default D7)

int button1_State = 0, button2_State = 0;
int count_value = maxTEMP;
int counter = 0;
int read_relais1;
int read_relais2;

//OLED Display Code
#include "U8g2lib.h" //Libary Display
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
//OLED Dsiplay Code end

//TEMP Sesnor
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(tempSensorPin);
DallasTemperature sensors(&oneWire);
//TEMP Sensor END

/*
  Arduino Water flow meter
  YF- S401 Hall Effect Water Flow Sensor
  Water Flow Sensor output processed to read in litres/hour
*/
//Water flow
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int lhour; // Calculated litres/hour
unsigned char flowsensor = flowSensorPin; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void flow () // Interrupt function
{
  flow_frequency++;
}
//Water flow end

void setup()
{  
  pinMode(relais1, OUTPUT);
  digitalWrite(relais1, HIGH);
  pinMode(relais2, OUTPUT);
  digitalWrite(relais2, HIGH);
  pinMode(relais3, OUTPUT);
  digitalWrite(relais3, HIGH);
  pinMode(relais4, OUTPUT);
  digitalWrite(relais4, HIGH);

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  //Water flow
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(0, flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
  //Water flow end

  u8g2.begin();
  Serial.begin(9600);
}

void loop ()
{
  //Counter for Display status
  counter=counter+1;
  if (counter == 2) {
    counter=0;
    }
  
  //Water flow
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if (currentTime >= (cloopTime + 1000))
  {
    cloopTime = currentTime; // Updates cloopTime
    // Pulse frequency (Hz) = 93Q, Q is flow rate in L/min.
    lhour = (flow_frequency * 60 / 93); // (Pulse frequency x 60 min) / 93Q = flowrate in L/hour
    flow_frequency = 0; // Reset Counter

    if (lhour < minFLOW) {
      digitalWrite(relais2, LOW);
    }
    else {
      digitalWrite(relais2, HIGH);
    }

    sensors.requestTemperatures();

    if (sensors.getTempCByIndex(0) > count_value) {
      digitalWrite(relais1, LOW);
    }
    else {
      digitalWrite(relais1, HIGH);
    }

  }
  //Water flow end

  sensors.requestTemperatures(); //TEMP Sensor

  button1_State = digitalRead(buttonPin1);
  button2_State = digitalRead(buttonPin2);

  if (button1_State == HIGH && button2_State == LOW && count_value < maxSetTEMP) {
    count_value++;
  }
  if (button2_State == HIGH && button1_State == LOW && count_value > minSetTEMP) {
    count_value--;
  }
  if (button1_State == HIGH && button2_State == HIGH) {
    count_value = maxTEMP;
  }

  float f_val = sensors.getTempCByIndex(0);
  char outstr[15];
  dtostrf(f_val, 2, 1, outstr);

  float f_val2 = lhour;
  char outstr2[15];
  dtostrf(f_val2, 4, 0, outstr2);

  float f_val3 = count_value;
  char outstr3[15];
  dtostrf(f_val3, 4, 0, outstr3);

  read_relais1 = digitalRead(relais1);
  read_relais2 = digitalRead(relais2);
  
  //OLED Display
  u8g2.firstPage();
  do {

    u8g2.setFont(u8g_font_7x14);
    //u8g2.setFont(u8g_font_unifont);
    u8g2.clearBuffer();
    u8g2.drawStr(8, 10, "-K40 Display-");
    if (counter < 1) {
      u8g2.drawStr(2, 10, "*");
    }
      if (read_relais1==LOW) {
      u8g2.drawStr(100, 10, "1");
    }
      if (read_relais2==LOW) {
      u8g2.drawStr(107, 10, "2");
    }
    u8g2.drawLine(0, 13, 128, 13);
    u8g2.setFont(u8g_font_9x15B);
    u8g2.drawStr(2, 35, outstr);
    u8g2.drawStr(36, 35, "\xb0" "C");
    u8g2.drawLine(55, 23, 59, 35);
    u8g2.drawStr(62, 35, outstr2);
    u8g2.drawStr(92, 35, " l/h");
    u8g2.drawLine(0, 45, 128, 45);
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(2, 61, "set Temp = ");
    u8g2.drawStr(70, 61, outstr3);
    u8g2.drawStr(102, 61, "\xb0" "C");

  } while ( u8g2.nextPage() );

  //OLED Display end
  //delay(1);
}
