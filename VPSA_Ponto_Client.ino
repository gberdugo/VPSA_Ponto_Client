#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <DS1307.h>
#include <SoftwareSerial.h>
#include <SeeedRFIDLib.h>
#include <SerialLCD.h>

/* LCD Configuration:
 * Pin 2 and 3
*/
#define LCD_PIN1  2
#define LCD_PIN2  3
SerialLCD lcd(LCD_PIN1, LCD_PIN2);

/* RFID Reader Configuration:
 * Reader RX pin to pin 1
 * Reader TX pin to pin 0
*/
#define RFID_RX_PIN 1
#define RFID_TX_PIN 0

// Configure the Library in UART Mode
SeeedRFIDLib rfid(RFID_RX_PIN, RFID_TX_PIN);
RFIDTag tag;

/* RTC Configuration */
DS1307 clock;

/* Ethernet Configuration */
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;

#define LOOP_INTERVAL 1000L
long time = 0;

char lcdMessage[16]; 

void setup() {
  configureLCD();
  delay(1000);
  configureRTC();
  delay(1000);
  //configureEthernet();
  time = millis();
}

void configureLCD() {
  lcd.begin();
  delay(1000);
  lcd.clear();
  delay(1000);
  lcd.print("LCD OK");
}

void configureRTC() {
  clock.begin();
  clock.fillByYMD(2013, 9, 23);
  clock.fillByHMS(22, 9, 00);
  clock.fillDayOfWeek(MON);
  clock.setTime();

  lcd.clear();
  delay(1000);
  lcd.print("RTC OK");
  delay(1000);
}

void configureEthernet() {
  lcd.clear();
  lcd.print("Ethernet INIT");
  if (Ethernet.begin(mac) == 0) {
    lcd.clear();
    lcd.print("Ethernet ERROR!");
  }
  delay(1000);

  lcd.clear();
  lcd.print("ETHERNET OK");
}

void loop() {
  if (rfid.isIdAvailable()) {
    tag = rfid.readId();

    lcd.clear();
    lcd.print("ID: " + tag.id);
  } else {
    if (millis() - time >= LOOP_INTERVAL) {
      lcd.clear();
      clock.getTime();
      
      imprimeData();
      time = millis();
    }
  }
}
  
void imprimeData() {
  lcd.setCursor(0, 0);
  
  switch (clock.dayOfWeek) {
    case MON: {
      lcd.print("SEG   ");
    } break;
    case TUE: {
      lcd.print("TER   ");
    } break;
    case WED: {
      lcd.print("QUA   ");
    } break;
    case THU: {
      lcd.print("QUI   ");
    } break;
    case FRI: {
      lcd.print("SEX   ");
    } break;
    case SAT: {
      lcd.print("SAB   ");
    } break;
    case SUN: {
      lcd.print("DOM   ");
    } break;
  }
  sprintf(lcdMessage, "%02d/%02d/%04d", clock.dayOfMonth, clock.month, clock.year + 2000);
  lcd.print(lcdMessage);
  lcd.setCursor(0, 1);
  sprintf(lcdMessage, "%02d:%02d:%02d", clock.hour, clock.minute, clock.second);
  lcd.print(lcdMessage);
}

