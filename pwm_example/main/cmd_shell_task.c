#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "cmd_shell_task.h"

#include "i2c_manager.h"

static const char *TAG = "cmd";

static QueueHandle_t cmd_shell_queue;

static char cmd_buf[CMD_BUF_SIZE];
static uint16_t cmd_buf_tail = 0;
static bool cmdShellInitialized = false;

static void CmdShell_TakeCommand(void)
{
  printf("CMD SHELL CMD: %s %d\n", cmd_buf, cmd_buf_tail);
  cmd_buf_tail = 0;
  memset(&cmd_buf, 0x00, CMD_BUF_SIZE);
  I2CMan_Test();
}

bool CmdShell_TaskInit(void)
{
  cmdShellInitialized = true;
  printf("CMD SHELL Initialized\n");
  return true;
}

void CmdShell_AppendToCommandBuf(char *buf, uint16_t len)
{
  if (!cmdShellInitialized)
  {
    ESP_LOGI(TAG, "Cant take command, not initialized!\n");
    return;
  }
  if (len + cmd_buf_tail > CMD_BUF_SIZE)
  {
    ESP_LOGI(TAG, "Command buffer overflow!\n");
    cmd_buf_tail = 0;
    return;
  }

  memcpy(&cmd_buf[cmd_buf_tail], buf, len);
  cmd_buf_tail += len;

  printf("CURRENT COMBUF: %s %d\n", cmd_buf, cmd_buf_tail);
 
  // New command entered
  if (cmd_buf[cmd_buf_tail-1] == ']')
  {
    CmdShell_TakeCommand();
  }
}

void CmdShell_Task(void *params)
{
  printf("CMD SHELL Task created\n");
  while(1) 
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
