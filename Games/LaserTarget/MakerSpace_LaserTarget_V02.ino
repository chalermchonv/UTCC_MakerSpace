 
 // 11/12/2023 v2
//===============
// you had to adjest Lower and Upper of left and Right in ecah mahcine becuase LDRs may differy value 

#include <Ticker.h>
int L_Lower = 2000;  int L_Upper = 3000;
int R_Lower = 3100;  int R_Upper = 3300;

int endScore = 20;
int diffDelay = 100;
int TargetAt = 0;
int counterLoop;
int endCounter = 10;

#define L_Target 36  
#define R_Target 35
#define L_Led 21
#define C_Led 19 
#define R_Led 18
#define St_Btn 12

int isPlay = 1,shDelay = 200, L_score = 0, R_score = 0; 

int L_TargetValue, R_TargetValue,  L_TargetValue_Ref, R_TargetValue_Ref;

// check shooted
int L_shooted = 0,   R_shooted = 0, L_lowShooted = 0, L_upShooted = 0,  R_lowShooted = 0, R_upShooted = 0;
int L_cUpShooted =0,     L_cLowShooted = 0,  L_cUpNegShooted = 0, L_cLowNegShooted = 0;
int R_cUpShooted = 0,    R_cLowShooted =0,   R_cUpNegShooted = 0, R_cLowNegShooted = 0;

// Set Shooting diff value change
int L_base= 300, R_base= 300, diff = 200 ;
int IsShooted = 0;
int isL_shooted = 0,isR_shooted=0;
int L_diff,R_diff  ;
int IsDebug = 0,isSendToPc,btn,IsMinus = 0;

// timer
Ticker  count_time1;
int h = 0, minn = 0, sec = 0, i = 0, cSec = 0;
void c_time(){
  i++;
  if(i >= 10)   {    sec++;    i = 0;  cSec ++;  }
  if(sec >= 60) {    minn++;   sec = 0;  }
  if(minn >= 60){    h++;     minn = 0;  }
}
void resetTime() {h = 0, minn = 0, sec = 0, i = 0; cSec = 0;}

void showStartUp() {
  Serial.println("Start LaserTarget V02 (s 19200)");    
   printIntEnd("l_base=",L_base,",");  
   printIntEnd("r_base=",R_base,",");
   printIntEnd("l_diff=",L_diff,",");  
   printIntEnd("r_diff=",R_diff,"/n");

   printIntEnd("l_low=",L_Lower,",");  
   printIntEnd("l_up=",L_Upper,",");
   printIntEnd("r_low=",R_Lower,",");  
   printIntEnd("r_up=",R_Upper,",");
   printIntEnd("endScore=",endScore,",");
   Serial.println("Set by ex: l_low=int? ");  

   //Serial.print("l_base="); Serial.print(L_base);    Serial.print(",");    
   //Serial.print("r_base="); Serial.print(R_base);    Serial.print(",");    
   //Serial.print("l_diff="); Serial.print(L_diff);    Serial.print(",");    
   //Serial.println("r_diff="); Serial.print(R_diff);      

   //Serial.print("l_low="); Serial.print(L_Lower);    Serial.print(",");    
   //Serial.print("l_up="); Serial.print(L_Upper);    Serial.print(",");    
   //Serial.print("r_low="); Serial.print(R_Lower);    Serial.print(",");    
   //Serial.println("r_up="); Serial.print(R_Upper);    Serial.print(",");    
   //Serial.println("Set by ex: l_low=int? ");  
}
// =================================================================
void setup() {
  // initialize serial communication at 9600 bits per second:
   Serial.begin(19200);
   count_time1.attach(0.1, c_time);     //Interrupt ทุกๆ 0.1 วินาที

   pinMode(L_Led, OUTPUT);
   pinMode(C_Led, OUTPUT);
   pinMode(R_Led, OUTPUT);
   pinMode(St_Btn, INPUT_PULLUP);

   L_TargetValue_Ref   = analogRead(L_Target);
   R_TargetValue_Ref = analogRead(R_Target);
 
    L_Lower = L_TargetValue_Ref - diff;    L_Upper = L_TargetValue_Ref + diff;
    R_Lower = R_TargetValue_Ref - diff;    R_Upper = R_TargetValue_Ref + diff;

   showStartUp();  
}
 
