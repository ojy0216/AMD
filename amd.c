#include <Stepper.h>

Stepper step1(2048, 7, 9, 8, 10); //INA ~ IND: 7, 8, 9, 10

const int out = -1300, in = 1500, detecting_range = 100;
int i, ir, but, warning;
float duration, distance;

//Pin Number Set
const int R_led = 2, G_led = 3, B_led = 4;
const int Button = 5, IR = 6, US_Input = 11, US_Output = 12, Fine_Dust_Data = 13;

void ButInput();
void LedRed();
void LedGreen();
void LedBlue();
void LedOff();

void setup() {
	step1.setSpeed(15); //Max_RPM = 18

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(R_led, OUTPUT);
	pinMode(G_led, OUTPUT);
	pinMode(B_led, OUTPUT);
	pinMode(Button, INPUT); //LOW -> Pressed
	pinMode(IR, INPUT); //HIGH = 0;
	pinMode(US_Input, INPUT); //echo
	pinMode(US_Output, OUTPUT); //trig
	pinMode(Fine_Dust_Data, INPUT);
}

void loop() {
	digitalWrite(LED_BUILTIN, LOW);
	LedOff();

	digitalWrite(US_Output, HIGH);
	delay(1000);
	digitalWrite(US_Output, LOW);

	duration = pulseIn(US_Input, HIGH);
	distance = ((float)(340 * duration) / 10000) / 2;

	if (distance <= detecting_range) {
		warning = digitalRead(Fine_Dust_Data);
		if (warning == HIGH) {
			LedRed();
			step1.step(out);
			for (i = 0; i < 10; i++) {
				ir = digitalRead(IR);
				if ((i == 9) && (ir == 0)) {
					step1.step(in);
					break;
				}
				else if (ir == 1) {
					ButInput();
					break;
				}
				delay(1000);
			}
		}
		else {
			LedGreen();
			delay(1500);
			ButInput();
		}
	}
	delay(2000);
}

void ButInput() {
	int k;
	for (k = 0; k < 10; k++) {
		LedBlue();
		but = digitalRead(Button);
		if (but == LOW) {
			step1.step(out);
			break;
		}
		delay(1000);
	}
}

void LedRed() {
	digitalWrite(R_led, HIGH);
	digitalWrite(G_led, LOW);
	digitalWrite(B_led, LOW);
}

void LedGreen() {
	digitalWrite(R_led, LOW);
	digitalWrite(G_led, HIGH);
	digitalWrite(B_led, LOW);
}

void LedBlue() {
	digitalWrite(R_led, LOW);
	digitalWrite(G_led, LOW);
	digitalWrite(B_led, HIGH);
}

void LedOff() {
	digitalWrite(R_led, LOW);
	digitalWrite(G_led, LOW);
	digitalWrite(B_led, LOW);
}