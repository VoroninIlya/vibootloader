/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v1.0_Cube
  * @brief          : Memory management layer.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "vistm32flash.h"
#include "viflashdrv.h"
#include "ffconf.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

//#define STORAGE_LUN_NBR                  1
//#define STORAGE_BLK_NBR                  0x10000
//#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_HS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_HS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_HS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceHS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_HS(uint8_t lun);
static int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_HS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_HS(uint8_t lun);
static int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_HS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_HS =
{
  STORAGE_Init_HS,
  STORAGE_GetCapacity_HS,
  STORAGE_IsReady_HS,
  STORAGE_IsWriteProtected_HS,
  STORAGE_Read_HS,
  STORAGE_Write_HS,
  STORAGE_GetMaxLun_HS,
  (int8_t *)STORAGE_Inquirydata_HS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the storage unit (medium).
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_HS(uint8_t lun)
{
  /* USER CODE BEGIN 9 */
  if(0 == lun && 
    VIFLASH_InitDriver((VIFLASH_Program_t) HAL_FLASH_Program,
      (VIFLASH_Unlock_t) HAL_FLASH_Unlock, (VIFLASH_Lock_t) HAL_FLASH_Lock,
      (VIFLASH_EraseSector_t)HAL_FLASHEx_Erase, (VIFLASH_SectorToAddress_t)VIFLASH_SectorToAddress,
      (VIFLASH_AddressToSector_t)VIFLASH_AddressToSector, (VIFLASH_SectorSize_t)VIFLASH_SectorSize,
      VIFLASH_SectorToAddress(FLASH_SECTOR_6), VIFLASH_STOP_ADDRESS, _MIN_SS)){
      VIFLASH_SetPrintfCb(printf);
      VIFLASH_SetDebugLvl(VIFLASH_DEBUG_INFO);
      return (USBD_OK);
    }
  return (USBD_FAIL);
  /* USER CODE END 9 */
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number.
  * @param  block_num: Number of total block number.
  * @param  block_size: Block size.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_HS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 10 */
  if(0 == lun)
  {
    if(VIFLASH_RESULT_OK != VIFLASH_Ioctl (VIFLASH_GET_SECTOR_COUNT, (void*)block_num))
      return (USBD_FAIL);
    if(VIFLASH_RESULT_OK != VIFLASH_Ioctl (VIFLASH_GET_SECTOR_SIZE, (void*)block_size))
      return (USBD_FAIL);
    return (USBD_OK);
  }
  return (USBD_FAIL);
  /* USER CODE END 10 */
}

/**
  * @brief   Checks whether the medium is ready.
  * @param  lun:  Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_HS(uint8_t lun)
{
  /* USER CODE BEGIN 11 */
  UNUSED(lun);

  return (USBD_OK);
  /* USER CODE END 11 */
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_HS(uint8_t lun)
{
  /* USER CODE BEGIN 12 */
  if(VIFLASH_IsWriteProtected())
    return (USBD_BUSY);
  return (USBD_OK);
  /* USER CODE END 12 */
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 13 */
  if(0 == lun)
  {
    __disable_irq();
    if(VIFLASH_RESULT_OK != VIFLASH_Read(buf, blk_addr, blk_len)) {
      __enable_irq();
      return (USBD_EMEM);
    }
    __enable_irq();
    return (USBD_OK);
  }
  return (USBD_FAIL);
  /* USER CODE END 13 */
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_HS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 14 */
  if(0 == lun)
  {
    __disable_irq();
    if(VIFLASH_RESULT_OK != VIFLASH_Write(buf, blk_addr, blk_len)) {
      __enable_irq();
      return (USBD_EMEM);
    }
    __enable_irq();
    return (USBD_OK);
  }
  return (USBD_FAIL);
  /* USER CODE END 14 */
}

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number.
  */
int8_t STORAGE_GetMaxLun_HS(void)
{
  /* USER CODE BEGIN 15 */
  return 1; //(STORAGE_LUN_NBR - 1);
  /* USER CODE END 15 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

