/*
 * QuantumOS Copyright (c) 2022-2023
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <userspace/syscalls.h>

#include <sys/memory.h>
#include <sys/pic.h>
#include <sys/isr.h>

#include <core/string.h>
#include <core/print.h>
#include <core/time.h>

#include <drivers/keyboard.h>
#include <fs/vfs.h>

#include <quantum/init.h>

const syscall_func_t SYSCALLS[MAX_SYSCALLS] = {
    NULL, // 0
    NULL, // 1
    NULL, // 2
    sys_read, // sys_read 3 
    sys_write, // sys_write 4
    sys_open, // sys_open 5
    NULL, // 6
    NULL, // 7
    sys_creat, // sys_creat 8
    NULL, // 9
    NULL, // 10
    NULL, // 11
    NULL, // 12
    sys_time, // sys_time 13
    NULL, // 14
    NULL, // 15
    NULL, // 16
    NULL, // 17
    NULL, // 18
    NULL, // 19
    NULL, // 20
    NULL, // 21
    NULL, // 22
    NULL, // 23
    NULL, // 24
    NULL, // 25
    NULL, // 26
    NULL, // 27
    NULL, // 28
    NULL, // 29
    NULL, // 30
    NULL, // 31
    NULL, // 32
    NULL, // 33
    NULL, // 34
    NULL, // 35
    NULL, // 36
    NULL, // 37
    NULL, // 38
    NULL, // 39
    NULL, // 40
    NULL, // 41
    NULL, // 42
    NULL, // 43
    NULL, // 44
    NULL, // 45
    NULL, // 46
    NULL, // 47
    NULL, // 48
    NULL, // 49
    NULL, // 50
    NULL, // 51
    NULL, // 52
    NULL, // 53
    NULL, // 54
    NULL, // 55
    NULL, // 56
    NULL, // 57
    NULL, // 58
    NULL, // 59
    NULL, // 60
    NULL, // 61
    NULL, // 62
    NULL, // 63
    NULL, // 64
    NULL, // 65
    NULL, // 66
    NULL, // 67
    NULL, // 68
    NULL, // 69
    NULL, // 70
    NULL, // 71
    NULL, // 72
    NULL, // 73
    NULL, // 74
    NULL, // 75
    NULL, // 76
    NULL, // 77
    NULL, // 78
    NULL, // 79
    NULL, // 80
    NULL, // 81
    NULL, // 82
    NULL, // 83
    NULL, // 84
    NULL, // 85
    NULL, // 86
    NULL, // 87
    NULL, // 88
    NULL, // 89
    NULL, // 90
    NULL, // 91
    NULL, // 92
    NULL, // 93
    NULL, // 94
    NULL, // 95
    NULL, // 96
    NULL, // 97
    NULL, // 98
    NULL, // 99
    NULL, // 100
    NULL, // 101
    NULL, // 102
    NULL, // 103
    NULL, // 104
    NULL, // 105
    NULL, // 106
    NULL, // 107
    NULL, // 108
    sys_uname, // 109
    NULL, // 110
    NULL, // 111
    NULL, // 112
    NULL, // 113
    NULL, // 114
    NULL, // 115
    NULL, // 116
    NULL, // 117
    NULL, // 118
    NULL, // 119
    NULL, // 120
    NULL, // 121
    NULL, // 122
    NULL, // 123
    NULL, // 124
    NULL, // 125
    NULL, // 126
    NULL, // 127
    NULL, // 128
    NULL, // 129
    NULL, // 130
    NULL, // 131
    NULL, // 132
    NULL, // 133
    NULL, // 134
    NULL, // 135
    NULL, // 136
    NULL, // 137
    NULL, // 138
    NULL, // 139
    NULL, // 140
    NULL, // 141
    NULL, // 142
    NULL, // 143
    NULL, // 144
    NULL, // 145
    NULL, // 146
    NULL, // 147
    NULL, // 148
    NULL, // 149
    NULL, // 150
    NULL, // 151
    NULL, // 152
    NULL, // 153
    NULL, // 154
    NULL, // 155
    NULL, // 156
    NULL, // 157
    NULL, // 158
    NULL, // 159
    NULL, // 160
    NULL, // 161
    NULL, // 162
    NULL, // 163
    NULL, // 164
    NULL, // 165
    NULL, // 166
    NULL, // 167
    NULL, // 168
    NULL, // 169
    NULL, // 170
    NULL, // 171
    NULL, // 172
    NULL, // 173
    NULL, // 174
    NULL, // 175
    NULL, // 176
    NULL, // 177
    NULL, // 178
    NULL, // 179
    NULL, // 180
    NULL, // 181
    NULL, // 182
    NULL, // 183
    NULL, // 184
    NULL, // 185
    NULL, // 186
    NULL, // 187
    NULL, // 188
    NULL, // 189
    NULL, // 190
    NULL, // 191
    NULL, // 192
    NULL, // 193
    NULL, // 194
    NULL, // 195
    NULL, // 196
    NULL, // 197
    NULL, // 198
    NULL, // 199
    NULL, // 200
    NULL, // 201
    NULL, // 202
    NULL, // 203
    NULL, // 204
    NULL, // 205
    NULL, // 206
    NULL, // 207
    NULL, // 208
    NULL, // 209
    NULL, // 210
    NULL, // 211
    NULL, // 212
    NULL, // 213
    NULL, // 214
    NULL, // 215
    NULL, // 216
    NULL, // 217
    NULL, // 218
    NULL, // 219
    NULL, // 220
    NULL, // 221
    NULL, // 222
    NULL, // 223
    NULL, // 224
    NULL, // 225
    NULL, // 226
    NULL, // 227
    NULL, // 228
    NULL, // 229
    NULL, // 230
    NULL, // 231
    NULL, // 232
    NULL, // 233
    NULL, // 234
    NULL, // 235
    NULL, // 236
    NULL, // 237
    NULL, // 238
    NULL, // 239
    NULL, // 240
    NULL, // 241
    NULL, // 242
    NULL, // 243
    NULL, // 244
    NULL, // 245
    NULL, // 246
    NULL, // 247
    NULL, // 248
    NULL, // 249
    NULL, // 250
    NULL, // 251
    NULL, // 252
    NULL, // 253
    NULL, // 254
    NULL, // 255
    NULL, // 256
    NULL, // 257
    NULL, // 258
    NULL, // 259
    NULL, // 260
    NULL, // 261
    NULL, // 262
    NULL, // 263
    NULL, // 264
    NULL, // 265
    NULL, // 266
    NULL, // 267
    NULL, // 268
    NULL, // 269
    NULL, // 270
    NULL, // 271
    NULL, // 272
    NULL, // 273
    NULL, // 274
    NULL, // 275
    NULL, // 276
    NULL, // 277
    NULL, // 278
    NULL, // 279
    NULL, // 280
    NULL, // 281
    NULL, // 282
    NULL, // 283
    NULL, // 284
    NULL, // 285
    NULL, // 286
    NULL, // 287
    NULL, // 288
    NULL, // 289
    NULL, // 290
    NULL, // 291
    NULL, // 292
    NULL, // 293
    NULL, // 294
    NULL, // 295
    NULL, // 296
    NULL, // 297
    NULL, // 298
    NULL, // 299
    NULL, // 300
    NULL, // 301
    NULL, // 302
    NULL, // 303
    NULL, // 304
    NULL, // 305
    NULL, // 306
    NULL, // 307
    NULL, // 308
    NULL, // 309
    NULL, // 310
    NULL, // 311
    NULL, // 312
    NULL, // 313
    NULL, // 314
    NULL, // 315
    NULL, // 316
    NULL, // 317
    NULL, // 318
    NULL, // 319
    NULL, // 320
    NULL, // 321
    NULL, // 322
    NULL, // 323
    NULL, // 324
};

void syscall_interrupt_handler(__registers_t* regs) 
{
    syscall_func_t syscall = SYSCALLS[regs->eax];

    quantum_info(0, " Syscall", "Got %d syscall", regs->eax);
    if (syscall == NULL) 
    {
        quantum_info(1, " Syscall", "Syscall with number %d is not implemented", regs->eax);
        return;
    }

    int syscall_ret = syscall(
        regs->eax, regs->ebx, 
        regs->ecx, regs->edx, 
        regs->esi, regs->edi, 
        regs->ebp
    );

    quantum_info(0, " Syscall", "Syscall %d returned %d", regs->eax, syscall_ret);
}

void quantum_syscalls_init() 
{
    isr_register_interrupt_handler(0x80, syscall_interrupt_handler);
    quantum_info(0, " Syscall", "Successfully initialized syscall handler (0x%x)", 0x80);
}

int sys_read(int eax, int fd, int buf, int count, int esi, int edi, int ebp) 
{
    printf("sys_read: this syscall is not implemented");
    return -1;
}

int sys_write(int eax, int fd, int buf, int count, int esi, int edi, int ebp) 
{
    const char* _buf = (const char*)buf;
    for (int i = 0; i < count; i++) 
        putc(_buf[i]);

    return 0;
}

int sys_creat(int eax, int filename, int mode, int edx, int esi, int edi, int ebp) 
{
    int flag = O_CREAT;
    return sys_open(0x05, filename, flag, mode, 0, 0, 0);
}

int sys_open(int eax, int filename, int flag, int mode, int esi, int edi, int ebp) 
{
    printf("sys_open: this syscall is not implemented");
    return -1;
}

int sys_time(int eax, int time_ptr, int ecx, int edx, int esi, int edi, int ebp) 
{
    if (time_ptr != 0) 
    {
        date_t now_date = get_date_cmos();
        unsigned long timestamp = date_to_timestamp(&now_date);

        unsigned long* _time_ptr = (unsigned long*)&time_ptr;
        *_time_ptr = timestamp;
        return 0;
    }
    return -1;
}

int sys_uname(int eax, int uname_ptr, int ecx, int edx, int esi, int edi, int ebp) 
{
    printf("sys_uname: this syscall is not implemented");
    return -1;
}
