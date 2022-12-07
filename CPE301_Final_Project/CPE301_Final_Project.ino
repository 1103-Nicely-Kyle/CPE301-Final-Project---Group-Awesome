#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int adc_id = 0;
int HistoryValue = 0;
char printBuffer[128];
static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
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
    int value = analogRead(adc_id);

    if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
    {
      if(value < 200) {
        sprintf(printBuffer,"Water level low\n");
        Serial.print(printBuffer);
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
}
