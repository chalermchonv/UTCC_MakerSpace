 // 11/12/2023 v2 on stick per dropping 
#include <avr/wdt.h>
const int relayCount = 6;
const int relayPins[] = {3, 4, 5, 6, 7, 8};  // replace with your actual pin numbers
int relayOrder[relayCount];

int array1[36][6] = {
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4},
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4},
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4},
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4},
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4},
  {1,3,4,2,6,5},{2,4,5,3,1,6},{3,5,6,4,2,1},{4,6,1,5,3,2},{5,1,2,6,4,3},{6,2,3,1,5,4}
 };
int ot[6] = {3,4,5,6,7,8};
int opot;
bool isRun =false;
void setup(){
  wdt_enable(WDTO_8S);
  Serial.begin(9600);
   pinMode(2, INPUT_PULLUP);    
   pinMode(13,OUTPUT); //LED

   

    for (int i = 0; i < relayCount; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);  // turn off all relays initially
    }


   Serial.println("Catching Stick");

  }

int rd;
int n = 6;
int delayRd ;
String  inkey;

void loop(){
 
  if (Serial.available() > 0) {
    // read the incoming byte:
    String inpKey = Serial.readStringUntil('\n');
    //if ( inpKey.substring(0,1) == "s") { playing();  }
    //if ( inpKey == "start") { playing(); }
    if ( inpKey == "s") {  playing();  }
    if ( inpKey == "t") {   Serial.println("test");    Test(); }
  }
 

  int sensorVal = digitalRead(2);
  if (sensorVal == LOW  &&   isRun ==false) {
         Serial.println(" Press Switch  ");
     playing();
     
  }      
       
  //Running();
 delay(50);
  wdt_reset();
}

void OneHandRandomRelay() {
  
  // Generate a random order for relay activation
    for (int i = 0; i < relayCount; i++) {
      relayOrder[i] = i;
    }
    for (int i = relayCount - 1; i > 0; i--) {
      int j = random(i + 1);
      // Swap relayOrder[i] and relayOrder[j]
      int temp = relayOrder[i];
      relayOrder[i] = relayOrder[j];
      relayOrder[j] = temp;    
    }

  Serial.println(" Start Game  ");
  isRun =true;  digitalWrite(13, HIGH);
  // Activate relays in the random order
    for (int i = 0; i < relayCount; i++) {
      rd = relayPins[relayOrder[i]];
      digitalWrite(rd, HIGH );  delay(250); digitalWrite(rd,LOW);
      delayRd =  random(300,800);  delay(delayRd);  // adjust delay as needed      
      Serial.print(rd) ;   Serial.print(" (");   Serial.print(delayRd) ; Serial.print(" ) , ");
      wdt_reset();
    }
      Serial.println();
      isRun =false;  digitalWrite(13, LOW);
      Serial.println("Finish Game:");
}


void playing() {
      wdt_reset();
      Serial.println(" Start Game  ");
      for (int i=0;i<n;i++) {  digitalWrite(ot[i], LOW); delay(100); }
        
      isRun =true; digitalWrite(13, HIGH);
      rd =random(35);
     
      Serial.println(rd);
      //PrintData();

    wdt_reset();
    for (int i=0;i<n;i++) {
              
              Serial.print(i); Serial.print(":");  Serial.print(array1[rd][i]);
               
              opot=ot[array1[rd][i]-1];
 
              Serial.print("  output : ");  Serial.println(opot) ;
              delayRd =  random(300,800);  Serial.print("      delayRd : ");  Serial.println(delayRd) ;

              delay (delayRd);   digitalWrite(opot, HIGH);  delay(300);  digitalWrite(opot, LOW);              
              wdt_reset();
      }
   isRun =false;
   digitalWrite(13, LOW);
   Serial.println("Finish Game:");
}

void Test() {
  isRun =false;

   for (int i = 0; i < relayCount; i++) {
        digitalWrite(relayPins[i], HIGH );  delay(250); digitalWrite(relayPins[i], LOW);     
        delay(500);  // adjust delay as needed
  }
}
void PrintData() {
    Serial.println("j--");
    for (int j=0;j<n;j++) {
       Serial.print("   "); Serial.print(array1[rd][j]); Serial.print("--");  Serial.print(ot[array1[rd][j]-1]); 
    }
    Serial.println("--");
}
