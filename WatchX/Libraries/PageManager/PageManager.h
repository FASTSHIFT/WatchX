/*
 * MIT License
 * Copyright (c) 2018-2020 _VIFEXTech
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the follo18wing conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
/**
  ******************************************************************************
  * @file    PageManager.h
  * @author  _VIFEXTech
  * @version v1.4
  * @date    2020-7-28
  * @brief   基于状态机的异步页面调度器，不依赖任何GUI框架，支持操作系统
  * @finished: 2018.10.6  - v1.0 拥有简单的页面切换管理，事件传递调度
  * @upgrade:  2019.4.1   - v1.1 增加注释，重写事件传递调度，减少重复代码
  * @upgrade:  2019.10.5  - v1.2 使用栈结构管理页面嵌套，默认栈深度10层(可调)
  * @upgrade:  2019.12.4  - v1.3 修复在Setup和Exit阻塞情况下页面切换的BUG，添加析构
  * @upgrade:  2020.7.28  - v1.4 修改EventFunction_t形参顺序，判断指针使用 "ptr != NULL" 的形式，添加MIT开源协议
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PAGEMANAGER_H
#define __PAGEMANAGER_H

#include <stdint.h>

class PageManager {
    typedef void(*CallbackFunction_t)(void);
    typedef void(*EventFunction_t)(void*,int);
    typedef struct {
        CallbackFunction_t SetupCallback;
        CallbackFunction_t LoopCallback;
        CallbackFunction_t ExitCallback;
        EventFunction_t EventCallback;
    } PageList_TypeDef;
public:
    PageManager(uint8_t pageMax, uint8_t pageStackSize = 10);
    ~PageManager();
    uint8_t NowPage, LastPage, NextPage;
    uint8_t NewPage, OldPage;

    bool PageRegister(
        uint8_t pageID,
        CallbackFunction_t setupCallback,
        CallbackFunction_t loopCallback,
        CallbackFunction_t exitCallback,
        EventFunction_t eventCallback
    );
    bool PageClear(uint8_t pageID);
    bool PagePush(uint8_t pageID);
    bool PagePop();
    void PageChangeTo(uint8_t pageID);
    void PageEventTransmit(void* obj, int event);
    void PageStackClear();
    void Running();
private:
    PageList_TypeDef* PageList;
    uint8_t *PageStack;
    uint8_t PageStackSize;
    uint8_t PageStackTop;
    uint8_t MaxPage;
    bool IsPageBusy;
};

#endif
