#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/pwm.h"

#include "pwm_task.h"

// pwm pin number
const uint32_t pin_num[4] = {
    PWM_0_OUT_IO_NUM,
    PWM_1_OUT_IO_NUM,
    PWM_2_OUT_IO_NUM,
    PWM_3_OUT_IO_NUM
};

// duties table, real_duty = duties[x]/PERIOD
uint32_t duties[4] = {
    0, 500, 500, 500,
};

// phase table, delay = (phase[x]/360)*PERIOD
int16_t phase[4] = {
    0, 0, 90, -90,
};

void Pwm_TaskInit(void)
{
  pwm_init(PWM_PERIOD, duties, 4, pin_num);
  pwm_set_channel_invert(0x1 << 0);
  pwm_set_phases(phase);
  pwm_start();
  printf("PWM TASK INITIALIZED\n");
}

void Pwm_Task(void *params)
{
  printf("PWM TASK Started\n");
  int16_t count = 0;
  uint16_t duty = 0;
  bool add = true;
  uint8_t rate = 1;
  bool rate_add = true;
  while (1) {
    count++;
    duty += (add) ? rate : -rate;
    if (duty > 400 && add)
    {
      add = false;
    }
    if (duty == 0 && !add)
    {
      add = true;
      rate += (rate_add) ? 1 : -1;
      if (rate > 40 && rate_add)
      {
        rate_add = false;
      }
      if (rate == 1 && !rate_add)
      {
        rate_add = true;
      }
    }

    vTaskDelay(10 / portTICK_RATE_MS);
    pwm_stop(0x3);
    pwm_set_duty(0, duty);
    pwm_start();
  }
}
