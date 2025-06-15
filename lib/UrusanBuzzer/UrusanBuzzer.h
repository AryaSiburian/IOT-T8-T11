#ifndef UrusanBuzzer_h
#define UrusanBuzzer_h


#include "Arduino.h"


class UrusanBuzzer {
  public:
    UrusanBuzzer(uint8_t pinBuz, uint16_t freq = 1600); // Default frequency is 1600 Hz
    void beep(uint32_t delayMs, uint8_t beepCount); // Blocking beep function
    void update(); // Call this in your loop to handle beeping  
  private:
    uint8_t _pinBuzzer;
    uint16_t _freq;
    // Tambahkan di UrusanBuzzer.h (private:)
    bool _isBeeping = false;
    uint8_t _beepCount = 0;
    uint8_t _currentBeep = 0;
    uint32_t _beepDelay = 0;
    unsigned long _lastToggle = 0;
    bool _buzzerOn = false;
};


#endif
