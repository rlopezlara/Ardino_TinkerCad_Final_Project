//FINAL PROJECT ARDUINO
//RODRIGO LOPEZ
//Description: This game is focusing on guessing words. I will have to guess 5 words, relate to the course. to complete this mission you will have 10 attempts. However tu will have as many opportunities as you want.You will be accompanied with colours lights.
//Initialise the Serial LCD.
#include <LiquidCrystal.h>

//initialise the neopixel
//initialise PIXELS_JEWEL

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> 
#endif
#define PIN_NEO_PIXEL  13  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS     4  // The number of LEDs (pixels) on NeoPixel
#define NUM_PIXELS_JEWEL 19 //number of pixeles in the JEWEL
#define PIN 10 //Arduino pin that connects to JEWEL
Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS_JEWEL, PIN, NEO_GRB + NEO_KHZ800);


LiquidCrystal lcd( 12,11,5,4,3,2); //LCD pins
int button1Pin = 7;//setup button1
int button2Pin = 6;//setup button2
bool game = false;//to play again
int maxAttempts = 10;//lives
int count = 1;//count the questions
String guessedWord[] = { "_______","_________","_______","________", "_____"};
String targetWord [] = {"ARDUINO", "TINKERCAD","MATTHEW", "COMPUTER", "ARRAY"};//array words to guess
int totalWords = sizeof(targetWord) / sizeof(targetWord[0]);//count the size of the words
int currentWordIndex = 0;



// all the setup
void setup() {
   
  lcd.begin(16,2);//setup LCD
  NeoPixel.begin();//setup NEO pixels
  Serial.begin(9600);//setup Serial Monitor
  pinMode(button1Pin, INPUT);//Button1
  pinMode(button2Pin, INPUT);//button2
  welcomeMessage();//Welcome presentation
  instructions();//aditional information in the serial monitor
}

void loop() { //this reapets always

 // press the button1 to start the game
if (digitalRead(button1Pin)==HIGH) {
  //LCD message
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("word number: " + String(count));
  delay(1000);
  
  lcd.clear();
  lcd.print("Word: " + guessedWord[currentWordIndex]);
  lcd.setCursor(0, 1);
  lcd.print("Attempts left:" + String(maxAttempts));
  
  	game = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("word number: " + String(count));
    delay(1000);

    lcd.clear();
    lcd.print("Word: " + guessedWord[currentWordIndex]);
    lcd.setCursor(0, 1);
    lcd.print("Attempts left:" + String(maxAttempts));

  
  while (game){// while the game is true keep going
    playGame();
  	lightNEO();
    
    
   if (digitalRead(button2Pin) == HIGH) { //turn off button2
    game = false;
    lostMessage();
    for(int k = 14; k >=0 ; k--){	//function message move from right to left
    lcd.clear();
    lcd.setCursor(k, 0);
  	lcd.print("TURNNING OFF...");
    lcd.setCursor(k, 1);
  	lcd.print("BYE BYE!!!");
    delay(300);
    }
    
    NeoPixel.clear();
	NeoPixel.show();
	strip.clear();
 	strip.show();
    lcd.clear();
    while (true);
  }
}
  
}
   
}
void playGame(){	// play the game function
  
  if (maxAttempts == 0) { //when you lose your lives
    lcd.clear();
    lcd.print("Out of attempts!");
    lostMessage();
    delay(2000);
    
    lcd.clear();
    lcd.print("Another change?");
    lcd.setCursor(0, 1);
    lcd.print("Answer(Y/N)");
    delay(1500);
    
    while(!Serial.available())// waiting the answer
    {
    }
    char answer = Serial.read();
      if(answer =='Y'||answer =='y'){
        
    lcd.clear();
    lcd.print("Restarting...");
    delay(1000);
    lcd.clear();
    maxAttempts = 10;
      
     
      lcd.print("Word: " + guessedWord[currentWordIndex]);
      lcd.setCursor(0, 1);
      lcd.print("Attempts left:" + String(maxAttempts));
      delay(1000);
      return;//going back to the game
      } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Thanks for playing!");
      lcd.setCursor(0, 1);
      lcd.print("Goodbye!");
      delay(2000);
      lcd.clear();
      while (true); // Stay here indefinitely
    }
  }
    
  

	
  if (guessedWord[currentWordIndex] == targetWord[currentWordIndex]) { //when you find the word out
    if(count < 5){
    lightNEO();
    lcd.clear();
    lcd.print("You guessed it!");
   	lcd.setCursor(0, 1);
    lcd.print("Next word");
    guessLighet();
    
    delay(2000);
    lcd.clear();
    count++;
    moveToNextWord();//moving to the next question function
    lightJEWEL();
    }
    if (count ==5 && guessedWord[currentWordIndex] == targetWord[currentWordIndex]){
     count++;
      lightJEWEL();
      winMessage();   
      
    }
  }

