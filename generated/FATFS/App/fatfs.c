/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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

#include <stdio.h>
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
static uint32_t* ffBuff;
static uint8_t isFileSystemInitialized;
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  TCHAR userPath[4];
  MX_FATFS_ConvertToUnicode(userPath, USERPath, 4);

  if(0 == retUSER) {
  // Initialize file system in flash
    ffBuff = (uint32_t*)malloc(_MIN_SS);
    FRESULT res = f_mount(&USERFatFS, userPath,	1);
    
    if(FR_OK == res) 
      isFileSystemInitialized = 1;
    else {
      res = f_mkfs(userPath, FM_ANY, 4*_MIN_SS, (void*)ffBuff, _MIN_SS);
      if(FR_OK == res){
        res = f_mount(&USERFatFS, userPath,	1);
        if(FR_OK == res)
          isFileSystemInitialized = 1;
      }
    }
  }
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
void MX_FATFS_Deinit(void) {
  FATFS_UnLinkDriver(USERPath);
  free(ffBuff);
}

uint8_t MX_FATFS_IsFileSystemInitialized(void) {
  return isFileSystemInitialized;
}

void MX_FATFS_ConvertToUnicode(WCHAR* dest, const char* src, size_t len) {
  for(uint8_t i = 0; i < len; i++){
    dest[i] = ff_convert (src[i], 1);
  }
  dest[len] = 0;
}
/* USER CODE END Application */
