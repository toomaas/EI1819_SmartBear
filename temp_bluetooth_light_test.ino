#include <NeoPixelBus.h> 
int tempSensorPin = 5; //temperature sensor analog 0
const int timeDelay = 100;   //delay var 
const int numData = 10;
float temperature; // temperature reads value
float realTemperature; //the real temperature is a value that is checked(is it a real value or a fault from the sensors)
int numTimesWrongTemp = 0;
float data[numData];
int iter = 0;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(4, 2);
RgbColor blue =RgbColor(0, 0,255);
RgbColor yellow =RgbColor(255,255,0);
RgbColor green =RgbColor(0, 255,0);
RgbColor red =RgbColor(255, 0,0);
RgbColor blueWhite =RgbColor(0,216,255);
RgbColor  colorValue;
int NUMBER_LED=2;

void setup() 
{ 
  calculateTemperature(); //
  realTemperature = temperature;  //first value on setup
  Serial.begin(9600); //we will send to serial the values that the bluetooth will broadcast
  while (!Serial); // wait for serial attach Serial.flush();
  strip.Begin();
  strip.Show();
}

void loop() 
{
  calculateTemperature(); //calculate the temperature
  validateTemperature();  //check if the temperature is not a random value
  addToArray();
  sendTemperatureToBluetooth(); //send to serial
  delay(timeDelay);
}

void addToArray(){
  //Serial.println(realTemperature);
  if(realTemperature > 20 && realTemperature < 50)
  {
  data[iter] = realTemperature;
  iter++;
  }
  //Serial.print(iter);
}

void validateTemperature(){ //function to prevent high variances in temperature
  if(temperature >= realTemperature-3 && temperature <= realTemperature +3){  //if the next temperature is +-3 degrees it is accepted
    realTemperature = temperature;
    numTimesWrongTemp = 0;
  }else{
    numTimesWrongTemp++;
  }
  if(numTimesWrongTemp >= 3){
      realTemperature = temperature;
  }
}

void calculateTemperature(){  //function to read the value from the temp36 sensor and translate it to a real temperature value
 int reading = analogRead(tempSensorPin); //getting the voltage reading from the temperature sensor
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 temperature = (voltage - 0.5) * 100 +5; //convert it to a real temperature
}

void sendTemperatureToBluetooth(){  //sends to serial the temperature
  
  if(iter >= numData)
  {
    
    float val = 0;
    for(int i = 0; i <numData;i++)
    {
      val += data[i];
    }
    val = val/numData;
    changeRgb(val);  //change the rgb
    iter = 0;
    Serial.print(val);  //converting from 10 mv per degree wit 500 mV offset
    Serial.println("I");  //this helps the android app to parse the values and prevent errors
  }
}

void changeRgb(float value){ //change an rgb light acording with the temperature
  if(value < 35)
  {
   displayLed(1); //blue
  }
  else if(value >=35 && value <37)
  {
  displayLed(2); //bluewhite
  }
  else if(value >=37 && value <38.5)
  {
   displayLed(3); //green
  }
  else if(value >=38.5 && value <40)
  {
  displayLed(4); //yellow
  }
  else if(value >=40)
  {
   displayLed(5);//red
  }
}

void displayLed(int value){
  
  switch (value) {
    case 1:
      colorValue=blue;
      break;
    case 2:
      colorValue=blueWhite;
      break;
    case 3:
      colorValue=green;
      break;
    case 4:
      colorValue=yellow; 
      break;
    case 5:
      colorValue=red; 
      break;
    default:
      break;
  }   
  for(int i=0;i<NUMBER_LED;i++){
      strip.SetPixelColor(i,colorValue); //azul marinho
      strip.Show();
  }  
}
