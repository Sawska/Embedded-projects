# 🔧 State Machine Project

**Project:** _LED Traffic Light Controller with Pedestrian Button_

- **Goal:** Practice designing a clear state machine for event-driven control.
    
- **Description:**
    
    - Use LEDs to simulate a traffic light (red, yellow, green).
        
    - Create states: `RED`, `RED_YELLOW`, `GREEN`, `YELLOW`.
        
    - Add a button for pedestrians — when pressed, the state machine transitions to give them priority after the current cycle.
        

```
#define RED_PIN     15
#define YELLOW_PIN  4
#define GREEN_PIN   2

#define BUTTON_PIN  18

enum TrafficState {
  RED,
  RED_YELLOW,
  GREEN,
  YELLOW
};

TrafficState currentState = RED;
bool pedestrianRequest = false; 
unsigned long stateStartTime = 0; 

// State durations (ms)
const unsigned long RED_TIME        = 4000;
const unsigned long RED_YELLOW_TIME = 1500;
const unsigned long GREEN_TIME      = 4000;
const unsigned long YELLOW_TIME     = 2000;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP); 

  Serial.begin(115200);

  setState(RED); 
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    pedestrianRequest = true;
  }

  switch (currentState) {
    case RED:
      if (millis() - stateStartTime >= RED_TIME) {
        setState(RED_YELLOW);
      }
      break;

    case RED_YELLOW:
      if (millis() - stateStartTime >= RED_YELLOW_TIME) {
        setState(GREEN);
      }
      break;

    case GREEN:
      if (millis() - stateStartTime >= GREEN_TIME) {
        setState(YELLOW);
      }
      break;

    case YELLOW:
      if (millis() - stateStartTime >= YELLOW_TIME) {
        if (pedestrianRequest) {
          pedestrianRequest = false; 
          request
          setState(RED); // Go to RED for pedestrians
        } else {
          setState(RED);
        }
      }
      break;
  }
}

void setState(TrafficState newState) {
  currentState = newState;
  stateStartTime = millis();

  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  switch (newState) {
    case RED:
      digitalWrite(RED_PIN, HIGH);
      break;
    case RED_YELLOW:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, HIGH);
      break;
    case GREEN:
      digitalWrite(GREEN_PIN, HIGH);
      break;
    case YELLOW:
      digitalWrite(YELLOW_PIN, HIGH);
      break;
  }

  Serial.print("State changed to: ");
  Serial.println(stateName(newState));
}

String stateName(TrafficState state) {
  switch (state) {
    case RED: return "RED";
    case RED_YELLOW: return "RED_YELLOW";
    case GREEN: return "GREEN";
    case YELLOW: return "YELLOW";
  }
  return "UNKNOWN";
}
```


### Photo