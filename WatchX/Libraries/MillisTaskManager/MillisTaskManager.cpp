#include "MillisTaskManager.h"

/**
  * @brief  初始化任务列表
  * @param  TaskNum_MAX_Set:设定任务列表总长度
  * @param  priorityEnable:设定是否开启优先级
  * @retval 无
  */
MillisTaskManager::MillisTaskManager(TaskNum_t TaskNum_MAX_Set, bool priorityEnable)
{
    PriorityEnable = priorityEnable;
    if(TaskList != 0) delete TaskList;//清空列表
    TaskList = new MillisTaskManager_TypeDef[TaskNum_MAX_Set];//为任务列表申请内存
    TaskNum_MAX = TaskNum_MAX_Set;//记录任务列表总长度

    for(TaskNum_t i = 0; i < TaskNum_MAX; i++)//初始化任务列表，清零
        TaskClear(i);
}

/**
  * @brief  调度器析构，释放任务列表内存
  * @param  无
  * @retval 无
  */
MillisTaskManager::~MillisTaskManager()
{
    if(TaskList != 0) delete TaskList;
}

/**
  * @brief  不带越界判断地清除一个任务(private)
  * @param  ID:任务ID
  * @retval 无
  */
void MillisTaskManager::TaskClear(TaskNum_t ID)
{
    TaskList[ID].Function = 0;
    TaskList[ID].State = false;
    TaskList[ID].IntervalTime = 0;
    TaskList[ID].TimePoint = 0;
}

/**
  * @brief  在任务列表内的一个位置注册一个任务，设定间隔执行时间
  * @param  ID:任务ID
  * @param  Function:任务函数指针
  * @param  TimeSetMs:时间设定(毫秒)
  * @param  TaskState:任务开关
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskRegister(TaskNum_t ID, void_TaskFunction_t Function, uint32_t TimeSetMs, bool TaskState)
{
    if(TaskList[ID].Function != 0)//判断是否占用
        return false;
    
    if(ID < TaskNum_MAX)//判断是否越界
    {
        TaskList[ID].Function = Function;//注册函数指针
        TaskList[ID].State = TaskState;//初始状态设定
        TaskList[ID].IntervalTime = TimeSetMs;//注册时间
        return true;//注册成功
    }
    else 
        return false;//注册失败
}

/**
  * @brief  寻找任务,返回任务注册地址
  * @param  Function:任务函数指针
  * @param  *ID:任务注册地址指针
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskFind(void_TaskFunction_t Function, TaskNum_t *ID)
{
    for(TaskNum_t i = 0; i < TaskNum_MAX; i++)
    {
        if(TaskList[i].Function == Function)
        {
            *ID = i;
            return true;
        }
    }
    return false;
}

/**
  * @brief  寻找任务,返回任务注册地址
  * @param  Function:任务函数指针
  * @retval -1:未找到 ; 其他:任务ID
  */
int16_t MillisTaskManager::TaskFind(void_TaskFunction_t Function)
{
    TaskNum_t id;
    if(TaskFind(Function, &id) == true)
        return id;
    else
        return -1;
}

/**
  * @brief  注销任务
  * @param  Function:任务函数指针
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskLogout(void_TaskFunction_t Function)
{
    TaskNum_t ID = 0;
    if(TaskFind(Function, &ID) == true)
    {
        TaskClear(ID);
        return true;
    }
    else
        return false;
}

/**
  * @brief  注销任务
  * @param  ID:任务ID
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskLogout(TaskNum_t ID)
{
    if(ID < TaskNum_MAX)
    {
        TaskClear(ID);
        return true;
    }
    else
        return false;
}

/**
  * @brief  任务状态控制
  * @param  Function:任务函数指针
  * @param  TaskState:任务状态
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskStateCtrl(void_TaskFunction_t Function, bool TaskState)
{
    TaskNum_t ID = 0;
    if(TaskFind(Function, &ID) == true)
    {
        TaskList[ID].State = TaskState;
        return true;
    }
    else
        return false;
}

/**
  * @brief  任务状态控制
  * @param  ID:任务ID
  * @param  TaskState:任务状态
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskStateCtrl(TaskNum_t ID, bool TaskState)
{
    if(ID < TaskNum_MAX)
    {
        TaskList[ID].State = TaskState;
        return true;
    }
    else
        return false;
}

/**
  * @brief  任务间隔时间设置
  * @param  Function:任务函数指针
  * @param  TaskState:任务间隔时间
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskSetIntervalTime(void_TaskFunction_t Function, uint32_t TimeSetMs)
{
    TaskNum_t ID = 0;
    if(TaskFind(Function, &ID) == true)
    {
        TaskList[ID].IntervalTime = TimeSetMs;
        return true;
    }
    else
        return false;
}

#ifdef _SUPPORT_CPU_USAGE
#include "Arduino.h"
static uint32_t UserFuncLoopUs;
float MillisTaskManager::GetCPU_Usage()
{
    static uint32_t MtmStartUs;
    float usage = (float)UserFuncLoopUs / (micros() - MtmStartUs) * 100.0f;
    
    if(usage > 100.0f)
        usage = 100.0f;
    
    MtmStartUs = micros();
    UserFuncLoopUs = 0;
    return usage;
}
#endif

/**
  * @brief  任务间隔时间设置
  * @param  ID:任务注册的位置
  * @param  TaskState:任务间隔时间
  * @retval true:成功 ; false:失败
  */
bool MillisTaskManager::TaskSetIntervalTime(TaskNum_t ID, uint32_t TimeSetMs)
{
    if(ID < TaskNum_MAX)
    {
        TaskList[ID].IntervalTime = TimeSetMs;
        return true;
    }
    else
        return false;
}

/**
  * @brief  获取任务单次耗费时间(us)
  * @param  ID:任务注册的位置
  * @retval 任务单次耗费时间(us)
  */
uint32_t MillisTaskManager::GetTaskTimeCost(TaskNum_t ID)
{
    if(ID < TaskNum_MAX)
    {
        return TaskList[ID].TimeCost;
    }
    else
        return 0;
}

/**
  * @brief  调度器(内核)
  * @param  MillisSeed:提供一个精确到毫秒的系统时钟变量
  * @retval 无
  */
void MillisTaskManager::Running(uint32_t MillisSeed)
{
    for(TaskNum_t i = 0; i < TaskNum_MAX; i++)//遍历任务列表
    {
        if(TaskList[i].Function)//判断是否为空函数
        {
            if(TaskList[i].State && (MillisSeed - TaskList[i].TimePoint >= TaskList[i].IntervalTime))//判断是否运行任务，是否到达触发时间点
            {
                TaskList[i].TimePoint = MillisSeed;//标记下一个时间点
                
#ifdef _SUPPORT_CPU_USAGE                
                uint32_t start = micros();
#endif
                TaskList[i].Function();//执行任务
                
#ifdef _SUPPORT_CPU_USAGE
                TaskList[i].TimeCost = micros() - start;
                UserFuncLoopUs += TaskList[i].TimeCost;
#endif

                if(PriorityEnable)//判断是否开启优先级
                    break;
            }
        }
    }
}
