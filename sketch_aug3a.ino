// --- Arduino 1 (COM3): RFID, LCD, Button, Efficiency ---

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// Define Pins
#define BUTTON_PIN 2    // KY-002 Button
#define SS_PIN 10       // RFID Chip Select
#define RST_PIN 9       // RFID Reset

// Initialize Libraries/Objects
// *** NOTE: Adjust 0x27 if your LCD address is different ***
LiquidCrystal_I2C lcd(0x27, 16, 2); 
MFRC522 rfid(SS_PIN, RST_PIN);

// Global Variables
int garments = 0;
float efficiency = 0.0;
String name = "Rahul";
// Target used for efficiency calculation (e.g., 10 garments = 100% efficiency)
const float TARGET_GARMENTS = 10.0; 

void setup() {
  Serial.begin(9600);
  
  // Pin Setup
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Assumes button pulls pin LOW when pressed

  // LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // RFID/SPI Setup
  SPI.begin();
  rfid.PCD_Init();

  // Initial LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Ready to Work");
  lcd.setCursor(0, 1);
  lcd.print(name);
}

void loop() {
  // 1. RFID Scan (Optional: for logging in/out)
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    lcd.clear();
    lcd.print("RFID Detected");
    delay(500);
    // Add logic here to read UID and change the 'name' variable
    rfid.PICC_HaltA();
  }

  // 2. Button press increments garment count
  // We check for LOW because INPUT_PULLUP is used
  if (digitalRead(BUTTON_PIN) == LOW) { 
    garments++;
    
    // Calculate efficiency (capped at 100% for display sanity)
    efficiency = (garments * 100.0) / TARGET_GARMENTS;
    if (efficiency > 100.0) efficiency = 100.0;

    // Update LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("G:");
    lcd.print(garments);
    lcd.print(" Eff:");
    lcd.print(efficiency, 1); // Display 1 decimal place
    lcd.setCursor(0, 1);
    lcd.print(name);

    // 3. Send JSON