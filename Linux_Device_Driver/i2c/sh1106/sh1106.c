/***************************************************************************/ /**
*  \file       sh1106.c
*
*  \details    Simple I2C driver explanation (SH_1106 OLED Display Interface)
*
*
* *******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>

#include <linux/string.h>

#include "sh1106.h"
// #include "objects.h"

#define I2C_BUS_AVAILABLE (1)          // I2C Bus available in our Raspberry Pi
#define SLAVE_DEVICE_NAME ("ETX_OLED") // Device and Driver Name
#define SH1106_SLAVE_ADDR (0x3C)      // SH1106 OLED Slave Address

static struct i2c_adapter *etx_i2c_adapter = NULL;    // I2C Adapter Structure
static struct i2c_client  *etx_i2c_client_oled = NULL; // I2C Cient Structure (In our case it is OLED)

// SSH1106_t is a struct variable in sh1106.h
SSH1106_t display;

/*
** This function writes the data into the I2C client
**
**  Arguments:
**      buff -> buffer to be sent
**      len  -> Length of the data
**   
*/
static int I2C_Write(unsigned char *buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address with R/W bit, 
    ** ACK/NACK and Stop condtions will be handled internally.
    */


    /**
    * i2c_master_send - issue a single I2C message in master transmit mode
    * @client: Handle to slave device
    * @buf: Data that will be written to the slave
    * @count: How many bytes to write, must be less than 64k since msg.len is u16
    *
    * Returns negative errno, or else the number of bytes written.
    */

    int ret = i2c_master_send(etx_i2c_client_oled, buf, len); // from i2c-core.c

    return ret;
}


/*
** This function reads one byte of the data from the I2C client
**
**  Arguments:
**      out_buff -> buffer wherer the data to be copied
**      len      -> Length of the data to be read
** 
*/
static int I2C_Read(unsigned char *out_buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address with R/W bit, 
    ** ACK/NACK and Stop condtions will be handled internally.
    */

    /**
    * i2c_master_recv - issue a single I2C message in master receive mode
    * @client: Handle to slave device
    * @buf: Where to store data read from slave
    * @count: How many bytes to read, must be less than 64k since msg.len is u16
    *
    * Returns negative errno, or else the number of bytes read.
    */

    int ret = i2c_master_recv(etx_i2c_client_oled, out_buf, len); //from i2c-core.c

    return ret;
}


/*
** This function is specific to the SH_1106 OLED.
** This function sends the command/data to the OLED.
**
**  Arguments:
**      is_cmd -> true = command, flase = data
**      data   -> data to be written
** 
*/

// Mỗi lần gửi sẽ gửi 2 bytes, bytes 0 là control byte để xác định byte 1 là command hay data 
// byte 1 là dữ liệu cần gửi, tùy vào byte 0 là 0x00 hay 0x40 mà byte 1 sẽ là command hoặc data  

static void SH1106_Write(bool is_cmd, unsigned char data)
{
    unsigned char buf[2] = {0};
    int ret;

    /*
    ** First byte is always control byte. Data is followed after that.
    **
    ** There are two types of data in SH_1106 OLED.
    ** 1. Command
    ** 2. Data
    **
    ** Control byte decides that the next byte is, command or data.
    **
    ** -------------------------------------------------------                        
    ** |              Control byte's | 6th bit  |   7th bit  |
    ** |-----------------------------|----------|------------|    
    ** |   Command                   |   0      |     0      |
    ** |-----------------------------|----------|------------|
    ** |   data                      |   1      |     0      |
    ** |-----------------------------|----------|------------|
    ** 
    ** Please refer the datasheet for more information. 
    **    
    */

    if (is_cmd == true)
    {
        buf[0] = 0x00; // 8'b0000_0000 - command
    }
    else
    {
        buf[0] = 0x40; // 8'b0100_0000 - data
    }

    buf[1] = data;

    ret = I2C_Write(buf, 2); // Ghi 2 bytes từ master ra slave
}

// Ghi dữ liệu (data hoặc command) kèm theo 1 control byte ở đầu  
void WriteData(uint8_t *_buffer, size_t buff_size)
{
    uint8_t buf[150];
    buf[0] = 0x40;
    memcpy(buf + 1, _buffer, buff_size);
    I2C_Write(buf, buff_size + 1);
}


