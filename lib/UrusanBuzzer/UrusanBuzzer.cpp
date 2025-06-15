#include "UrusanBuzzer.h"


#define BUZZER_CHANNEL 2 // Pastikan channel tidak bentrok dengan channel blower


UrusanBuzzer::UrusanBuzzer(uint8_t pinBuz, uint16_t freq)
  : _pinBuzzer(pinBuz), _freq(freq) {
  pinMode(_pinBuzzer, OUTPUT);
  ledcAttachPin(_pinBuzzer, BUZZER_CHANNEL);
  ledcSetup(BUZZER_CHANNEL, _freq, 8); // 8 bit resolution
  ledcWrite(BUZZER_CHANNEL, 0); // Pastikan buzzer mati
}


void UrusanBuzzer::beep(uint32_t delayMs, uint8_t beepCount) {
  _beepDelay = delayMs;
  _beepCount = beepCount * 2; // ON dan OFF dihitung sebagai satu siklus
  _currentBeep = 0;
  _isBeeping = true;
  _lastToggle = millis();
  _buzzerOn = false;
}


void UrusanBuzzer::update() {
  if (!_isBeeping) return;
  if (millis() - _lastToggle >= _beepDelay) {
    _lastToggle = millis();
    _buzzerOn = !_buzzerOn;
    if (_buzzerOn) {
      ledcWrite(BUZZER_CHANNEL, 128); // 50% duty cycle (nyala)
    } else {
      ledcWrite(BUZZER_CHANNEL, 0);   // duty cycle 0 (mati)
    }
    _currentBeep++;
    if (_currentBeep >= _beepCount) {
      _isBeeping = false;
      ledcWrite(BUZZER_CHANNEL, 0); // Pastikan buzzer mati
    }
  }
}
