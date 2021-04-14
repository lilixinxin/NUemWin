#include "GUI.h"

#define DEF_NUEMWIN_DEMO_NAME "nuemwin_demo"

extern void MainTask(void);
static void test_thread(void *param)
{
    MainTask();
}

int nuemwin_demo(int argc, char **argv)
{
    rt_thread_t tid = rt_thread_find(DEF_NUEMWIN_DEMO_NAME);
    if (!tid)
    {
        tid = rt_thread_create(DEF_NUEMWIN_DEMO_NAME,
                               test_thread, RT_NULL,
                               5120,
                               RT_THREAD_PRIORITY_MAX - 2, 10);
        if (tid != RT_NULL)
            rt_thread_startup(tid);
    }
    else
        rt_kprintf("%s is running\n", DEF_NUEMWIN_DEMO_NAME);

    return 0;
}
MSH_CMD_EXPORT(nuemwin_demo, test nuemwin);
