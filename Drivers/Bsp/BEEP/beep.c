#include "beep.h"
#include "tim.h"
#include "oled.h"

//定义音调表
const uint16_t tone[]={
	0,
	3817,3610,3401,3215,3030,2865,2703,2551,2410,2272,2146,2024,
	1912,1805,1703,1608,1517,1432,1351,1275,1203,1136,1073,1012,
	956,902,851,803,758,715,676,637,602,568,536,506,100
};

//定义简谱表：音调,时值
const uint16_t Music[]={
//孤独摇滚 吉他与孤独与蓝色星球（70）
	H1,2,M7,2,M6,2,M7,4,M1,2,M2,2,M3,4,M1,4,L7,4,L7,2,L7,4,
	H1,2,M7,2,M6,2,M7,4,M2,2,M3,4,M1,2,M5,4,M6,2,M5,4,
	H1,2,M7,2,M6,2,M7,4,M1,2,M2,2,M3,4,M1,4,L7,4,L7,2,L7,4,
	H2,4,H3,4,M6,4,H1,2,H2,4,H3,4,H5,4,H3,4,
	H6,4,H5,4,H2,2,H3,4,H3,4,M6,4,M5,2,M6,4,M6,4,
	
	M6,4,M6,2,M6,2,M6,2,M5,2,M5,2,M5,2,M3,4,M5,4,M3,4,M5,4,M3,6,
	M5,2,M3,6,M5,2,M3,6,M5,2,M3,6,M2,2,M1,10,
	M1,4,H1,4,M7,4,M5,4,M3,4,M2,4,M3,4,M2,4,M1,4,M3,6,M3,2,M2,8,
	
	M6,4,M6,2,M6,2,M6,2,M5,2,M5,2,M5,2,M3,4,M5,4,M3,4,M5,4,M3,6,
	M5,2,M3,6,M5,2,M3,6,M5,2,M3,6,M2,2,M1,10,
	M1,4,H1,4,M7,4,M5,4,M3,4,M2,4,M3,4,M2,4,M1,4,M3,6,M4,2,M2,8,
	
	M2,6,M1,2,M1,6,M1,2,M2,6,M1,2,M1,6,M1,2,M2,4,M1,4,M2,4,M5,4,M2,6,M1,2,M1,4,M1,6,
	M1,2,M2,6,M1,2,M1,4,M1,4,M2,6,M1,2,M1,4,M1,4,M2,4,M1,4,M2,4,M5,4,M6,4,M5,2,M5,2,M5,2,	
	M5,2,M6,6,M5,2,M5,4,M5,4,M6,6,M5,2,M5,4,M5,4,M6,4,M5,4,M6,4,H1,4,M7,4,M5,4,M3,4,
	M5,4,M1,12,M3,4,M2,2,M1,2,M1,6,M2,4,M3,6,M2,2,M2,8,M4,6,M3,2,M3,12,
	
	H1,4,M7,4,M5,4,M5,2,M6,2,M6,4,M5,2,M6,2,M6,4,M7,4,H1,4,H2,4,M7,4,M7,4,H1,4,M7,4,M5,4,M3,4,	
	M3,4,M6,2,M5,2,M3,2,M1,2,M2,2,M3,4,M1,2,M2,2,M3,2,M1,4,M2,2,M3,2,M1,4,M2,2,M3,2,M5,2,M3,2,M2,4,M3,4,M2,4,M1,4,M3,8,
	M6,4,M5,4,M5,2,M6,2,M6,4,M5,2,M6,2,M6,4,M7,4,H1,4,H2,4,M7,4,M5,4,H4,6,H3,2,H3,4,
	H1,4,H2,4,H3,4,H4,2,H3,2,H3,2,H2,2,H1,2,M7,2,M7,4,M6,2,M7,2,M7,4,H1,2,H1,4,H1,16,V0,8,
	H1,4,H2,4,H3,4,H4,2,H3,2,H3,2,H2,2,H1,2,M7,8,V0,2,M5,2,M7,4,H1,4,

	//结束标志位
	V0,16,0xFF  
};

void show_tone(void);

void beep_init(void)
{
    HAL_TIM_Base_Start(&htim4);         // 启动 TIM4 基础计数器
    HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1); // 启动 TIM4 通道 1 的比较功能
}

void beep_start(void)
{
    // 清中断标志位（可选）
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC1);

    // 启动定时器中断
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE); // 周期起点
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);    // 占空点
}

void beep_stop(void)
{
    __HAL_TIM_SET_AUTORELOAD(&htim4, 499);    //改回默认值
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0); // 占空比 0%

    // 关闭中断
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);

    // 拉低蜂鸣器引脚
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}


/**
 * @brief 设置蜂鸣器响度（占空比百分比）
 * @param percent 占空比百分比（0~100）
 */
void beep_set_volume(uint8_t percent)
{
    if (percent > 100) percent = 100;

    uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim4);
    uint16_t duty = (arr + 1) * percent / 100;

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, duty);
}


void beep_set_freq(uint32_t freq)   //设置音调（频率）
{
    if(freq == 0) return; // 防止除以0

    uint32_t timer_clk = 1000000; // 假设定时器时钟是1MHz
    uint16_t new_arr = (timer_clk / freq) - 1;

    // 保持占空比比例：duty / (old_arr + 1) = duty_new / (new_arr + 1)
    uint16_t old_arr = __HAL_TIM_GET_AUTORELOAD(&htim4);
    uint16_t old_duty = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);

    // 计算新的 duty
    uint16_t new_duty = (uint32_t)old_duty * (new_arr + 1) / (old_arr + 1);

    // 更新 ARR 和 CCR
    __HAL_TIM_SET_AUTORELOAD(&htim4, new_arr);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, new_duty);
}

