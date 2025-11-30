#include "Display.h"

Display::Display() : tft() {}

void Display::init() {
    tft.init();
    tft.setRotation(1);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
}

void Display::drawScreen(const String& nodeName, const MessageBuffer& buf) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    tft.setTextFont(2);
    tft.setCursor(0, 0);
    tft.println("Node: " + nodeName);
    tft.println("----------------");

    int y = tft.getCursorY() + 2;

    for (uint8_t i = 0; i < buf.count; i++) {
        tft.setCursor(0, y);
        tft.println(buf.lines[i]);
        y += 16;
    }
}
