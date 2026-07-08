void SystemClock_Config(void)
{
  //LL_FLASH_SetLatency(LL_FLASH_LATENCY_3); // 64MHz
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, 0x3U)

  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_3)
  {
  }

  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_HSI_SetDivider(LL_RCC_HSI_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  }

  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);
  LL_SetSystemCoreClock(64000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_6
                          |LL_GPIO_PIN_7|LL_GPIO_PIN_9);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_4|LL_GPIO_PIN_6
                          |LL_GPIO_PIN_7|LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  //LL_ICACHE_SetMode(LL_ICACHE_1WAY);

  //LL_ICACHE_Enable();
  SET_BIT(ICACHE->CR, ICACHE_CR_EN); // ICACHE_CR_EN==1
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}


int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef gpio_init_structure;
  static BSP_EXTI_LineCallback ButtonCallback[BUTTON_NBR] = {BUTTON_USER_EXTI_Callback};
  static uint32_t  BSP_BUTTON_PRIO [BUTTON_NBR] = {BSP_BUTTON_USER_IT_PRIORITY};
  static const uint32_t BUTTON_EXTI_LINE[BUTTON_NBR] = {BUTTON_USER_EXTI_LINE};

  /* Enable the BUTTON clock */
  //BUTTON_USER_GPIO_CLK_ENABLE();
  do { \
    volatile uint32_t tmpreg; \
    ((((RCC_TypeDef *) (((0x40000000UL) + 0x04020000UL) + 0x0C00UL))->AHB2ENR) |= ((0x1UL << (2U)))); \
    /* Delay after an RCC peripheral clock enabling */ \
    tmpreg = ((((RCC_TypeDef *) (((0x40000000UL) + 0x04020000UL) + 0x0C00UL))->AHB2ENR) & ((0x1UL << (2U)))); \
    ((void)(tmpreg)); \
  } while(0)
  gpio_init_structure.Pin = BUTTON_PIN [Button]; // 1<<13
  gpio_init_structure.Pull = GPIO_PULLDOWN;      // 2
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH; // 2

  /* (ButtonMode == BUTTON_MODE_EXTI) */
  {
    /* Configure Button pin as input with External interrupt */
    gpio_init_structure.Mode = GPIO_MODE_IT_RISING; // 0x10110000

    HAL_GPIO_Init(BUTTON_PORT[Button], &gpio_init_structure);

    (void)HAL_EXTI_GetHandle(&hpb_exti[Button], BUTTON_EXTI_LINE[Button]);
    (void)HAL_EXTI_RegisterCallback(&hpb_exti[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }

  return BSP_ERROR_NONE;
}

void HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, const GPIO_InitTypeDef *pGPIO_Init)
{
  uint32_t tmp;
  uint32_t iocurrent;
  uint32_t position = 0U;

  /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
  tmp = GPIOC->MODER; //
  tmp &= ~(GPIO_MODER_MODE0 << (B1_PIN * GPIO_MODER_MODE1_Pos));
  tmp |= ((GPIO_MODE_IT_RISING & GPIO_MODE) << (B1_PIN * GPIO_MODER_MODE1_Pos));
  GPIOC->MODER = tmp;

  /* Activate the Pull-up or Pull down resistor for the current IO */
  tmp = GPIOC->PUPDR;
  tmp &= ~(GPIO_PUPDR_PUPD0 << (B1_PIN * GPIO_PUPDR_PUPD1_Pos));
  tmp |= (GPIO_PULLDOWN << (B1_PIN * GPIO_PUPDR_PUPD1_Pos));
  GPIOC->PUPDR = tmp;

  /*--------------------- EXTI Mode Configuration ------------------------*/
  tmp = EXTI->EXTICR[position >> 2U];
  tmp &= ~((0x0FUL) << ((B1_PIN & 0x03U) * EXTI_EXTICR1_EXTI1_Pos));
  tmp |= (0x2 << ((B1_PIN & 0x03U) * EXTI_EXTICR1_EXTI1_Pos));
  EXTI->EXTICR[B1_PIN >> 2U] = tmp;

  /* Clear Rising Falling edge configuration */
  tmp = EXTI->RTSR1;
  tmp &= ~((uint32_t)(1U << B1_PIN));
  tmp |= (1U << B1_PIN);
  EXTI->RTSR1 = tmp;

  tmp = EXTI->FTSR1;
  tmp &= ~((uint32_t)(1U << B1_PIN));
  EXTI->FTSR1 = tmp;

  /* Clear EXTI line configuration */
  tmp = EXTI->EMR1;
  tmp &= ~((uint32_t)(1U << B1_PIN));
  EXTI->EMR1 = tmp;

  tmp = EXTI->IMR1;
  tmp &= ~((uint32_t)(1U << B1_PIN));
  tmp |= (1U << B1_PIN);
  EXTI->IMR1 = tmp;
}




