 // 11/12/2023 v3 two hand
 // please test TwoHandPlaying function befor use
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




int L_ot[3] = {3,4,5};
int R_ot[3] = {6,7,8};
int L_rd[3];
int R_rd[3];


int ot[6] = {3,4,5,6,7,8};
int opot;
bool isRun =false;
void setup(){
      wdt_enable(WDTO_8S);
      Serial.begin(9600);
      pinMode(2, INPUT_PULLUP);
      
      pinMode(13,OUTPUT); // LED

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

//===================================================================================
void loop() 
{

   if (Serial.available() > 0) {
    // read the incoming byte:
    String inpKey = Serial.readStringUntil('\n');
    //if ( inpKey.substring(0,1) == "s") { playing();  }
    //if ( inpKey == "start") { playing(); }
    if ( inpKey == "s") {  TwoHandPlaying();  }
    if ( inpKey == "so") {  OneHandPlaying();  }
    if ( inpKey == "t") {   Serial.println("test");    Test(); }
    if ( inpKey == "w") {   SetTwoHandRnd(); }
  }
    

  int sensorVal = digitalRead(2);
  if (sensorVal == LOW  &&   isRun ==false) {
     TwoHandPlaying();    Serial.println(" Press Switch  ");     
  }      
       

  //Running();
  delay(50);  wdt_reset();

} //================================================================================
void OneHandRandomRelay() {
  
  // Generate a random order for relay activation
    for (int i = 0; i < relayCount; i++) {
      relayOrder[i] =relayPins[i];
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
void OneHandPlaying() {
    wdt_reset();  Serial.println(" Start Game  ");

    for (int i=0;i<n;i++) { 
       digitalWrite(ot[i], LOW); delay(100);
    }   

     isRun =true;  digitalWrite(13, HIGH);

     rd =random(35); Serial.println(rd);
   //PrintData();
    wdt_reset();
    for (int i=0;i<n;i++) {              
              Serial.print(i);  Serial.print(":");  Serial.print(array1[rd][i]);
              delayRd =  random(300,800);    Serial.print("      delayRd : ");   Serial.println(delayRd) ;
              opot=ot[array1[rd][i]-1];      Serial.print("  output : ");   Serial.println(opot) ;
              digitalWrite(opot, HIGH);    delay(300);       digitalWrite(opot, LOW);
              delay (delayRd);      wdt_reset();
      }

      isRun =false;
      digitalWrite(13, LOW);
      Serial.println("Finish Game:");
}
void TwoHandPlaying() {
    wdt_reset();  Serial.println(" Start Game  ");
    for (int i=0;i<n;i++) { 
       digitalWrite(ot[i], LOW);
       delay(100);
    }   
    SetTwoHandRnd();    
      isRun =true; digitalWrite(13, HIGH);        
      wdt_reset();
      for (int i=0;i<3;i++) {              
              Serial.print(i);  Serial.print("L-R :");  Serial.print(L_rd[i]); Serial.print(" - ");  Serial.print(R_rd[i]);
              delayRd =  random(300,800);    Serial.print("   delayRd : ");   Serial.println(delayRd) ;

              digitalWrite(L_rd[i], HIGH);    delay(300);       digitalWrite(L_rd[i], LOW);   delay(100);
              digitalWrite(R_rd[i], HIGH);    delay(300);       digitalWrite(R_rd[i], LOW);              
              
              delay (delayRd);    wdt_reset();
      }
      isRun =false; digitalWrite(13, LOW); Serial.println("Finish Game:");
}


void Test() {
  isRun =false;
    digitalWrite(3, HIGH);delay(100); digitalWrite(3, LOW);delay(1000);
    digitalWrite(4, HIGH);delay(100);digitalWrite(4, LOW);delay(1000);
    digitalWrite(5, HIGH);delay(100);digitalWrite(5, LOW);delay(1000);
    digitalWrite(6, HIGH);delay(100); digitalWrite(6, LOW);delay(1000);
    digitalWrite(7, HIGH);delay(100); digitalWrite(7, LOW);delay(1000);
    digitalWrite(8, HIGH);delay(100);digitalWrite(8, LOW);delay(1000);
}
void PrintData() {
    Serial.println("j--");
    for (int j=0;j<n;j++) {
       Serial.print("   ");  Serial.print(array1[rd][j]); Serial.print("--");   Serial.print(ot[array1[rd][j]-1]); 
    }
    Serial.println("--");
}

void SetTwoHandRnd() {
int rlc = 3;
    for (int i = 0; i < relayCount; i++) {
      L_rd[i] =relayPins[i];
      R_rd[i] =relayPins[i+rlc];
    }
    for (int i = rlc - 1; i > 0; i--) {
      int j = random(i + 1);
      // Swap relayOrder[i] and relayOrder[j]
      int temp = L_rd[i];  L_rd[i] = L_rd[j]; L_rd[j] = temp;    
       int k = random(i + 1);
      // Swap relayOrder[i] and relayOrder[j]
      int temp2 = R_rd[i];  R_rd[i] = L_rd[k]; L_rd[k] = temp2;   
    }
    PrintL_rd();  PrintR_rd();
}



void SetTwoHandRnd2() {
    int rd ;

    rd =random(1,3);
    if (rd == 1) {
        L_rd[0] = 1-1;   rd =random(0,1);
        if (rd == 0) { L_rd[1] = 2-1;  L_rd[2] = 3-1;   }
                else { L_rd[1] = 3-1;  L_rd[2] = 2-1;  }      
    } else if (rd == 2) {
        L_rd[0] = 2-1;   rd =random(0,1);
        if (rd == 0) { L_rd[1] = 1-1;  L_rd[2] = 3-1;   }
                else { L_rd[1] = 3-1;  L_rd[2] = 1-1;  }     
    } else if (rd == 3) {
        L_rd[0] = 3-1;   rd =random(0,1);
        if (rd == 0) { L_rd[1] = 1-1;  L_rd[2] = 2-1;   }
                else { L_rd[1] = 2-1;  L_rd[2] = 1-1;  }     
    }

    rd =random(4,6);  
    if (rd == 4) {
        R_rd[0] = 4-1;   rd =random(0,1);
        if (rd == 0) { R_rd[1] = 5-1;  R_rd[2] = 6-1;   }
                else { R_rd[1] = 6-1;  R_rd[2] = 5-1;  }      
    } else if (rd == 5) {
        R_rd[0] = 5-1;   rd =random(0,1);
        if (rd == 0) { R_rd[1] = 4-1;  R_rd[2] = 6-1;   }
                else { R_rd[1] = 6-1;  R_rd[2] = 4-1;  }     
    } else if (rd == 6) {
        R_rd[0] = 6-1;   rd =random(0,1);
        if (rd == 0) { R_rd[1] = 4-1;  R_rd[2] = 5-1;   }
                else { R_rd[1] = 5-1;  R_rd[2] = 4-1;  }   
    }   
    PrintL_rd();  PrintR_rd();
}

void PrintL_rd() {
    Serial.print("L_rd --");    
    for (int j=0;j<3;j++) {
       Serial.print("   ");      Serial.print(L_rd[j]);
    }
    Serial.println("--");
}
void PrintR_rd() {
    Serial.print("R_rd --");    
    for (int j=0;j<3;j++) {
       Serial.print("   ");   Serial.print(R_rd[j]);
    }
    Serial.println("--");
}