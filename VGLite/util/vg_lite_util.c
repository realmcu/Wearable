#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "vg_lite_util.h"
#include "lodepng.h"


static int read_int(FILE *fp)
{
    unsigned char b0, b1; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);

    return ((int)b0 << 8 | b1);
}

// Read a 32-bit signed integer.
static int read_long(FILE *fp)
{
    unsigned char b0, b1, b2, b3; /* Bytes from file */

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

// Write a 32-bit signed integer.
static int write_long(FILE *fp, int  l)
{
    putc(l, fp);
    putc(l >> 8, fp);
    putc(l >> 16, fp);
    return (putc(l >> 24, fp));
}
int vg_lite_load_png(vg_lite_buffer_t *buffer, const char *name)
{
    unsigned char *png_out = 0;
    unsigned int width;
    unsigned int height;

    if (lodepng_decode32_file(&png_out, &width, &height, name) != 0)
    {
        //rt_kprintf("PNG Decode Fail = %d\n", __LINE__);
        //RT_ASSERT(png_out != NULL);
    }
    else
    {
        //rt_kprintf("PNG Load Success width= %d, height = %d\n", width, height);
    }
    buffer->width = width;
    buffer->height = height;
    buffer->address = (uint32_t)png_out;
    buffer->memory = png_out;
    buffer->format = VG_LITE_RGBA8888;
    buffer->stride = width * 4;
    buffer->tiled = VG_LITE_LINEAR;
    //rt_kprintf("first png_out = %d\n", png_out);
//    rt_realloc(png_out, 64 + width * height * 4);
//    rt_kprintf("new after realloc png_out = %d\n", png_out);

//    if ((uint32_t)png_out % 64 != 0)
//    {
//        rt_kprintf("!!!!!!!!warining!!!!!! png_out = %d\n", png_out);
//        rt_kprintf("new after move png_out = %d\n", png_out + 64 - (int)png_out % 64);
//        uint8_t *new_buf = png_out + 64 - (int)png_out % 64;
//        rt_memmove(new_buf, png_out, width * height * 4);
//        buffer->address = (void *)new_buf;
//        buffer->memory = new_buf;
//        return 0;
//    }
//    else
    {
        return 0;
    }

}
int vg_lite_save_png(const char *name, vg_lite_buffer_t *buffer)
{
    if (buffer->format <= VG_LITE_BGRX8888)
    {
        lodepng_encode32_file(name, buffer->memory, buffer->width, buffer->height);
    }
    else if (buffer->format <= VG_LITE_BGRA5551)
    {
        lodepng_encode24_file(name, buffer->memory, buffer->width, buffer->height);
    }
    return 0;
}
int vg_lite_yv12toyv24(vg_lite_buffer_t *buffer1, vg_lite_buffer_t *buffer2)
{
    int status = 0;
    int i = 0;
    int j = 0;
    int width = buffer2->width;
    int height = buffer2->height;
    buffer1->width = buffer2->width;
    buffer1->height = buffer2->height;
    buffer1->format = VG_LITE_YV24;

    if (buffer2->format != VG_LITE_YV12)
    {
        return -1;
    }

    vg_lite_allocate(buffer1);

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            *((uint8_t *)buffer1->memory + j * width + i) = *((uint8_t *)buffer2->memory + j * width + i);
        }
    }

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            *((uint8_t *)buffer1->yuv.uv_memory + j * width + i) = *((uint8_t *)buffer2->yuv.uv_memory +
                                                                     (j >> 1) * width / 2 + (i >> 1));
        }
    }

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            *((uint8_t *)buffer1->yuv.v_memory + j * width + i) = *((uint8_t *)buffer2->yuv.v_memory +
                                                                    (j >> 1) * width / 2 + (i >> 1));
        }
    }

    status = 0;
    return status;
}

int vg_lite_yv12toyv16(vg_lite_buffer_t *buffer1, vg_lite_buffer_t *buffer2)
{
    int status = 0;
    int i = 0;
    int j = 0;
    int width  = buffer2->width;
    int height = buffer2->height;
    buffer1->width = buffer2->width;
    buffer1->height = buffer2->height;
    buffer1->format = VG_LITE_YV16;

    if (buffer2->format != VG_LITE_YV12)
    {
        return -1;
    }

    vg_lite_allocate(buffer1);

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width; i++)
        {
            *((uint8_t *)buffer1->memory + j * width + i) = *((uint8_t *)buffer2->memory + j * width + i);
        }
    }

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width / 2; i++)
        {
            *((uint8_t *)buffer1->yuv.uv_memory + j * width / 2 + i) = *((uint8_t *)buffer2->yuv.uv_memory +
                                                                         (j >> 1) * width / 2 + i);
        }
    }

    for (j = 0; j < height; j++)
    {
        for (i = 0; i < width / 2; i++)
        {
            *((uint8_t *)buffer1->yuv.v_memory + j * width / 2 + i) = *((uint8_t *)buffer2->yuv.v_memory +
                                                                        (j >> 1) * width / 2 + i);
        }
    }

    status = 0;
    return status;
}

