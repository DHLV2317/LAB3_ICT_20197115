//
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

const int TMP_PIN   = A0;
const int MOTOR_PIN = 8;
const int LED_ON    = 5;
const int LED_OFF   = 6;

int temp = 0;
bool motorOn = false;

const int T_ON  = 28;  // Enciende motor si T > 28°C
const int T_OFF = 26;  // Apaga motor si T < 26°C

void setup()
{
  pinMode(TMP_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_ON, OUTPUT);
  pinMode(LED_OFF, OUTPUT);

  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{
  // Conversión de sensor TMP36 → temperatura aproximada
  temp = map(((analogRead(TMP_PIN) - 20) * 3.04), 0, 1023, -20, 120);

  // ---- Control con histéresis ----
  if (!motorOn && temp > T_ON) {
    motorOn = true;
  }
  if (motorOn && temp < T_OFF) {
    motorOn = false;
  }

  // Actualizar salidas
  digitalWrite(MOTOR_PIN, motorOn ? HIGH : LOW);
  digitalWrite(LED_ON,    motorOn ? HIGH : LOW);
  digitalWrite(LED_OFF,   motorOn ? LOW  : HIGH);

  // ---- LCD ----
  lcd_1.setCursor(0, 0);
  lcd_1.print("Temp. Monitr. Sys ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Temp=");
  lcd_1.print(temp);
  lcd_1.print((char)223); // símbolo ° en ASCII extendido
  lcd_1.print("C ");
  lcd_1.print(motorOn ? "ON " : "OFF");

  // ---- Serial ----
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | Motor: ");
  Serial.println(motorOn ? "ON" : "OFF");

  delay(500);
}
