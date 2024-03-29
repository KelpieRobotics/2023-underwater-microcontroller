/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PI_TX_Pin GPIO_PIN_0
#define PI_TX_GPIO_Port GPIOA
#define PRESSURE_ADC_Pin GPIO_PIN_1
#define PRESSURE_ADC_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_2
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_3
#define DEBUG_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define PWM_Servo1_Pin GPIO_PIN_6
#define PWM_Servo1_GPIO_Port GPIOA
#define PWM_Servo2_Pin GPIO_PIN_7
#define PWM_Servo2_GPIO_Port GPIOA
#define PWM_Servo3_Pin GPIO_PIN_0
#define PWM_Servo3_GPIO_Port GPIOB
#define PWM_LightModule_Pin GPIO_PIN_1
#define PWM_LightModule_GPIO_Port GPIOB
#define IMU_SCL_Pin GPIO_PIN_10
#define IMU_SCL_GPIO_Port GPIOB
#define LEAK_EXTI_Pin GPIO_PIN_13
#define LEAK_EXTI_GPIO_Port GPIOB
#define LEAK_EXTI_EXTI_IRQn EXTI15_10_IRQn
#define THRUSTER_5_Pin GPIO_PIN_6
#define THRUSTER_5_GPIO_Port GPIOC
#define THRUSTER_6_Pin GPIO_PIN_7
#define THRUSTER_6_GPIO_Port GPIOC
#define THRUSTER_7_Pin GPIO_PIN_8
#define THRUSTER_7_GPIO_Port GPIOC
#define THRUSTER_8_Pin GPIO_PIN_9
#define THRUSTER_8_GPIO_Port GPIOC
#define THRUSTER_1_Pin GPIO_PIN_8
#define THRUSTER_1_GPIO_Port GPIOA
#define THRUSTER_2_Pin GPIO_PIN_9
#define THRUSTER_2_GPIO_Port GPIOA
#define THRUSTER_3_Pin GPIO_PIN_10
#define THRUSTER_3_GPIO_Port GPIOA
#define THRUSTER_4_Pin GPIO_PIN_11
#define THRUSTER_4_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define PI_RX_Pin GPIO_PIN_11
#define PI_RX_GPIO_Port GPIOC
#define IMU_SDA_Pin GPIO_PIN_12
#define IMU_SDA_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define SENSOR_SCL_Pin GPIO_PIN_6
#define SENSOR_SCL_GPIO_Port GPIOB
#define SENSOR_SDA_Pin GPIO_PIN_7
#define SENSOR_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
