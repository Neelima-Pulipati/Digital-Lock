#include <Servo.h>
#include <Keypad.h>
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);  // Change the address (0x27) to match your LCD's address

// Initialize Servo
Servo servo;

// Define keypad layout
const byte ROWS = 4;
const byte COLS = 4;
char keymap[][4] = { 
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*', '0', '#', 'D'} 
}; 

byte rowPins[] = {2, 3, 4, 5}; 
byte colPins[] = {6, 7, 8, 9}; 

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, 4, 4);

// Define variables
String password = "1234";  // Change this to your desired password
String current="";
boolean resetMode = false;

void setup() {
  // LCD setup
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  pinMode(10, OUTPUT);   
  pinMode(11, OUTPUT);   
}

void loop() {
    char key = keypad.getKey();
    //Serial.print(key);
	if(key != NO_KEY){ 
      current += key; 
      // if the length is still lower, display it. Else, process the inputted password
      if(current.length() < password.length()){
      lcd.print(key);     
      } 
      else {  // some 4 digit password was entered 
           if(current == password){     // password entered was correct 
                // clear lcd and print welcome         
                lcd.clear(); 
                lcd.print("Welcome to Home!"); 
                unlockDoor();
           } 
           else if(current=="####"){   // trying to reset password
               lcd.clear(); 
               lcd.print("Enter Password");
               current="";
               lcd.setCursor(0,1);
               while(1){
                  char key=keypad.getKey();
                  if(key!=NO_KEY){
                     current+=key;
                     if(current.length()<password.length()){
                     lcd.print(key);
                     }
                     else{
                      break;
                     }
                   }  
                  } //some password was entered to reset password
               if(current==password){  // resetting password
                  lcd.clear();
                  lcd.print("Enter New Password");
                  lcd.setCursor(0,1);
                  password="";
                  while(1){
                     char key=keypad.getKey();
                     if(key!=NO_KEY){
                       password+=key;
                       if(password.length()<4){
                       lcd.print(key);
                       }
                       else{
                       break;
                       }
                       }
                   }
                }
                else{ // if wrong password was entered to reset password
                 lcd.clear();
                 lcd.print("Wrong Password");
                }
           lcd.clear();
           current="";
           }
           else {      // wrong password entered
               // clear lcd and print incorrect 
               lcd.clear(); 
               lcd.print("Incorrect!");
               buzz();
           } 
           servo.write(0); 
           lcd.clear(); 
           lcd.print("Enter Password:");       
           lcd.setCursor(0, 1); 
           // reset the inputted password 
           current = ""; 
         } 
  	} 
}  // loop() ends

void unlockDoor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Unlocked!");
  servo.attach(10);
  servo.write(90);  // Rotate the servo to unlock
  delay(3000);
  servo.write(0);   // Lock the servo
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
}

void buzz() {
  digitalWrite(11, HIGH);
  delay(1000);
  digitalWrite(11, LOW);
}