//receiving the letter that you are typing
  
  if (Serial.available() > 0) {
    char guess = Serial.read();
    if (isAlpha(guess)) {
      guess = toupper(guess);
      bool found = false;
      
     
      for (int i = 0; i < targetWord[currentWordIndex].length(); i++){
        if (targetWord[currentWordIndex][i] == guess) {
          guessedWord[currentWordIndex][i] = guess;
          found = true;
        }
        
      }
      //if you put the wrong letter
      if (!found) {
        maxAttempts--; //lose one live
        errorLighet();
      }
      lcd.clear();
      lcd.print("Word: " + guessedWord[currentWordIndex]);
      lcd.setCursor(0, 1);
      lcd.print("Attempts left:" + String(maxAttempts));
    }
  }
}
void moveToNextWord() {
  // Move to the next word   
  currentWordIndex = (currentWordIndex + 1) % totalWords; 
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("word number: " + String(count));
  delay(1000);
  lcd.clear();
  resetGuessedWord();
  lcd.print("Word: " + guessedWord[currentWordIndex]);
  lcd.setCursor(0, 1);
  lcd.print("Attempts left:" + String(maxAttempts));
    
}
void winMessage(){ //message when you complete all the words
  		
  		byeLight();//adding lights
  		
  for (int i = 0; i < 17; i++){			
        lcd.clear();
    	lcd.setCursor(i, 0);
        lcd.print("Congratulations!");
   		lcd.setCursor(i, 1);
        lcd.print("you win!!");
       	delay(300);
  }
  	lcd.clear();
  	lcd.setCursor(0, 0);
    lcd.print("Play again?");
 	lcd.setCursor(0, 1);
   	lcd.print("(Y/N)");
  	delay(1000);
  	//when you finish the game you can decide to play again
  while(!Serial.available())
    {
    }
    char answer = Serial.read();
      if(answer =='Y'||answer =='y'){
        
    	lcd.clear();
    	lcd.print("Playing again...");
    	delay(1000);
    	
    //restart all the variables.    
    maxAttempts = 10;
    count = 1;
    currentWordIndex = 0; 
    resetGuessedWord();
   lcd.clear();
   lcd.print("Word: " + guessedWord[currentWordIndex]);
   lcd.setCursor(0, 1);
   lcd.print("Attempts left:" + String(maxAttempts));
   
   delay(1000);
   playGame();
  
   }
  
  else{//bye bye message
    	lcd.clear();
  		for (int i = 14; i >= 0; i--){
        lcd.clear();
        lcd.setCursor(i, 0);
        lcd.print("See you next!!");
    	lcd.setCursor(i, 1);
        lcd.print("Good bye!!");
  		delay(500);
       
		}
        lcd.clear();
  		NeoPixel.clear();
  		NeoPixel.show();
  		strip.clear();
  		strip.show();     
        while (true); // Stay here indefinitely
      } 
 }
void lightNEO(){
   //set the lights to turn on when you move to the next question
  
    NeoPixel.clear(); 
    
    for (int i = 0; i < count; i++) {
        NeoPixel.setPixelColor(i, NeoPixel.Color(0, 0, 255)); // Set color to blue for guessed words
    }
    
    NeoPixel.show(); 
    delay(100);
 	NeoPixel.clear();
  	
    }


void lightJEWEL(){
   //this control qhe jewelpixeles, turn on to the last two question
  strip.clear(); 
    
    if (count == 5) {
        for (int i = 0; i < 7; i++) {
            strip.setPixelColor(i, strip.Color(0, 0, 255)); // Set pixel to green (R, G, B)
        }
    } else if (count == 6) {
        for (int j = 0; j < 19; j++) {
            strip.setPixelColor(j, strip.Color(0, 0, 255)); // Set pixel to green (R, G, B)
        }
    }
    
    strip.show(); 
    delay(100);
    strip.clear(); 
  
}

