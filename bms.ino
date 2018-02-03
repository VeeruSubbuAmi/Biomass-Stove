#include <max6675.h>
#include <SPI.h>

#include <UTFT.h> 
#include <URTouch.h>

//==== Creating Objects
UTFT    myGLCD(ITDB32S,38,39,40,41); 
URTouch  myTouch( 6, 5, 4, 3, 2);
char stCurrent[20]="";
int stCurrentLen=0;
char stLast[20]="";

int i;
//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
const int prifan = 10;
const int secfan = 8; 
const int fuel = 9;
int slow=0;
int quick=0;
int medium=0;
int x, y;
int pf=38;
int sf=38;
int fp=38;
float temp=0;
char currentPage, selectedUnit;
long duration;
int distanceInch, distanceCm;
int coval=0;
int smokeval=0;
double readThermocouple();

#define MAX6675_CS   17
#define MAX6675_SO   18
#define MAX6675_SCK  16
int smokeA0 = A5;
void setup() {
 
  Serial.begin(9600);
  pinMode(15,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(13,OUTPUT);
   pinMode(smokeA0, INPUT);
   myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
 currentPage = '0';
  drawHomeScreen();
 //double readThermocouple();
}
void loop() { 
  int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  delay(100);
   Serial.print(readThermocouple());
  Serial.println('c');
  digitalWrite(15,HIGH);
  digitalWrite(14,LOW);
  digitalWrite(13,HIGH);
 // delay(1000);
 
 if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); // X coordinate where the screen has been pressed
      y=myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button 
      if ((x>=35) && (x<=285) && (y>=70) && (y<=110)) {
       // drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '1'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawSelectScreen();
      }
       if ((x>=35) && (x<=285) && (y>=120) && (y<=160)) {
       // drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '2'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawFanControl();
      
      }
      }
      }
     if (currentPage == '1') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); // X coordinate where the screen has been pressed
      y=myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button 
      if ((x>=10) && (x<=60) && (y>=10) && (y<=36)) {
       // drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '0'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawHomeScreen();
      }
       if ((x>=35) && (x<=285) && (y>=70) && (y<=110)) {
       // 35, 70, 285, 110
       // drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '3'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawTeaQuant();
        i=0;
          }
      
       if ((x>=35) && (x<=285) && (y>=120) && (y<=160)) {
       // 35, 120, 285, 160
       // drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '6'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawTeaQuant();
        i=0;
          }
      }
      }  
      if (currentPage == '2') {
         setFanSpeed();
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
         digitalWrite(10,LOW);
        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          //drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
          // Turns the LED off
          analogWrite(prifan, 0);
          analogWrite(secfan, 0);
          analogWrite(fuel, 0);
        }
    }
  }
   if (currentPage == '3') {
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();


         if ((x>=160) && (x<=300) &&(y>=140) && (y<=180)) {
         // 160, 140, 300, 180
          currentPage = '4';
          myGLCD.clrScr();
          

        }
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          currentPage = '1';
          myGLCD.clrScr();
          drawSelectScreen();

        }
    } 
 }
 if(currentPage== '4'){
   if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
      drawTimeReq();

if( (x>=35) && (x<=285))
{
  if(  (y>=35) && (y<=110))
  {
    slow= 60;
    Serial.println(slow);
    currentPage='5';
    }
  if( (y>=120) && (y<=160) )
  {
    medium=45;
    Serial.println(medium);
     currentPage='5';
    }
    if( (y>=170) && (y<=210)  )
    {

     quick=30; 
     Serial.println(quick);
      currentPage='5';
      }
  
  }

     // (35, 70, 285, 110)
     // (35, 120, 285, 160)
     // 35, 170, 285, 210
     //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          currentPage = '3';
          myGLCD.clrScr();
          drawTeaQuant();
        }
  }
   }

  if (currentPage == '5') {
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
         digitalWrite(10,HIGH);
        drawTeaThank();
       

        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          //drawFrame(10, 10, 60, 36);
          currentPage = '4';
          myGLCD.clrScr();
          drawTeaQuant();
          // Turns the LED off
        }
         if ((x>=205) && (x<=270) &&(y>=10) && (y<=36)) {
          //drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
          // Turns the LED off
        }
        
        
    }
  }
  if(currentPage=='6'){

       if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        drawTeaQuant();

        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          //drawFrame(10, 10, 60, 36);
          currentPage = '1';
          myGLCD.clrScr();
          drawSelectScreen();          // Turns the LED off
        }
    }
    } 
}    
void drawHomeScreen() {
 int analogSensor = analogRead(smokeA0);
 myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("BIO-MASS STOVE", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,32,319,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by RGUKT RK Valley", CENTER, 41); // Prints the string
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 70, 285, 110); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 70, 285, 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Select Food", CENTER, 82); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 120, 285, 160);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 120, 285, 160);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("FAN CONTROL", CENTER, 132);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(0, 255, 0);
  myGLCD.print("TEMP:", 20, 170);
   if ( readThermocouple() <=800) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(0, 0 ,255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(readThermocouple(),40, 200, 3,'0');
    myGLCD.setFont(BigFont);
    
   }
   myGLCD.setColor(0, 255, 0);
   myGLCD.print("CO:", 140, 170);
     if ( coval<=140) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(0, 0 ,255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(coval,138, 200, 3,'0');
    myGLCD.setFont(BigFont);
   }
   myGLCD.setColor(0, 255, 0);
   myGLCD.print("SMOKE:", 220, 170);
    if (analogSensor <=400) {
    myGLCD.setFont(BigFont);
    myGLCD.setColor(0, 0 ,255);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(analogSensor,230, 200, 3,'0');
    myGLCD.setFont(BigFont);
   }
  myGLCD.setFont(SmallFont);  //units
    myGLCD.setColor(255, 255, 255);
  myGLCD.print("C", 100, 200);
   myGLCD.setColor(255, 255, 255);
  myGLCD.print("PPM", 195, 200);
   myGLCD.setColor(255, 255, 255);
  myGLCD.print("PPM", 290, 200);
  readThermocouple();
}
void drawSelectScreen() 