//============================================= VVVVVVVVVVVVVVVV =====================================
void loop() {

   if (Serial.available() > 0)  ReadCommand(); 

   btn = digitalRead(St_Btn);  
   if (btn == LOW) {  StartGame(); PrintScore();   } // L_score = 0;  R_score =0;  isPlay = 1;  
   
    // Play Loop
   if (isPlay == 1)
   { 
       // Read Target and Check Shooted Target
       L_TargetValue = analogRead(L_Target);     L_diff = abs(L_TargetValue-L_TargetValue_Ref);        
       R_TargetValue = analogRead(R_Target);     R_diff = abs(R_TargetValue-R_TargetValue_Ref);
       
       // Set Up and Low Target Shooted by debug=1;  cheack value and swap _Upper and _Lower 
        if (L_diff > L_base && L_TargetValue > L_Upper  )  L_lowShooted = 1;
        if (L_diff > L_base && L_TargetValue < L_Lower   )  L_upShooted = 1;

        if (R_diff > R_base && R_TargetValue > R_Upper   )  R_lowShooted = 1;
        if (R_diff > R_base && R_TargetValue < R_Lower  )  R_upShooted = 1;
        
        if (TargetAt == 0 ) {       
              // Low Target                       
              if (L_lowShooted == 1 && isL_shooted ==0) {isL_shooted =1; isSendToPc = 1; L_score = L_score + 1;  L_cLowShooted ++; } //OnOffLed(L_Led,shDelay);  
              if (R_lowShooted == 1 && isR_shooted ==0) {isR_shooted= 1; isSendToPc = 1;R_score = R_score + 1;  R_cLowShooted++ ; }//OnOffLed(R_Led,shDelay);  
              if (IsMinus ==1) {
                  if (L_upShooted == 1 && isR_shooted ==0)  {  L_score = L_score - 1;  L_cLowNegShooted++; } 
                  if (R_upShooted == 1 && isR_shooted ==0)  {  R_score = R_score - 1;  R_cLowNegShooted++;  }
              }
        }  
        else {
              // upTarget
              if (L_upShooted == 1 && isL_shooted ==0)  {isL_shooted =1;isSendToPc = 1; L_score = L_score + 2; L_cUpShooted++; }  //OnOffLed2(L_Led,shDelay);  
              if (R_upShooted == 1 && isR_shooted ==0)  {isR_shooted =1;isSendToPc = 1; R_score = R_score + 2; R_cUpShooted++; }  //OnOffLed2(R_Led,shDelay);  
              if (IsMinus ==1) {
                 if (L_lowShooted == 1 && isL_shooted ==0) {isSendToPc = 1; L_score = L_score -1; L_cUpNegShooted++; } 
                 if (R_lowShooted == 1 && isR_shooted ==0 ) {isSendToPc = 1; R_score = R_score -1; R_cUpNegShooted++; }
              }
        }
        if (isL_shooted ==1 || isR_shooted == 1) {
             SendToPc();  isSendToPc =0;  
             ShowLedScore;
             digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);        
             delay(random(800,1400));
             TargetAt =random(0,1);
           
             if (random(1,10)>5 ) {  
                 TargetAt =1; digitalWrite(L_Led, LOW); digitalWrite(C_Led, HIGH); digitalWrite(R_Led, LOW); 
              } else {              
                 TargetAt =0;  digitalWrite(L_Led, HIGH); digitalWrite(C_Led, LOW); digitalWrite(R_Led, HIGH); 
              }           
                printlnInt("Rnd : ",TargetAt);
           isL_shooted = 0;isR_shooted = 0;
        }
       // print to pc
        if ((IsDebug ==1) && (sec%5 ==0)) PrintScore();           
       // diff 
       delay(diffDelay);  L_TargetValue_Ref = L_TargetValue;   R_TargetValue_Ref = R_TargetValue; 
       // clear shooted
       L_lowShooted = 0;L_upShooted =0;   R_lowShooted = 0;R_upShooted =0;
  
    } 

     // Finish
     if (isPlay == 1 && (L_score >= endScore || R_score >= endScore)) {  isPlay = 0;  PrinFinishGame();  }
 
} //========================================================  WWWWWWWWWWWWW   ================
 
