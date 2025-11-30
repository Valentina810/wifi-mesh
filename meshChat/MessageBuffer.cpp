#include "MessageBuffer.h"

void MessageBuffer::clear() {
    for (uint8_t i = 0; i < MAX_LINES; i++) {
        lines[i] = "";
    }
    count = 0;
}

void MessageBuffer::add(const String& line) {
    if (count < MAX_LINES) {
        lines[count++] = line;
    } else {
        for (uint8_t i = 1; i < MAX_LINES; i++) {
            lines[i - 1] = lines[i];
        }
        lines[MAX_LINES - 1] = line;
    }
}

void MessageBuffer::addWrapped(const String& line) {
    const uint8_t maxWidth = 50;
    int len = line.length();
    int start = 0;

    while (start < len) {
        int end = start + maxWidth;
        if (end > len) end = len;
        add(line.substring(start, end));
        start = end;
    }
}
