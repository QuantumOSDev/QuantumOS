#ifndef MODULE_H
#define MODULE_H

#define MODULE_NAME(__NAME) static char *__MODULE_NAME __attribute__((unused))         = __NAME;
#define MODULE_INIT(__FUNC) static void *__MODULE_INIT_POINTER __attribute__((unused)) = &__FUNC;
#define MODULE_EXIT(__FUNC) static void *__MODULE_EXIT_POINTER __attribute__((unused)) = &__FUNC;

#define EXPORT_SYMBOL(__FUNC) __module_add(#__FUNC, __FUNC);

void __module_init(void);
void __module_call(char *__func);
void __module_add(char *__func, unsigned int __addr);

void __module_wakeup(void);

#endif