uint32_t counter = 0;

void setup() {
  Serial.begin(57600);
}

void loop() {
    Serial.print(counter);
    Serial.print("\n");
    counter++;
}