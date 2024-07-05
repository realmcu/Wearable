#ifndef ADDRESS_MAP_H
#define ADDRESS_MAP_H

/* ================================================================================ */
/* ================                  Address Map                   ================ */
/* ======== Reference: BB2Ultra_AddrMap_IRQs_DmaPorts_20240627_v0.xlsx   ============ */
/* ================================================================================ */

#define CM33_ROM_BASE            0x0
#define CM33_RAM_BASE            0x100000
#define ICACHE_MEM_BASE          0x2000000
#define CM33_MEM_BASE            0x20000000
#define DATA_SRAM0_BASE          0x20200000
#define DATA_SRAM1_BASE          0x20220000
#define OTP_MEM_BASE             0x20340000
#define BUFFER_SRAM_BASE         0x20400000
#define DSP_SRAM_BASE            0x20500000
#define SPIC0_MEM_BASE           0x4000000
#define SPIC1_MEM_BASE           0x22000000
#define SPIC2_MEM_BASE           0x60000000
#define NAND_LOGICAL_MEM_BASE    0x80000000
#define SYSON_BASE               0x40000000
#define GPIO0_BASE               0x40001000
#define TIMER0_BASE              0x40002000
#define QDEC_BASE                0x40004000
#define GPIO1_BASE               0x40004800
#define PPE_CFG_BASE             0x40005000
#define SOC_VENDOR_REG_BASE      0x40006000
#define ARM_AHB_CACHE_BASE       0x40007000
#define MODEMRFCPI_BASE          0x40008000
#define AUX_ADC_BASE             0x40010000
#define UART0_BASE               0x40011000
#define UART1_BASE               0x40012000
#define UART2_BASE               0x40012400
#define UART3_BASE               0x40012800
#define UART4_BASE               0x40012c00
#define SPI0_BASE                0x40013000
#define AES_ENGINE_BASE          0x40014000
#define I2C0_BASE                0x40015000
#define I2C1_BASE                0x40015400
#define I2C2_BASE                0x40015800
#define SPI_SLAVE_BASE           0x40015c00
#define SPI2_BASE                0x40016000
#define ISO7816_BASE             0x40016400
#define IR_BASE                  0x40016800
#define SPIC_PHY_BASE            0x40016c00
#define DISPLAY_CTRL_BASE        0x40017000
#define LALU_HMAC_BASE           0x40018000
#define SPI1_JDI_BASE            0x40019000
#define ICACHE_CFG_BASE          0x4001a000
#define PREFETCH_CTRL_BASE       0x4001b000
#define FLASH_SEC_BASE           0x4001c000
#define I2S0_BASE                0x40020000
#define I2S1_BASE                0x40021000
#define I2S2_BASE                0x40022000
#define H2D_D2H_REG_BASE         0x40023000
#define UART5_BASE               0x40024000
#define TIMER1_BASE              0x40025000
#define RXI_300_BASE             0x40026000
#define RXI350_DMA_CFG_BASE      0x40027000
#define CAN0_BASE                0x40028000
#define CAN1_BASE                0x40029000
#define MJPEG_CFG_BASE           0x4002a000
#define TIMER2_BASE              0x4002b000
#define SPI_CODEC_BASE           0x4002c000
#define BLUEWIZ_BASE             0x40050000
#define ZIGBEE_BASE              0x40054000
#define PROPRIETARY_2P4G_BASE    0x40056000
#define BT_VENDOR_REG_BASE       0x40058000
#define SDIO_HOST0_CFG_BASE      0x40060000
#define USB_OTG_CFG_BASE         0x40100000
#define SPI1_HS_BASE             0x40070000
#define SPIC0_BASE               0x40080000
#define SPIC1_BASE               0x40084000
#define SPIC2_BASE               0x40088000
#define IDU_BASE                 0x40090000
#define ETHERNET_CFG_BASE        0x400a0000
#define LALU_PKE_BASE            0x400c0000


#endif //#define ADDRESS_MAP_H 
