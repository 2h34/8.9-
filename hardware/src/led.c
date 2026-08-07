/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"

//外部变量声明
extern int signal;


//结构体定义
typedef struct
{
    uint8_t led_num;
    uint16_t on_ms;
    uint16_t off_ms; 
} blink_config;

//枚举定义
typedef enum
{
    FLOW_SINGLE = 0,
    FLOW_PAIR,
    FLOW_ALL    
} flow_mode;

// 流水灯模式变量
static flow_mode current_mode = FLOW_SINGLE;


/* 点亮 LED */
static void led_on(uint8_t led_num)
{
    if (!IS_VALID_LED(led_num))
    {
        return;
    }
    switch(led_num)
    {
        case 0U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
            break;
        case 1U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
            break;
        case 2U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_SET);
            break;
        case 3U:    
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_SET);
            break;
        default:
            break;       
    }
    
}


/* 熄灭 LED */
static void led_off(uint8_t led_num)
{
    if (!IS_VALID_LED(led_num))
    {
        return;
    }
    switch(led_num)
    {
        case 0U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
            break;
        case 1U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
            break;
        case 2U:
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET);
            break;
        case 3U:    
            HAL_GPIO_WritePin(LED_GPIO_PORT, LED4_PIN, GPIO_PIN_RESET);
            break;
        default:
            break;       
    }

}

static void blink(blink_config config);

static void update_mode(void);

//流水灯函数
void flow_led(void)
{
    update_mode();

    switch (current_mode)
    {
        case FLOW_SINGLE:
            for (uint8_t i = 0U; i < LED_COUNT; i++)
            {
                update_mode();//让它在每控制一颗灯之前重新检查一次信号
                if (current_mode != FLOW_SINGLE)    
                {
                    break;
                }

                blink_config config = {i, 250U, 250U};
                blink(config);
            }
            break;
        case FLOW_PAIR:
            for (uint8_t i = 0U; i < LED_COUNT-1U; i++)
            {
                update_mode();//让它在每控制一颗灯之前重新检查一次信号
                if (current_mode != FLOW_PAIR)    
                {
                    break;
                }

                led_on(i);
                led_on(i+1U);

                HAL_Delay(250U);

                led_off(i);
                led_off(i+1U);

                HAL_Delay(250U);
            }
            break;
        case FLOW_ALL:
            for (uint8_t i = 0U; i < LED_COUNT; i++)
            {
                led_on(i);
            }

            HAL_Delay(250U);

            for (uint8_t i = 0U; i < LED_COUNT; i++)
            {
                led_off(i);
            }

            HAL_Delay(250U);
            break;
        default:
            break;
    }
    
}


// 闪烁 LED 函数
static void blink(blink_config config)
{
    if (!IS_VALID_LED(config.led_num))
    {
        return;
    }

    led_on(config.led_num);
    HAL_Delay(config.on_ms);

    led_off(config.led_num);
    HAL_Delay(config.off_ms);
}

static void update_mode(void)
{
    if (signal == 0)
    {
        current_mode = FLOW_SINGLE;
    }
    else if (signal == 1)
    {
        current_mode = FLOW_PAIR;
    }
    else if (signal == 2)
    {
        current_mode = FLOW_ALL;
    }
}