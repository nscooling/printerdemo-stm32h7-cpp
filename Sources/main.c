/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "stm32h735g_discovery.h"
#include "stm32h735g_discovery_conf.h"
#include "stm32h735g_discovery_lcd.h"
#include "stm32h735g_discovery_ts.h"
#include "stm32h7xx_hal.h"
#include <stdint.h>

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 520000000 (CPU Clock)
 *            HCLK(Hz)                       = 260000000 (AXI and AHBs Clock)
 *            AHB Prescaler                  = 2
 *            D1 APB3 Prescaler              = 2 (APB3 Clock  130MHz)
 *            D2 APB1 Prescaler              = 2 (APB1 Clock  130MHz)
 *            D2 APB2 Prescaler              = 2 (APB2 Clock  130MHz)
 *            D3 APB4 Prescaler              = 2 (APB4 Clock  130MHz)
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 5
 *            PLL_N                          = 104
 *            PLL_P                          = 1
 *            PLL_Q                          = 4
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Flash Latency(WS)              = 3
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void) {
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device
     is clocked below the maximum system frequency, to update the voltage
     scaling value regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
  }

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 104;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK) {
    while (1) {
    };
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType =
      (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 |
       RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
  if (ret != HAL_OK) {
    while (1) {
    };
  }
}

/**
 * @brief  Configure the MPU attributes as Write Through for External HYPERRAM.
 * @note   The Base Address is 0x70000000 .
 *         The Configured Region Size is 16MB.
 * @param  None
 * @retval None
 */
static void MPU_Config(void) {
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU as Strongly ordered for not defined regions */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x00;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Configure the MPU attributes as WT for OctoSPI RAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x70000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

uint32_t InitTouchScreen(void) {
  uint32_t ret = 0;
  uint32_t x_size, y_size;

  BSP_LCD_GetXSize(0, &x_size);
  BSP_LCD_GetYSize(0, &y_size);

  TS_Init_t hTS;
  hTS.Width = x_size;
  hTS.Height = y_size;
  hTS.Orientation = TS_SWAP_XY;
  hTS.Accuracy = 0;
  /* Touchscreen initialization */
  if (BSP_TS_Init(0, &hTS) != BSP_ERROR_NONE) {
    ret = 1;
  }

  return ret;
}

void run_printer(void);

void init_slint(unsigned int lcd_layer_0_address,
                unsigned int lcd_layer_1_address, unsigned int width,
                unsigned int height);

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void) {
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

int main(void) {

  uint32_t lcd_width, lcd_height, i, j;

  /* Configure the MPU attributes as Write Through for external HYPERRAM*/
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  /* STM32H7xx HAL library initialization:
      - Systick timer is configured by default as source of time base, but user
        can eventually implement his proper time base source (a general purpose
        timer for example or other time source), keeping in mind that Time base
        duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
        handled in milliseconds basis.
      - Set NVIC Group Priority to 4
      - Low Level Initialization
    */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure LED1 and LED2 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);

  /*##-1- LCD Configuration ##################################################*/
  BSP_LCD_InitEx(0, LCD_ORIENTATION_LANDSCAPE, LCD_PIXEL_FORMAT_RGB565,
                 LCD_DEFAULT_WIDTH, LCD_DEFAULT_HEIGHT);

  /* Get the LCD width and height */
  BSP_LCD_GetXSize(0, &lcd_width);
  BSP_LCD_GetYSize(0, &lcd_height);

  BSP_LCD_DisplayOn(0);
  BSP_LCD_SetActiveLayer(0, 0);

  /* Clear the LCD */
  for (i = 0; i < lcd_height; i++) {
    for (j = 0; j < lcd_width; j++) {
      *((unsigned short *)(LCD_LAYER_1_ADDRESS) + i * lcd_width + j) =
          LCD_COLOR_RGB565_YELLOW;
    }
  }

  InitTouchScreen();

  init_slint(LCD_LAYER_0_ADDRESS, LCD_LAYER_1_ADDRESS, LCD_DEFAULT_WIDTH,
             LCD_DEFAULT_HEIGHT);
  BSP_LED_On(LED1);
  run_printer();
  BSP_LED_On(LED2);

  ///*##-2- DMA2D configuration
  /// ################################################*/
  // LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_DMA2D);
  // NVIC_SetPriority(DMA2D_IRQn, 0);
  // NVIC_EnableIRQ(DMA2D_IRQn);
  //
  // DMA2D_Config();
  //
  ///*##-3- Start DMA2D transfer
  /// ###############################################*/
  // LL_DMA2D_FGND_SetMemAddr(
  //     DMA2D, (uint32_t)&RGB565_240x160); /* Source buffer in format RGB565
  //     and
  //                                           size 240x160                 */
  // LL_DMA2D_SetOutputMemAddr(
  //     DMA2D, (uint32_t)LCD_LAYER_0_ADDRESS); /* LCD data address */
  //
  // LL_DMA2D_ConfigSize(DMA2D, LAYER_SIZE_Y,
  //                    LAYER_SIZE_X); /* Configure DMA2D transfer number of
  //                    lines
  //                                      and number of pixels per line */
  // LL_DMA2D_SetLineOffset(
  //    DMA2D,
  //    lcd_width - LAYER_SIZE_X); /* Configure DMA2D output line offset to LCD
  //                                  width - image width for display */
  //
  ///* Enable the transfer complete, transfer error and configuration error
  // * interrupts */
  // LL_DMA2D_EnableIT_TC(DMA2D);
  // LL_DMA2D_EnableIT_TE(DMA2D);
  // LL_DMA2D_EnableIT_CE(DMA2D);
  //
  ///* Enable the Peripheral */
  // LL_DMA2D_Start(DMA2D);
  //
  ///*##-4- Wait until DMA2D transfer is over
  // * ################################################*/
  // while ((blended_image_ready == 0) && (dma2d_error_status == 0)) {
  //  ;
  //}
  //
  // if (dma2d_error_status != 0) {
  //  /* Call Error function */
  //  OnError_Handler(1);
  //} else {
  //  /* Turn LED1 On */
  //  BSP_LED_On(LED1);
  //}

  while (1) {
    ;
  }
}

void SysTick_Handler(void) { HAL_IncTick(); }