void StartGame() {
    L_score = 0;  R_score =0;  TargetAt == 0;
    isL_shooted = 0;isR_shooted = 0; 
    L_cLowShooted = 0; R_cLowShooted = 0;
    L_cUpShooted = 0; R_cUpShooted = 0;
    TargetAt = 0;
    int dt = 200; 
    // say what you got:
      Serial.println("Start Playing: ");
      for (int i = 0 ; i <3;i++) {
          digitalWrite(L_Led, HIGH); digitalWrite(C_Led, HIGH); digitalWrite(R_Led, HIGH); delay(dt);   
          digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);  delay(dt);       
      }         

      digitalWrite(L_Led, HIGH);  digitalWrite(C_Led, LOW); digitalWrite(R_Led, HIGH);    
      isPlay = 1; resetTime(); PrintScore();    SendToPc() ;
}
void ShowLedScore() {
  int dt =250;int n = 5;
  if (isL_shooted == 1 && isL_shooted == 1) {
      for (int i = 0 ; i <n;i++) {
          digitalWrite(L_Led, HIGH); digitalWrite(C_Led, LOW); digitalWrite(R_Led, HIGH); delay(dt);   
          digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);  delay(dt);       
      }  
  } else  if (isL_shooted == 1 && isL_shooted == 0) {
       for (int i = 0 ; i <n;i++) {
          digitalWrite(L_Led, HIGH); digitalWrite(C_Led, LOW); digitalWrite(R_Led, LOW); delay(dt);   
          digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);  delay(dt);       
      }  

  }else  if (isL_shooted == 0 && isL_shooted == 1 ) {
       for (int i = 0 ; i <n;i++) {
          digitalWrite(L_Led, LOW); digitalWrite(C_Led, LOW); digitalWrite(R_Led, HIGH); delay(dt);   
          digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);  delay(dt);       
      }  
  }
 

}
void ReadCommand() {
    // read the incoming byte:
      String inpKey = Serial.readStringUntil('\n');
      //if ( inpKey.substring(0,1) == "p") {  StartGame();  }
      if ( inpKey == "p") {  StartGame();  }
      if ( inpKey == "start") {  StartGame(); }
      if ( inpKey == "ps") {  PrintParameters(); }
     /* switch (inpKey.substring(0,6)) {
        case "l_low=":  L_Lower = SetValueInt(inpKey,"=") ;     break;       
          default :    break;      } */ 

      String setStr = inpKey.substring(0,6);

      if ( setStr == "l_low=")       {  L_Lower = SetValueInt(inpKey,"=") ; }
      else if ( setStr == "l_up=")   {  L_Upper = SetValueInt(inpKey,"=") ; }     
      else if ( setStr == "r_low=")  {  R_Lower = SetValueInt(inpKey,"=") ;}
      else if ( setStr == "r_up=")   {  R_Upper = SetValueInt(inpKey,"=") ; }     
      else if ( setStr == "l_base=") {  L_base = SetValueInt(inpKey,"=") ;}     
      else if ( setStr == "r_base=") {  R_base = SetValueInt(inpKey,"=") ;}     
      else if ( setStr == "l_diff=") {  L_diff = SetValueInt(inpKey,"=") ;}     
      else if ( setStr == "r_diff=") {  R_diff = SetValueInt(inpKey,"=") ; }  
      else if ( setStr == "debug=") {  IsDebug = SetValueInt(inpKey,"=") ; }  
      else if ( setStr== "minus=") {  IsMinus = SetValueInt(inpKey,"=") ; }  
      else if ( inpKey.substring(0,9) == "endScore=") {  endScore = SetValueInt(inpKey,"=") ; }  
      
  
}
//+++++++++++++++++++++++++++++++++++++++++++