/*
** This function sends the commands that need to used to Initialize the OLED.
**
**  Arguments:
**      none
** 
*/
static int SH1106_DisplayInit(void)
{
    msleep(100); // delay

    /*
    ** Commands to initialize the SSD_1306 OLED Display
    */
    /*     SH1106_Write(true, 0xAE); // Entire Display OFF
    SH1106_Write(true, 0xD5); // Set Display Clock Divide Ratio and Oscillator Frequency
    SH1106_Write(true, 0x80); // Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended
    SH1106_Write(true, 0xA8); // Set Multiplex Ratio
    SH1106_Write(true, 0x3F); // 64 COM lines
    SH1106_Write(true, 0xD3); // Set display offset
    SH1106_Write(true, 0x00); // 0 offset
    SH1106_Write(true, 0x40); // Set first line as the start line of the display
    SH1106_Write(true, 0x8D); // Charge pump
    SH1106_Write(true, 0x14); // Enable charge dump during display on
    SH1106_Write(true, 0x20); // Set memory addressing mode
    SH1106_Write(true, 0x00); // Horizontal addressing mode
    SH1106_Write(true, 0xA1); // Set segment remap with column address 127 mapped to segment 0
    SH1106_Write(true, 0xC8); // Set com output scan direction, scan from com63 to com 0
    SH1106_Write(true, 0xDA); // Set com pins hardware configuration
    SH1106_Write(true, 0x12); // Alternative com pin configuration, disable com left/right remap
    SH1106_Write(true, 0x81); // Set contrast control
    SH1106_Write(true, 0x80); // Set Contrast to 128
    SH1106_Write(true, 0xD9); // Set pre-charge period
    SH1106_Write(true, 0xF1); // Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK
    SH1106_Write(true, 0xDB); // Set Vcomh deselect level
    SH1106_Write(true, 0x20); // Vcomh deselect level ~ 0.77 Vcc
    SH1106_Write(true, 0xA4); // Entire display ON, resume to RAM content display
    SH1106_Write(true, 0xA6); // Set Display in Normal Mode, 1 = ON, 0 = OFF
    SH1106_Write(true, 0x2E); // Deactivate scroll
    SH1106_Write(true, 0xAF); // Display ON in normal mode
        */

    SH1106_Write(true, DISPLAYOFF           ); // Entire Display OFF
    SH1106_Write(true, SETDISPLAYCLOCKDIV   ); // Set Display Clock Divide Ratio and Oscillator Frequency
    SH1106_Write(true, DEFAULTDISPLAYCLOCK  ); // Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended
    SH1106_Write(true, SETMULTIPLEX         ); // Set Multiplex Ratio
    SH1106_Write(true, _64COMLINE           ); // 64 COM lines
    SH1106_Write(true, SETDISPLAYOFFSET     ); // Set display offset
    SH1106_Write(true, _0OFFSET             ); // 0 offset
    SH1106_Write(true, SETSTARTLINE         ); // Set first line as the start line of the display
    SH1106_Write(true, CHARGEPUMP           ); // Charge pump
    SH1106_Write(true, ENABLE_CHARGE_PUMP   ); // Enable charge dump during display on
    SH1106_Write(true, MEMORYMODE           ); // Set memory addressing mode
    SH1106_Write(true, SETLOWCOLUMN         ); // Horizontal addressing mode
    SH1106_Write(true, SETSEGMENTREMAP      ); // Set segment remap with column address 127 mapped to segment 0
    SH1106_Write(true, COMSCANDEC           ); // Set com output scan direction, scan from com63 to com 0
    SH1106_Write(true, SETCOMPINS           ); // Set com pins hardware configuration
    SH1106_Write(true, 0x12                 ); // Alternative com pin configuration, disable com left/right remap
    SH1106_Write(true, SETCONTRAST          ); // Set contrast control
    SH1106_Write(true, DISPLAYON            ); // Set Contrast to 128
    SH1106_Write(true, SETPRECHARGE         ); // Set pre-charge period
    SH1106_Write(true, 0xF1                 ); // Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK
    SH1106_Write(true, SETVCOMDESELECT      ); // Set Vcomh deselect level
    SH1106_Write(true, SETSTARTLINE         ); // Vcomh deselect level ~ 0.77 Vcc
    SH1106_Write(true, DISPLAY_ALL_ON_RESUME); // Entire display ON, resume to RAM content display
    SH1106_Write(true, NORMALDISPLAY        ); // Set Display in Normal Mode, 1 = ON, 0 = OFF
    SH1106_Write(true, 0x2E                 ); // Deactivate scroll
    SH1106_Write(true, DISPLAYON            ); // Display ON in normal mode

    return 0;
}


