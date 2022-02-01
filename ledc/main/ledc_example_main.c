/* LEDC (LED Controller) fade example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define BUF_SIZE (1024)

#define GPIO_OUTPUT_IO_0 (2)
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0)

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_HS_CH0_GPIO       (12)
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_HS_CH1_GPIO       (14)
#define LEDC_HS_CH1_CHANNEL    LEDC_CHANNEL_1

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_LS_CH2_GPIO       (4)
#define LEDC_LS_CH2_CHANNEL    LEDC_CHANNEL_2
#define LEDC_LS_CH3_GPIO       (15)
#define LEDC_LS_CH3_CHANNEL    LEDC_CHANNEL_3

#define LEDC_TEST_CH_NUM       (4)
#define LEDC_TEST_DUTY         (4096)
#define LEDC_TEST_FADE_TIME    (3000)

void app_main()
{
  // Configure parameters of an UART driver,
  // communication pins and install the driver
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1, 
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE                 
  };
  uart_param_config(UART_NUM_0, &uart_config);
  uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);

  printf("Hello world!\n");

  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GPIO15/16
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 1;
  //disable pull-up mode
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);

  esp_err_t err = gpio_set_level(GPIO_OUTPUT_IO_0, 0);

  printf("gpio_set_level %d\n", err);

#if 0
  int ch;

  /*
   * Prepare and set configuration of timers
   * that will be used by LED Controller
   */
  ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
    .speed_mode = LEDC_HS_MODE,           // timer mode
    .timer_num = LEDC_HS_TIMER            // timer index
  };
  // Set configuration of timer0 for high speed channels
  ledc_timer_config(&ledc_timer);

  // Prepare and set configuration of timer1 for low speed channels
  ledc_timer.speed_mode = LEDC_LS_MODE;
  ledc_timer.timer_num = LEDC_LS_TIMER;
  ledc_timer_config(&ledc_timer);

  /*
   * Prepare individual configuration
   * for each channel of LED Controller
   * by selecting:
   * - controller's channel number
   * - output duty cycle, set initially to 0
   * - GPIO number where LED is connected to
   * - speed mode, either high or low
   * - timer servicing selected channel
   *   Note: if different channels use one timer,
   *         then frequency and bit_num of these channels
   *         will be the same
   */
  ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
    {
      .channel    = LEDC_HS_CH0_CHANNEL,
      .duty       = 0,
      .gpio_num   = LEDC_HS_CH0_GPIO,
      .speed_mode = LEDC_HS_MODE,
      .hpoint     = 0,
      .timer_sel  = LEDC_HS_TIMER
    },
    {
      .channel    = LEDC_HS_CH1_CHANNEL,
      .duty       = 0,
      .gpio_num   = LEDC_HS_CH1_GPIO,
      .speed_mode = LEDC_HS_MODE,
      .hpoint     = 0,
      .timer_sel  = LEDC_HS_TIMER
    },
    {
      .channel    = LEDC_LS_CH2_CHANNEL,
      .duty       = 0,
      .gpio_num   = LEDC_LS_CH2_GPIO,
      .speed_mode = LEDC_LS_MODE,
      .hpoint     = 0,
      .timer_sel  = LEDC_LS_TIMER
    },
    {
      .channel    = LEDC_LS_CH3_CHANNEL,
      .duty       = 0,
      .gpio_num   = LEDC_LS_CH3_GPIO,
      .speed_mode = LEDC_LS_MODE,
      .hpoint     = 0,
      .timer_sel  = LEDC_LS_TIMER
    },
  };

  // Set LED Controller with previously prepared configuration
  for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
    ledc_channel_config(&ledc_channel[ch]);
  }

  // Initialize fade service.
  ledc_fade_func_install(0);

  while (1) {
    printf("1. LEDC fade up to duty = %d\n", LEDC_TEST_DUTY);
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
      ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
          ledc_channel[ch].channel, LEDC_TEST_DUTY, LEDC_TEST_FADE_TIME);
      ledc_fade_start(ledc_channel[ch].speed_mode,
          ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    }
    vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

    printf("2. LEDC fade down to duty = 0\n");
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
      ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
          ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
      ledc_fade_start(ledc_channel[ch].speed_mode,
          ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    }
    vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);

    printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
      ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_TEST_DUTY);
      ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    printf("4. LEDC set duty = 0 without fade\n");
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
      ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
      ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
#endif
}
