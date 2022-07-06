byte PWM_PIN9 = 9;
byte PWM_PIN6 = 6;
byte PWM_PIN5 = 5;
byte PWM_PIN3 = 3;

int pwm9_value;
int pwm6_value;
int pwm5_value;
int pwm3_value;

void setup() {
    // put your setup code here, to run once:
    pinMode(PWM_PIN9, INPUT);
    pinMode(PWM_PIN6, INPUT);
    pinMode(PWM_PIN5, INPUT);
    pinMode(PWM_PIN3, INPUT);
    Serial.begin(115200);
}

void loop() {
    // put your main code here, to run repeatedly:
    pwm9_value = pulseIn(PWM_PIN9, HIGH);
    pwm6_value = pulseIn(PWM_PIN6, HIGH);
    pwm5_value = pulseIn(PWM_PIN5, HIGH);
    pwm3_value = pulseIn(PWM_PIN3, HIGH);
    Serial.print("Channel 1: ");
    Serial.print(pwm9_value);
    Serial.print(" Channel 2: ");
    Serial.print(pwm3_value);
    Serial.print(" Channel 3: ");
    Serial.print(pwm5_value);
    Serial.print(" Channel 4: ");
    Serial.println(pwm6_value);
}