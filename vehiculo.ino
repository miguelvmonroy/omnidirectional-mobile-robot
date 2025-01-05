
#include <Ultrasonic.h>

Ultrasonic sensorfrontal(53,52); // (Trig PIN,Echo PIN)

// variables para canales del sensores
// de los dos encoder: ini
const int channelPinA = 18;
const int channelPinB = 19;
const int channelPinC = 20;
const int channelPinD = 21;
// variables para canales del sensores
// de los dos encoder: fin

// variables para motores: ini
#define motor1A 8 // In1
#define motor1B 7 // In2
#define motor1_PWM 9/////////// //PWM_1
#define motor2A 5 //In3
#define motor2B 6 //In4
#define motor2_PWM 4 ////////////PWM_2
// variables para motores: fin

// variables de entrada: ini
int pwm_ideal_1;
int pwm_ideal_2;
// variables de entrada: fin

// variables del encoder: ini
const int timeThreshold = 0;
const int timeThreshold2 = 0;
long timeCounter = 0;
long timeCounter2 = 0;
long const int maxSteps = 1000000;
long const int maxSteps2 = 1000000;
long const int minSteps = -1000000;
long const int minSteps2 = -1000000;
volatile long ISRCounter = 0;
volatile long ISRCounter2 = 0;
int counter = 0;
int counter2 = 0;
bool IsCW = true;
bool IsCW2 = true;
// variables del encoder: fin


void setup()
{
delay(3000);//tiempo de inicio
Serial.begin(115200);
pinMode(channelPinA, INPUT_PULLUP);
pinMode(channelPinC, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(channelPinA), doEncodeA, CHANGE);//change: ocurre cuando el pin cambia de estado
attachInterrupt(digitalPinToInterrupt(channelPinB), doEncodeB, CHANGE);// rising + falling
attachInterrupt(digitalPinToInterrupt(channelPinC), doEncodeD, CHANGE); //rising:ocurre un flanco de subida de low a high
attachInterrupt(digitalPinToInterrupt(channelPinD), doEncodeC, CHANGE);// falling, ocurre en el flanco de bajada de high a low
// motor: ini
pinMode(motor1A, OUTPUT); //In1 Activa el motor su salida
pinMode(motor1B, OUTPUT); //In2 Activa el motor su salida
pinMode(motor2A, OUTPUT); //In3 Activa el motor su salida
pinMode(motor2B, OUTPUT); //In4 Activa el motor su salida
// motor: fin
}

void loop()
{
//atras
// avanzar(230,.75);
// giro(220,45);
// avanzar(230,.51);
// giro(220,46);
// avanzar(230,.38);
// giro(-220,115);
// avanzar(230,.88);
// giro(-220,95);
// avanzar(230,.35);
//avanzar
giro(-255,9.2);
avanzar(230,.35);
giro(255,48);
avanzar(230,.78);
giro(255,90);
avanzar(230,.27);
giro(-255,99.5);
avanzar(230,.57);
giro(-255,126);
avanzar(230,.84);
giro(255,130);
exit(0); // Salir del éxito
}

void doEncodeA()
{
if (digitalRead(channelPinA) == digitalRead(channelPinB))
{
IsCW = true;
if (ISRCounter + 1 <= maxSteps) ISRCounter++;
}
else
{
IsCW = false;
if (ISRCounter - 1 > minSteps) ISRCounter--;
}
}

void doEncodeB()
{
if (digitalRead(channelPinA) != digitalRead(channelPinB))
{
IsCW = true;
if (ISRCounter + 1 <= maxSteps) ISRCounter++;
}
else
{
IsCW = false;
if (ISRCounter - 1 > minSteps) ISRCounter--;
}
}

void doEncodeC()
{
if (digitalRead(channelPinC) == digitalRead(channelPinD))
{
IsCW2 = true;
if (ISRCounter2 + 1 <= maxSteps2) ISRCounter2++;
}
else
{
IsCW2 = false;
if (ISRCounter2 - 1 > minSteps2) ISRCounter2--;
}
}

void doEncodeD()
{
if (digitalRead(channelPinC) != digitalRead(channelPinD))
{
IsCW2 = true;
if (ISRCounter2 + 1 <= maxSteps2) ISRCounter2++;
}
else
{
IsCW2 = false;
if (ISRCounter2 - 1 > minSteps2) ISRCounter2--;
}
}

// funciones para motores: ini
int mi_sat(int pwm)
{
if (pwm>255){
return(255);
}
else if (pwm<0){
return(0);
}
else {
return(pwm);
}
}

void mi_pwm1(int mot_pwm,int x)
{
if (x > 0){
digitalWrite(motor1A,HIGH);
digitalWrite(motor1B,LOW);
}
else{
digitalWrite(motor1A,LOW);
digitalWrite(motor1B,HIGH);
}
analogWrite(mot_pwm,mi_sat(abs(x)));
}

void mi_pwm2(int mot_pwm,int y)
{
if (y > 0){
digitalWrite(motor2A,HIGH);
digitalWrite(motor2B,LOW);
}
else{
digitalWrite(motor2A,LOW);
digitalWrite(motor2B,HIGH);

}
analogWrite(mot_pwm,mi_sat(abs(y)));
}
// funciones para motores: fin

// funcion que hace que el carro avance
void avanzar(int vel, float dist)
{
//variable dada por el usuario para que no choque
int d1=15;
///variable para distancia: fin

//variable distancia que mide sensor ultrasónico máximo
int obs;
///variable distancia que mide sensor ultrasónico máximo: fin

int pwm_ideal_1;
int pwm_ideal_2;
float dist1;
pwm_ideal_1 = vel; // velocidad ideal 0-255
pwm_ideal_2 = vel;
dist1 = dist*9541;
ISRCounter = 0;
ISRCounter2 = 0;
while (abs(ISRCounter2)<abs(dist1))
{
obs = sensorfrontal.Ranging(CM);
if (obs>d1)
{
mi_pwm1(motor1_PWM,pwm_ideal_1);
mi_pwm2(motor2_PWM,pwm_ideal_2);
}
else
{
mi_pwm1(motor1_PWM,0); // 0 - 255
mi_pwm2(motor2_PWM,0);
}
}
mi_pwm1(motor1_PWM,0); // 0 - 255
mi_pwm2(motor2_PWM,0);
}
// funcion que hace que el carro gire


void giro(int vel, float ang)
{
int pwm_ideal_1;
int pwm_ideal_2;
float ang1;
pwm_ideal_1 = vel; // velocidad ideal 0-255
pwm_ideal_2 = vel;
ang1 = 5300*ang/360; // 5200 funciona bien hacia derecha, pero no hacia izquierda, por eso 5300
ISRCounter = 0;
ISRCounter2 = 0;
while (abs(ISRCounter2)<abs(ang1)){
mi_pwm1(motor1_PWM,pwm_ideal_1); // 0 - 255 // proporcional y regula la velocidad de la rueda 1 para
// ajustarse a la rueda 2
mi_pwm2(motor2_PWM,-pwm_ideal_2);
}
mi_pwm1(motor1_PWM,0); // 0 - 255
mi_pwm2(motor2_PWM,0);
}
void curva(int vel, float r, int lados)
{
int i=1;
for (i=1;i<=lados;i++)
{
avanzar(vel,r*sin(180/lados*PI/180)/sin((90-180/lados)*PI/180));
giro(vel,360/lados);
avanzar(vel,r*sin(180/lados*PI/180)/sin((90-180/lados)*PI/180));
}
}
int sign(int x)
{
if (x<0) return (-1);
else{
if (x>0) return (1);
else return(0);
}
}