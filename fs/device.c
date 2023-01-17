#include <quantum/init.h>

#include <sys/memory.h>

#include <fs/device.h>

__device_t *__devices = 0;

unsigned int __lastid = 0;

void device_init(void)
{
    __devices = (__device_t *) kmalloc(64 * sizeof(__device_t));

    kmemset(__devices, 0, 64 * sizeof(__device_t));

    __lastid = 0;

    quantum_info(0, " DEVICE-MGR\t", "Device Manager Initialized!");
}

void device_print(void)
{
    for (int i = 0; i < __lastid; i++)
    {
        quantum_info(0, " DEVICE-MGR\t", "ID: %d | UNIQUE: %d, %s, %s", i, __devices[i].__unique_id, __devices[i].__dev_type == DEVICE_CHAR ? "CHAR" : "BLOCK", __devices[i].__name);
    }
}

int device_add(__device_t *__device)
{
    __devices[__lastid] = *__device;

    quantum_info(0, " DEVICE-MGR\t", "Registered Device [%s] (%d) as Device #%d\n", __device->__name, __device->__unique_id, __lastid);

    __lastid++;

    return (__lastid - 1);
}

__device_t *device_get(unsigned int __id)
{
    return &__devices[__id];
}

__device_t *device_get_by_id(unsigned int __id)
{
    for (int i = 0; i < 64; i++)
    {
        if (__devices[i].__unique_id == __id)
        {
            return &__devices[i];
        }
    }

    return 0;
}

int device_get_number(void)
{
    return __lastid;
}