void errorLighet(){
  //error message when the letter is wrong
  
  for (int k = 0; k < 19; k++) {
    strip.setPixelColor(k, strip.Color(255, 0, 0)); // Set pixel to red
    
  }
  strip.show(); // 
    delay(800);
    strip.clear();
  	strip.show();
}
void byeLight(){
  
  //bye light show, with NEOpixels and Jewelpixels.
  for (int pixel = 0; pixel < NUM_PIXELS_JEWEL ; pixel++) {
    if (pixel % 3 == 0) {
      strip.setPixelColor(pixel, strip.Color(255, 0, 0)); // Red
    } else if (pixel % 3 == 1) {
      strip.setPixelColor(pixel, strip.Color(0, 255, 0)); // Green
    } else if (pixel % 3 == 2) {
      strip.setPixelColor(pixel, strip.Color(0, 0, 255)); // Blue
    }
    strip.show();
    delay(100);
    
}
      strip.clear();
  
  //light show in Neopixels
  for (int i = 0; i < 4; i++) {
   
   NeoPixel.setPixelColor(i, NeoPixel.Color(0, 255, 255));
   NeoPixel.show();  
   delay(100);
  }
    
  for (int i = 0; i < 4; i++) {
   
   NeoPixel.setPixelColor(i, NeoPixel.Color(255, 255, 0));
   NeoPixel.show();  
   delay(100);
  }
  for (int i = 0; i < 4; i++) {
   
   NeoPixel.setPixelColor(i, NeoPixel.Color(255, 0, 255));
   NeoPixel.show();  
   delay(100);
  }
   delay(100);
 NeoPixel.clear();
  	
}
void guessLighet(){
  //when you guess a word, Jewel pixels turn on green
  
  for (int k = 0; k < 19; k++) {
    strip.setPixelColor(k, strip.Color(0, 255, 0)); // Set pixel to green 
    
  }
  strip.show(); 
  delay(800);
  strip.clear();
  strip.show();
}	

void lostMessage(){
  //you lost your 10 attempts
  for(int t = 0; t < 4; t++){
  NeoPixel.clear();
  strip.clear();
  //jewel lights red
  for (int k = 0; k < 20; k++) {
    strip.setPixelColor(k, strip.Color(255, 0, 0)); // Set pixel to green (R, G, B)
    NeoPixel.setPixelColor(k, NeoPixel.Color(255, 0, 0));
    
  }
  strip.show(); // Update the NeoPixel hardware
   NeoPixel.show(); 
    delay(1000);
    strip.clear();
  	strip.show();
  	 NeoPixel.clear();
   NeoPixel.show();
  }
  
    strip.clear();
  	strip.show();
  	 NeoPixel.clear();
   NeoPixel.show();
  
}

  void welcomeMessage(){
   //welcome message to the game with some explanation
  byeLight();
  for (int i = 0; i < 17; i++){
  lcd.clear();
  lcd.setCursor(i, 0);
  lcd.print("WELCOME TO FINAL");
  lcd.setCursor(i, 1);
  lcd.print("ARDUINO PROJECT");
  delay(300);
  }
    
  lcd.setCursor(0, 0);
  lcd.print("GUESSING GAME");
  lcd.setCursor(0, 1);
  lcd.print("Guess 5 words!!");
  delay(2000);
    
    for(int k = 14; k >=0 ; k--){ //adding some movements 
    lcd.clear();
    lcd.setCursor(k, 0);
  	lcd.print("Put a letter in the");
    lcd.setCursor(k, 1);
  	lcd.print("Serial monitor");
    delay(300);
    }
    delay(500);
    for(int k = 14; k >=0 ; k--){
    lcd.clear();
    lcd.setCursor(k, 0);
  	lcd.print("You will have");
    lcd.setCursor(k, 1);
  	lcd.print("10 attempts Total");
    delay(300);
    }
  delay(2000);
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Press:!!");
  lcd.setCursor(0, 1);
  lcd.print("START BUTTON!");
  delay(1000);

 NeoPixel.clear();
 NeoPixel.show();
 strip.clear();
 strip.show();
    
  delay(2000);
   
  }
void instructions(){
  //I added some doblecheck instructions on the serial monitor
  Serial.println("DobleCkech Instructions");
  Serial.println("WELCOME Guessing game");
  Serial.println("To win this game to have tu guess 5 words");
  Serial.println("Your will have 10 attempts to complete this game");
  Serial.println("To start to have press the start button");
  Serial.println("You can turn off the game anytime");
  Serial.println("so, let's play!!!");
  Serial.println("*******************************************");
 }
void resetGuessedWord() {
  //this variable is in order to reset the words after winning the game.
    for (int i = 0; i < targetWord[currentWordIndex].length(); i++) {
        guessedWord[currentWordIndex][i] = '_';
    }
}
//ends