{
   
   myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 70, 285, 110); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 70, 285, 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Tea/Coffee/Milk", CENTER, 82); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 120, 285, 160);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 120, 285, 160);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Rice", CENTER, 132);

   // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 170, 285, 210);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 170, 285, 210);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Curry", CENTER, 182);
//back button
 myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  readThermocouple();
   int analogSensor = analogRead(smokeA0);
}
 void drawFanControl() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("Fans Speed Control", CENTER, 50);
  myGLCD.print("Fan Speed:", 10, 95);
  myGLCD.print("P", 10, 135);
  myGLCD.print("S", 10, 175);
   myGLCD.print("F", 10, 215);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,75,319,75); 
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(30, 138, 310, 148); // R - Slider
  myGLCD.drawRect(30, 178, 310, 188);
  myGLCD.drawRect(30, 218, 310, 228); 
   readThermocouple();
    int analogSensor = analogRead(smokeA0);
 }
 void setFanSpeed() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();        
    // Area of the Red color slider
    if( (y>=130) && (y<=156)) {
      pf=x; // Stores the X value where the screen has been pressed in to variable xR
      if (pf<=38) { // Confines the area of the slider to be above 38 pixels
        pf=38;
      }
      if (pf>=303){ /// Confines the area of the slider to be under 310 pixels
        pf=303;
      }
    }
    // Area of the Green color slider
    if( (y>=170) && (y<=196)) {
      sf=x;
      if (sf<=38) {
        sf=38;
      }
      if (sf>=303){
        sf=303;
      }
    }
      if( (y>=210) && (y<=236)) {
      fp=x;
      if (fp<=38) {
        fp=38;
      }
      if (fp>=303){
        fp=303;
      }
    }    
  }
   int pfC = map(pf,38,310,0,255);
  int sfC = map(sf,38,310,0,255);
   int fpC = map(fp,38,310,0,255);
  
   // Sends PWM signal to the pins of the led
  analogWrite(prifan, pfC);
  analogWrite(secfan, sfC);
  analogWrite(fuel, fpC);
 // analogWrite(blueLed, xBC);
  
  // Draws a rectangle with the latest color combination 
  myGLCD.setColor(pfC, sfC, fpC);
  myGLCD.fillRoundRect(175, 87, 310, 119);
  //values

  
  // Draws the positioners
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(pf,139,(pf+4),147); // Positioner
  myGLCD.setColor(pfC, 0, 0);
  myGLCD.fillRect(31, 139, (pf-1), 147);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((pf+5), 139, 309, 147);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(sf,179,(sf+4),187);
  myGLCD.setColor(0, sfC, 0);
  myGLCD.fillRect(31, 179, (sf-1), 187);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((sf+5), 179, 309, 187);

    myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(fp,219,(fp+4),227);
  myGLCD.setColor(0, 0, fpC);
  myGLCD.fillRect(31, 219, (fp-1), 227);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((fp+5), 219, 309, 227);
  readThermocouple();
   int analogSensor = analogRead(smokeA0);
  }
  void drawTeaQuant(){

    
       
          
  /* myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 70, 285, 110); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 70, 285, 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Tea/Coffee", CENTER, 82); // Prints the string*/
      myGLCD.setFont(BigFont);
          for (x=0; x<5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10+(x*60), 50, 60+(x*60), 90);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10+(x*60), 50, 60+(x*60), 90);
    myGLCD.setBackColor (0, 0, 255);
    myGLCD.printNumI(x+1, 27+(x*60), 62);
  }
