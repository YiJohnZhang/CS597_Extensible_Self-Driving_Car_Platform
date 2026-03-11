#include "CH59x_common.h"

/* ========================= Pin Definition ========================= */
/*
 *
 * Left Upper Wheel:
 *   PA0 -> LU_IN1
 *   PA1 -> LU_IN2
 *
 * Left Lower Wheel:
 *   PA2 -> LL_IN1
 *   PA3 -> LL_IN2
 *
 * Right Upper Wheel:
 *   PA4 -> RU_IN1
 *   PA5 -> RU_IN2
 *
 * Right Lower Wheel:
 *   PA6 -> RL_IN1
 *   PA7 -> RL_IN2
 */

#define LU_IN1_PIN      GPIO_Pin_0
#define LU_IN2_PIN      GPIO_Pin_1

#define LL_IN1_PIN      GPIO_Pin_2
#define LL_IN2_PIN      GPIO_Pin_3

#define RU_IN1_PIN      GPIO_Pin_4
#define RU_IN2_PIN      GPIO_Pin_5

#define RL_IN1_PIN      GPIO_Pin_6
#define RL_IN2_PIN      GPIO_Pin_7

#define DUTY_MAX        4095
#define DUTY_MIN       -4095

/* ========================= Type Definition ========================= */
typedef struct
{
    uint8_t initialized;
} Smart_Car;

/* ========================= Function Declaration ========================= */
void Motor_GPIO_Init(void);

void Smart_Car_Init(Smart_Car *car);
void Smart_Car_Close(Smart_Car *car);

int Duty_Limit(int duty);
void Duty_Range(int *duty1, int *duty2, int *duty3, int *duty4);

void Left_Upper_Wheel(int duty);
void Left_Lower_Wheel(int duty);
void Right_Upper_Wheel(int duty);
void Right_Lower_Wheel(int duty);

void Set_Motor_Model(int duty1, int duty2, int duty3, int duty4);

/* ========================= GPIO Init ========================= */
void Motor_GPIO_Init(void)
{
    GPIOA_ResetBits(LU_IN1_PIN | LU_IN2_PIN |
                    LL_IN1_PIN | LL_IN2_PIN |
                    RU_IN1_PIN | RU_IN2_PIN |
                    RL_IN1_PIN | RL_IN2_PIN);

    GPIOA_ModeCfg(LU_IN1_PIN | LU_IN2_PIN |
                  LL_IN1_PIN | LL_IN2_PIN |
                  RU_IN1_PIN | RU_IN2_PIN |
                  RL_IN1_PIN | RL_IN2_PIN,
                  GPIO_ModeOut_PP_5mA);
}

/* ========================= Car Init / Close ========================= */
void Smart_Car_Init(Smart_Car *car)
{
    if(car == 0)
    {
        return;
    }

    Motor_GPIO_Init();
    car->initialized = 1;
}

void Smart_Car_Close(Smart_Car *car)
{
    Set_Motor_Model(0, 0, 0, 0);

    if(car != 0)
    {
        car->initialized = 0;
    }
}

/* ========================= Duty Helper ========================= */
int Duty_Limit(int duty)
{
    if(duty > DUTY_MAX)
    {
        duty = DUTY_MAX;
    }
    else if(duty < DUTY_MIN)
    {
        duty = DUTY_MIN;
    }

    return duty;
}

void Duty_Range(int *duty1, int *duty2, int *duty3, int *duty4)
{
    if(duty1 != 0) *duty1 = Duty_Limit(*duty1);
    if(duty2 != 0) *duty2 = Duty_Limit(*duty2);
    if(duty3 != 0) *duty3 = Duty_Limit(*duty3);
    if(duty4 != 0) *duty4 = Duty_Limit(*duty4);
}

/* ========================= Wheel Control ========================= */
/*
 * Python logic:
 *   duty > 0 : forward
 *   duty < 0 : backward
 *   duty = 0 : stop
 *
 * For CH592F GPIO version:
 *   forward  -> IN1=1, IN2=0
 *   backward -> IN1=0, IN2=1
 *   stop     -> IN1=0, IN2=0
 */

/* Left Upper Wheel */
void Left_Upper_Wheel(int duty)
{
    if(duty > 0)
    {
        GPIOA_SetBits(LU_IN1_PIN);
        GPIOA_ResetBits(LU_IN2_PIN);
    }
    else if(duty < 0)
    {
        GPIOA_ResetBits(LU_IN1_PIN);
        GPIOA_SetBits(LU_IN2_PIN);
    }
    else
    {
        GPIOA_ResetBits(LU_IN1_PIN);
        GPIOA_ResetBits(LU_IN2_PIN);
    }
}

/* Left Lower Wheel */
void Left_Lower_Wheel(int duty)
{
    if(duty > 0)
    {
        GPIOA_SetBits(LL_IN1_PIN);
        GPIOA_ResetBits(LL_IN2_PIN);
    }
    else if(duty < 0)
    {
        GPIOA_ResetBits(LL_IN1_PIN);
        GPIOA_SetBits(LL_IN2_PIN);
    }
    else
    {
        GPIOA_ResetBits(LL_IN1_PIN);
        GPIOA_ResetBits(LL_IN2_PIN);
    }
}

/* Right Upper Wheel */
void Right_Upper_Wheel(int duty)
{
    if(duty > 0)
    {
        GPIOA_SetBits(RU_IN1_PIN);
        GPIOA_ResetBits(RU_IN2_PIN);
    }
    else if(duty < 0)
    {
        GPIOA_ResetBits(RU_IN1_PIN);
        GPIOA_SetBits(RU_IN2_PIN);
    }
    else
    {
        GPIOA_ResetBits(RU_IN1_PIN);
        GPIOA_ResetBits(RU_IN2_PIN);
    }
}

/* Right Lower Wheel */
void Right_Lower_Wheel(int duty)
{
    if(duty > 0)
    {
        GPIOA_SetBits(RL_IN1_PIN);
        GPIOA_ResetBits(RL_IN2_PIN);
    }
    else if(duty < 0)
    {
        GPIOA_ResetBits(RL_IN1_PIN);
        GPIOA_SetBits(RL_IN2_PIN);
    }
    else
    {
        GPIOA_ResetBits(RL_IN1_PIN);
        GPIOA_ResetBits(RL_IN2_PIN);
    }
}

/* ========================= Motor Model ========================= */
void Set_Motor_Model(int duty1, int duty2, int duty3, int duty4)
{
    Duty_Range(&duty1, &duty2, &duty3, &duty4);

    Left_Upper_Wheel(duty1);
    Left_Lower_Wheel(duty2);
    Right_Upper_Wheel(duty3);
    Right_Lower_Wheel(duty4);
}

/* ========================= Main ========================= */
int main(void)
{
    Smart_Car car;

    SetSysClock(CLK_SOURCE_PLL_60MHz);
    Delay_Init();

    Smart_Car_Init(&car);

    while(1)
    {
        Set_Motor_Model(2000, 2000, 2000, 2000);      // Forward
        DelayMs(1000);

        Set_Motor_Model(-2000, -2000, -2000, -2000);  // Back
        DelayMs(1000);

        Set_Motor_Model(-2000, -2000, 2000, 2000);    // Left
        DelayMs(1000);

        Set_Motor_Model(2000, 2000, -2000, -2000);    // Right
        DelayMs(1000);

        Set_Motor_Model(0, 0, 0, 0);                  // Stop
        DelayMs(1000);
    }
}
