/**
 * @file    led.c
 * @brief   LED 驱动实现文件。
 */
#include "led.h"

//外部变量声明
volatile extern int signal;


// //结构体定义
// typedef struct
// {
//     uint8_t led_num;
//     uint16_t on_ms;
//     uint16_t off_ms; 
// } blink_config;

//题目五计时器结构体定义
typedef struct
{
    uint32_t start_tick;
    uint32_t duration_ms;
    uint8_t active;
} timer_t;



//流水灯模式枚举定义
typedef enum
{
    FLOW_IDLE =0,
    FLOW_SINGLE,
    FLOW_PAIR,
    FLOW_ALL    
} flow_mode;

//灯的亮灭状态枚举定义
typedef enum
{
    LED_STEP_ON = 0,
    LED_STEP_OFF
} led_step_t;




// 流水灯模式变量
static flow_mode current_mode = FLOW_IDLE;

//题目五计时器变量
static timer_t led_timer;

//灯亮灭状态变量
static led_step_t current_step = LED_STEP_ON;

//当前亮的灯
static uint8_t current_led = 0U;



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

// static void blink(blink_config config);

static void update_mode(void);
static void timer_start(timer_t *timer,uint32_t duration_ms);
static uint8_t timer_expired(timer_t *timer);
static void mode_init(void);

//流水灯函数
void flow_led(void)
{
    flow_mode previuos_mode = current_mode;
    update_mode();
    if (current_mode != previuos_mode)
    {
        mode_init();
    }

    switch (current_mode)
    {
        case FLOW_IDLE:
            for (uint8_t i = 0U; i < LED_COUNT; i++)
            {
                led_off(i);
            }
            break;

        case FLOW_SINGLE:
            if (timer_expired(&led_timer))
            {
                if (current_step == LED_STEP_ON)
                {
                    /* code */
                    led_off(current_led);
                    current_step = LED_STEP_OFF;
                    timer_start(&led_timer,250U);
                }
                else
                {
                    current_led++;
                    if (current_led >= LED_COUNT)
                    {
                        current_led = 0U;
                    }
                    led_on(current_led);
                    current_step = LED_STEP_ON;
                    timer_start(&led_timer, 250U);
                }
            }
            break;

        case FLOW_PAIR:
            if (timer_expired(&led_timer))
            {
                if (current_step == LED_STEP_ON)
                {
                    /* code */
                    led_off(current_led);
                    led_off(current_led + 1U);
                    current_step = LED_STEP_OFF;
                    timer_start(&led_timer,250U);
                }
                else
                {
                    current_led++;
                    if (current_led >= LED_COUNT-1)
                    {
                        current_led = 0U;
                    }
                    led_on(current_led);
                    led_on(current_led + 1U);
                    current_step = LED_STEP_ON;
                    timer_start(&led_timer, 250U);
                }
            }
            break;

        case FLOW_ALL:
            if (timer_expired(&led_timer))
            {
                if (current_step == LED_STEP_ON)
                {
                    /* code */
                    for (uint8_t i = 0U; i < LED_COUNT; i++)
                    {
                        led_off(i);
                    }
                    current_step = LED_STEP_OFF;
                    timer_start(&led_timer,250U);
                }
                else
                {
                    for (uint8_t i = 0U; i < LED_COUNT; i++)
                    {
                        led_on(i);
                    }
                    current_step = LED_STEP_ON;
                    timer_start(&led_timer, 250U);
                }
            }
            break;

        default:
            break;
    }
    
}


// // 闪烁 LED 函数
// static void blink(blink_config config)
// {
//     if (!IS_VALID_LED(config.led_num))
//     {
//         return;
//     }

//     led_on(config.led_num);
//     HAL_Delay(config.on_ms);

//     led_off(config.led_num);
//     HAL_Delay(config.off_ms);
// }

static void update_mode(void)
{

    if (signal == 0)
    {
        current_mode = FLOW_IDLE;
    }
    else if (signal == 1)
    {
        current_mode = FLOW_SINGLE;
    }
    else if (signal == 2)
    {
        current_mode = FLOW_PAIR;
    }
    else if (signal == 3)
    {
        current_mode = FLOW_ALL;
    }
}

//定时器启动函数
static void timer_start(timer_t *timer,uint32_t duration_ms)
{
    timer->start_tick = HAL_GetTick();
    timer->duration_ms= duration_ms;
    timer->active = 1U;
}

//定时器查询是否到期函数
static uint8_t timer_expired(timer_t *timer)
{
    if (timer->active == 0U)
    {
        return 0U;
    }
    
    if ((HAL_GetTick() - timer->start_tick) >= timer->duration_ms)
    {
        /* code */
        timer->active = 0U;//单次触发，到期后不重新启动就会失效
        return 1U;
    }

    return 0U;  
}

//初始化函数
static void mode_init(void)
{
    for (uint8_t i = 0U; i < LED_COUNT; i++)
    {
        led_off(i);
    }

    current_led = 0U;
    current_step = LED_STEP_ON;

    led_timer.active = 0U;

    switch (current_mode)
    {
        case FLOW_IDLE:
            break;
        
        case FLOW_SINGLE:
            led_on(current_led);
            timer_start(&led_timer,250U);
            break;
        
        case FLOW_PAIR:
            led_on(current_led);
            led_on(current_led + 1U);
            timer_start(&led_timer, 250U);
            break;

        case FLOW_ALL:
            for (uint8_t i = 0U; i < LED_COUNT; i++)
            {
                led_on(i);
            }
            timer_start(&led_timer, 250U);
            break;

        default:
            break;

    }
}
