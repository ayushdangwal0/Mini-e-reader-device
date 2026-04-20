#include <GxEPD2_BW.h>
#include <SPI.h>
#include <SD.h>

// Pins
#define EPD_CS   5
#define EPD_DC   2
#define EPD_RST  15
#define EPD_BUSY 4

#define SD_CS 13

#define BTN_NEXT 32
#define BTN_PREV 33

GxEPD2_BW<GxEPD2_154_GDEY0154D67, GxEPD2_154_GDEY0154D67::HEIGHT> display(
  GxEPD2_154_GDEY0154D67(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

File file;

String lines[100];
int currentPage = 0;
int totalLines = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);

  display.init();
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  

  if (!SD.begin(SD_CS)) {
    Serial.println("SD Fail");
    while (1);
  }

  file = SD.open("/book.txt");
  if (!file) {
    Serial.println("File error");
    while (1);
  }

  readFile();
  showPage();
}

void loop() {
  if (digitalRead(BTN_NEXT) == LOW) {
    currentPage++;
    delay(250);
    showPage();
  }

  if (digitalRead(BTN_PREV) == LOW) {
    currentPage--;
    if (currentPage < 0) currentPage = 0;
    delay(250);
    showPage();
  }
}

void readFile() {
  totalLines = 0;

  while (file.available() && totalLines < 100) {
    lines[totalLines++] = file.readStringUntil('\n');
  }
}

void showPage() {
  display.setFullWindow();
  display.firstPage();
  Serial.println("Hello, Screen is working");
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0, 15);

    int start = currentPage * 5;

    for (int i = 0; i < 5; i++) {
      if (start + i < totalLines) {
        display.println(lines[start + i]);
      }
    }

  } while (display.nextPage());
}