
#define I2C_EXAMPLE_MASTER_SCL_IO           2
#define I2C_EXAMPLE_MASTER_SDA_IO           14
#define I2C_EXAMPLE_MASTER_NUM              I2C_NUM_0

#define OLED_DISPLAY_OFF 0xae
#define OLED_DISPLAY_ON 0xaf
#define OLED_NOP 0xe3

#define I2C_OLED_ADDR 0x7a

void I2CMan_Init(void);
void I2CMan_Scan(void);

void I2CMan_Test();