int vg_lite_load_raw_yuv(vg_lite_buffer_t *buffer, const char *name)
{
    FILE *fp;
    uint8_t *p = NULL;
    uint32_t pos_line;
    int status = 0;
    int i, j, k;
    int Y_height;
    int U_height;
    int V_height;
    int Y_width;
    int U_width;
    int V_width;
    int width = buffer->width;
    int height = buffer->height;

    if (buffer->width == 0 || buffer->height == 0)
    {
        return -1;
    }

    switch (buffer->format)
    {
    case VG_LITE_NV12:
        Y_height = height;
        U_height = height / 2;
        V_height = 0;
        Y_width = width;
        U_width = width;
        V_width = 0;
        break;

    case VG_LITE_YV12:
        Y_height = height;
        U_height = height / 2;
        V_height = height / 2;
        Y_width = width;
        U_width = width / 2;
        V_width = width / 2;
        break;

    case VG_LITE_NV16:
        Y_height = height;
        U_height = height;
        V_height = 0;
        Y_width = width;
        U_width = width;
        V_width = 0;
        break;
    default:
        return -1;
    }

    fp = fopen(name, "rb");
    if (fp != NULL)
    {
        int flag;
        if (vg_lite_allocate(buffer) != VG_LITE_SUCCESS)
        {
            fclose(fp);
            return -1;
        }

        p = (uint8_t *) buffer->memory;
        pos_line = ftell(fp);
        for (i = 0; i < Y_height; i++)
        {
            pos_line = ftell(fp);
            flag = fread(p, 1, Y_width, fp);
            if (flag != Y_width)
            {
                //rt_kprintf("failed to read raw buffer data\n");
                fclose(fp);
                return -1;
            }
            p += buffer->stride;
            fseek(fp, pos_line + Y_width, SEEK_SET);
        }

        p = (uint8_t *) buffer->yuv.uv_memory;
        pos_line = ftell(fp);
        for (j = 0; j < U_height; j++)
        {
            pos_line = ftell(fp);
            flag = fread(p, 1, U_width, fp);
            if (flag != U_width)
            {
                //rt_kprintf("failed to read raw buffer data\n");
                fclose(fp);
                return -1;
            }
            p += buffer->yuv.uv_stride;
            fseek(fp, pos_line + U_width, SEEK_SET);
        }

        p = (uint8_t *) buffer->yuv.v_memory;
        pos_line = ftell(fp);
        for (k = 0; k < V_height; k++)
        {
            pos_line = ftell(fp);
            flag = fread(p, 1, V_width, fp);
            if (flag != V_width)
            {
                //rt_kprintf("failed to read raw buffer data\n");
                fclose(fp);
                return -1;
            }
            p += buffer->yuv.v_stride;
            fseek(fp, pos_line + V_width, SEEK_SET);
        }

        fclose(fp);
        fp = NULL;
        status = 0;
        return status;
    }

    return -1;
}

int vg_lite_load_pkm_info_to_buffer(vg_lite_buffer_t *buffer, const char *name)
{
    FILE *fp;

    int status = 1;

    if (buffer == NULL)
    {
        return -1;
    }

    fp = fopen(name, "rb");
    if (fp != NULL)
    {
        read_long(fp);
        read_long(fp);
        buffer->width = read_int(fp);
        buffer->height = read_int(fp);
        buffer->format = VG_LITE_RGBA8888_ETC2_EAC;
        buffer->tiled = VG_LITE_TILED;
        fclose(fp);
        fp = NULL;
        status = 0;
    }
    return status;
}

int vg_lite_load_pkm(vg_lite_buffer_t *buffer, const char *name)
{
    FILE *fp;

    int status = 1;

    if (buffer == NULL)
    {
        return -1;
    }

    fp = fopen(name, "rb");
    if (fp != NULL)
    {
        int flag;

        fseek(fp, 16, SEEK_SET);
        flag = fread(buffer->memory, buffer->stride * buffer->height, 1, fp);
        if (flag != 1)
        {
            //rt_kprintf("failed to read pkm buffer data\n");
            fclose(fp);
            return -1;
        }

        fclose(fp);
        fp = NULL;
        status = 0;
    }

    return status;
}