static void clearScreen(void)
{
    uint16_t i = 0;

    for (i = 0; i < _BUFFER_SIZE; i++)
    {

        display.buffer[i] = 0x00;
    }
}


static void blackScreen(void)
{
    uint16_t i = 0;

    for (i = 0; i < _BUFFER_SIZE; i++)
    {

        display.buffer[i] = 0xFF;
    }
}


static void updateScreen(void)
{
    uint8_t i = 0;
    //uint8_t j = 0;
    for (i = 0; i < (_HEIGHT / 8); i++)
    {
        // We have 8 pages: 0 - 7 and 132 columns: 0 - 131
        // Each page has 8 rows
        // Set the page start address of the target display location by command B0h to B7h

        SH1106_Write(true, 0XB0 + i);
        SH1106_Write(true, 0X00); // Set low columns
        SH1106_Write(true, 0X10); // Set high columns

        // WriteData(&display.buffer [_WIDTH * i], _WIDTH);
        WriteData(&display.buffer[_WIDTH * i], _WIDTH);
    }
}


void moveCursor(int8_t x, int8_t y)
{
    display.CurX += x;
    display.CurY += y;
}


void setPosition(uint8_t x, uint8_t y)
{
    display.CurX = x + 2; // ????????????????
    display.CurY = y;
}


void drawPixel(uint8_t x, uint8_t y, Color_t color)
{
    //	Check x, y in screen
    if (x >= _WIDTH || y >= _HEIGHT)
    {
        //		printf("FAIL HERE\n");
        return;
    }
    if (color == white)
    {
        // set this pixel
        // x là cột, y là hàng -> y / 8 là số thứ tự của page
        // buffer là mảng lưu giá trị đen/trắng được trải phẳng 
        // x + y / 8 là vị trí của pixel tính theo page/column trong mảng buffer
        // y % 8 là vị trí của pixel (x, y) trong page
        // phép or dùng để set bit ở vị trí thứ y % 8 của page và giữ nguyên giá trị của các bit khác trong page 
        display.buffer[x + (y / 8) * _WIDTH] |= (1 << (y % 8));
    }
    else // black
    {
        // reset this pixel
        // x là cột, y là hàng -> y / 8 là số thứ tự của page
        // buffer là mảng lưu giá trị đen/trắng được trải phẳng 
        // x + y / 8 là vị trí của pixel tính theo page/column trong mảng buffer
        // y % 8 là vị trí của pixel (x, y) trong page
        // phép or dùng để clear bit ở vị trí thứ y % 8 của page và giữ nguyên giá trị của các bit khác trong page
        display.buffer[x + (y / 8) * _WIDTH] &= ~(1 << (y % 8));
    }
}


char putChar(char _char, Objects_t *Font, Color_t color)
{
    uint8_t hor_cnt = 0; //Horizon current cursor - hàng ngang
    uint8_t ver_cnt = 0; //Vertical current cursor - hàng dọc
    uint16_t temp_point = 0;

    if ((display.CurX + Font->oWidth) >= _WIDTH || (display.CurY + Font->oHeight) >= _HEIGHT)
    {
        return 0;
    }

    for (ver_cnt = 0; ver_cnt < Font->oHeight; ver_cnt++)
    {
        temp_point = Font->data[(_char - 32) * Font->oHeight + ver_cnt];
        for (hor_cnt = 0; hor_cnt < Font->oWidth; hor_cnt++)
        {
            if ((temp_point << hor_cnt) & 0x8000)
            //			if ((temp_point << hor_cnt) & 0x80)
            {
                drawPixel(display.CurX + hor_cnt, display.CurY + ver_cnt, color);
            }
            else
            {
                drawPixel(display.CurX + hor_cnt, display.CurY + ver_cnt, !color);
            }
        }
    }
    display.CurX += Font->oWidth;
    return _char;
}


