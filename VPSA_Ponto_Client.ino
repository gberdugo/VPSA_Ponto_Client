#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <DS1307.h>
#include <SoftwareSerial.h>
#include <SeeedRFIDLib.h>
#include <LiquidCrystal.h>

/* LCD Configuration:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 1
 * LCD D7 pin to digital pin 0
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
#define LCD_RS	  7
#define LCD_E	  6
#define LCD_D4	  5
#define LCD_D5	  4
#define LCD_D6	  1
#define LCD_D7	  0
#define LCD_COLS 16
#define LCD_ROWS  2

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* RFID Reader Configuration:
 * Reader RX pin to pin 2
 * Reader TX pin to pin 3
*/
#define RFID_RX_PIN 2
#define RFID_TX_PIN 3

// Configure the Library in UART Mode
SeeedRFIDLib rfid(RFID_RX_PIN, RFID_TX_PIN);
RFIDTag tag;

/* RTC Configuration */
DS1307 clock;

/* Ethernet Configuration */
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient client;

void setup() {
	configureLCD();
	delay(1000);
	configureRTC();
	delay(1000);
	configureEthernet();
}

void configureLCD() {
	// Configures the LCD number of rows and lines
	lcd.begin(LCD_COLS, LCD_ROWS);

	lcd.clear();
	lcd.print("LCD OK");
}

void configureRTC() {
	clock.begin();
	clock.fillByYMD(2013, 6, 6);
	clock.fillByHMS(15, 28, 30);
	clock.fillDayOfWeek(SAT);
	clock.setTime();

	lcd.clear();
	lcd.print("RTC OK");
}

void configureEthernet() {
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
	}
}
