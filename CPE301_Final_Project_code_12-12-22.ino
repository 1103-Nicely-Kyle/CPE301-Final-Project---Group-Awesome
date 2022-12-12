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

volatile unsigned char* port_b = (unsigned char*) 0x25; //Ports for LEDs and Buttons
volatile unsigned char* ddr_b  = (unsigned char*) 0x24; 
volatile unsigned char* pin_b  = (unsigned char*) 0x23; 

volatile unsigned char* port_c = (unsigned char*) 0x28; 
volatile unsigned char* ddr_c  = (unsigned char*) 0x27; 
volatile unsigned char* pin_c  = (unsigned char*) 0x26; 

volatile unsigned char* port_d = (unsigned char*) 0x2B; 
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

//yellow LED = pin 42 - PL7     OUTPUT
//red LED = pin 48 - PL1        OUTPUT
//green LED = pin 50 - PB3      OUTPUT
//blue LED = pin 52 - PB1       OUTPUT
//DC motor ENABLE = pin 3 - PE5 OUTPUT
//DC motor = pin 5 PE3          OUTPUT
//start button = 37 - PC0       INPUT
//stop button = 36 - PC1        INPUT
//step up button = 35 - PC2     INPUT
//step down button = 34 - PC3   INPUT

bool start = false;
bool stopped = false;

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);

  *ddr_l |= 0x82; //Set PL1 and PL7 to output
  *ddr_e |= 0x28; //Set PE5 and PE3 to output
  *ddr_b |= 0xA; //Set PB1 and PB3 to output
  *ddr_c &= 0xF0; //Set PC0, PC1, PC2, PC3 to input
  
  *pin_l &= 0x0; //Initialize LEDs to Low
  *pin_b &= 0x0;

  //State = Disabled
  //Set LED to Yellow
  *pin_l |= 0x80;
  //Fan Off
  *pin_e &= 0x0;
  //Fan Enabled
  *pin_e |= 0x20;
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
  if(*pin_c & 0x1){
    Serial.print("button pressed");
    stopped = false; 
    //State = Idle
    //Set LED to Green
    *pin_b &= 0x0;
    *pin_l &= 0x0;
    //delay(100);
    *pin_b |= 0x8;
    //Fan Off
    *pin_e &= 0x0;
    start = true;
  }

  //while stop button is not pressed and system is started
  while(start == true){

    int value = analogRead(adc_id);

    if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
    {
      if(value < 200) {
        sprintf(printBuffer,"Water level low\n");
        Serial.print(printBuffer);
        //State = Error 
        //Set the LED to Red
        *pin_b &= 0x0;
        *pin_l &= 0x0;
        //delay(100);
        *pin_l |= 0x2;
        //Fan Off
        *pin_e &= 0x0;
        start = false;
      }
      HistoryValue = value;
    }
    
    int value2 = analogRead(adc_id);

    float temperature;
    float humidity;

    if( measure_environment( &temperature, &humidity ) == true )
    {
      Serial.print("environment measured");
      lcd.setCursor(0, 0);
      lcd.print("T = ");
      lcd.print(temperature, 1);
      lcd.print(" deg. C");
      lcd.setCursor(0, 1);
      lcd.print("H = ");
      lcd.print(humidity, 1);
      lcd.print("%");
    }

    //if temperature less than tempThreshold
    if(temperature < 21){
      //State = idle
      //Set the LED to Green
      *pin_b &= B00000000;
      *pin_l &= B00000000;
      //delay(100);
      *pin_b |= 0x8;
      //Turn off fan
      *pin_e &= 0x0;
    }
    else{
      //State = Running
      //Set the LED to Blue
      *pin_b &= B00000000;
      *pin_l &= B00000000;
      //delay(100);
      *pin_b |= 0x2;
      //Turn on Fan
      *pin_e |= 0x8;
    }

    //if step up button is pressed
    if(*pin_c & 0x4){
      myStepper.step(1);
      stepCount++;
    }

    //if step down is pressed
    if(*pin_c & 0x8){
      myStepper.step(-1);
      stepCount--;
    }

    //if stop button is pressed
    if(*pin_c & 0x2){ 
    stopped = true;
    //State = Disabled
    //Set LED to Yellow
    *pin_b &= B00000000;
    *pin_l &= B00000000;
    //delay(100);
    *pin_l |= 0x80;
    //Turn off Fan
    *pin_e &= 0x0;
    start = false;
    }
  }
  
  //if stop button is pressed
  if(stopped == true){ 
    //State = Disabled
    //Set LED to Yellow
    *pin_b &= 0x0;
    *pin_l &= 0x0;
    //delay(100);
    *pin_l |= 0x80;
    //Turn off Fan
    *pin_e &= 0x0;
    start = false;
    
  }
}
