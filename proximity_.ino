// --- Arduino 2 (COM5): PIR and Vibration Sensor Status ---

#define PIR_PIN 3   // PIR Sensor (Presence)
#define VIB_PIN 4   // Vibration Sensor (Machine Status)

void setup() {
  Serial.begin(9600);
  
  // Pin Setup
  pinMode(PIR_PIN, INPUT);
  pinMode(VIB_PIN, INPUT);
}

void loop() {
  // Read sensor states (HIGH = Present/Running)
  String presence = digitalRead(PIR_PIN) == HIGH ? "Present" : "Absent";
  String machine = digitalRead(VIB_PIN) == HIGH ? "Running" : "Idle";

  // Send JSON to Serial Port (COM5)
  // CRITICAL: Must be in this exact JSON format
  Serial.print("{\"presence\":\"");
  Serial.print(presence);
  Serial.print("\",\"machine\":\"");
  Serial.print(machine);
  Serial.println("\"}"); // Adds the necessary newline

  // Short delay for continuous updates
  delay(1000); 
}