int vg_lite_load_raw(vg_lite_buffer_t *buffer, const char *name)
{
    FILE *fp;

    // Set status.
    int status = 1;
    // Check the result with golden.
    fp = fopen(name, "rb");
    if (fp != NULL)
    {
        int flag;

        // Get width, height, stride and format info.
        buffer->width  = read_long(fp);
        buffer->height = read_long(fp);
        buffer->stride = read_long(fp);
        buffer->format = read_long(fp);
        // Allocate the VGLite buffer memory.
        if (vg_lite_allocate(buffer) != VG_LITE_SUCCESS)
        {
            fclose(fp);
            return -1;
        }

        fseek(fp, 16, SEEK_SET);
        flag = fread(buffer->memory, buffer->stride * buffer->height, 1, fp);
        if (flag != 1)
        {
            //rt_kprintf("failed to read raw buffer data\n");
            fclose(fp);
            return -1;
        }

        fclose(fp);
        fp = NULL;
        status = 0;
    }

    // Return the status.
    return status;
}

int vg_lite_load_raw_byline(vg_lite_buffer_t *buffer, const char *name)
{
    FILE *fp;

    // Set status.
    int status = 1;

    // Check the result with golden.
    fp = fopen(name, "r");
    if (fp != NULL)
    {
        int flag;

        // Get width, height, stride and format info.
        buffer->width  = read_long(fp);
        buffer->height = read_long(fp);
        buffer->stride = read_long(fp);
        buffer->format = read_long(fp);

        // Allocate the VGLite buffer memory.
        if (vg_lite_allocate(buffer) != VG_LITE_SUCCESS)
        {
            fclose(fp);
            return -1;
        }

        fseek(fp, 16, SEEK_SET);
        flag = fread(buffer->memory, buffer->width * buffer->height * (buffer->stride / buffer->width), 1,
                     fp);
        if (flag != 1)
        {
            //rt_kprintf("failed to read raw buffer data\n");
            fclose(fp);
            return -1;
        }

        fclose(fp);
        fp = NULL;
        status = 0;
    }

    // Return the status.
    return status;
}

int vg_lite_save_raw(const char *name, vg_lite_buffer_t *buffer)
{
    FILE *fp;
    int status = 1;

    fp = fopen(name, "wb");

    if (fp != NULL)
    {
        // Save width, height, stride and format info.
        write_long(fp, buffer->width);
        write_long(fp, buffer->height);
        write_long(fp, buffer->stride);
        write_long(fp, buffer->format);

        // Save buffer info.
        fwrite(buffer->memory, 1, buffer->stride * buffer->height, fp);

        fclose(fp);
        fp = NULL;

        status = 0;
    }

    // Return the status.
    return status;
}

int vg_lite_save_raw_byline(const char *name, vg_lite_buffer_t *buffer)
{
    FILE *fp;
    int status = 1;
    unsigned char *pt;
    int loop, mem;

    fp = fopen(name, "w");

    if (fp != NULL)
    {
        // Save width, height, stride and format info.
        write_long(fp, buffer->width);
        write_long(fp, buffer->height);
        write_long(fp, buffer->stride);
        write_long(fp, buffer->format);

        // Save buffer info.
        pt = (unsigned char *) buffer->memory;
        for (loop = 0; loop < buffer->height; loop++)
        {
            for (mem = 0; mem < buffer->width * (buffer->stride / buffer->width); mem++)
            {
                putc(pt[mem], fp);
            }
        }

        fclose(fp);
        fp = NULL;

        status = 0;
    }

    // Return the status.
    return status;
}


bool compare_file(const char *filename, vg_lite_buffer_t *new_gen)
{
    unsigned char *png_out = NULL;
    unsigned int width;
    unsigned int height;
    lodepng_decode32_file(&png_out, &width, &height, filename);

    //rt_kprintf("png_out = 0x%x; new_gen->memory = 0x%x\n", png_out, new_gen->memory);
    if (memcmp(png_out, new_gen->memory, new_gen->width * new_gen->height * 4) == 0)
    {
        //rt_kprintf("compare ok\n");
        free(png_out);
        return true;
    }
    else
    {
        free(png_out);
        //rt_kprintf("compare fail\n");
        return false;
    }
}