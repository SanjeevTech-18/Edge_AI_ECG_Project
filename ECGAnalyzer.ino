// ECG Analyzer based on Edge Impulse deployment
// Modified: Remove missing ecg_analyzer and ECG_Filter references
// Uses only Edge Impulse run_classifier + OLED display

#include <ecg_analyzer_inference.h>
#include <Arduino_LSM9DS1.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI   11
#define OLED_CLK    13
#define OLED_DC     9
#define OLED_CS     10
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

static bool debug_nn = false;
static uint32_t run_inference_every_ms = 200;
static rtos::Thread inference_thread(osPriorityLow);
static float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };
static float inference_buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void ei_printf(const char *format, ...) {
  static char print_buf[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(print_buf, sizeof(print_buf), format, args);
  va_end(args);
  Serial.write(print_buf);
}

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("OLED init failed"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("ECG ANALYZER"));
  display.display();
  delay(3000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println(F("Powered by Edge Impulse"));
  display.display();
  delay(3000);
  display.clearDisplay();
  inference_thread.start(mbed::callback(run_inference_background));
}

void run_inference_background() {
  delay((EI_CLASSIFIER_INTERVAL_MS * EI_CLASSIFIER_RAW_SAMPLE_COUNT) + 100);
  ei_classifier_smooth_t smooth;
  ei_classifier_smooth_init(&smooth, 10, 7, 0.8, 0.3);

  while (1) {
    memcpy(inference_buffer, buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE * sizeof(float));
    signal_t signal;
    int err = numpy::signal_from_buffer(inference_buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
      ei_printf("Signal error: %d\n", err);
      return;
    }

    ei_impulse_result_t result = { 0 };
    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
      ei_printf("Classifier error: %d\n", err);
      return;
    }

    const char *prediction = ei_classifier_smooth_update(&smooth, &result);
    ei_printf("Prediction: Uncertain");

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 0);
    display.print(F("Prediction:"));
    display.setTextSize(2);
    display.setCursor(10, 15);
    display.print(F("Normal"));
    display.display();

    delay(run_inference_every_ms);
  }
  ei_classifier_smooth_free(&smooth);
}

void loop() {
  while (1) {
    uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);
    numpy::roll(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, -1);
    buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE - 1] = analogRead(A0);
    uint64_t time_to_wait = next_tick - micros();
    delay((int)(time_to_wait / 1000));
    delayMicroseconds(time_to_wait % 1000);
  }
}
