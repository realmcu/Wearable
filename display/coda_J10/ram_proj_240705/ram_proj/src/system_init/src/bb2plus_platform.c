#include "hwtest_config.h"
#include "rtl876x.h"

void HardFault_Handler(void)
{
    while (1);
}
#if 0
extern void GPIO0_Handler(void);
extern void GPIO1_Handler(void);
extern void GPIO2_Handler(void);
extern void GPIO3_Handler(void);
extern void GPIO4_Handler(void);
extern void GPIO5_Handler(void);
extern void GPIO6_Handler(void);
extern void GPIO7_Handler(void);
extern void GPIO8_Handler(void);
extern void GPIO9_Handler(void);
extern void GPIO10_Handler(void);
extern void GPIO11_Handler(void);
extern void GPIO12_Handler(void);
extern void GPIO13_Handler(void);
extern void GPIO14_Handler(void);
extern void GPIO15_Handler(void);
extern void GPIO16_Handler(void);
extern void GPIO17_Handler(void);
extern void GPIO18_Handler(void);
extern void GPIO19_Handler(void);
extern void GPIO20_Handler(void);
extern void GPIO21_Handler(void);
extern void GPIO22_Handler(void);
extern void GPIO23_Handler(void);
extern void GPIO24_Handler(void);
extern void GPIO25_Handler(void);
extern void GPIO26_Handler(void);
extern void GPIO27_Handler(void);
extern void GPIO28_Handler(void);
extern void GPIO29_Handler(void);
extern void GPIO30_Handler(void);
extern void GPIO31_Handler(void);

void Peripheral_Handler(void)
{
    IRQ_Fun pFun;
    uint32_t PeriIrqStatus, ExactIrqStatus;
    IRQ_Fun *Vectors = (IRQ_Fun *)SCB->VTOR;

    PeriIrqStatus = PERIPHINT->STATUS;
    //Save exact IRQ status
    ExactIrqStatus = PeriIrqStatus & (PERIPHINT->EN);

    for (uint32_t i = 1; i < 5; i ++)
    {
        if (ExactIrqStatus & BIT(i))
        {
            pFun = Vectors[GPIO2_VECTORn + i - 1];
            pFun();
        }
    }

    if (ExactIrqStatus & BIT13) // ADP DET
    {
        pFun = Vectors[ADP_DET_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT12) // VBAT DET
    {
        pFun = Vectors[VBAT_DET_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT19) // Flash Power Detect
    {
        pFun = Vectors[FlashPower_DET_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT11) // Low Power Comparator
    {
        pFun = Vectors[LPCOMP_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT18) // DSP Watchdog
    {
        pFun = Vectors[DSP_WDG_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT17) // PTA Mailbox
    {
        pFun = Vectors[PTA_Mailbox_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT5) // TIMER3
    {
        pFun = Vectors[Timer3_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT6) //TIMER4
    {
        pFun = Vectors[Timer4_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT7) //TIMER5
    {
        pFun = Vectors[Timer5_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT8) //TIMER6
    {
        pFun = Vectors[Timer6_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT9) //TIMER7
    {
        pFun = Vectors[Timer7_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT14) // HW ASRC1
    {
        pFun = Vectors[ASRC1_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT15) // HW ASRC2
    {
        pFun = Vectors[ASRC2_VECTORn];
        pFun();
    }

    //GPIO6To31_IRQ
    if (ExactIrqStatus & BIT16)
    {
        uint32_t gpio_int = GPIO->INTSTATUS;

        for (uint32_t i = 6; i < 32; i ++)
        {
            if (gpio_int & BIT(i))
            {
                pFun = Vectors[GPIO6_VECTORn + i - 6];
                pFun();
            }
        }
    }

    if (ExactIrqStatus & BIT25) // SPORT0 TX
    {
        pFun = Vectors[SPORT0_TX_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT26) // SPORT0 RX
    {
        pFun = Vectors[SPORT0_RX_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT27) // SPORT1 TX
    {
        pFun = Vectors[SPORT1_TX_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT28) // SPORT1 RX
    {
        pFun = Vectors[SPORT1_RX_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT29) // SPDIF
    {
        pFun = Vectors[ANC_AMP0_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT30) // ANC_AMP1
    {
        pFun = Vectors[ANC_AMP1_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT31) // RNG
    {
        pFun = Vectors[RNG_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT10) // MFB_DET_L
    {
        pFun = Vectors[MFB_DET_L_VECTORn];
        pFun();
    }
    if (ExactIrqStatus & BIT0) // SPI FLASH
    {
        pFun = Vectors[SPIFlash_VECTORn];
        pFun();
    }

    //Clear sub-rout IRQ
    HAL_WRITE32(PERI_INT_REG_BASE, 0, PeriIrqStatus);
}
#endif

uint8_t AON_Read(uint8_t offset)
{
    uint8_t read_data;
    uint32_t AON_REG = 0;
    AON_REG |= AON_ADDR(offset);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG);
    read_data = (uint8_t)IO_READ32(SYSON_ADDRESS_BASE, REG_AON_ENTRY);
    return read_data;
}

void AON_Write(uint8_t offset, uint8_t data)
{
    uint32_t AON_REG_1 = 0;
    uint32_t AON_REG_2 = 0;
    AON_REG_1 |= AON_ADDR(offset)
                 | AON_WDATA(data)
                 | AON_WSTROBE(1) ;
    AON_REG_2 |= AON_ADDR(offset)
                 | AON_WDATA(data);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_2);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_1);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_2);
}


uint8_t AON_Read_Safe(uint8_t offset)
{
    uint8_t read_data;
    uint32_t AON_REG = 0;
    uint32_t AON_REG_RESTORE;

    AON_REG_RESTORE = IO_READ32(SYSON_ADDRESS_BASE, REG_AON_ENTRY);

    AON_REG = 0;
    AON_REG |= AON_ADDR(offset);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG);
    read_data = (uint8_t)IO_READ32(SYSON_ADDRESS_BASE, REG_AON_ENTRY);

    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_RESTORE);
    return read_data;
}


void AON_Write_Safe(uint8_t offset, uint8_t data)
{
    uint32_t AON_REG_RESTORE;
    uint32_t AON_REG_1 = 0;
    uint32_t AON_REG_2 = 0;

    AON_REG_RESTORE = IO_READ32(SYSON_ADDRESS_BASE, REG_AON_ENTRY);

    AON_REG_1 |= AON_ADDR(offset)
                 | AON_WDATA(data)
                 | AON_WSTROBE(1) ;
    AON_REG_2 |= AON_ADDR(offset)
                 | AON_WDATA(data);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_1);
    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_2);

    IO_WRITE32(SYSON_ADDRESS_BASE, REG_AON_ENTRY, AON_REG_RESTORE);
}

