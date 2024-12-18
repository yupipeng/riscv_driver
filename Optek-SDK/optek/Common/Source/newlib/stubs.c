/* See LICENSE of license details. */
#include <stdint.h>
#include <errno.h>
#include <time.h>
// #include <unistd.h>
#include "nmsis_gcc.h"
#include "core_feature_base.h"

#include "hw_uart.h"
#include "hal_dma.h"

#define TERMINAL_SUPPORT_CRLF 1

#undef errno
extern int errno;
/*******lptest********/
// #include "fifo2.h"
// #define FIFO_BUFFER_SIZE 128
// uint8_t fifo_buffer[FIFO_BUFFER_SIZE];
// fifo_t printf_fifo;
// #include "os_global.h"
#include "fifo2.h"
/***********end **********/

/* Key stub function for uart io via printf/scanf and heap management */
#undef putchar

int putchar(int dat)
{
    if (dat == '\n'&& TERMINAL_SUPPORT_CRLF) {
        hw_uart_write(UART0, '\r', 1);
    }
    hw_uart_write(UART0, dat, 1);
    return dat;
}

__WEAK size_t _write(int fd, const void* ptr, size_t len)
{
    if (!isatty(fd)) {
        return -1;
    } 

    hw_uart_write(UART0, (const char *)ptr, len);

    return len;
}


// __WEAK int _write(int fd, const void *buf, size_t count) {
//     if (!isatty(fd)) {
//         return -1;
//     } 
//     const char *p = (const char *)buf;
//     for (size_t i = 0; i < count; i++) {
//         putchar(p[i]);
//     }
//     return count;
// }

// /**************** */
// int putchar(uint8_t *dat)
// {
//     if (*dat == '\n') {
//         fifo2_put(&printf_fifo, '\r'); // 处理换行符
//     }
//     if (fifo2_put(&printf_fifo, dat) < 0) {
//         return -1; // FIFO 满时返回错误
//     }

//     return dat;
    
// }
// // 覆写 _write 函数，将数据写入 FIFO
// __WEAK int _write(int fd, const void *buf, size_t count) {
//     const char *p = (const char *)buf;
//     uint8_t data[256];

//     for (size_t i = 0; i < count; i++) {
//         if (fifo2_put(&printf_fifo, p[i]) < 0) { // 如果FIFO满了，数据丢失
           
//             return -1; 
//         }
//         }

//          if (fifo2_get(&printf_fifo, &data) == 0) { // 从FIFO读取数据
//             hw_uart_write(UART0, &data, 1); // 使用串口发送数据
//         } else{
//             return 0;
//         }
        
         


//     }
    
    
// #define UART_AUTO_ECHO

#undef getchar

int getchar(void)
{
    int dat;

//     dat = (int)uart_read(SOC_DEBUG_UART);
// #ifdef UART_AUTO_ECHO
//     uart_write(SOC_DEBUG_UART, (uint8_t)dat);
// #endif
    return dat;
}

ssize_t _read(int fd, void* ptr, size_t len)
{
//    if (fd != STDIN_FILENO) {
//        return -1;
//    }
    ssize_t cnt = 0;
    // uint8_t* readbuf = (uint8_t*)ptr;
    // for (cnt = 0; cnt < len; cnt ++) {
    //     readbuf[cnt] = getchar();
    //     /* Return partial buffer if we get EOL */
    //     if (readbuf[cnt] == '\n') {
    //         return cnt;
    //     }
    // }

    return cnt;
}

__WEAK void* _sbrk(ptrdiff_t incr)
{
    extern char __heap_start[];
    extern char __heap_end[];
    static char* curbrk = __heap_start;

    if ((curbrk + incr < __heap_start) || (curbrk + incr > __heap_end)) {
        return (void*)(-1);
    }

    curbrk += incr;
    return (void*)(curbrk - incr);
}

/* Other newlib stub functions, see https://sourceware.org/newlib/libc.html#Stubs */

int errno;
__WEAK void* __dso_handle = NULL;
// /* version of environ for no OS. */
char *__env[1] = { 0 };
char **environ = __env;

/* Get resolution of clock. */
__WEAK int clock_getres(clockid_t clock_id, struct timespec* res)
{
    // res->tv_sec = 0;
    // res->tv_nsec = 1000000000 / SystemCoreClock;

    return 0;
}

