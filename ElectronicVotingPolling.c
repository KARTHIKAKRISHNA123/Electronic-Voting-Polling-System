#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8
#define BUZZER    7
#define BUTTON1   2
#define BUTTON2   3
#define BUTTON3   4
#define CTRL_BTN  5  // Control button to enable/disable voting

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

int votes[3] = {0, 0, 0}; // Vote counters for 3 candidates
bool votingEnabled = false;  // Flag to enable/disable voting
int activeButton = -1;  // Variable to track the active vote button

void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(CTRL_BTN, INPUT_PULLUP);  // Set control button
  pinMode(BUZZER, OUTPUT);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  displayCandidates();
}

void loop() {
  // Check if the control button is pressed
  if (digitalRead(CTRL_BTN) == LOW) {
    votingEnabled = !votingEnabled;  // Toggle voting enabled/disabled
    tone(BUZZER, 1000, 200);  // Feedback buzzer sound
    delay(500);  // Debounce delay
    displayCandidates();  // Update screen based on new voting state
  }

  // Register vote if voting is enabled and a button is pressed
  if (votingEnabled) {
    if (digitalRead(BUTTON1) == LOW && activeButton == -1) {
      registerVote(0);
      activeButton = 0;
    }
    if (digitalRead(BUTTON2) == LOW && activeButton == -1) {
      registerVote(1);
      activeButton = 1;
    }
    if (digitalRead(BUTTON3) == LOW && activeButton == -1) {
      registerVote(2);
      activeButton = 2;
    }
  }

  // Disable all buttons if voting is disabled
  if (!votingEnabled) {
    activeButton = -1;
  }
}

void registerVote(int candidate) {
  votes[candidate]++;
  tone(BUZZER, 1500, 100);
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(50, 120);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN);
  tft.println("Vote Registered!");
  delay(1000);
  displayCandidates();
}

void displayCandidates() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);

  tft.setCursor(20, 30);
  tft.println("1.Stalin - DMK");

  tft.setCursor(20, 80);
  tft.println("2.Modi- BJP");

  tft.setCursor(20, 130);
  tft.println("3. Vijay - TVK");

  // Display voting status based on whether voting is enabled
  tft.setCursor(20, 200);
  tft.setTextSize(1);
  if (votingEnabled) {
    tft.println("Voting Enabled. Press button to vote.");
  } else {
    tft.println("Voting Disabled. Press control button to enable.");
  }
}
