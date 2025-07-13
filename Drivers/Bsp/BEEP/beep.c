#include "beep.h"
#include "tim.h"
#include "oled.h"

//����������
const uint16_t tone[]={
	0,
	3817,3610,3401,3215,3030,2865,2703,2551,2410,2272,2146,2024,
	1912,1805,1703,1608,1517,1432,1351,1275,1203,1136,1073,1012,
	956,902,851,803,758,715,676,637,602,568,536,506,100
};

//������ױ�����,ʱֵ
const uint16_t Music[]={
//�¶�ҡ�� ������¶�����ɫ����70��
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

	//������־λ
	V0,16,0xFF  
};

void show_tone(void);

void beep_init(void)
{
    HAL_TIM_Base_Start(&htim4);         // ���� TIM4 ����������
    HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1); // ���� TIM4 ͨ�� 1 �ıȽϹ���
}

void beep_start(void)
{
    // ���жϱ�־λ����ѡ��
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_CC1);

    // ������ʱ���ж�
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE); // �������
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_CC1);    // ռ�յ�
}

void beep_stop(void)
{
    __HAL_TIM_SET_AUTORELOAD(&htim4, 499);    //�Ļ�Ĭ��ֵ
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0); // ռ�ձ� 0%

    // �ر��ж�
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE_IT(&htim4, TIM_IT_CC1);

    // ���ͷ���������
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}


/**
 * @brief ���÷�������ȣ�ռ�ձȰٷֱȣ�
 * @param percent ռ�ձȰٷֱȣ�0~100��
 */
void beep_set_volume(uint8_t percent)
{
    if (percent > 100) percent = 100;

    uint16_t arr = __HAL_TIM_GET_AUTORELOAD(&htim4);
    uint16_t duty = (arr + 1) * percent / 100;

    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, duty);
}


void beep_set_freq(uint32_t freq)   //����������Ƶ�ʣ�
{
    if(freq == 0) return; // ��ֹ����0

    uint32_t timer_clk = 1000000; // ���趨ʱ��ʱ����1MHz
    uint16_t new_arr = (timer_clk / freq) - 1;

    // ����ռ�ձȱ�����duty / (old_arr + 1) = duty_new / (new_arr + 1)
    uint16_t old_arr = __HAL_TIM_GET_AUTORELOAD(&htim4);
    uint16_t old_duty = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);

    // �����µ� duty
    uint16_t new_duty = (uint32_t)old_duty * (new_arr + 1) / (old_arr + 1);

    // ���� ARR �� CCR
    __HAL_TIM_SET_AUTORELOAD(&htim4, new_arr);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, new_duty);
}

void pwm_settone(uint16_t Prescaler)
{
    __HAL_TIM_SET_PRESCALER(&htim4, Prescaler);
}

static uint16_t choose=0,time=0;  //���ڱ�������

void beep_music(void)
{

    //�·�time++���Σ��ñ��Ƕ�ά����
    //��һ��ȡ����tone���ڶ���ȡ����time��������tone��ʱtime΢��
    choose=Music[time];
    time++;  //1
    pwm_settone(tone[choose]);
    HAL_Delay(77.5*Music[time]);    //�����޸�1/16ʱֵ
    time++;  //2
    OLED_ShowNum(64,0,time,2,12,1);
    show_tone();
}

//��ʾ����
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

// �жϻص�
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET); // PWMβ�� �� ��
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM4)
    {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);   // PWM������� �� ��
    }
}

/**
 * @brief ���ö�ʱ�� PWM Ƶ�ʺ�ռ�ձ�
 * @param htim        ʹ�õĶ�ʱ��������� &htim2
 * @param channel     ͨ������ TIM_CHANNEL_1
 * @param freq_hz     PWMƵ�ʣ���λHz��
 * @param duty_percent ռ�ձȣ�0 ~ 100��
 */
void pwm_set_freq_and_duty(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t freq_hz, uint8_t duty_percent)
{
    if (freq_hz == 0) return;
    if (duty_percent > 100) duty_percent = 100;

    static uint32_t tim_clk = 72000000;  // ���ݾ���оƬ���ö�ʱ������ʱ�ӣ�F103һ��Ϊ72MHz
    uint32_t prescaler = 0;
    uint32_t arr = 100;
    uint32_t target_clk = freq_hz;

    // �ҵ����ʵ� prescaler �� arr��ȷ�� arr <= 65535
    for (prescaler = 0; prescaler < 0xFFFF; prescaler++)
    {
        arr = tim_clk / ((prescaler + 1) * target_clk);
        if (arr <= 0xFFFF) break;
    }

    // ���¶�ʱ��������ע����ͣ�ٸģ�
    __HAL_TIM_DISABLE(htim);

    __HAL_TIM_SET_PRESCALER(htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(htim, arr);
    __HAL_TIM_SET_COMPARE(htim, channel, (arr + 1) * duty_percent / 100);
    __HAL_TIM_SET_COUNTER(htim, 0);

    __HAL_TIM_ENABLE(htim);
}

