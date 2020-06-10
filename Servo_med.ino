#include "Servo.h"

// definicja zmiennych
Servo Sev1;
#define Sen_up A1           //podłączenie pierwszego sygnału czujnika EMG do analogowego pinu 1 
#define Sen_down A2         //podłączenie drugiego sygnału czujnika EMG do analogowego pinu 2

// ustawienia
void setup() 
{
  Serial.begin(115200);     //115300 bit/sek
  pinMode(Sen_up,INPUT);    //ustawienie sygnałów z czujnika EMG jako wejście
  pinMode(Sen_down,INPUT);
  Sev1.attach(5);           //podłączenie serwa do pinu 7 (pin pwm)
}

void loop() 
{
  int x = count_deg();                    //funkcja obliczająca stopnie, o które powinno być wychylone serwo
  if(Sev1.read()<x)       move_CW(x);     //jeżeli odczytane z serwa wychylenie jest większe od wymaganego, obróć serwo zgodnie ze wskazówkami zegara
  else if (Sev1.read()>x) move_CCW(x);    //jeżeli odczytane wychylenie jest mniejsze, obróć serwo przeciwnie do ruchu wskazówek zegara
}

void move_CW(int deg)                     //funkcja obracająca serwo zgodnie ze wskazówkami zegara do zadanego wychylenia
{
  for(int i = Sev1.read(); i <= (Sev1.read()+deg); i++)
  {
    int j = 0;
    if(i < 163) j = i;                   //przypisanie wartości 163 do serwa, odpowiada maksymalnemu wychyleniu ręki w górę (73 stopnie)
    else j = 163;                        //zabezpieczenie przed przekroczeniem maksymalnej dopuszczalnej prędkości
    Sev1.write(j);
    delay(10);
  }
}

void move_CCW(int deg)                  //funkcja obracająca serwo przeciwnie do wskazówek zegara do zadanego wychylenia
{
    for(int i = Sev1.read(); i >= (Sev1.read()-deg); i--)
  {
    int j = 0;
    if(i >= 15) j = i;                  //przypisanie wartości 15 do serwa, odpowiada maksymalnemu wychyleniu ręki w dół (-75 stopni)
    else j = 15;                        //zabezpieczenie przed przekroczeniem minimalnej dopuszczalnej prędkości
    Sev1.write(j);
    delay(10);
  }
}

int count_deg()
{
  int deg = 0;
  float volts_up = analogRead(Sen_up)*(5/1024);       //5V/1024
  float volts_down = analogRead(Sen_down)*(5/1024);
  float difference = volts_up - volts_down;           //obliczanie różnicy w napięciach wejściowych z czujnika EMG
  if (abs(difference) < 0.5)                          //0.5V - próg dopuszczalnej niedokładności, aby wyeliminować szumy
  {
    deg = 0;
  }
  else if (difference > 0.5)
  {
    deg = count_up(difference);
  } 
  else if (difference < -0.5)
  {
    deg = count_down(difference);
  }
  return deg;
}

int count_up(float volts)
{
  int y = map(volts, 0, 1.78712, 90, 163);    //przyporządkowywanie wartości stopni, o które w górę może wychylić się serwo 
  return y;                                   //w stosunku do wyjściowego napięcia czujnika EMG (pierwsze napięcie wejściowe, Sen_up)
}

int count_down(float volts)
{
 int y = map(volts, 0, 1.95978, 90, 15);    //przyporządkowywanie wartości stopni, o które w dół może wychylić się serwo
 return y;                                  //w stosunku do wyjściowego napięcia czujnika EMG (drugie napięcie wejściowe, Sen_down)
}
