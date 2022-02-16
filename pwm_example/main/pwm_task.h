
#define PWM_0_OUT_IO_NUM   2
#define PWM_1_OUT_IO_NUM   13
#define PWM_2_OUT_IO_NUM   14
#define PWM_3_OUT_IO_NUM   15

// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD    (1000)

bool Pwm_TaskInit(void);
void Pwm_Task(void *params);
