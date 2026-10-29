// const int flamePin = A3;      // Your sensor is on A3
// const int ledPin = 13;        // Onboard LED for testing
// int threshold = 950;          // ADJUST THIS to change sensitivity!

// void setup() {
//   Serial.begin(9600);
//   pinMode(ledPin, OUTPUT);
// }

// void loop() {
//   int sensorValue = analogRead(flamePin); // Read A3 (0 to 1023)

//   Serial.print("Flame Intensity: ");
//   Serial.println(sensorValue);

//   // If value drops below threshold, a flame is detected
//   // (Lower value = more infrared light/stronger flame)
//   if (sensorValue < threshold) {
//     digitalWrite(ledPin, HIGH); 
//     Serial.println("🔥 FLAME DETECTED! 🔥");
//   } else {
//     digitalWrite(ledPin, LOW);
//   }

//   delay(200); 
// }




#include <AFMotor.h>
#include <Servo.h>

/* --- 1. HARDWARE CONFIGURATION --- */
AF_DCMotor motorL(3); // Left Motor on M3
AF_DCMotor motorR(2); // Right Motor on M2
Servo scannerServo;

const int flamePin = A2;

/* --- 2. CALIBRATION & BIAS SETTINGS --- */
const int L_MAX = 155; 
const int L_MIN = 75;
const int R_MAX = 120; 
const int R_MIN = 90;

// Hard Pivot reduction (80% of Max)
const int L_PVPOT_SPEED = 75;
const int R_PVPOT_SPEED = 60;

// Logic Thresholds
const int ambientThreshold = 975;  
const int criticalThreshold = 100; 

/* --- 3. SYSTEM SETUP --- */
void setup() {
  Serial.begin(9600);
  Serial.println("--- Flame-Targeting Rover Initialized ---");
  scannerServo.attach(9); 
  scannerServo.write(90); 
  delay(1000);            
}

/* --- 4. OPERATIONAL LOGIC --- */
void loop() {
  int bestVal = 1024; 
  int bestAngle = 90;

  Serial.println("Scanning Environment...");

  // Phase 1: Environment Scan (5 Blocks)
  for (int angle = 0; angle <= 180; angle += 45) {
    scannerServo.write(angle);
    delay(350); 
    
    int currentVal = analogRead(flamePin);
    
    // Telemetry: Show reading for each sector
    Serial.print("Angle: "); Serial.print(angle);
    Serial.print(" | Intensity: "); Serial.println(currentVal);

    if (currentVal < bestVal) {
      bestVal = currentVal;
      bestAngle = angle;
    }
  }

  Serial.print("--> BEST SIGNAL: "); Serial.print(bestVal);
  Serial.print(" at "); Serial.print(bestAngle); Serial.println(" degrees.");

  // Phase 3 & Logic Gate: Intensity Check
  if (bestVal > ambientThreshold) {
    Serial.println("STATE: Idle (No Flame Detected)");
    fullHalt();
  } 
  else if (bestVal < criticalThreshold) {
    Serial.println("STATE: Target Reached! Halting.");
    fullHalt();
  } 
  else {
    Serial.print("STATE: Moving to Target at "); Serial.println(bestAngle);
    executeMovement(bestAngle);
  }
  
  Serial.println("---------------------------------------");
}

/* --- 5. MOVEMENT ACTUATION --- */
void executeMovement(int angle) {
  switch (angle) {
    case 0:   
      Serial.println("ACTION: Hard Pivot Right");
      pivotRight(); 
      break;   
    case 45:  
      Serial.println("ACTION: Soft Turn Right");
      turnRight();  
      break;   
    case 90:  
      Serial.println("ACTION: Driving Forward");
      driveForward(); 
      break; 
    case 135: 
      Serial.println("ACTION: Soft Turn Left");
      turnLeft();   
      break;   
    case 180: 
      Serial.println("ACTION: Hard Pivot Left");
      pivotLeft();  
      break;   
  }
  
  // Burst duration logic
  if (angle == 0 || angle == 180) {
    delay(500); 
  } else {
    delay(400); 
  }
  
  fullHalt(); 
}

// --- Directional Functional Blocks ---

void driveForward() {
  motorL.setSpeed(L_MAX);
  motorR.setSpeed(R_MAX);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void pivotRight() {
  motorL.setSpeed(L_PVPOT_SPEED);
  motorR.setSpeed(R_PVPOT_SPEED);
  motorL.run(FORWARD);
  motorR.run(BACKWARD);
}

void pivotLeft() {
  motorL.setSpeed(L_PVPOT_SPEED);
  motorR.setSpeed(R_PVPOT_SPEED);
  motorL.run(BACKWARD);
  motorR.run(FORWARD);
}

void turnRight() {
  motorL.setSpeed(L_MAX);
  motorR.setSpeed(R_MIN);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void turnLeft() {
  motorL.setSpeed(L_MIN);
  motorR.setSpeed(R_MAX);
  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void fullHalt() {
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}