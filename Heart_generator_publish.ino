//lissajous heart
//originally from sine generator code: https://forum.arduino.cc/index.php?topic=361759.0
//which was base on http://forum.arduino.cc/index.php?topic=8563.0

//x = 5 (sin(t)).^3,
//y = 4 cos(t) − 1.3 cos(2t) − 0.6 cos(3t) − 0.2 cos(4t)

//connect pin 9 to ~8k-10k Ohm + (series with)100nF ceramic cap -> GND, tap the sinewave signal from the point at between the resistor and cap.
//connect pin 10 to ~8k Ohm + (series with)100nF ceramic cap -> GND, tap the sinewave signal from the point at between the resistor and cap.

float wav1[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
float wav2[3];//0 frequency, 1 unscaled amplitude, 2 is final amplitude
int average1; 
int average2; 
const int Pin1 = 9;   
const int Pin2 = 10;
float time; 
float percentage; 
float templitude1;
float templitude2;
float offset = 2.5; // default value 2.5 volt as operating range voltage is 0~5V
float minOutputScale = 0.0;
float maxOutputScale = 5.0;
const int resolution = 1; //this determines the update speed. A lower number means a higher refresh rate.
const float pi = 3.14159;

void setup() {
  wav1[0] = 50; //frequency of the sine wave
  wav1[1] = 2.5; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"
  wav2[0] = 50; //frequency of the sine wave
  wav2[1] = 2.5; // 0V - 2.5V amplitude (Max amplitude + offset) value must not exceed the "maxOutputScale"
  
  TCCR1B = TCCR1B & 0b11111000 | 1;//set timer 1B (pin 9 and 10) to 31250Hz.
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
}

void loop() {
  time = micros()% 1000000;
  percentage = time / 1000000;
  templitude1 = pow(sin(((percentage) * wav1[0]) * 2 * pi),3);
  wav1[2] = (templitude1 * wav1[1]) + offset; //shift the origin of sinewave with offset.

  templitude2 = 0.8*cos(((percentage) * wav2[0]) * 2 * pi) - 0.26*cos(((percentage) * wav2[0]) * 2 * pi * 2) - 0.12*cos(((percentage) * wav2[0]) * 2 * pi * 3) - 0.04*cos(((percentage) * wav2[0]) * 2 * pi * 4);
  wav2[2] = (templitude2 * wav2[1]) + offset;
  
  average1 = mapf(wav1[2],minOutputScale,maxOutputScale,0,255);
  average2 = mapf(wav2[2],minOutputScale,maxOutputScale,0,255);
  
  analogWrite(9, average1);//set output "voltage"
  analogWrite(10, average2);//set output "voltage"
  delayMicroseconds(resolution);//this is to give the micro time to set the "voltage"

}

// function to map float number with integer scale - courtesy of other developers.
long mapf(float x, float in_min, float in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
