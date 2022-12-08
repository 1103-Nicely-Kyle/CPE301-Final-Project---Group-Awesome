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

volatile unsigned char *portDDRB = (unsigned char *) 0x24; //Example Ports for LEDs and Buttons
volatile unsigned char *portB =    (unsigned char *) 0x25;
//red LED = pin 48
//green LED = pin 50
//blue LED = pin 52
//DC motor = pin 3
//start button = 34
//stop button = 35
//step up button = 36
//step down button = 37

bool start = false;


void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  //*portDDRB |= 0x40; //Set PB6 to output
  //*portB &= 0xBF; //Initialize to low

  //State = Disabled
  //Set LED to Yellow
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
  if(start pushed is pressed){
  //start = true;
  //State = Idle
  //Set LED to Green
  }*/

  //while(stop button is not pushed && start == true){

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

  /* if(temperature < tempThreshold){
  //State = idle
  //Set the LED to Green
  }

  if(stepper button 1 is pressed){
    myStepper.step(1);
    stepCount++;
  }

  if(stepper button 2 is pressed){
    myStepper.step(-1);
    stepCount--;
  }
  */
}
