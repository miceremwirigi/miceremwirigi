#include <Wire.h>                  //
#include <LiquidCrystal_I2C.h>     // Import LCD display library
#include <DHT.h>                   // Import Humidity and Temperature sensor library
#include <DHT_U.h>  
#define Type DHT11                 // define sensor type in use


int readPin=A1;                            
int lights = 11;
int redPin = 5;
int greenPin = 6;
int bluePin = 7;
int pushBtn = 4;
float ledVal = 0;
float readVal;
int pirSensor = 8;
float V2=0;
int delayTime = 100;
int blinkTime(250);
int readBtn = 0;
int dhtPin = 2;                   
float humidity;
float tempC;
float tempF;
int setTime = 1000;
int fan = 13;
int btn =0;


DHT HT (dhtPin,Type);                         //Declare object of class DHT
LiquidCrystal_I2C lcd(0x27,16,2);             //Declare object of class LiquidCrystal_12C


void setup() {
                                              // setup code put here to run once:
  pinMode(readPin, INPUT);                    //set pin A1 as input
  pinMode(pushBtn, INPUT_PULLUP);             //set pin 4 as input
  pinMode(lights, OUTPUT);                    //set pin A11  as output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(fan, OUTPUT);                       // set fan as output
  HT.begin();                                 // start the temp sensor  
  lcd.init();                                 // start display
  lcd.backlight();                            // light up display
  lcd.clear();                                // clear display
  Serial.begin(115200);
}


void loop() {                                 // main code put here to run repeatedly:
  /*Light and Motion Sensors*/
    buttonState(readBtn, btn);                 //update button state
         
  /*Humidity and Temperature Sensors*/
  
    meaSure(tempC, humidity);
    serialShowmeasurement(tempC, humidity, readBtn, btn);
    lcdShowmeasurement( tempC, humidity, setTime);

  while(tempC > 24.5)                 // Run fan while temp is high. Show temp and humidiy
  {
    digitalWrite(fan, HIGH);
    redLight(redPin, greenPin, bluePin);
    buttonState(readBtn, btn);                  //update button state
    
    lcdShowmeasurement( tempC, humidity, setTime);
    serialShowmeasurement(tempC, humidity, readBtn, btn); 
    meaSure(tempC, humidity); 
    switchMode(btn);                            //switch between two functions when button is pressed
    delay(setTime);  
    lcd.clear();
  
  }
    digitalWrite(fan,LOW);           // stop fan when temperature is okay
    greenLight(redPin, greenPin, bluePin);
    delay(setTime);  
    switchMode(btn);                            //switch between two functions when button is pressed
    lcd.clear();                                // clear display 
                     
  
}


    /*Function Definitions*/ 
    
    void lightDependentlighting(float x, float y){
      readVal = analogRead(readPin);              // call analogRead function to read amount of light

      if (readVal < 450){
          V2 = 4.5;
            }
      else {
          V2 = 5.*((1023.-readVal)/573.);
           }
      ledVal = 255*((4.5 - V2)/4.5);              // set output brightness
      analogWrite (lights, ledVal);               // give output,turn on or turn off lights 
      }

    void motionSensorlighting(float x){
       int sensorValue = digitalRead(pirSensor);   // declare variable to store motion value, true or false
                                                              // read motion value
       if (sensorValue == 1){
       ledVal = 255;
       analogWrite (lights, ledVal);            // turn on lights if motion detected
       }
       else {
      digitalWrite(lights, LOW);               //turn off lights if no motion             
    }
    }
      
    void buttonState(int y,int z){
        readBtn = digitalRead(pushBtn);   //check if button is pressed
        if(readBtn < 1)                                        
        {
          btn = btn+1;                //increment btn if button is pressed
        }
        if (btn > 2){                 //reset i for next while loop
          btn  = 0;
        } 
      
    } 
     void switchMode(int x){
     switch(btn){                                   //switch between two functions when button is pressed
        
          case 0:
             lightDependentlighting(readVal, ledVal);
             purpleLight(redPin, greenPin, bluePin);
             break;                  
          case 1:
             motionSensorlighting(ledVal);  
             blueLight(redPin, greenPin, bluePin);                 
             break;
           default:
             analogWrite(lights, 0);               //turn off lights completely.     
             offLight(redPin, greenPin, bluePin);                 
             break;                 
        }        
     }
     
    void meaSure(float x, float y){
    tempC = HT.readTemperature();     // update temperature to see if exit condition is met
    humidity = HT.readHumidity();     // update humidity
    } 

    void serialShowmeasurement(float x, float y, int z, int w){ 
    Serial.print(readBtn);           //show if buttton is pressed and case under execution
    Serial.print(" , ");
    Serial.print(btn);   
    Serial.print(" , "); 
    Serial.print(tempC);             // print temp and humidity values on monitor
    Serial.print(" , ");       
    Serial.println(humidity); 
    }

    void lcdShowmeasurement( float x, float y, int z)
    {
    lcd.setCursor(0,0);              // begin display at positions given and
    
    lcd.print(" Temp: ");
    lcd.print( tempC);
    lcd.setCursor(4,1);
    lcd.print(" Celcius");
    delay(setTime);
    lcd.clear();                      // clear display
    
    lcd.setCursor(0,0);               // begin display at positions given and
    lcd.print("Humidity: ");
    lcd.print( humidity);             // print measurements on lcd
    lcd.setCursor(4,1);
    lcd.print(" Percent");
    delay(setTime);
    lcd.clear();                   
    }


     void purpleLight(int a,int b,int c){
      
      analogWrite(redPin, 238);
      analogWrite(greenPin, 130);
      analogWrite(bluePin, 238);
      delay(blinkTime);
      }
      void blueLight(int a,int b,int c){
      
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 255);
      delay(blinkTime);
      }
      
      void greenLight(int a,int b,int c){
      
      analogWrite(redPin, 0);
      analogWrite(greenPin, 255);
      analogWrite(bluePin, 0);
      delay(blinkTime);
      }
      void redLight(int a,int b,int c){
      
      analogWrite(redPin, 255);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      delay(blinkTime);
      }
      void offLight(int a,int b,int c){
      
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
      delay(blinkTime);
      }
