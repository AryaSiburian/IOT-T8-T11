#include <main.h>


void setup() {
  // tulis setup kode mu di sini, untuk dijalankan sekali saja:  
  Serial.begin(115200);
  cetakIdentitasDeveloper();


  urusanWiFi.konek();
  while(urusanWiFi.apakahKonek() == 0){
    Serial.print(".");
    buzzer.beep(1000, 1);
  }
  buzzer.beep(50, 6);


  urusanIoT.konek();
  urusanIoT.penangkapPesan(penangkapPesan);


  if(urusanIoT.apakahKonek() == 1){
    subscribe();
  }




  kontrolBlower(true, 40, true); // Nyalakan blower
  delay(3000);
  kontrolBlower(false, 0, true); // Matikan blower
}


void loop() {
  // tulis kode utama mu di sini, untuk dijalankan berulang-ulang :
  buzzer.update();
  urusanIoT.proses();


  if(urusanIoT.apakahKonek() == 0){
    Serial.println("UrusanIoT: Koneksi terputus, mencoba untuk menyambung kembali...");
    urusanIoT.konek();
    if(urusanIoT.apakahKonek() == 1){
      subscribe();
    }
  }

 if(millis() - lastSent > 1000 || flagUpdate){
    lastSent = millis();
    String payload;

    JsonDocument doc;
    doc["state"] = lastState;
    doc["speed"] = lastSpeed;
    doc["direction"] = lastDirection;
    serializeJson(doc, payload);


    urusanIoT.publish("northkorearooster/gadadar/status", payload);
    flagUpdate = false;
  }

  delay(10);
}


void subscribe() {
  urusanIoT.subscribe("northkorearooster/gadadar/command");
}

void penangkapPesan(String topic, String message){
  Serial.printf("penangkapPesan: topic: %s | message: %s\r\n", topic.c_str(), message.c_str());
  JsonDocument dataMasuk;
  DeserializationError galatParseJson = deserializeJson(dataMasuk, message);
  if(galatParseJson == DeserializationError::Ok){
    if(!dataMasuk["state"].isNull() && !dataMasuk["speed"].isNull() && !dataMasuk["direction"].isNull()){
      lastState = dataMasuk["state"].as<bool>();
      lastSpeed = dataMasuk["speed"].as<uint8_t>();
      // Periksa apakah direction adalah boolean atau integer
      lastDirection = dataMasuk["direction"].as<bool>();
      kontrolBlower(lastState, lastSpeed, lastDirection);
    }
  }else
  {
    Serial.printf("penangkapPesan: Gagal parse JSON: %s\r\n", galatParseJson.c_str());
  }
}


/**
 * @brief Mengontrol blower/fan dengan ON/OFF, kecepatan, dan arah menggunakan PWM ESP32.
 * @param state true = ON, false = OFF
 * @param speed nilai PWM (0-255)
 * @param direction true = maju, false = mundur
 */
void kontrolBlower(bool state, uint8_t speed, bool direction) {
    const uint8_t channelINA = 1;
    const uint8_t channelINB = 0;
    const uint16_t freq = 5000;
    const uint8_t resolution = 8;


    static bool initialized = false;
    if (!initialized) {
        ledcAttachPin(pinINA, channelINA);
        ledcAttachPin(pinINB, channelINB);
        ledcSetup(channelINA, freq, resolution);
        ledcSetup(channelINB, freq, resolution);
        initialized = true;
    }


    if (!state) {
        ledcWrite(channelINA, 0);
        ledcWrite(channelINB, 0);
        return;
    }
    if (direction) {
        ledcWrite(channelINA, speed);
        ledcWrite(channelINB, 0);
    } else {
        ledcWrite(channelINA, 0);
        ledcWrite(channelINB, speed);
    }
    Serial.printf("kontrolBlower: state=%d, speed=%d, direction=%d\r\n", state, speed, direction);
}


// tulis definisi fungsi mu di sini:
void cetakIdentitasDeveloper() {
  Serial.printf("42330022: %d\r\n", NIM);  
  Serial.printf("Arya Prodigy Siburian: %s\r\n", NAMA_LENGKAP);
  Serial.printf("NorthKoreaRooster: %s\r\n", NAMA_KELOMPOK);
  Serial.printf("Firmware: %s %s\r\n", CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
}