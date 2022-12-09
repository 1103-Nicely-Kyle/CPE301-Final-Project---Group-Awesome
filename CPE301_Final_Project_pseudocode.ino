//By Kyle Nicely, Eric Westenburg, Luke Bowler, Phil Hensley

#include <Stepper.h>
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 22, 24, 26, 28); //pins
int stepCount = 0;         // number of steps the motor has taken

#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int adc_id = 0;
int HistoryValue = 0;
char printBuffer[128];
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

volatile unsigned char* port_d = (unsigned char*) 0x2B; //Ports for LEDs and Buttons
volatile unsigned char* ddr_d  = (unsigned char*) 0x2A; 
volatile unsigned char* pin_d  = (unsigned char*) 0x29; 

volatile unsigned char* port_e = (unsigned char*) 0x2E; 
volatile unsigned char* ddr_e  = (unsigned char*) 0x2D; 
volatile unsigned char* pin_e  = (unsigned char*) 0x2C; 

volatile unsigned char* port_g = (unsigned char*) 0x34; 
volatile unsigned char* ddr_g = (unsigned char*) 0x33; 
volatile unsigned char* pin_g  = (unsigned char*) 0x32; 

volatile unsigned char* port_l = (unsigned char*) 0x10B; 
volatile unsigned char* ddr_l  = (unsigned char*) 0x10A; 
volatile unsigned char* pin_l  = (unsigned char*) 0x109; 

//red LED = pin 48 - PD5       OUTPUT
//green LED = pin 50 - PD7     OUTPUT
//blue LED = pin 52 - PG1      OUTPUT
//DC motor = pin 3 - PE1       OUTPUT
//start button = 35 - PL0      INPUT
//stop button = 36 - PL1       INPUT
//step up button = 37 - PL2    INPUT
//step down button = 38 - PL3  INPUT

bool start = false;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  *ddr_d |= 0xA0; //Set PD7 and PD5 to output
  *ddr_e |= 0x2; //Set PE1 to output
  *ddr_g |= 0x2; //Set PG1 to output
  *ddr_l &= 0x0 //Set PL0, PL1, PL2, PL3 to input
  //*portB &= 0xBF; //Initialize to low

  //State = IDLE
  //Set LED to Green 
}

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}

void loop()
{
  //if start button is high 
  if(*pin_l & 0x1){
    start = true;
    //State = Idle
    //Set LED to Green
  }*/

  while((!(*pin_l) & 0x2) && start == true){

    int value = analogRead(adc_id);

    if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
    {
      if(value < 200) {
        sprintf(printBuffer,"Water level low\n");
        Serial.print(printBuffer);
        //State = Error 
        //Set the LED to Red
      }
      HistoryValue = value;
    }
      int value2 = analogRead(adc_id);

    float temperature;
    float humidity;

    if( measure_environment( &temperature, &humidity ) == true )
    {
      lcd.setCursor(0, 0);
      lcd.print("T = ");
      lcd.print(temperature, 1);
      lcd.print(" deg. C");
      lcd.setCursor(0, 1);
      lcd.print("H = ");
      lcd.print(humidity, 1);
      lcd.print("%");
    }

    /* 
    if(temperature < tempThreshold){
    //State = idle
    //Set the LED to Green
    }

    //if step up button is pressed
    if(*pin_l & 0x4){
      myStepper.step(1);
      stepCount++;
    }

    //if step down is pressed
    if(*pin_l & 0x8){
      myStepper.step(-1);
      stepCount--;
    }

    //if stop button is pressed
    if(*pin_l & 0x2){ 
      //State = Disabled
      //Set LED to Yellow
      start = false;
    }
    */
  }
}
