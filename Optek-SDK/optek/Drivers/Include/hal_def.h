#ifndef __HAL_DEF_H__
#define __HAL_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
  * @brief  HAL Status structures definition
  */
typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;



#ifdef __cplusplus
}
#endif

#endif // __HAL_DEF_H__