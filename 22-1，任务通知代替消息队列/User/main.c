/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   FreeRTOS V9.0.0  + STM32 ����֪ͨ������Ϣ����
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "limits.h"
/* ������Ӳ��bspͷ�ļ� */
#include "stm32h7xx.h"
#include "main.h"
#include "bsp_led.h"
#include "bsp_key.h" 
#include "core_delay.h"   
#include "bsp_debug_usart.h"

/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Receive1_Task_Handle = NULL;/* Receive1_Task������ */
static TaskHandle_t Receive2_Task_Handle = NULL;/* Receive2_Task������ */
static TaskHandle_t Send_Task_Handle = NULL;/* Send_Task������ */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */


/******************************* �궨�� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩ�궨�塣
 */
#define  USE_CHAR  0  /* �����ַ�����ʱ������Ϊ 1 �����Ա�������Ϊ 0  */

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void Receive1_Task(void* pvParameters);/* Receive1_Task����ʵ�� */
static void Receive2_Task(void* pvParameters);/* Receive2_Task����ʵ�� */

static void Send_Task(void* pvParameters);/* Send_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  BSP_Init();
	printf("����һ��[Ұ��]-STM32ȫϵ�п�����-FreeRTOS����֪ͨ������Ϣ����ʵ�飡\n");
  printf("����KEY1����KEY2����������Ϣ֪ͨ���� \n");
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���

  /* ����Receive1_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Receive1_Task, /* ������ں��� */
                        (const char*    )"Receive1_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Receive1_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����Receive1_Task����ɹ�!\r\n");
  
  /* ����Receive2_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Receive2_Task, /* ������ں��� */
                        (const char*    )"Receive2_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Receive2_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����Receive2_Task����ɹ�!\r\n");
  
  /* ����Send_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Send_Task,  /* ������ں��� */
                        (const char*    )"Send_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )4, /* ��������ȼ� */
                        (TaskHandle_t*  )&Send_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����Send_Task����ɹ�!\n\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� Receive_Task
  * @ ����˵���� Receive_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Receive1_Task(void* parameter)
{	
  BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
#if USE_CHAR
  char *r_char;
#else
  uint32_t r_num;
#endif
  while (1)
  {
    /* BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry, 
                                  uint32_t ulBitsToClearOnExit, 
                                  uint32_t *pulNotificationValue, 
                                  TickType_t xTicksToWait ); 
     * ulBitsToClearOnEntry����û�н��յ�����֪ͨ��ʱ������ֵ֪ͨ��˲�����ȡ
       ��ֵ���а�λ�����㣬���˲���ΪOxfffff����ULONG_MAX��ʱ��ͻὫ����ֵ֪ͨ���㡣
     * ulBits ToClearOnExit��������յ�������֪ͨ����������Ӧ�Ĵ����˳�����֮ǰ��
       ����ֵ֪ͨ��˲�����ȡ��ֵ���а�λ�����㣬���˲���Ϊ0xfffff����ULONG MAX��ʱ��
       �ͻὫ����ֵ֪ͨ���㡣
     * pulNotification Value���˲���������������ֵ֪ͨ��
     * xTick ToWait������ʱ�䡣
     *
     * ����ֵ��pdTRUE����ȡ��������֪ͨ��pdFALSE������֪ͨ��ȡʧ�ܡ�
     */
    //��ȡ����֪ͨ ,û��ȡ����һֱ�ȴ�
		xReturn=xTaskNotifyWait(0x0,			//���뺯����ʱ���������bit
                            ULONG_MAX,	  //�˳�������ʱ��������е�bit
#if USE_CHAR
                            (uint32_t *)&r_char,		  //��������ֵ֪ͨ
#else
                            &r_num,		  //��������ֵ֪ͨ
#endif                        
                            portMAX_DELAY);	//����ʱ��
    if( pdTRUE == xReturn )
#if USE_CHAR
      printf("Receive1_Task ����֪ͨ��ϢΪ %s \n",r_char);                      
#else
      printf("Receive1_Task ����֪ͨ��ϢΪ %d \n",r_num);                      
#endif  
     
    
		LED1_TOGGLE;
  }
}

/**********************************************************************
  * @ ������  �� Receive_Task
  * @ ����˵���� Receive_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Receive2_Task(void* parameter)
{	
  BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
#if USE_CHAR
  char *r_char;
#else
  uint32_t r_num;
#endif
  while (1)
  {
    /* BaseType_t xTaskNotifyWait(uint32_t ulBitsToClearOnEntry, 
                                  uint32_t ulBitsToClearOnExit, 
                                  uint32_t *pulNotificationValue, 
                                  TickType_t xTicksToWait ); 
     * ulBitsToClearOnEntry����û�н��յ�����֪ͨ��ʱ������ֵ֪ͨ��˲�����ȡ
       ��ֵ���а�λ�����㣬���˲���ΪOxfffff����ULONG_MAX��ʱ��ͻὫ����ֵ֪ͨ���㡣
     * ulBits ToClearOnExit��������յ�������֪ͨ����������Ӧ�Ĵ����˳�����֮ǰ��
       ����ֵ֪ͨ��˲�����ȡ��ֵ���а�λ�����㣬���˲���Ϊ0xfffff����ULONG MAX��ʱ��
       �ͻὫ����ֵ֪ͨ���㡣
     * pulNotification Value���˲���������������ֵ֪ͨ��
     * xTick ToWait������ʱ�䡣
     *
     * ����ֵ��pdTRUE����ȡ��������֪ͨ��pdFALSE������֪ͨ��ȡʧ�ܡ�
     */
    //��ȡ����֪ͨ ,û��ȡ����һֱ�ȴ�
		xReturn=xTaskNotifyWait(0x0,			//���뺯����ʱ���������bit
                            ULONG_MAX,	  //�˳�������ʱ��������е�bit
#if USE_CHAR
                            (uint32_t *)&r_char,		  //��������ֵ֪ͨ
#else
                            &r_num,		  //��������ֵ֪ͨ
#endif                        
                            portMAX_DELAY);	//����ʱ��
    if( pdTRUE == xReturn )
