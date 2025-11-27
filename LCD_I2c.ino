#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

int arriba;
int abajo;
int contador=128;     // Variable contador, inicialmente en 125
int DutyCycle;

float tempC=0;
float temp;
int mode = 0;
int L1=0;
int L2=0;
int L3=0;
int L4=0;
int L5=0;
int L6=0;
int R1=0;
const int  SetTempUp          = 10;        // pin 10 de Arduino con boton de incremento de temperatura
const int  SetTempDown        = 11;        // pin 11 de Arduino con boton de decremento de temperatura
byte boton=12;
static const byte ledPin = 9;
int PWM_LED = 8;
byte pot=A1;
static const byte sensorPin = A0; //LM35

int TemButtonCounter          = 32;       // contador de veces que se incrementa o decrementa el ajuste de temperatura (valor inicial es de 32 grados)
bool TempButtonUpState        = 0;  // estado actual del boton de incremento de temperatura
bool TempButtonDownState      = 0;  // estado actual del boton de decremento de temperatura
bool lastTempButtonUpState    = 0;  // ultimo estado (si se incremento o decremento la temperatura)
bool lastTempButtonDownState  = 0;

byte menupot,vboton,menus, menuactual,cambiomenu;

void setup()
{
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(PWM_LED,OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(SetTempUp, INPUT);
  pinMode(SetTempDown, INPUT);
  
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("-----Domo HOGAR-----");
 lcd.setCursor(0,1);
 lcd.print("Control digital de  ");
 lcd.setCursor(0,2);
 lcd.print(" Temp-Luz-Intensidad");
 lcd.setCursor(0,3);
 lcd.print("--------V1.1--------");
 delay(1000);
 lcd.clear();
}

void loop()
{
  temp = analogRead(sensorPin);
  temp = (5.0 * temp * 100.0)/1024.0;
  CtrlTEMP();
 lcd.setCursor(0,0);lcd.print("Temp:"); lcd.setCursor(5,0); lcd.print(temp,1);
 lcd.setCursor(12,0);lcd.print("SetT:");lcd.setCursor(17,0);lcd.print(TemButtonCounter, 1);
 lcd.setCursor(0,1);
 lcd.print("L1 L2 L3 L4 L5 L6 R1");
 lcd.setCursor(0,2);
 lcd.print(L1);
 lcd.setCursor(4,2);
 lcd.print(L2);
 lcd.setCursor(7,2);
 lcd.print(L3);
 lcd.setCursor(10,2);
 lcd.print(L4);
 lcd.setCursor(13,2);
 lcd.print(L5);
 lcd.setCursor(16,2);
 lcd.print(L6);
 lcd.setCursor(19,2);
 lcd.print(R1);
  lcd.setCursor(0,3);
  lcd.print("-----Enter(set)-----");
 delay(200);
 menu();
}

void menu()
{
  sboton();
  if (vboton==1)//Si se ha pulsado el boton entramos
  {
 lcd.setCursor(0,0);
 lcd.print("--------------------");
 lcd.setCursor(0,1);
 lcd.print("  Entrando al menu  ");
 lcd.setCursor(0,2);
 lcd.print("                    ");
 lcd.setCursor(0,3);
 lcd.print("--------------------");

      delay(1500);
      cambiomenu==1;//Para refresacar el lcd si movemos el potenciometro
      iniciomenu: //Punto de control para volver con un goto
      botpot(1,4);//creamos 3 posiciones de menu para el firo del potenciometro
 
      switch (menupot)
      
          {
            case 1: //Menu 1
                  SetTEMP();
                  cambiomenu==1;
                   break;
            
            case 2: //Menu 2 
                    CtrlRELAY();
                    cambiomenu==1;
                    break;

            case 3: //Menu 3
                    intensidadLED();
                    cambiomenu==1;
                    break; 

            case 4: //Menu 4
                    verDISPLAY();
                    cambiomenu==1;
                    break;         
            }
            
  goto iniciomenu;
  finmenu :
  lcd.clear();
  delay(200);
  }
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
 void sboton()//Comprobar si se ha pulsado el boton
{
 vboton=0;
 vboton=digitalRead(boton);delay(100);
 if (vboton==1) {lcd.noBacklight();delay(200);lcd.backlight();menuactual=0;}
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void botpot(byte menuini, byte menufin)//Comprobar si se ha movido el potenciometro
{
 menupot=0;byte i=0;int valpot=0;int leepot=0;
 leepot= analogRead(A1);
 while (i<5)
 {
 leepot= (leepot+ analogRead(A1));
 i++;
 }
 valpot=(leepot/5);
 menupot = map (valpot,0,1000,menuini,menufin);
 if (menupot==menuactual) {cambiomenu=0;}
 else {menuactual=menupot;cambiomenu=1;}
    
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void CtrlTEMP()
{
  if (temp < TemButtonCounter)
  {
      digitalWrite(ledPin, HIGH);
      R1=1;
  }
  else 
  {
      digitalWrite(ledPin, LOW);
      R1=0;
  }
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void SetTEMP()
{
  temp = analogRead(sensorPin);
  temp = (5.0 * temp * 100.0)/1024.0;
                        lcd.clear();
                        lcd.setCursor(0,0);
                        lcd.print("-SET Temperatura----");
                        lcd.setCursor(0,1);
                        lcd.print("Temp:");
                        lcd.setCursor(6,1);
                        lcd.print(temp, 1);
                        lcd.setCursor(0,2);
                        lcd.print("SetT:");
                        lcd.setCursor(6,2);
                        lcd.print(TemButtonCounter, 1);
                        lcd.setCursor(0,3);
                        lcd.print("--------------------");
                        delay (700);
CtrlTEMP();
//lectura de pulsadores para el set de temperatura
TempButtonUpState = digitalRead(SetTempUp);     // lectura del boton de incremento de temperatura
    if (TempButtonUpState == 1)   
      { 
          TemButtonCounter ++;
          delay(50);
      }
TempButtonDownState = digitalRead(SetTempDown);  // lectura del boton de decremento de temperatura
      if (TempButtonDownState == 1)
      {
          TemButtonCounter--;
          delay(50);
      }
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void CtrlRELAY()
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("-Set LUCES----------");
 lcd.setCursor(0,1);
 lcd.print("L1 L2 L3 L4 L5 L6   ");
 lcd.setCursor(0,2);
 lcd.print(L1);
 lcd.setCursor(4,2);
 lcd.print(L2);
 lcd.setCursor(7,2);
 lcd.print(L3);
 lcd.setCursor(10,2);
 lcd.print(L4);
 lcd.setCursor(13,2);
 lcd.print(L5);
 lcd.setCursor(16,2);
 lcd.print(L6);
 lcd.setCursor(0,3);
 lcd.print("--------------------");
  
if (digitalRead(SetTempUp)== HIGH)
   {
    mode=mode+1;
    delay (400);
   }
if (digitalRead(SetTempDown)== HIGH)
   {
    mode=mode-1;
    delay (400);
    }
analogWrite(PWM_LED, contador); // Enviamos a LED8 el PWM   
if (mode == 0){PORTD = B00000000;L1=0;L2=0;L3=0;L4=0;L5=0;L6=0;digitalWrite(PWM_LED, LOW);}
if (mode == 1){PORTD = B00000100;L1=1;L2=0;L3=0;L4=0;L5=0;L6=0;analogWrite(PWM_LED, contador); }
if (mode == 2){PORTD = B00001100;L1=1;L2=1;L3=0;L4=0;L5=0;L6=0;analogWrite(PWM_LED, contador); }
if (mode == 3){PORTD = B00011100;L1=1;L2=1;L3=1;L4=0;L5=0;L6=0;analogWrite(PWM_LED, contador); }
if (mode == 4){PORTD = B00111100;L1=1;L2=1;L3=1;L4=1;L5=0;L6=0;analogWrite(PWM_LED, contador); }
if (mode == 5){PORTD = B01111100;L1=1;L2=1;L3=1;L4=1;L5=1;L6=0;analogWrite(PWM_LED, contador); }
if (mode == 6){PORTD = B11111100;L1=1;L2=1;L3=1;L4=1;L5=1;L6=1;analogWrite(PWM_LED, contador); }
if (mode == 7){mode=0;}
delay(700);
} 
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void intensidadLED()
{
  arriba = digitalRead(SetTempUp); // Lee el contador del pin8 y se lo asigna a arriba. (Puede ser 0 o 1)
  abajo  = digitalRead(SetTempDown); // Lee el contador del pin9 y se lo asigna a abajo. (Puede ser 0 o 1)
  DutyCycle =  map(contador, 0, 255, 0, 100); // Cambiamos los 255 valores a 100.
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("-Intensidad LUCES---");
 lcd.setCursor(0,1);
 lcd.print("Duty Cicle:");lcd.print(DutyCycle);
 lcd.setCursor(0,2);
 lcd.print("                    ");
 lcd.setCursor(0,3);
 lcd.print("--------------------");
 delay (500);
if (arriba == HIGH) {
  contador = contador + 1; 
  if (contador > 255) {
  contador=255;
  }
  //enciende(); 
}
if (abajo == HIGH)
{
  contador = contador - 1;
  if (contador < 0)
  {
  contador=0;
  }
 //enciende();                             
}
analogWrite(PWM_LED, contador); // Enviamos a LED8 el PWM
delay(20);
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void verDISPLAY()
{
 temp = analogRead(sensorPin);
  temp = (5.0 * temp * 100.0)/1024.0;
  CtrlTEMP();
 lcd.clear();
 lcd.setCursor(0,0);lcd.print("Temp:"); lcd.setCursor(5,0); lcd.print(temp,1);
 lcd.setCursor(12,0);lcd.print("SetT:");lcd.setCursor(17,0);lcd.print(TemButtonCounter, 1);
 lcd.setCursor(0,1);
 lcd.print("L1 L2 L3 L4 L5 L6 R1");
 lcd.setCursor(0,2);
 lcd.print(L1);
 lcd.setCursor(4,2);
 lcd.print(L2);
 lcd.setCursor(7,2);
 lcd.print(L3);
 lcd.setCursor(10,2);
 lcd.print(L4);
 lcd.setCursor(13,2);
 lcd.print(L5);
 lcd.setCursor(16,2);
 lcd.print(L6);
 lcd.setCursor(19,2);
 lcd.print(R1);
 lcd.setCursor(0,3);
 lcd.print("Intensidad LUZ:");lcd.print(DutyCycle);
 delay (1000);
}