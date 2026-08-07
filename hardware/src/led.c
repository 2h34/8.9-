/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"


/* 点亮 LED */
void led_on(uint8_t led_num)
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
void led_off(uint8_t led_num)
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

//流水灯函数
void flow_led(void)
{
    for (int i = 0; i < LED_COUNT; i++)
    {
        led_on(i);
        HAL_Delay(250U);
        led_off(i);
        HAL_Delay(250U);
    }
}