#if USE_CHAR
      printf("Receive2_Task ����֪ͨ��ϢΪ %s \n",r_char);                      
#else
      printf("Receive2_Task ����֪ͨ��ϢΪ %d \n",r_num);                      
#endif  
		LED2_TOGGLE;
  }
}

/**********************************************************************
  * @ ������  �� Send_Task
  * @ ����˵���� Send_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Send_Task(void* parameter)
{	 
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
#if USE_CHAR
  char test_str1[] = "this is a mail test 1";/* ������Ϣtest1 */
  char test_str2[] = "this is a mail test 2";/* ������Ϣtest2 */
#else
  uint32_t send1 = 1;
  uint32_t send2 = 2;
#endif
  

  
  while (1)
  {
    /* KEY1 ������ */
    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
    {
      /* ԭ��:BaseType_t xTaskNotify( TaskHandle_t xTaskToNotify, 
                                      uint32_t ulValue, 
                                      eNotifyAction eAction ); 
       * eNoAction = 0��֪ͨ�������������ֵ֪ͨ��
       * eSetBits��     ��������ֵ֪ͨ�е�λ��
       * eIncrement��   ���������ֵ֪ͨ��
       * eSetvaluewithoverwrite�����ǵ�ǰ֪ͨ
       * eSetValueWithoutoverwrite �����ǵ�ǰ֪ͨ
       * 
       * pdFAIL��������eAction����ΪeSetValueWithoutOverwrite��ʱ��
       * �������ֵ֪ͨû�и��³ɹ��ͷ���pdFAIL��
       * pdPASS: eAction ����Ϊ����ѡ���ʱ��ͳһ����pdPASS��
      */
      xReturn = xTaskNotify( Receive1_Task_Handle, /*������*/
#if USE_CHAR 
                             (uint32_t)&test_str1, /* ���͵����ݣ����Ϊ4�ֽ� */
#else
                              send1, /* ���͵����ݣ����Ϊ4�ֽ� */
#endif
                             eSetValueWithOverwrite );/*���ǵ�ǰ֪ͨ*/
      
      if( xReturn == pdPASS )
        printf("Receive1_Task_Handle ����֪ͨ��Ϣ���ͳɹ�!\r\n");
    } 
    /* KEY2 ������ */
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
    {
      xReturn = xTaskNotify( Receive2_Task_Handle, /*������*/
#if USE_CHAR 
                             (uint32_t)&test_str2, /* ���͵����ݣ����Ϊ4�ֽ� */
#else
                              send2, /* ���͵����ݣ����Ϊ4�ֽ� */
#endif
                             eSetValueWithOverwrite );/*���ǵ�ǰ֪ͨ*/
      /* �˺���ֻ�᷵��pdPASS */
      if( xReturn == pdPASS )
        printf("Receive2_Task_Handle ����֪ͨ��Ϣ���ͳɹ�!\r\n");
    }
    vTaskDelay(20);
  }
}


/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
  /* ϵͳʱ�ӳ�ʼ����480MHz */
	SystemClock_Config();
  
  /* ��ʼ��SysTick */
  HAL_SYSTICK_Config( HAL_RCC_GetSysClockFreq() / configTICK_RATE_HZ );	

	/* Ӳ��BSP��ʼ��ͳͳ�����������LED�����ڣ�LCD�� */
    
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	
	
	/* usart �˿ڳ�ʼ�� */
  DEBUG_USART_Config();

  /* KEY �˿ڳ�ʼ�� */
  Key_GPIO_Config();
  
}
 
/**
  * @brief  System Clock ����
  *         system Clock ��������: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 480000000 (CPU Clock)
	*            HCLK(Hz)             = 240000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 192
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*ʹ�ܹ������ø��� */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* ��������ʱ��Ƶ�ʵ������ϵͳƵ��ʱ����ѹ���ڿ����Ż����ģ�
		 ����ϵͳƵ�ʵĵ�ѹ����ֵ�ĸ��¿��Բο���Ʒ�����ֲᡣ  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
 
  /* ����HSE������ʹ��HSE��ΪԴ����PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
 
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
	/* ѡ��PLL��Ϊϵͳʱ��Դ����������ʱ�ӷ�Ƶ�� */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK  | \
																 RCC_CLOCKTYPE_HCLK    | \
																 RCC_CLOCKTYPE_D1PCLK1 | \
																 RCC_CLOCKTYPE_PCLK1   | \
                                 RCC_CLOCKTYPE_PCLK2   | \
																 RCC_CLOCKTYPE_D3PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}
/****************************END OF FILE***************************/