void putString(char *_string, Objects_t *Font, Color_t _color)
{
    uint8_t cnt = 0; // current

    for (cnt = 0; cnt < strlen(_string); cnt++)
    {
        putChar(*(_string + cnt), Font, _color);
    }
}

// ????? Vẽ hình ???????
void drawBitmap(Objects_t *Obj, Color_t _color)
{
    uint8_t byteWidth = (Obj->oWidth + 7) / 8; //ByteWidth is number of byte will obtain by object
    uint8_t byteCursor = 0;                    //Cursor byte point to object array
    uint8_t ver_pixel_counter = 0;             //Vertical to pixel by pixel of object
    uint8_t hor_pixel_counter = 0;             //Horizontal to pixel by pixel of object

    for (ver_pixel_counter = 0; ver_pixel_counter < Obj->oHeight; ver_pixel_counter++)
    {
        for (hor_pixel_counter = 0; hor_pixel_counter < Obj->oWidth; hor_pixel_counter++)
        {
            // Shift bit in byte
            if (hor_pixel_counter & 7)
            {
                byteCursor <<= 1;
            }
            // Assign new value to new byte
            else
            {
                byteCursor = Obj->data[ver_pixel_counter * byteWidth
                                       //Line
                                       + hor_pixel_counter / 8];
                //Byte in line
            }
            if (byteCursor & 0x80)
            {
                drawPixel(display.CurX + hor_pixel_counter,
                          display.CurY + ver_pixel_counter, white);
            }
            else
            {
                drawPixel(display.CurX + hor_pixel_counter,
                          display.CurY + ver_pixel_counter, black);
            }
        }
    }

    display.CurX += Obj->oWidth;
}

/*
** This function Fills the complete OLED with this data byte.
**
**  Arguments:
**      data  -> Data to be filled in the OLED
** 
*/

// static void SH1106_Fill(unsigned char data)
// {
//     unsigned int total = 128 * 8; // 8 pages x 128 segments x 8 bits of data
//     unsigned int i = 0;

//     //Fill the Display
//     for (i = 0; i < total; i++)
//     {
//         SH1106_Write(false, data);
//     }
// }

/*
** This function getting called when the slave has been found
** Note : This will be called only once when we load the driver.
*/
static int etx_oled_probe(struct i2c_client *client,
                          const struct i2c_device_id *id)
{
    SH1106_DisplayInit();

    //fill the OLED with this data
    // SH1106_Fill(0xFF);

    clearScreen();
    setPosition(3, 30);

    putString("hello QA", &Font1118, white);
    // putChar('C', &Font1118, white);
    updateScreen();

    pr_info("OLED Probed!!!\n");

    return 0;
}

/*
** This function getting called when the slave has been removed
** Note : This will be called only once when we unload the driver.
*/
static int etx_oled_remove(struct i2c_client *client)
{
    //fill the OLED with this data
    // SH1106_Fill(0x00);
    clearScreen();
    updateScreen();
    pr_info("OLED Removed!!!\n");
    return 0;
}

/*
** Structure that has slave device id
*/
static const struct i2c_device_id etx_oled_id[] = {
    {SLAVE_DEVICE_NAME, 0},
    {}};
MODULE_DEVICE_TABLE(i2c, etx_oled_id);

/*
** I2C driver Structure that has to be added to linux
*/
static struct i2c_driver etx_oled_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE,
    },
    .probe = etx_oled_probe,
    .remove = etx_oled_remove,
    .id_table = etx_oled_id,
};

/*
** I2C Board Info strucutre
*/
static struct i2c_board_info oled_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, SH1106_SLAVE_ADDR)};

/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    int ret = -1;
    etx_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (etx_i2c_adapter != NULL)
    {
        etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);

        if (etx_i2c_client_oled != NULL)
        {
            i2c_add_driver(&etx_oled_driver);
            ret = 0;
        }

        i2c_put_adapter(etx_i2c_adapter);
    }

    pr_info("Driver Added!!!\n");
    return ret;
}

/*
** Module Exit function
*/
static void __exit etx_driver_exit(void)
{
    i2c_unregister_device(etx_i2c_client_oled);
    i2c_del_driver(&etx_oled_driver);
    pr_info("Driver Removed!!!\n");
}

module_init(etx_driver_init);
module_exit(etx_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("Simple I2C driver explanation (SSD_1306 OLED Display Interface)");
MODULE_VERSION("1.34");