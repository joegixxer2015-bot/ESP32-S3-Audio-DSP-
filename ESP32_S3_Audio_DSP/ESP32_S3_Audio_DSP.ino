#include <Arduino.h>
#include "driver/i2s.h"

#define I2S_BCLK 4   
#define I2S_LRCK 5   
#define I2S_DATA 6   

#define I2S_NUM     (i2s_port_t)0
#define DAC_BCLK    14  
#define DAC_LRCK    13  
#define DAC_DATA    12  

void setup_i2s() {
  i2s_config_t i2s_config_in = {
    .mode = (i2s_mode_t)(I2S_MODE_SLAVE | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  i2s_pin_config_t pin_config_in = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_DATA
  };

  i2s_config_t i2s_config_out = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = true
  };

  i2s_pin_config_t pin_config_out = {
    .bck_io_num = DAC_BCLK,
    .ws_io_num = DAC_LRCK,
    .data_out_num = DAC_DATA,
    .data_in_num = I2S_PIN_NO_CHANGE
  };

  i2s_driver_install(I2S_NUM, &i2s_config_in, 0, NULL);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Audio Passthrough Test Starting...");
  setup_i2s();
}

void loop() {
  uint8_t buffer[512];
  size_t bytesRead = 0;
  size_t bytesWritten = 0;

  i2s_read(I2S_NUM, (void*)buffer, sizeof(buffer), &bytesRead, portMAX_DELAY);

  if (bytesRead > 0) {
    i2s_write(I2S_NUM, (const void*)buffer, bytesRead, &bytesWritten, portMAX_DELAY);
  }
}
