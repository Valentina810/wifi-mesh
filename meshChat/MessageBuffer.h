#pragma once
#include <Arduino.h>

static const uint8_t MAX_LINES = 16;

class MessageBuffer {
public:
    String lines[MAX_LINES];
    uint8_t count = 0;

    void clear();
    void add(const String& line);
    void addWrapped(const String& line);
};
