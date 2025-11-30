#include "Display.h"
#include "App.h"
#include <pgmspace.h>
#include "busik128x128.h"

Display::Display() : tft() {}

void Display::init() {
    tft.init();
    tft.setRotation(1);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
}

void Display::drawScreen(const String& nodeName, const MessageBuffer& buf) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);

    tft.setTextFont(2);
    tft.setCursor(0, 0);
    tft.println(String("Network: ") + MESH_PREFIX + " | Node: " + nodeName);
    tft.println("----------------");

    int y = tft.getCursorY() + 2;

    for (uint8_t i = 0; i < buf.count; i++) {
        tft.setCursor(0, y);
        tft.println(buf.lines[i]);
        y += 16;
    }
}

void Display::drawStartupImage() {
    tft.fillScreen(TFT_BLACK);

    const int W = 128;
    const int H = 128;
    const int SCALE = 2;               
                                    
    int drawW = W * SCALE;
    int drawH = H * SCALE;

    int startX = (tft.width()  - drawW) / 2;
    int startY = (tft.height() - drawH) / 2;

    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {

            uint8_t g = pgm_read_byte(&busik128x128[y * W + x]); 
            uint16_t color = tft.color565(g, g, g);

            tft.fillRect(
                startX + x * SCALE,
                startY + y * SCALE,
                SCALE,
                SCALE,
                color
            );
        }
    }

    delay(5000);
}
