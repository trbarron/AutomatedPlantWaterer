const int pumpPower = 0; //assignes a pin to power the sensor. optional
const int sensor0Power = 1;
const int sensor1Power = 2;
const int sensor0Data = 3;  //assignes a pin for the first soil sensor
const int sensor1Data = 4;
const int buttonPin = 4;    

const int readings = 6;   // num of readings to take before reporting soil moisture levels
int dirtVal0;    //variable to store one sensor reading
int dirtVal1;    //variable to store one sensor reading
int buttonVal;
int water_level;
int water_level_thresh = 800; //amount of water before we trigger a refill
int water_timer = 0;
int water_timer_thresh = 2*60*60*10; // hours between checking water level
int missed_waterings = 0;
int missed_waterings_thresh = 12; //number of water_timer_thresh's before forcing a watering
double avgDirt; //variable to store average of 10 sensor readings

void setup() 
{
  Serial.begin(9600);   //serial monitor for debugging purposes
  pinMode(pumpPower,OUTPUT); //Initialize pins
  pinMode(sensor0Power,OUTPUT);
  pinMode(sensor1Power,OUTPUT);
  pinMode(sensor0Data,INPUT);
  pinMode(sensor1Data,INPUT);
  pinMode(buttonPin, INPUT_PULLUP); //"the internal pull-up resistor enabled"
}

void loop() 
{
  //if button pressed then water plant
  buttonVal = digitalRead(buttonPin);
  if (buttonVal) == LOW
  {
    water_plant();
  }
  if (water_timer >= water_timer_thresh)
  {
    water_level = check_water();
    if (water_level > water_level_thresh || missed_waterings >= missed_waterings_thresh)
    {
      water_plant();
    }
    else
    {
        missed_waterings++;
    }
  }
  delay(100); //Delay button polling for this long
  water_timer++;
} 
 
void water_plant()
{
    digitalWrite(pumpPower, HIGH); //provides power to the sensor
    Serial.println("POWERING UP");
    delay(7*1000); //delays turning it off for seven seconds
    digitalWrite(pumpPower, LOW);
    Serial.println("powering down"); // Powers down.
    missed_waterings = 0;
    water_timer = 0;
    return;
}

int check_water_level()
{
    //Start by turning on sensors, waiting 20 seconds for them to "warm up"
    DigitalWrite(sensor0Power, HIGH);
    DigitalWrite(sensor1Power, HIGH);
    delay(20*1000);
    
    //calculate average of "readings" soil readings
    for(int i = 0; i < readings; i++)   
    {
      dirtVal0 = analogRead(sensor0Data);
      dirtVal1 = analogRead(sensor1Data);
      avgDirt += dirtVal0 + dirtVal1;
      delay(10*1000);
    }
    avgDirt = avgDirt / (readings * 2); //Times 2 because there are two sensors
    Serial.println(avgDirt); //for debugging, log the moisture level
    
    //Power off the sensors to reduce oxidation
    DigitalWrite(sensor0Power, LOW);
    DigitalWrite(sensor1Power, LOW);
    return(avgDirt);
}