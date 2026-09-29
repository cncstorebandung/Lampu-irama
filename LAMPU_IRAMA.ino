#include <Adafruit_NeoPixel.h>

// =====================================================
// PIN
// =====================================================

#define SOUND_PIN A0
#define LED_PIN   6

// Jumlah LED WS2812B
#define NUM_LEDS 8

// =====================================================
// WS2812B
// =====================================================

Adafruit_NeoPixel strip(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// =====================================================
// PENGATURAN
// =====================================================

// Brightness LED: 0 - 255
int brightness = 100;

// Batas minimal suara/noise
int noiseThreshold = 20;

// Nilai suara maksimum
int maxSound = 300;

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  pinMode(SOUND_PIN, INPUT);

  // Mulai WS2812B
  strip.begin();

  // Matikan semua LED saat awal
  strip.clear();
  strip.show();

  // Atur brightness
  strip.setBrightness(brightness);

  Serial.println("==============================");
  Serial.println(" SOUND REACTIVE LED");
  Serial.println(" Arduino Nano + WS2812B");
  Serial.println("==============================");
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  // ---------------------------------------------------
  // Membaca level suara
  // ---------------------------------------------------

  int signalMax = 0;
  int signalMin = 1023;

  unsigned long startTime = millis();

  // Sampling selama 30 ms
  while (millis() - startTime < 30) {

    int sample = analogRead(SOUND_PIN);

    if (sample > signalMax) {
      signalMax = sample;
    }

    if (sample < signalMin) {
      signalMin = sample;
    }
  }

  // ---------------------------------------------------
  // Hitung kekuatan suara
  // ---------------------------------------------------

  int soundLevel = signalMax - signalMin;

  // Tampilkan nilai ke Serial Monitor
  Serial.print("Sound Level : ");
  Serial.println(soundLevel);

  // ---------------------------------------------------
  // Hilangkan noise kecil
  // ---------------------------------------------------

  if (soundLevel < noiseThreshold) {
    soundLevel = 0;
  }

  // Batasi nilai
  soundLevel = constrain(soundLevel, 0, maxSound);

  // ---------------------------------------------------
  // Ubah suara menjadi jumlah LED
  // ---------------------------------------------------

  int ledCount = map(
    soundLevel,
    0,
    maxSound,
    0,
    NUM_LEDS
  );

  ledCount = constrain(
    ledCount,
    0,
    NUM_LEDS
  );

  // ---------------------------------------------------
  // Bersihkan LED
  // ---------------------------------------------------

  strip.clear();

  // ---------------------------------------------------
  // Nyalakan LED sesuai suara
  // ---------------------------------------------------

  for (int i = 0; i < ledCount; i++) {

    uint32_t color;

    // LED 1-2
    if (i < 2) {

      color = strip.Color(
        0,      // R
        255,    // G
        0       // B
      );
    }

    // LED 3-5
    else if (i < 5) {

      color = strip.Color(
        255,    // R
        150,    // G
        0       // B
      );
    }

    // LED 6-8
    else {

      color = strip.Color(
        255,    // R
        0,      // G
        0       // B
      );
    }

    strip.setPixelColor(i, color);
  }

  // Tampilkan LED
  strip.show();

  // Sedikit delay
  delay(5);
}