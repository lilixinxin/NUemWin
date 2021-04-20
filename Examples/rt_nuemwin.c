#include "GUI.h"

#define DEF_NUEMWIN_DEMO_NAME "nuemwin_demo"

static rt_thread_t nuemwin_tid = RT_NULL;
extern void MainTask(void);
static void test_thread(void *param)
{
    MainTask();
}

int nuemwin_start(int argc, char **argv)
{
    nuemwin_tid = rt_thread_find(DEF_NUEMWIN_DEMO_NAME);
    if (!nuemwin_tid)
    {
        nuemwin_tid = rt_thread_create(DEF_NUEMWIN_DEMO_NAME,
                                       test_thread, RT_NULL,
                                       5120,
                                       RT_THREAD_PRIORITY_MAX - 2, 10);
        if (nuemwin_tid != RT_NULL)
            rt_thread_startup(nuemwin_tid);
    }
    else
        rt_kprintf("%s is running\n", DEF_NUEMWIN_DEMO_NAME);

    return 0;
}
MSH_CMD_EXPORT(nuemwin_start, start nuemwin demo);

static rt_err_t nuemwin_stop(int argc, char **argv)
{
    rt_err_t result;

    if (nuemwin_tid)
    {
        result = rt_thread_delete(nuemwin_tid);
        RT_ASSERT(result == RT_EOK);

        rt_kprintf("%s is stoped\n", DEF_NUEMWIN_DEMO_NAME);

        nuemwin_tid = RT_NULL;
    }
    return 0;
}
MSH_CMD_EXPORT(nuemwin_stop, e.g: stop nuemwin demo);
