#include <SCoop.h>
#include <Wire.h>
#include <math.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int BH1750address = 0x23;//BH1750 I2C address
byte buff[2];

defineTask(light_control);  /*define child thread for light control*/
defineTask(heat_control);   /*define child thread for heat control*/
defineTask(motor_control);  /*define child thread for motor control*/
defineTask(NTU_control);    /*define child thread for NTU control*/
defineTask(Sencor_control);

#define pinLED_525nm 13     /*define pin 13 as 525nm LED control pin*/
#define pinLED_610nm 11     /*define pin 12 as 610nm LED control pin*/
#define pinButton_little 10 /*define pin 10 as button signal input pin*/
#define pinButton_much 9    /*define pin 9 as button signal input pin*/
#define pinMotor_In 6       /*define pin 6 as liquid input motor control pin*/
#define pinMotor_Out 5      /*define pin 5 as liquid output motor control control pin*/
#define pinHeater 2         /*define pin 2 as heater control pin*/
#define pinSencor_Temp A0   /*define pin 11 as temperature sensor data input pin*/
#define pinSencor_Light A1  /*define pin 10 as light sensor data input pin*/

OneWire oneWire(pinSencor_Temp);    /* Initiate single bus connected to the port */
DallasTemperature sensors(&oneWire);

float NTU[] = {100, 100, 100};

/*------------------------Sencor---------------------------*/
double BH1750() //BH1750 equipment code
{
  int i = 0;
  double  val = 0;
  //start I2C read and write
  Wire.beginTransmission(BH1750address);
  Wire.write(0x10);//1lx reolution 120ms//send command
  Wire.endTransmission();

  sleep(200);
  //get value
  Wire.beginTransmission(BH1750address);
  Wire.requestFrom(BH1750address, 2);
  while (Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();
  if (2 == i)
  {
    val = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  return val;
}

void Sencor_control::setup()
{
  Wire.begin();
  Serial.begin(9600);
  sensors.begin();                /* Initiate library */
}


void Sencor_control::loop()
{
  float tempReading = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();  /* get temperature from sensor*/


  Serial.print( BH1750() );
  Serial.println("[lux]");
  Serial.print("Temperature is：");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println("℃");
  Serial.print("tempreading:");
  Serial.println(tempReading);
  sleep(100);



}

/*----------------------light_control------------------------*/

void light_control::setup()
{
  pinMode(pinLED_610nm, OUTPUT);
  analogWrite(pinLED_610nm, 254);

}

void light_control::loop()
{
  analogWrite(pinLED_610nm, 254);
  sleep(2000);
  NTU[0] = float(BH1750());        /*tell the NTU of system*/
  sleep(500);
  digitalWrite(pinLED_610nm, 255); /*If value from sensor lower than 6.44, recieve more an save them in array NTU[]*/
  if (NTU[0] <= 6.44)
  {
    for (int k = 0; k <= 2; k++)
    {
      NTU[k] = float(BH1750());
      sleep(2000);
    }
  }
  sleep(300000);

}

/*----------------------NTU_control-------------------------*/

void NTU_control::setup()
{
  pinMode(pinLED_525nm, OUTPUT);
}

void NTU_control::loop()
{
  if (NTU[0] <= 6.44 && NTU[1] <= 6.44 &&  NTU[2] <= 6.44) digitalWrite(pinLED_525nm, HIGH);

}

/*-----------------------heat_control--------------------------*/
float desiredTemp = 30.00 ;

void heat_control::setup()
{
  pinMode(pinHeater, OUTPUT);
  digitalWrite(pinHeater, HIGH);
}

float tempReading = sensors.getTempCByIndex(0);

void heat_control::loop()
{
  tempReading = sensors.getTempCByIndex(0);
  if (tempReading < desiredTemp)
  {
    digitalWrite(pinHeater, LOW);

    while (1 == 1)
    {
      tempReading = sensors.getTempCByIndex(0);
      if (1 == 1)break;
      sleep(500);
    }
  }
  else
  {
    digitalWrite(pinHeater, HIGH);
  }
  sleep(500);
}

/*----------------------motor_control----------------------------*/

void motor_control::setup()
{
  pinMode(pinMotor_In, OUTPUT);
  pinMode(pinMotor_Out, OUTPUT);
  digitalWrite(pinMotor_Out, HIGH);
  digitalWrite(pinMotor_In, HIGH);
  pinMode(pinButton_little, INPUT);
  pinMode(pinButton_much, INPUT);
}

void motor_control::loop()
{
  if (digitalRead(pinButton_little) == LOW)
  {
    sleep(100);
    if (digitalRead(pinButton_little) == LOW)
    {
      digitalWrite(pinMotor_Out, HIGH);
    } else {
      digitalWrite(pinMotor_Out, LOW);
    }
  }

  /* 100ml/70s */

  if (digitalRead(pinButton_much) == HIGH)
  {
    sleep(1000);
    if (digitalRead(pinButton_much) == HIGH)
    {
      digitalWrite(pinMotor_Out, LOW);
      sleep(175000);
      digitalWrite(pinMotor_Out, HIGH);
      sleep(1000);
      digitalWrite(pinMotor_In, LOW);
      sleep(205000);
      digitalWrite(pinMotor_In, HIGH);

    } else {
      digitalWrite(pinMotor_Out, HIGH);
      digitalWrite(pinMotor_In, HIGH);
    }

  }
}


/*-------------------------main----------------------------*/


void setup() {
  mySCoop.start();

}

void loop() {

  yield();
}
