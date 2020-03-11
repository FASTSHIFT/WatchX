
/**********     超轻量级分时合作式任务调度器     **********/
/**********        Designed By _VIFEXTech        **********/

//Finished 2018.7.26  v1.0 可以替代旧的millis()循环判断的方案，不依赖ArduinoAPI
//Update   2018.10.9  v1.1 将任务状态标志位类型换成bool类型
//Update   2018.11.17 v1.2 将typedef转移至class内部
//Update   2018.11.18 v1.3 修复50天后数值溢出导致任务停止的bug
//Update   2018.12.17 v1.4 将 TaskCtrl 修改为 TaskStateCtrl，添加修改任务间隔时间的接口，添加 TaskFind 用于遍历列表寻找任务
//Update   2019.2.5   v1.5 添加析构函数，用于释放内存
//Update   2019.3.4   v1.6 将FuncPos改为ID，添加TaskFind(void_TaskFunction_t Function)
//Update   2019.3.21  v1.7 支持设定优先级，优先级排列为任务ID号，数字越小优先级越高
//Update   2019.4.24  v1.8 添加GetCPU_Useage()可获得CPU占用率
//Update   2019.5.8   v1.9 为TaskRegister添加防冲突判断
//Update   2019.9.3   v2.0 添加TimeCost任务时间开销计算

#ifndef __MILLISTASKMANAGER_H
#define __MILLISTASKMANAGER_H

#define _SUPPORT_CPU_USAGE

#include "stdint.h"

class MillisTaskManager
{
public:
    typedef void(*void_TaskFunction_t)(void);//回调函数定义
    typedef uint8_t TaskNum_t;
    typedef struct
    {
        bool State; //任务状态
        void_TaskFunction_t Function; //任务函数指针
        uint32_t IntervalTime; //任务触发间隔时间
        uint32_t TimePoint; //任务触发时间点
        uint32_t TimeCost; //任务时间开销(us)
    } MillisTaskManager_TypeDef; //任务类型定义

    MillisTaskManager(TaskNum_t TaskNum_MAX_Set, bool priorityEnable = false);
    ~MillisTaskManager();
    bool TaskRegister(TaskNum_t ID, void_TaskFunction_t Function, uint32_t TimeSetMs, bool TaskState = true);
    bool TaskFind(void_TaskFunction_t Function, TaskNum_t *ID);
    int16_t TaskFind(void_TaskFunction_t Function);
    bool TaskLogout(void_TaskFunction_t Function);
    bool TaskLogout(TaskNum_t ID);
    bool TaskStateCtrl(void_TaskFunction_t Function, bool TaskState);
    bool TaskStateCtrl(TaskNum_t ID, bool TaskState);
    bool TaskSetIntervalTime(void_TaskFunction_t Function, uint32_t TimeSetMs);
    bool TaskSetIntervalTime(TaskNum_t ID, uint32_t TimeSetMs);
    uint32_t GetTaskTimeCost(TaskNum_t ID);
#ifdef _SUPPORT_CPU_USAGE
    float GetCPU_Usage();
#endif
    void Running(uint32_t MillisSeed);

private:
    MillisTaskManager_TypeDef* TaskList;//任务列表
    TaskNum_t TaskNum_MAX;//任务列表长度
    bool PriorityEnable;
    void TaskClear(TaskNum_t ID);//清除任务
};

#endif
