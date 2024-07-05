
/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     pin_def.h
* \brief    The header file of all pin define.
* \details  This file provides all pin define.
* \author
* \date
* \version
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef PIN_DEF_H
#define PIN_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup SOC_PIN_DEFINE       PIN_DEFINE
  * \brief
  * \{
  */

/*============================================================================*
 *                         Pin Number
 *============================================================================*/
/** \defgroup Pin_Number       Pin Number
  * \brief
  * \{
  */

#define ADC_0              0         /*!< GPIOA0 */
#define ADC_1              1         /*!< GPIOA1 */
#define ADC_2              2         /*!< GPIOA2 */
#define ADC_3              3         /*!< GPIOA3 */
#define ADC_4              4         /*!< GPIOB21 */
#define ADC_5              5         /*!< GPIOB22 */
#define ADC_6              6         /*!< GPIOB23 */
#define ADC_7              7         /*!< GPIOB24 */
#define P1_0               8         /*!< GPIOA7 */
#define P1_1               9         /*!< GPIOA8 */
#define P1_2               10        /*!< GPIOA9 */
#define P1_3               11        /*!< GPIOA10 */
#define P1_4               12        /*!< GPIOA11 */
#define P1_5               13        /*!< GPIOA12 */
#define P2_0               14        /*!< GPIOA15 */
#define P2_1               15        /*!< GPIOA16 */
#define P2_2               16        /*!< GPIOA17 */
#define P2_3               17        /*!< GPIOA18 */
#define P2_4               18        /*!< GPIOA19 */
#define P2_5               19        /*!< GPIOA20 */
#define P2_6               20        /*!< GPIOA21 */
#define P2_7               21        /*!< GPIOA22 */
#define P3_0               22        /*!< GPIOA23 */
#define P3_1               23        /*!< GPIOA24 */
#define P3_2               24        /*!< GPIOA4 */
#define P3_3               25        /*!< GPIOA5 */
#define P3_4               26        /*!< GPIOA6 */
#define P3_5               27        /*!< GPIOB25 */
#define P4_0               28        /*!< GPIOB7 */
#define P4_1               29        /*!< GPIOB8 */
#define P4_2               30        /*!< GPIOB9 */
#define P4_3               31        /*!< GPIOB10 */
#define P4_4               32        /*!< GPIOB11 */
#define P4_5               33        /*!< GPIOB12 */
#define P4_6               34        /*!< GPIOB13 */
#define P4_7               35        /*!< GPIOB14 */
#define P5_0               36        /*!< GPIOB15 */
#define P5_1               37        /*!< GPIOB16 */
#define P5_2               38        /*!< GPIOB17 */
#define P5_3               39        /*!< GPIOB18 */
#define P5_4               40        /*!< GPIOA13 */
#define P5_5               41        /*!< GPIOA14 */
#define P5_6               42        /*!< GPIOA15 */
#define P6_0               43        /*!< GPIOA25 */
#define P6_1               44        /*!< GPIOA26 */
#define P6_2               45        /*!< GPIOA27 */
#define P6_3               46        /*!< GPIOA28 */
#define P6_4               47        /*!< GPIOA29 */
#define P6_5               48        /*!< GPIOA30 */
#define P6_6               49        /*!< GPIOA31 */
#define P7_0               50        /*!< GPIOB15 */
#define P7_1               51        /*!< GPIOB16 */
#define P7_2               52        /*!< GPIOB17 */
#define P7_3               53        /*!< GPIOB18 */
#define P7_4               54        /*!< GPIOB19 */
#define P7_5               55        /*!< GPIOB20 */
#define P7_6               56        /*!< GPIOB21 */
#define P7_7               57        /*!< GPIOB22 */
#define SPIC1_SIO2         58        /*!< GPIOB23 */
#define SPIC1_SIO1         59        /*!< GPIOB24 */
#define SPIC1_CSN          60        /*!< GPIOB25 */
#define SPIC1_SIO0         61        /*!< GPIOB26 */
#define SPIC1_SCK          62        /*!< GPIOB27 */
#define SPIC1_SIO3         63        /*!< GPIOB28 */
#define P8_0               64        /*!< GPIOB26 */
#define P8_1               65        /*!< GPIOB27 */
#define P8_2               66        /*!< GPIOB28 */
#define P8_3               67        /*!< GPIOB29 */
#define P8_4               68        /*!< GPIOB30 */
#define P8_5               69        /*!< GPIOB31 */
#define P8_6               70        /*!< GPIOB19 */
#define P8_7               71        /*!< GPIOB20 */
#define MIC1_P             72        /*!< GPIOA25 */
#define MIC1_N             73        /*!< GPIOA26 */
#define MIC2_P             74        /*!< GPIOA27 */
#define MIC2_N             75        /*!< GPIOA28 */
#define MICBIAS            76        /*!< GPIOA29 */
#define DAOUT1_P           77        /*!< GPIOA30 */
#define DAOUT1_N           78        /*!< GPIOA31 */
#define DAOUT2_P           79        /*!< GPIOB13 */
#define DAOUT2_N           80        /*!< GPIOB14 */
#define P9_0               81        /*!< GPIOB0 */
#define P9_1               82        /*!< GPIOB1 */
#define P9_2               83        /*!< GPIOB2 */
#define P9_3               84        /*!< GPIOB3 */
#define P9_4               85        /*!< GPIOB4 */
#define P9_5               86        /*!< GPIOB5 */
#define P9_6               87        /*!< GPIOB6 */
#define P10_0              88        /*!< GPIOB29 */
#define P10_1              89        /*!< GPIOB30 */
#define P10_2              90        /*!< GPIOB31 */
#define P10_3              91        /*!< GPIOA10 */
#define P10_4              92        /*!< GPIOA11 */
#define P10_5              93        /*!< GPIOA12 */
#define P10_6              94        /*!< GPIOA13 */
#define P10_7              95        /*!< GPIOA14 */
#define DP                 96        /*!< GPIOA16 */
#define DM                 97        /*!< GPIOA17 */

#define TOTAL_PIN_NUM         (98)
#define IS_PIN_NUMBER(NUMBER) ((NUMBER) < TOTAL_PIN_NUM)

/** End of Pin_Number
  * \}
  */

/** End of PIN DEFINE
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* PIN_DEF_H */

/******************* Reference: BB2Ultra_pin_mux_20240628_v0.xlsx *******************/
