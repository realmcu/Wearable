#ifndef _BB_ROM_DEFINE_H_
#define _BB_ROM_DEFINE_H_

#define _IS_BB_SERIES_                                  1
#define _IS_ASIC_
#define ROM_VERION_STRING                               "BB2Plus Boot ROM: 0x%08x%08x%08x%08x"
#define CHIP_ID_IN_ROM                                  54 // Need Check By Henry
#define _ROM_VER_                                       0
#define RTL_NUM1                                        0x8773 // Need Check By Henry
#define RTL_NUM2                                        0xe // Need Check By Henry
#define SECURE_BOOT_ENABLE                              1
#define SIMPLE_IMG_VERIFICATION                         (SECURE_BOOT_ENABLE == 0)
#define MAX_PAYLOAD_SIZE                                0x0800000 //8MB
#define OCCD_ADDRESS                                    0x2002000
#define OCCD_PAYLOAD_SIZE                               (4 * 1024)
#define BOOTPATCH_BANK0_ADDRESS                         0x2004000
#define BOOTPATCH_BANK1_ADDRESS                         0x2007000
#define BOOTPATCH_NOR_DEFAULT_SIZE                      0x3000
#define BOOTPATCH_NAND_START_ADDRESS                    0x2000000
#define BOOTPATCH_NAND_DEFAULT_SIZE                     0x5000
#define TOTAL_ROM_SIZE                                  704512 //(688 * 1024), no caculation for script usage
#define BOOT_ROM_ADDRESS                                0x0
#define BOOT_ROM_SIZE                                   40960 //(40 * 1024), no caculation for script usage
#define SYS_ROM_ADDRESS                                 (BOOT_ROM_ADDRESS + BOOT_ROM_SIZE)
#define SYS_ROM_SIZE                                    101376 //(99 * 1024), no caculation for script usage
#define STACK_ROM_ADDRESS                               (SYS_ROM_ADDRESS + SYS_ROM_SIZE)
#define STACK_ROM_SIZE                                  (TOTAL_ROM_SIZE - SYS_ROM_SIZE - BOOT_ROM_SIZE)
#define OUTPUT_PLATROM_NAME                             "platform"
#define OUTPUT_LOWERSTACK_NAME                          "lowerstack"
#define OUTPUT_UPPERSTACK_NAME                          "upperstack"

#ifdef _IS_ASIC_
#define CPU_CLOCK                                       (40000000)
#define USE_EXTERNAL_RF                                 0
#else
#define CPU_CLOCK                                       (20000000)
#define USE_EXTERNAL_RF                                 1
#endif

/*--------------------------------------------*/
/* fw sim compiler flags                      */
/*--------------------------------------------*/
// #define FOR_SIMULATION
#define FAKE_UART_ADDRESS                               0x34CFFC
// #define _DLPS_SIMU_TEST_
#endif /* !_BB_ROM_DEFINE_H_ */

