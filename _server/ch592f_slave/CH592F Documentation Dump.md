CH592F Documentation Dump
- [I2C Example](https://github.com/openwch/ch592/blob/main/EVT/EXAM/I2C/src/Main.c)
- [ADC Example](https://github.com/openwch/ch592/blob/main/EVT/EXAM/ADC/src/Main.c)
- [PWM Example](https://github.com/openwch/ch592/blob/main/EVT/EXAM/PWMX/src/Main.c)

# I2C (`CH59x.i2c.h`)
```c
// i2c mode
typedef enum
{
    I2C_Mode_I2C = 0x0000,
    I2C_Mode_SMBusDevice = 0x0002,	// slave
    I2C_Mode_SMBusHost = 0x000A,	// master
} I2C_ModeTypeDef;


void I2C_Init(I2C_ModeTypeDef I2C_Mode, UINT32 I2C_ClockSpeed, I2C_DutyTypeDef I2C_DutyCycle,
              I2C_AckTypeDef I2C_Ack, I2C_AckAddrTypeDef I2C_AckAddr, UINT16 I2C_OwnAddress1);
```


# GPIO (`ch59x.gpio....`)
## Pin Definitions
```c
#define GPIO_Pin_0      (0x00000001) /*!< Pin 0 selected */
#define GPIO_Pin_1      (0x00000002) /*!< Pin 1 selected */
#define GPIO_Pin_2      (0x00000004) /*!< Pin 2 selected */
#define GPIO_Pin_3      (0x00000008) /*!< Pin 3 selected */
#define GPIO_Pin_4      (0x00000010) /*!< Pin 4 selected */
#define GPIO_Pin_5      (0x00000020) /*!< Pin 5 selected */
#define GPIO_Pin_6      (0x00000040) /*!< Pin 6 selected */
#define GPIO_Pin_7      (0x00000080) /*!< Pin 7 selected */
#define GPIO_Pin_8      (0x00000100) /*!< Pin 8 selected */
#define GPIO_Pin_9      (0x00000200) /*!< Pin 9 selected */
#define GPIO_Pin_10     (0x00000400) /*!< Pin 10 selected */
#define GPIO_Pin_11     (0x00000800) /*!< Pin 11 selected */
#define GPIO_Pin_12     (0x00001000) /*!< Pin 12 selected */
#define GPIO_Pin_13     (0x00002000) /*!< Pin 13 selected */
#define GPIO_Pin_14     (0x00004000) /*!< Pin 14 selected */
#define GPIO_Pin_15     (0x00008000) /*!< Pin 15 selected */
#define GPIO_Pin_16     (0x00010000) /*!< Pin 16 selected */
#define GPIO_Pin_17     (0x00020000) /*!< Pin 17 selected */
#define GPIO_Pin_18     (0x00040000) /*!< Pin 18 selected */
#define GPIO_Pin_19     (0x00080000) /*!< Pin 19 selected */
#define GPIO_Pin_20     (0x00100000) /*!< Pin 20 selected */
#define GPIO_Pin_21     (0x00200000) /*!< Pin 21 selected */
#define GPIO_Pin_22     (0x00400000) /*!< Pin 22 selected */
#define GPIO_Pin_23     (0x00800000) /*!< Pin 23 selected */
```
## Config GPIO
```c
typedef enum
{
    GPIO_ModeIN_Floating, //浮空输入
    GPIO_ModeIN_PU,       //上拉输入 (pullup)
    GPIO_ModeIN_PD,       //下拉输入 (pulldown)
    GPIO_ModeOut_PP_5mA,  //推挽输出最大5mA
    GPIO_ModeOut_PP_20mA, //推挽输出最大20mA

} GPIOModeTypeDef;

/**
 * @brief   GPIOA端口引脚模式配置
 *
 * @param   pin     - PA4-PA15,对应芯片12个GPIO引脚
 * @param   mode    - 输入输出类型
 */
void GPIOA_ModeCfg(uint32_t pin, GPIOModeTypeDef mode);

/**
 * @brief   GPIOB端口引脚模式配置
 *
 * @param   pin     - PB0,PB4,PB6-PB7,PB10-PB15,PB22-PB23,对应芯片12个GPIO引脚
 * @param   mode    - 输入输出类型
 */
void GPIOB_ModeCfg(uint32_t pin, GPIOModeTypeDef mode);

/* return 32 bits from GPIOA, lower 16-bits are valid
 *
 * @return  GPIOA端口32位数据
 */


/**
 * @brief   digital read
 *
 * @param   pin     - PA4-PA15,对应芯片12个GPIO引脚
 *
 * @return  GPIOA端口引脚状态
 */
#define GPIOA_ReadPortPin(pin)    (R32_PA_PIN & (pin))

/**
 * @brief   digital read
 *
 * @param   pin     - PB0,PB4,PB6-PB7,PB10-PB15,PB22-PB23,对应芯片12个GPIO引脚
 *
 * @return  GPIOB端口引脚状态
 */
#define GPIOB_ReadPortPin(pin)    (R32_PB_PIN & (pin))

/**
 * @brief   config gpioa INTR mode
 *
 * @param   pin     - PA4-PA15,对应芯片12个GPIO引脚
 * @param   mode    - 触发类型
 */
void GPIOA_ITModeCfg(uint32_t pin, GPIOITModeTpDef mode);
```