void PrintScore() {  
    printInt(">> Score L : ",L_score);     printInt(" R : ",R_score); 
    printInt(" -Value L ",L_TargetValue);  printInt(" R ",R_TargetValue);   
    printInt(" Ref L ",L_TargetValue_Ref); printlnInt(" R ",R_TargetValue);    
               
}
void SendToPc() {
    String data;

    // Send Score
    data = "#,";   
    data.concat(L_score) ; data += ",";   data.concat(R_score) ;
    data +=  (L_score >= endScore || R_score >=endScore) ? ",Finish Game": ",Playing";       
    Serial.println(data);

    // Send Target Info
    data = "?/" ;   //Shooted  L_Up/L_Low/L_negUp/L_negLow
    data.concat(L_cLowShooted) ; data += "/" ; data.concat(L_cUpShooted) ; 
    data += "/" ;   data.concat(L_cLowNegShooted) ; data += "/" ;   data.concat(L_cUpNegShooted) ; 
    data += "/" ; // Shooted  R_Up/R_Low/R_negUp/R_negLow  
    data.concat(R_cLowShooted) ; data += "/" ; data.concat(R_cUpShooted) ; 
    data += "/" ;   data.concat(R_cLowNegShooted) ; data += "/" ;   data.concat(R_cUpNegShooted) ;      

    Serial.println(data);     
}

void PrintParameters() {
  
    printInt("L_Lower  : ",L_Lower);     printlnInt(" L_Upper : ",L_Upper); 
    printInt("R_Lower L ",R_Lower);  printlnInt(" R_Upper ",R_Upper);   
}

void PrinFinishGame() {
    Serial.println("-----------------------------"); 
    Serial.println("Finish Game ######################"); 
                    
    digitalWrite(L_Led, HIGH);  digitalWrite(C_Led, HIGH);  digitalWrite(R_Led, HIGH);
    delay(1000);digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);
    delay(1000);digitalWrite(L_Led, HIGH);  digitalWrite(C_Led, HIGH);  digitalWrite(R_Led, HIGH);
    delay(1000);digitalWrite(L_Led, LOW);  digitalWrite(C_Led, LOW);  digitalWrite(R_Led, LOW);

    if (L_score == R_score ) {
         digitalWrite(L_Led, HIGH);  digitalWrite(C_Led, HIGH);  digitalWrite(R_Led, HIGH);
         Serial.println("Left and Righ Win Game  "); 
     } else {
          if (L_score > R_score )  {
               digitalWrite(L_Led, HIGH);  digitalWrite(C_Led, HIGH);  digitalWrite(R_Led, LOW);
               Serial.println("Left Win Game  "); 
          }else {
                digitalWrite(L_Led, LOW);  digitalWrite(C_Led, HIGH);  digitalWrite(R_Led, HIGH);
                Serial.println("Righ Win Game  "); 
          }
     }
     Serial.println("-----------------------------");     
}


int SetValueInt(String setValue,String splitV) {  	
      int vint = (setValue.substring(setValue.indexOf(splitV)+1, setValue.length())).toInt();
      printIntEnd("Set "+ setValue + " = ",vint,"/n");    
    return vint;
}

