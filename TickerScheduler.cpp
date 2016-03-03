#include "TickerScheduler.hpp"
#include <Arduino.h>

void tickerFlagHandle(volatile bool *flag) {
    if (!*flag)
        *flag = true;
}

TickerScheduler::TickerScheduler(uint8_t size) {
    this->items = new TickerSchedulerItem[size];
    this->size = size;
}

TickerScheduler::~TickerScheduler() {
    for (uint8_t i = 0; i < this->size; i++) {
        this->remove(i);
        yield();
    }

    delete[] this->items;
    this->items = NULL;
    this->size = 0;
}

void TickerScheduler::handleTicker(uint8_t i) {
    if (items[i].flag) {
        items[i].flag = false;

        this->items[i].cb();

        if (this->items[i].limited) {
            if (--this->items[i].noOfRuns == 0) {
                remove(i);
            }
        }
    }
}

bool TickerScheduler::add(uint8_t i, uint32_t period, tscallback_t f,
                          uint8_t noOfRuns, bool shouldFireNow) {
    if (i >= this->size)
        return false;

    if (this->items[i].is_used)
        return false;

    if (noOfRuns == 0) {
        this->items[i].limited = false;
    } else {
        this->items[i].limited = true;
        this->items[i].noOfRuns = noOfRuns;
    }

    this->items[i].cb = f;
    this->items[i].flag = shouldFireNow;
    this->items[i].t.attach_ms(period, tickerFlagHandle, &this->items[i].flag);
    this->items[i].is_used = true;
    this->items[i].paused = false;
    this->items[i].period = period;

    return true;
}

bool TickerScheduler::remove(uint8_t i) {
    if (i >= this->size)
        return false;

    if (!this->items[i].is_used)
        return false;

    this->items[i].is_used = false;
    this->items[i].t.detach();
    this->items[i].flag = false;
    this->items[i].cb = NULL;

    return true;
}

void TickerScheduler::update() {
    for (uint8_t i = 0; i < this->size; i++) {
        yield();
        if (this->items[i].is_used)
            handleTicker(i);
    }
    yield();
}

bool TickerScheduler::pause(uint8_t i) {
    if (this->items[i].is_used && !this->items[i].paused) {
        this->items[i].paused = true;
        this->items[i].t.detach();
        return true;
    }
    return false;
}

bool TickerScheduler::unpause(uint8_t i) {
    if (this->items[i].is_used && this->items[i].paused) {
        this->items[i].paused = false;
        this->items[i].t.attach_ms(this->items[i].period, tickerFlagHandle,
                                   &this->items[i].flag);
        return true;
    }
    return false;
}
