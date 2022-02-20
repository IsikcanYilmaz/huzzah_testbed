
#include "i2c_manager.h"
#include "driver/i2c.h"

static esp_err_t i2c_example_master_init()
{
  int i2c_master_port = I2C_EXAMPLE_MASTER_NUM;
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = I2C_EXAMPLE_MASTER_SDA_IO;
  conf.sda_pullup_en = 1;
  conf.scl_io_num = I2C_EXAMPLE_MASTER_SCL_IO;
  conf.scl_pullup_en = 1;
  conf.clk_stretch_tick = 300; // 300 ticks, Clock stretch is about 210us, you can make changes according to the actual situation.
  ESP_ERROR_CHECK(i2c_driver_install(i2c_master_port, conf.mode));
  ESP_ERROR_CHECK(i2c_param_config(i2c_master_port, &conf));
  return ESP_OK;
}

static esp_err_t i2ctest(uint8_t reg_address, uint8_t *data, size_t data_len)
{
      esp_err_t ret;
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      ret = i2c_master_start(cmd);
      printf("25 %d\n", ret);
      ret = i2c_master_write_byte(cmd, I2C_OLED_ADDR << 1 | I2C_MASTER_WRITE, true);
      printf("27 %d\n", ret);
      ret = i2c_master_write_byte(cmd, reg_address, true);
      printf("29 %d\n", ret);
      ret = i2c_master_stop(cmd);
      printf("31 %d\n", ret);
      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
      printf("33 %d\n", ret);
      i2c_cmd_link_delete(cmd);
  
      if (ret != ESP_OK) {
          return ret;
      }
  
      /*cmd = i2c_cmd_link_create();*/
      ret = i2c_master_start(cmd);
      printf("42 %d\n", ret);
      ret = i2c_master_write_byte(cmd, I2C_OLED_ADDR << 1 | I2C_MASTER_READ, true);
      printf("45 %d\n", ret);
      ret = i2c_master_read(cmd, data, data_len, I2C_MASTER_LAST_NACK);
      printf("48 %d\n", ret);
      ret = i2c_master_stop(cmd);
      printf("51 %d\n", ret);
      ret = i2c_master_cmd_begin(I2C_EXAMPLE_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
      printf("53 %d\n", ret);
      i2c_cmd_link_delete(cmd);
  
      return ret;

}

void I2CMan_Init(void)
{
  esp_err_t i2c_init_ret = i2c_example_master_init();
  printf("I2C master init OK %d. (%d)\n", i2c_init_ret == ESP_OK, i2c_init_ret);
}

void I2CMan_Test(void)
{
  uint8_t testbuf[128];
  size_t testsize = 1;
  esp_err_t i2c_test_ret = i2ctest(OLED_DISPLAY_OFF, &testbuf, testsize);
  printf("I2C test %d\n", i2c_test_ret);
}

void I2CMan_Scan(void)
{

}
