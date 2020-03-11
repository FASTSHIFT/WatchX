#include "PageManager.h"

#define IS_PAGE(page)   ((page)<(MaxPage))

/**
  * @brief  初始化页面调度器
  * @param  pageMax: 页面最大数量
  * @param  eventMax: 事件最大数量
  * @retval 无
  */
PageManager::PageManager(uint8_t pageMax, uint8_t pageStackSize)
{
    MaxPage = pageMax;
    NewPage = 0;
    OldPage = 0;
    IsPageBusy = false;

    /* 申请内存，清空列表 */
    PageList = new PageList_TypeDef[MaxPage];
    for(uint8_t page = 0; page < MaxPage; page++)
    {
        PageClear(page);
    }
    
    /*页面栈配置*/
    PageStackSize = pageStackSize;
    PageStack = new uint8_t[PageStackSize];
    PageStackClear();
}

/**
  * @brief  页面调度器析构
  * @param  无
  * @retval 无
  */
PageManager::~PageManager()
{
    if(PageList)
    {
        delete[] PageList;
    }
    if(PageStack)
    {
        delete[] PageStack;
    }
}

/**
  * @brief  清除一个页面
  * @param  pageID: 页面编号
  * @retval true:成功 false:失败
  */
bool PageManager::PageClear(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = 0;
    PageList[pageID].LoopCallback = 0;
    PageList[pageID].ExitCallback = 0;
    PageList[pageID].EventCallback = 0;

    return true;
}

/**
  * @brief  注册一个基本页面，包含一个初始化函数，循环函数，退出函数，事件函数
  * @param  pageID: 页面编号
  * @param  setupCallback: 初始化函数回调
  * @param  loopCallback: 循环函数回调
  * @param  exitCallback: 退出函数回调
  * @param  eventCallback: 事件函数回调
  * @retval true:成功 false:失败
  */
bool PageManager::PageRegister(
    uint8_t pageID,
    CallbackFunction_t setupCallback,
    CallbackFunction_t loopCallback,
    CallbackFunction_t exitCallback,
    EventFunction_t eventCallback
)
{
    if(!IS_PAGE(pageID))
        return false;

    PageList[pageID].SetupCallback = setupCallback;
    PageList[pageID].LoopCallback = loopCallback;
    PageList[pageID].ExitCallback = exitCallback;
    PageList[pageID].EventCallback = eventCallback;
    return true;
}

/**
  * @brief  页面事件传递
  * @param  eventID: 事件编号
  * @param  param: 事件参数
  * @retval 无
  */
void PageManager::PageEventTransmit(int event, void* param)
{
    /*将事件传递到当前页面*/
    if(PageList[NowPage].EventCallback)
        PageList[NowPage].EventCallback(event, param);
}

/**
  * @brief  切换到指定页面
  * @param  pageID: 页面编号
  * @retval 无
  */
void PageManager::PageChangeTo(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return;
    
    /*检查页面是否忙碌*/
    if(!IsPageBusy)
    {
        /*新页面ID*/
        NextPage = NewPage = pageID;

        /*标记为忙碌状态*/
        IsPageBusy = true;
    }
}

/**
  * @brief  页面压栈，跳转至该页面
  * @param  pageID: 页面编号
  * @retval true:成功 false:失败
  */
bool PageManager::PagePush(uint8_t pageID)
{
    if(!IS_PAGE(pageID))
        return false;
    
    /*检查页面是否忙碌*/
    if(IsPageBusy)
       return false; 
    
    /*防止栈溢出*/
    if(PageStackTop >= PageStackSize - 1)
        return false;
    
    /*防止重复页面压栈*/
    if(pageID == PageStack[PageStackTop])
        return false;

    /*栈顶指针上移*/
    PageStackTop++;
    
    /*页面压栈*/
    PageStack[PageStackTop] = pageID;
    
    /*页面跳转*/
    PageChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  页面弹栈，跳转至上一个页面
  * @param  无
  * @retval true:成功 false:失败
  */
bool PageManager::PagePop()
{
    /*检查页面是否忙碌*/
    if(IsPageBusy)
       return false; 
    
    /*防止栈溢出*/
    if(PageStackTop == 0)
        return false;
    
    /*清空当前页面*/
    PageStack[PageStackTop] = 0;
    
    /*弹栈，栈顶指针下移*/
    PageStackTop--;
    
    /*页面跳转*/
    PageChangeTo(PageStack[PageStackTop]);
    
    return true;
}

/**
  * @brief  清空页面栈
  * @param  无
  * @retval 无
  */
void PageManager::PageStackClear()
{
    /*检查页面是否忙碌*/
    if(IsPageBusy)
       return; 
    
    /*清除栈中左右数据*/
    for(uint8_t i = 0; i < PageStackSize; i++)
    {
        PageStack[i] = 0;
    }
    /*栈顶指针复位*/
    PageStackTop = 0;
}

/**
  * @brief  页面调度器状态机
  * @param  无
  * @retval 无
  */
void PageManager::Running()
{
    /*页面切换事件*/
    if(NewPage != OldPage)
    {
        /*标记为忙碌状态*/
        IsPageBusy = true;

        /*执行旧页面的退出函数*/
        if(PageList[OldPage].ExitCallback && IS_PAGE(OldPage))
            PageList[OldPage].ExitCallback();
        
        /*标记旧页面*/
        LastPage = OldPage;
        
        /*标记新页面为当前页面*/
        NowPage = NewPage;

        /*执行新页面初始化函数*/
        if(PageList[NewPage].SetupCallback && IS_PAGE(NewPage))
            PageList[NewPage].SetupCallback();

        /*新页面初始化完成，标记为旧页面*/
        OldPage = NewPage;
    }
    else
    {
        /*标记页面不忙碌，处于循环状态*/
        IsPageBusy = false;
        
        /*页面循环事件*/
        if(PageList[NowPage].LoopCallback && IS_PAGE(NowPage))
            PageList[NowPage].LoopCallback();
    }
}