// Draw the center row of buttons
  for (x=0; x<5; x++)
  {
    myGLCD.setColor(0, 0, 255);
    myGLCD.fillRoundRect (10+(x*60), 95, 60+(x*60), 135);
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (10+(x*60), 95, 60+(x*60), 135);
    myGLCD.setBackColor (0, 0, 255);
    if (x<4)
      myGLCD.printNumI(x+6, 27+(x*60), 107);
  }
  myGLCD.print("0", 267, 107);
// Draw the lower row of buttons
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (10, 140, 150, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 140, 150, 180);
  myGLCD.print("Clear", 40, 152);
  myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (160, 140, 300, 180);
  myGLCD.setColor(255, 255,255);
  myGLCD.drawRoundRect (160, 140, 300, 180);
  myGLCD.setBackColor (0, 0, 255);
  myGLCD.print("Enter", 190, 152);

   myGLCD.setColor(0, 0, 255);
  myGLCD.fillRoundRect (240, 185, 300, 210);
  myGLCD.setColor(255, 255,255);
  myGLCD.drawRoundRect (240, 185, 300, 210);
  myGLCD.setBackColor (0, 0, 255);
  myGLCD.print("-->", 250, 190);
 //x>=160) && (x<=300) &&(y>=185) && (y<=235)
      
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Enter Quantity of the the Food", 70, 18);