__WEAK int _gettimeofday(struct timeval* tp, void* tzp)
{
    // uint64_t cycles;

    // cycles = __get_rv_cycle();

    // tp->tv_sec = cycles / SystemCoreClock;
    // tp->tv_usec = (cycles % SystemCoreClock) * 1000000 / SystemCoreClock;
    return 0;
}

__WEAK int _isatty(int fd)
{
    return 1;
}

__WEAK int _stat(char* file, struct stat* st)
{
    // st->st_mode = S_IFCHR;
    return 0;
}

__WEAK int _fstat(int file, struct stat* st)
{
    // if ((STDOUT_FILENO == file) || (STDERR_FILENO == file)) {
    //     st->st_mode = S_IFCHR;
        return 0;
    // } else {
    //     errno = EBADF;
    //     return -1;
    // }
}
__WEAK int _chown(const char *path, uid_t owner, gid_t group)
{
    errno = ENOSYS;
    return -1;
}


__WEAK int _open(const char* name, int flags, int mode)
{
    errno = ENOSYS;
    return -1;
}


__WEAK int _lseek(int file, int offset, int whence)
{
    return 0;
}

__WEAK int _link(char* path1, char* path2)
{
    errno = EMLINK;
    return -1;
}

__WEAK int _getpid(void)
{
    return 1;
}

__WEAK int _close(int fd)
{
    errno = EBADF;
    return -1;
}

__WEAK int _unlink(const char* path)
{
    return -1;
}

__WEAK int _fork(void)
{
    errno = ENOSYS;
    return -1;
}

__WEAK int _symlink(const char *path1, const char *path2)
{
    errno = ENOSYS;
    return -1;
}
/* Supply a definition of errno if one not already provided.  */


__WEAK int _execve(char* name, char** argv, char** env)
{
    errno = ENOMEM;
    return -1;
}

__WEAK int _readlink(const char *path, char *buf, size_t bufsize)
{
    errno = ENOSYS;
    return -1;
}

__WEAK int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}


__WEAK void _exit(int fd)
{
    while (1) {
        __WFI();
    }
}

__WEAK int _wait(int* status)
{
    // errno = ECHILD;
    return -1;
}

// extern int _gettimeofday(struct timeval* tp, void* tzp);

__WEAK clock_t _times(struct tms* buf)
{
    // static struct timeval t0;
    // struct timeval t;
    // long long utime;

    // /* When called for the first time, initialize t0. */
    // if (t0.tv_sec == 0 && t0.tv_usec == 0) {
    //     _gettimeofday(&t0, 0);
    // }

    // _gettimeofday(&t, 0);

    // utime = (t.tv_sec - t0.tv_sec) * 1000000 + (t.tv_usec - t0.tv_usec);
    // buf->tms_utime = utime * CLOCKS_PER_SEC / 1000000;
    // buf->tms_stime = buf->tms_cstime = buf->tms_cutime = 0;

    // return buf->tms_utime;
}

/* Set CLOCK to value TP. */
__WEAK int clock_settime(clockid_t clock_id, const struct timespec* tp)
{
    return -1;
}

/* Get current value of CLOCK and store it in tp.  */
__WEAK int clock_gettime(clockid_t clock_id, struct timespec* tp)
{
    // struct timeval tv;
    int retval = -1;

    // retval = _gettimeofday(&tv, NULL);
    // if (retval == 0) {
    //     TIMEVAL_TO_TIMESPEC(&tv, tp);
    // }

    return retval;
}

#include <stdint.h>
#include <time.h>
#include <nuclei_sdk_hal.h>


void exit(int fd)
{
}

// clock() function implementation is added in 0.4.0 sdk release
__WEAK clock_t clock(void)
{
}

// workaround for newlibc required
// __libc_fini_array and __libc_init_array function
__WEAK void __libc_fini_array(void)
{
}

// required by libncrt_2.0.0 in RISC-V GNU Toolchain 2022.06
__WEAK void __libc_init_array(void)
{
}

// weak symbol for back compatiable with previous toolchain
__WEAK void __SEGGER_RTL_init_heap(void *ptr, size_t size)
{

}
