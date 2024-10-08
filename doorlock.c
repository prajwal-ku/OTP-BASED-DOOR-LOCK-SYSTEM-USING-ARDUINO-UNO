#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define relay 4
#define red 16
#define green 15

SoftwareSerial mySerial(2, 3);


const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

LiquidCrystal_I2C lcd(0x27, 16, 2);

//SoftwareSerial sim800l(3, 2);
int irsensor = A0;
long otp;
String otpstring = "";
int i = 0;
void setup()
{

  pinMode(irsensor, INPUT_PULLUP);
  mySerial.begin(9600);
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Serial.print("Welcome to SIM800L Project");
  //sim800l.println("AT");
  //updateSerial();
  pinMode(relay, OUTPUT);
  pinMode(red, INPUT);
  pinMode(green, INPUT);
  digitalWrite(relay, LOW);
  delay(500);
  //sim800l.println("AT+CSQ");
  //updateSerial();
  delay(1000);

}
void updateSerial()
{
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("   OTP Based");
  lcd.setCursor(0, 1);
  lcd.print("   Door Lock");
  if (digitalRead(irsensor) == LOW)
  {
    otp = random(2000, 9999);
    otpstring = String(otp);
    Serial.println(otpstring);
    while (digitalRead(irsensor) == LOW) {}
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" OTP Sent to");
    lcd.setCursor(0, 1);
    lcd.print(" Your Mobile");
    Serial.print("OTP is ");
    delay(100);
    Serial.println(otpstring);
    delay(100);
    SendSMS();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter OTP :");
    getotp();
  }

}

void getotp() {

  String y = "";
  int a = y.length();
  while (a < 4)
  {
    char customKey = customKeypad.getKey();
    if (customKey) {
      lcd.setCursor(0, 1);
      y = y + customKey;
      lcd.print(y);
      a = y.length();
    }
  }
  Serial.print("Entered OTP is ");
  Serial.println(y);
  if (otpstring == y)
  {
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Door Opening");
    digitalWrite(relay, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(5000);
    digitalWrite(relay, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Access Failed");
    lcd.setCursor(0, 1);
    lcd.print("Try Again !!!");
    delay(3000);
  }


}
void SendSMS()
{
  Serial.println("Sending SMS...");
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+919967076090\"\r");
  delay(1000);
  //sim800l.print("Your OTP is " + otpstring + " Just Type OTP And Unlock The Door");
  //delay(500);
  Serial.println("OTP Just before sending sms");
  Serial.println(otpstring);
  mySerial.println(otpstring);
  delay(100);

  mySerial.println((char)26);
  delay(1000);
  //sim800l.println();
  Serial.println("Text Sent.");
  delay(500);

}