readThermocouple();
 int analogSensor = analogRead(smokeA0);
   while ((i==0 )&& (currentPage!=4) )
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      
      if ((y>=50) && (y<=90))  // Upper row
      {
        if ((x>=10) && (x<=60))  // Button: 1
        {
          waitForIt(10, 50, 60, 90);
          updateStr('1');
        }
        if ((x>=70) && (x<=120))  // Button: 2
        {
          waitForIt(70, 50, 120, 90);
          updateStr('2');
        }
        if ((x>=130) && (x<=180))  // Button: 3
        {
          waitForIt(130, 50, 180, 90);
          updateStr('3');
        }
        if ((x>=190) && (x<=240))  // Button: 4
        {
          waitForIt(190, 50, 240, 90);
          updateStr('4');
        }
        if ((x>=250) && (x<=300))  // Button: 5
        {
          waitForIt(250, 50, 300, 90);
          updateStr('5');
        }
      }

      if ((y>=95) && (y<=135))  // Center row
      {
        if ((x>=10) && (x<=60))  // Button: 6
        {
          waitForIt(10, 95, 60, 135);
          updateStr('6');
        }
        if ((x>=70) && (x<=120))  // Button: 7
        {
          waitForIt(70, 95, 120, 135);
          updateStr('7');
        }
        if ((x>=130) && (x<=180))  // Button: 8
        {
          waitForIt(130, 95, 180, 135);
          updateStr('8');
        }
        if ((x>=190) && (x<=240))  // Button: 9
        {
          waitForIt(190, 95, 240, 135);
          updateStr('9');
        }
        if ((x>=250) && (x<=300))  // Button: 0
        {
          waitForIt(250, 95, 300, 135);
          updateStr('0');
        }
      }

      if ((y>=140) && (y<=180))  // Upper row
      {
        if ((x>=10) && (x<=150))  // Button: Clear
        {
          waitForIt(10, 140, 150, 180);
          stCurrent[0]='\0';
          stCurrentLen=0;
          myGLCD.setColor(0, 0, 0);
          myGLCD.fillRect(0, 224, 319, 239);
        }
        if ((x>=160) && (x<=300))  // Button: Enter
        {
          waitForIt(160, 140, 300, 180);
          if (stCurrentLen>0)
          {
            for (x=0; x<=4; x++)
            {
              stLast[x]=stCurrent[x];
            }
            stCurrent[0]='\0';
            stCurrentLen=0;
            myGLCD.setColor(0, 0, 0);
            myGLCD.fillRect(0, 208, 319, 239);
            myGLCD.setColor(0, 255, 0);
            myGLCD.print(stLast, LEFT, 208);
             int luck = atoi(stLast);
             Serial.println(luck);
          }
          else
          {
            myGLCD.setColor(255, 0, 0);
            myGLCD.print("BUFFER EMPTY", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            delay(500);
            myGLCD.print("BUFFER EMPTY", CENTER, 192);
            delay(500);
            myGLCD.print("            ", CENTER, 192);
            myGLCD.setColor(0, 255, 0);
          }
        }
      }
    }
  
    if ((x>=200) && (x<=300) &&(y>=185) && (y<=220)) {
          i=1;
          //160, 140, 300, 180
          //drawFrame(10, 10, 60, 36);
          currentPage = '4';
          myGLCD.clrScr();
          drawTimeReq();
    }
     if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          i=1;
          //drawFrame(10, 10, 60, 36);
          currentPage = '1';
          myGLCD.clrScr();
          drawSelectScreen();
    }
  }
    }

    void updateStr(int val)
{
  if (stCurrentLen<20)
  {
    stCurrent[stCurrentLen]=val;
    stCurrent[stCurrentLen+1]='\0';
    stCurrentLen++;
    myGLCD.setColor(0, 255, 0);
    myGLCD.print(stCurrent, LEFT, 224);
  }
  else
  {
    myGLCD.setColor(255, 0, 0);
    myGLCD.print("BUFFER FULL!", CENTER, 192);
    delay(500);
    myGLCD.print("            ", CENTER, 192);
    delay(500);
    myGLCD.print("BUFFER FULL!", CENTER, 192);
    delay(500);
    myGLCD.print("            ", CENTER, 192);
    myGLCD.setColor(0, 255, 0);
  }
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
}
void drawTimeReq(){

 myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 70, 285, 110); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 70, 285, 110); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("Slow", CENTER, 82); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 120, 285, 160);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 120, 285, 160);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Medium", CENTER, 132);

   // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 170, 285, 210);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 170, 285, 210);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Quick", CENTER, 182);

  
   myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Time to Cook", 70, 18);
  }
  void drawTeaThank(){
  myGLCD.clrScr();
 myGLCD.setColor(16, 167, 103);
/*  myGLCD.fillRoundRect (35, 120, 285, 160);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 120, 285, 160);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);*/
  myGLCD.setColor(255, 255, 255);
   myGLCD.setFont(BigFont);
  myGLCD.print("Thank you.....", CENTER, 132);
  myGLCD.setFont(BigFont);
// myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("Press home", CENTER, 182);

   
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Time to Cook", 70, 18);
 
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (205, 10, 270, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (205, 10, 270, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("Home", 205, 15);
  

  }
  double readThermocouple() {

  uint16_t v;
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  // Read in 16 bits,
  //  15    = 0 always
  //  14..2 = 0.25 degree counts MSB First
  //  2     = 1 if thermocouple is open circuit  
  //  1..0  = uninteresting status
  
  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  
  digitalWrite(MAX6675_CS, HIGH);
  if (v & 0x4) 
  {    
    // Bit 2 indicates if the thermocouple is disconnected
    return NAN;     
  }

  // The lower three bits (0,1,2) are discarded status bits
  v >>= 3;

  // The remaining bits are the number of 0.25 degree (C) counts
  return v*0.25;
}

