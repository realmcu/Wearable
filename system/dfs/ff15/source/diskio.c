/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"         /* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM     0   /* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC     1   /* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB     2   /* Example: Map USB MSD to physical drive 2 */

static int RAM_disk_status()
{
    return RES_OK;
}
static int MMC_disk_status()
{
    return RES_OK;
}
static int USB_disk_status()
{
    return RES_OK;
}
static int RAM_disk_initialize()
{
    return RES_OK;
}
static int MMC_disk_initialize()
{
    return RES_OK;
}
static int USB_disk_initialize()
{
    return RES_OK;
}
#include "drv_sdio.h"
static DRESULT RAM_disk_read(
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    return RES_OK;
}
static DRESULT MMC_disk_read(
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DBG_DIRECT("MMC_disk_read sector, count:%d,%d", sector, count);
    SdEmmcRes_t Res = Sd_Read(SDIO0, sector + 0, count, buff);
    DBG_DIRECT("MMC_disk_read:%d", Res);
    return RES_OK;
}
static DRESULT USB_disk_read(
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;


    return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;
    int result;


    result = MMC_disk_read(buff, sector, count);

    // translate the reslut code here

    return result;

}


DRESULT RAM_disk_write(
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    return RES_OK;
}
DRESULT MMC_disk_write(
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DBG_DIRECT("MMC_disk_write:,%d,%d", sector, count);
    SdEmmcRes_t Res = Sd_Write(SDIO0, sector + 0, count, buff);
    DBG_DIRECT("MMC_disk_write:%d", Res);
    return RES_OK;
}
DRESULT USB_disk_write(
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    return RES_OK;
}
/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{

    int result;
    result = MMC_disk_write(buff, sector, count);

    // translate the reslut code here


    return result;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{

    switch (cmd)             //fatfs????cmd??
    {
    case GET_SECTOR_COUNT:  //sector count, ??sect_cnt
        *(DWORD *)buff = 10000;
        return RES_OK;
    case GET_SECTOR_SIZE:   //sector size, ??block size(SD),??bytes
        *(DWORD *)buff = 512;
        return RES_OK;
    case GET_BLOCK_SIZE:    //block size, ?????,??SD2.0???8192,?? 1
        *(DWORD *)buff = 1; //??? sector(FatFs)
        return RES_OK;
    case CTRL_SYNC:         //????,??FatFs?????????????
        return RES_OK;
    }




    return 0;
}

