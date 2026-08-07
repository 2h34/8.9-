/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"

typedef struct
{
    uint8_t led_num;
    uint16_t on_ms;
    uint16_t off_ms; 
} blink_config;

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


//流水灯函数
void flow_led(void)
{
    for (uint8_t i = 0U; i < LED_COUNT; i++)
    {
        blink_config config = {i, 250U, 250U};
        blink(config);
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