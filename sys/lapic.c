#include <sys/lapic.h>
#include <sys/mio.h>

unsigned int lapic_in(unsigned int __register)
{
    return mio_in32(__global_apic_addr + __register);
}

void lapic_out(unsigned int __register, unsigned int __data)
{
    mio_out32(__global_apic_addr + __register, __data);
}

void lapic_init(void)
{
    lapic_out(LAPIC_TPR, 0);
    
    lapic_out(LAPIC_DFR, 0xffffffff);
    lapic_out(LAPIC_LDR, 0x01000000);

    lapic_out(LAPIC_SVR, 0x100 | 0xff);
}

void lapic_send_init(unsigned int __id)
{
    lapic_out(LAPIC_ICRHI, __id << ICR_DESTINATION_SHIFT);
    lapic_out(LAPIC_ICRLO, ICR_INIT | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (lapic_in(LAPIC_ICRLO) & ICR_SEND_PENDING);
    {
        /* Wait */
    }
}

void lapic_send_startup(unsigned int __id, unsigned int __vector)
{
    lapic_out(LAPIC_ICRHI, __id << ICR_DESTINATION_SHIFT);
    lapic_out(LAPIC_ICRLO, ICR_STARTUP | ICR_PHYSICAL | ICR_ASSERT | ICR_EDGE | ICR_NO_SHORTHAND);

    while (lapic_in(LAPIC_ICRLO) & ICR_SEND_PENDING);
    {
        /* Wait */
    }
}

unsigned int lapic_getid(void)
{
    return lapic_in(LAPIC_ID) >> 24;
}