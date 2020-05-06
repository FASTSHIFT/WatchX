#include "GUI/DisplayPrivate.h"
#include "Basic/SysConfig.h"

static lv_disp_drv_t * disp_drv_p;
static lv_disp_buf_t disp_buf;
static lv_color_t lv_disp_buf1[LV_HOR_RES_MAX * 20];
static lv_color_t lv_disp_buf2[LV_HOR_RES_MAX * 20];

#if LV_USE_LOG != 0
/* Serial debugging */
static void log_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    delay(100);
}
#endif

static void disp_spi_dma_send(void* buf, uint32_t size)
{
    DMA_Cmd(DMA2_Stream3, DISABLE);                      //关闭DMA传输
    //while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE); //确保DMA可以被设置
    DMA2_Stream3->M0AR = (uint32_t)buf;
    DMA2_Stream3->NDTR = size;
    DMA_Cmd(DMA2_Stream3, ENABLE);                      //开启DMA传输
    
//    while(DMA_GetFlagStatus(DMA2_Stream3,DMA_FLAG_TCIF3) == RESET){} //等待DMA传输完成
//    DMA_ClearFlag(DMA2_Stream3, DMA_FLAG_TCIF3); // 清除标志
}

/* Display flushing */
static void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    disp_drv_p = disp;
    //screen.drawFastRGBBitmap(area->x1, area->y1, (uint16_t*)color_p, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
    int16_t w = (area->x2 - area->x1 + 1);
    int16_t h = (area->y2 - area->y1 + 1);
    uint32_t size = w * h * 2;
    
    screen.setAddrWindow(area->x1, area->y1, area->x2, area->y2);
    
    digitalWrite_LOW(TFT_CS_Pin);
    digitalWrite_HIGH(TFT_DC_Pin);
    
    disp_spi_dma_send((void*)color_p, size);
    
//    digitalWrite_HIGH(TFT_CS_Pin);
//    lv_disp_flush_ready(disp_drv_p);/* tell lvgl that flushing is done */
}

extern "C" {
void DMA2_Stream3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3) != RESET)
    {
        digitalWrite_HIGH(TFT_CS_Pin);
        lv_disp_flush_ready(disp_drv_p);/* tell lvgl that flushing is done */
        DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);
    }
}
}

static void lv_disp_spi_dma_init()
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA2时钟使能
    DMA_DeInit(DMA2_Stream3);
    while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE) {} //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_3;  //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->DR); //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)lv_disp_buf1;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = sizeof(lv_disp_buf1);//数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_Init(DMA2_Stream3, &DMA_InitStructure);//初始化DMA Stream
        
    SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE); // 使能DMA发送
        
//    NVIC_InitTypeDef NVIC_InitStructure;
//    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);
}

void lv_port_disp_init()
{
    lv_init();

    lv_disp_spi_dma_init();

#if LV_USE_LOG != 0
    lv_log_register_print(log_print); /* register print function for debugging */
#endif

    lv_disp_buf_init(&disp_buf, lv_disp_buf1, lv_disp_buf2, sizeof(lv_disp_buf1) / sizeof(lv_color_t));

    /*Initialize the display*/
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);
}


/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 0

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_templ.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
#if LV_USE_GPU
static void gpu_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void gpu_fill(lv_color_t * dest, uint32_t length, lv_color_t color);
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/

    /* LittlevGL requires a buffer where it draws the objects. The buffer's has to be greater than 1 display row
     *
     * There are three buffering configurations:
     * 1. Create ONE buffer with some rows:
     *      LittlevGL will draw the display's content here and writes it to your display
     *
     * 2. Create TWO buffer with some rows:
     *      LittlevGL will draw the display's content to a buffer and writes it your display.
     *      You should use DMA to write the buffer's content to the display.
     *      It will enable LittlevGL to draw the next part of the screen to the other buffer while
     *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
     *
     * 3. Create TWO screen-sized buffer:
     *      Similar to 2) but the buffer have to be screen sized. When LittlevGL is ready it will give the
     *      whole frame to display. This way you only need to change the frame buffer's address instead of
     *      copying the pixels.
     * */

    /* Example for 1) */
    static lv_disp_buf_t disp_buf_1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * 10];                      /*A buffer for 10 rows*/
    lv_disp_buf_init(&disp_buf_1, buf1_1, NULL, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_buf_t disp_buf_2;
    static lv_color_t buf2_1[LV_HOR_RES_MAX * 10];                        /*A buffer for 10 rows*/
    static lv_color_t buf2_2[LV_HOR_RES_MAX * 10];                        /*An other buffer for 10 rows*/
    lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 10);   /*Initialize the display buffer*/

    /* Example for 3) */
    static lv_disp_buf_t disp_buf_3;
    static lv_color_t buf3_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];            /*A screen sized buffer*/
    static lv_color_t buf3_2[LV_HOR_RES_MAX * LV_VER_RES_MAX];            /*An other screen sized buffer*/
    lv_disp_buf_init(&disp_buf_3, buf3_1, buf3_2, LV_HOR_RES_MAX * LV_VER_RES_MAX);   /*Initialize the display buffer*/


    /*-----------------------------------
     * Register the display in LittlevGL
     *----------------------------------*/

    lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = 480;
    disp_drv.ver_res = 320;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf_2;

#if LV_USE_GPU
    /*Optionally add functions to access the GPU. (Only in buffered mode, LV_VDB_SIZE != 0)*/

    /*Blend two color array using opacity*/
    disp_drv.gpu_blend = gpu_blend;

    /*Fill a memory array with a color*/
    disp_drv.gpu_fill = gpu_fill;
#endif

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your display and the required peripherals. */
static void disp_init(void)
{
    /*You code here*/
}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished. */
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = area->y1; y <= area->y2; y++)
    {
        for(x = area->x1; x <= area->x2; x++)
        {
            /* Put a pixel to the display. For example: */
            /* put_px(x, y, *color_p)*/
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp);
}


/*OPTIONAL: GPU INTERFACE*/
#if LV_USE_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_blend(lv_disp_drv_t * disp_drv, lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/

    uint32_t i;
    for(i = 0; i < length; i++)
    {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void gpu_fill_cb(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
                        const lv_area_t * fill_area, lv_color_t color);
{
    /*It's an example code which should be done by your GPU*/
    uint32_t x, y;
    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/

    for(y = fill_area->y1; y < fill_area->y2; y++)
    {
        for(x = fill_area->x1; x < fill_area->x2; x++)
        {
            dest_buf[x] = color;
        }
        dest_buf += dest_width;  /*Go to the next line*/
    }


    uint32_t i;
    for(i = 0; i < length; i++)
    {
        dest[i] = color;
    }
}

#endif  /*LV_USE_GPU*/

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
