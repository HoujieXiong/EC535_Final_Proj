// Relay test to make sure that relay and light is wired up properly for reverse logic triggering


#define RELAY 4     // Relay pin

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY, HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(2000);
  digitalWrite(RELAY, LOW);
  delay(1500);
  digitalWrite(RELAY, HIGH);

}
