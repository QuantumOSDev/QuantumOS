#include <quantum/module.h>
#include <quantum/init.h>

#include <core/string.h>

#include <sys/memory.h>

MODULE_NAME("QUANTUM_MODULE_C");

#define MODULES_MAX 256

static char **__modules_list;

static unsigned int *__modules_addr = 0;
static unsigned int  __modules_r    = 0;

void __module_init(void)
{
    __modules_list = (char **)        kmalloc(MODULES_MAX * sizeof(unsigned int));
    __modules_addr = (unsigned int *) kmalloc(MODULES_MAX * sizeof(unsigned int));
}

void __module_call(char *__func)
{
    for (int i = 0; i < __modules_r; i++)
    {
        if (strcmp(__func, __modules_list[i]) == 0)
        {
            asm volatile ("call *%%eax": :"a"(__modules_addr[i]));

            return;
        }
    }

    quantum_info(1, " KMOD\t", "Error: Kernel module: [%s] not found", __func);
}

void __module_add(char *__func, unsigned int __addr)
{
    quantum_info(0, " KMOD\t", "Initializing kernel module: [%s] Address: [0x%x]", __func, __addr);

    __modules_list[__modules_r] = __func;
    __modules_addr[__modules_r] = __addr;

    __modules_r++;
}

void __module_wakeup(void)
{
    for (int i = 0; i < __modules_r; i++)
    {
        quantum_info(0, " KMOD\t", "Waking up module: [%s] at address: [0x%x]", __modules_list[i], __modules_addr[i]);

        asm volatile ("call *%%eax": :"a"(__modules_addr[i]));
    }
}