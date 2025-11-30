#pragma once
#include <TFT_eSPI.h>
#include "MessageBuffer.h"

class Display {
public:
    TFT_eSPI tft;

    Display();
    void init();
    void drawScreen(const String& nodeName, const MessageBuffer& buf);
};
