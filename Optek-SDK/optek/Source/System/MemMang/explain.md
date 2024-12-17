# Memory Management
> some memory managment lib is list here, chosen by your require

every lib list here provided c and c++ version standard memory allocator

difference listed below

||TLSF|heap4|
|:-:|:-:|:-:|
|8000次内存分配耗时|33ms|13ms|
|3000次碎片化分配耗时|12ms|106ms|
|单个内存额外开销|8字节|8字节|
|内存固定开销|3224字节|16字节|
|内存碎片合并|支持|支持|
|线程安全|支持|支持|
|多块内存管理|支持|不支持|
|realloc接口|支持|不支持|
|对齐分配|不支持|不支持|
|堆内存溢出检测|不支持|不支持|
|double free检测|不支持|不支持|
|开源协议|LGPL|MIT|


## heap4



## tlsf





## MISC

堆内存使用中常见的4种问题
1. 内存泄漏 (memory leak)
内存使用的基本流程是申请->使用->释放。如果内存不释放就会导致对该段内存的管理失去了控制，如果频繁发生这种问题，就会导致可用内存越来越小。通常来说在程序正常运行过程出现严重的内存泄漏时，通过观察可用内存的变化量可以判定是否出现了内存泄漏问题。如何进一步定位这个内存泄漏点呢？这个可以通过内存申请释放日志分析，如果内存泄漏频率较高能够非常容易就定位到问题所在。对于大块内存的泄漏一般也比较容易发现。如果出现较小内存的泄漏且频率不高的话需要长时间的测试才能发现问题。

2. 重复释放(double free)
内存重复释放比较好理解，就是内存经历了申请->释放->再释放的过程。显然第二次释放这段内存会出现无法预料到的情况。该问题导致的具体现象也不确定。通常只能通过经验分析是否发生了该问题，出现该问题可能导致两个以上的线程出现异常行为。虽然不容易确认是否发生了double free，但是如果确认发生了double free，通常能够通过内存的申请释放日志找到问题点。但是如果某些情况可能会使问题变得比较棘手，即线程A申请内存指针p1，随机释放p1指针，线程B申请到同样的内存指针p1，线程A再次释放指针p1，即线程A释放了两次内存。如果使用log分析，能够看到这个地址的申请和释放都是成对的，很难定位问题点。对于前面这种情况，显然线程B会使用到一块不安全的内存，进一步导致UAF等问题。当出现这样的问题，设计一种malloc\free成对机制就能较快的定位问题，常见的作法是添加动态标记，这样其它线程就不能释放一个不属于它的指针了。

```C
static inline uint32_t malloc_flag_generate(void){
    static uint16_t id = 0;
    return (id++ << 16) | (rand() & 0xffff);
}
void *malloc_f(size_t size, uint32_t *flag){
    if(!flag) return NULL;
    uint32_t *ptr = malloc(size + sizeof(uint32_t));
    *ptr = *flag = malloc_flag_generate();
    return ptr++;
}
void free_f(void *ptr, uint32_t *flag){
    uint32_t *_ptr = ptr;
    _ptr--;
    assert(*flag == *_ptr);
    free(_ptr);
}
```
3. 内存越界(memory overflow)
内存越界不同于前面两种内存释放操作不合理导致的问题，它主要是由于内存申请的长度小于实际想要使用的长度，或者是实际使用的内存长度超过了分配的内存长度。内存越界可能导致篡改其它应用数据。但在某些内存管理器下可能出现破坏堆内存链表的情况，进一步导致的问题就是使得分配器分配出异常的内存空间地址。对于第二种问题情况，具体出现的问题现象也不确定，只能通过经验判断。
检查内存溢出是内存分配器的一个基础安全能力，业内通用的做法是在内存区域的边界添加redzone，在释放内存时检查redzone进行分析，判断是否发生溢出。如何构造redzone也是一个复杂的问题，包括redzone填充的数据以及数据长度，随机的redzone内容和长度是一种安全的算法，但在具体的实施上通常比较麻烦，且在面对精心构造的shellcode时也会出现绕过的情况，应对该问题时一些内存调试手段有助于分析该问题，例如内存断点、陷阱等。

4. 内存非法访问(UAF)
指针p指向一段合法申请的内存空间，当释放p指向的内存空间后，p指针未置NULL，所以理论上来说通过p指针任然能够访问该段内存下的数据。显然后续通过p指针访问数据都属于非法的访问。该问题配合堆内存管理器的一些特性能够导致系统出现严重的安全性问题。该问题通常不易发现，它如果没有造成明显的问题，很难定位问题点。
内存分配器加入一些检测机制能够规避某些场景下的use after free问题，释放内存时，对内存中填充关键字。在分配内存时再检测关键字有没有被污染，以此来判断UAF问题。但是这种检测机制作用有限，它不能检测到上面场景下发生的问题。