void Process() {
  
    // Left
    if (L_diff > 300) {
      PrintDate();
          if (L_TargetValue > L_TargetValue_Ref ) {
                Serial.println("L +++");  digitalWrite(L_Led, HIGH);        }
          else {
              Serial.println("L ---");    digitalWrite(L_Led, LOW); 
          }
    }
    if (R_diff >300) {
        PrintDate();
        if (R_TargetValue > R_TargetValue_Ref ) {
            Serial.println("R +++");    
            digitalWrite(R_Led, HIGH);   }
        else {
            Serial.println("R ---");  
            digitalWrite(R_Led, LOW);
        }
    }  

    delay(1500);

}

void OnOffLed(int pinLed,int timeDelay) {
    digitalWrite(pinLed, HIGH);     delay(timeDelay);       digitalWrite(pinLed, LOW); delay(timeDelay);  
}
void OnOffLed2(int pinLed,int timeDelay) {
    digitalWrite(pinLed, HIGH);     delay(timeDelay);       digitalWrite(pinLed, LOW); 
    delay(timeDelay);  
    digitalWrite(pinLed, HIGH);     delay(timeDelay);       digitalWrite(pinLed, LOW); 
}

void printlnInt(String lebel,int value) {
     Serial.print(lebel);   Serial.println(value);      
}

void printIntEnd(String lebel,int value,String endStr) {
     Serial.print(lebel);   Serial.print(value);   Serial.println(endStr);    
}
void printInt(String lebel,int value) {
     Serial.print(lebel);   Serial.print(value);      
}

void PrintDate() {
    printInt("L_TargetValue = ",L_TargetValue);   printlnInt(" <--> L_TargetValue_Ref = ",L_TargetValue_Ref);    
    printInt("L_diff = ",L_diff);                 printlnInt(" <--> R_diff = ",R_diff);                   
}

void PrintDateAll() {

    printInt("L_TargetValue = ",L_TargetValue);   // the raw analog reading
    printInt(" <--> L_TargetValue_Ref = ",L_TargetValue_Ref);   // the raw analog readingf
    printInt("L_diff = ",L_diff);   // the raw analog reading

    printInt("     R_TargetValue = ",R_TargetValue);   // the raw analog readingf    
    printInt(" <--> R_TargetValue_Ref = ",R_TargetValue_Ref);   // the raw analog readingf 
    printlnInt(" <--> R_diff = ",R_diff);   // the raw analog readingf
}


// old
void PlayGame() {

    // reads the input on analog pin (value between 0 and 4095)
       
       if (IsShooted == 0) { digitalWrite(C_Led, HIGH);  }     
    
          // PrintDateAll();
            //if (L_diff > L_base) { Serial.println("Left Shooted <=============="); }
       if (L_diff > L_base && IsShooted == 0) 
       { 
            IsShooted = 1; digitalWrite(C_Led, LOW); //   Serial.print("Left Shooted <==============");      // PrintDateAll();
                           
            if (L_TargetValue < L_Lower ) {                     
                L_score = L_score +2;      PrintScore();     OnOffLed2(L_Led,shDelay);  }  
            if (L_TargetValue > L_Upper ) { 
                L_score = L_score +1;       PrintScore();    OnOffLed(L_Led,shDelay);   }

            IsShooted = 0;
            digitalWrite(C_Led, HIGH);                  
        }
      
     if (R_diff > R_base  &&  IsShooted == 0) 
     {
             //   Serial.print("Right Shooted <==============");      // PrintDateAll();
          IsShooted = 1;  digitalWrite(C_Led, LOW);

          if (R_TargetValue < R_Lower ) {                      
                    R_score = R_score +2;  PrintScore();  OnOffLed2(R_Led,shDelay);  } 
          if (R_TargetValue > R_Upper )  {                     
                    R_score = R_score +1;   PrintScore(); OnOffLed(R_Led,shDelay); } 
          
                      
          IsShooted = 0;  digitalWrite(C_Led, HIGH);                     
      }
} //+++++++++++++++++++++++++++++++++++++++++