void pwm_settone(uint16_t Prescaler)
{
    __HAL_TIM_SET_PRESCALER(&htim4, Prescaler);
}

static uint16_t choose=0,time=0;  //用于遍历数组

void beep_music(void)
{

    //下方time++两次，好比是二维数组
    //第一次取的是tone，第二次取的是time，即：让tone延时time微秒
    choose=Music[time];
    time++;  //1
    pwm_settone(tone[choose]);
    HAL_Delay(77.5*Music[time]);    //数字修改1/16时值
    time++;  //2
    OLED_ShowNum(64,0,time,2,12,1);
    show_tone();
}

//显示音调
void show_tone(void)
{
    switch (Music[time])
    {
    case 0:
        OLED_ShowString(64,12,"---",12,1);
        break;
    case 1:
        OLED_ShowString(64,12,"-C ",12,1);
        break;
    case 2:
        OLED_ShowString(64,12,"-C#",12,1);
        break;
    case 3:
        OLED_ShowString(64,12,"-D ",12,1);
        break;
    case 4:
        OLED_ShowString(64,12,"-D#",12,1);
        break;
    case 5:
        OLED_ShowString(64,12,"-E ",12,1);
        break;
    case 6:
        OLED_ShowString(64,12,"-F ",12,1);
        break;
    case 7:
        OLED_ShowString(64,12,"-F#",12,1);
        break;
    case 8:
        OLED_ShowString(64,12,"-G ",12,1);
        break;
    case 9:
        OLED_ShowString(64,12,"-G#",12,1);
        break;
    case 10:
        OLED_ShowString(64,12,"-A ",12,1);
        break;
    case 11:
        OLED_ShowString(64,12,"-A#",12,1);
        break;
    case 12:
        OLED_ShowString(64,12,"-B ",12,1);
        break;
    case 13:
        OLED_ShowString(64,12," C ",12,1);
        break;
    case 14:
        OLED_ShowString(64,12," C#",12,1);
        break;
    case 15:
        OLED_ShowString(64,12," D ",12,1);
        break;
    case 16:
        OLED_ShowString(64,12," D#",12,1);
        break;
    case 17:
        OLED_ShowString(64,12," E ",12,1);
        break;
    case 18:
        OLED_ShowString(64,12," F ",12,1);
        break;
    case 19:
        OLED_ShowString(64,12," F#",12,1);
        break;
    case 20:
        OLED_ShowString(64,12," G ",12,1);
        break;
    case 21:
        OLED_ShowString(64,12," G#",12,1);
        break;
    case 22:
        OLED_ShowString(64,12," A ",12,1);
        break;
    case 23:
        OLED_ShowString(64,12," A#",12,1);
        break;
    case 24:
        OLED_ShowString(64,12," B ",12,1);
        break;
    case 25:
        OLED_ShowString(64,12,"+C ",12,1);
        break;
    case 26:
        OLED_ShowString(64,12,"+C#",12,1);
        break;
    case 27:
        OLED_ShowString(64,12,"+D ",12,1);
        break;
    case 28:
        OLED_ShowString(64,12,"+D#",12,1);
        break;
    case 29:
        OLED_ShowString(64,12,"+E ",12,1);
        break;
    case 30:
        OLED_ShowString(64,12,"+F ",12,1);
        break;
    case 31:
        OLED_ShowString(64,12,"+F#",12,1);
        break;
    case 32:
        OLED_ShowString(64,12,"+G ",12,1);
        break;
    case 33:
        OLED_ShowString(64,12,"+G#",12,1);
        break;
    case 34:
        OLED_ShowString(64,12,"+A ",12,1);
        break;
    case 35:
        OLED_ShowString(64,12,"+A#",12,1);
        break;
    case 36:
        OLED_ShowString(64,12,"+B ",12,1);
        break;
    default:
        break;
    }
		OLED_Refresh();
}

// 中断回调
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET); // PWM尾巴 → 低
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);   // PWM周期起点 → 高
    }
}

/**
 * @brief 设置定时器 PWM 频率和占空比
 * @param htim        使用的定时器句柄，如 &htim2
 * @param channel     通道，如 TIM_CHANNEL_1
 * @param freq_hz     PWM频率（单位Hz）
 * @param duty_percent 占空比（0 ~ 100）
 */
void pwm_set_freq_and_duty(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t freq_hz, uint8_t duty_percent)
{
    if (freq_hz == 0) return;
    if (duty_percent > 100) duty_percent = 100;

    static uint32_t tim_clk = 72000000;  // 根据具体芯片设置定时器输入时钟，F103一般为72MHz
    uint32_t prescaler = 0;
    uint32_t arr = 100;
    uint32_t target_clk = freq_hz;

    // 找到合适的 prescaler 和 arr，确保 arr <= 65535
    for (prescaler = 0; prescaler < 0xFFFF; prescaler++)
    {
        arr = tim_clk / ((prescaler + 1) * target_clk);
        if (arr <= 0xFFFF) break;
    }

    // 更新定时器参数（注意先停再改）
    __HAL_TIM_DISABLE(htim);

    __HAL_TIM_SET_PRESCALER(htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(htim, arr);
    __HAL_TIM_SET_COMPARE(htim, channel, (arr + 1) * duty_percent / 100);
    __HAL_TIM_SET_COUNTER(htim, 0);

    __HAL_TIM_ENABLE(htim);
}

