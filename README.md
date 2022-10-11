# Linux-.github.io
Linux系统编程【**文件IO、进程、进程间通信、信号、多线程、互斥**】:个人源码+个人笔记

# linux系统编程
个人通过学习，手打了一份48000字的Linux系统编程的笔记，包含了【**文件IO、进程、进程间通信、信号、多线程、互斥**】等知识点，并给出了大量的代码案例对每个重要的知识点进行了代码演示，通过理论和实操的结合，更好的透析每个知识点，为后续的unix网络编程打下基础。


注：本人运行环境mac/Ubuntu+clion+C99

目录：
第二章：gcc、库
第三章：makefile、文件IO、PCB概念
第四章：文件IO操作
第五章：进程
第六章：进程间通信
第七章：信号
第八章：多线程编程、互斥、条件变量、信号量
# 第二章
## 1. Vim三种工作模式
Vi有三种基本工作模式: 命令模式、文本输入模式、末行模式。
![在这里插入图片描述](https://img-blog.csdnimg.cn/c06dc943c1584f27b543875679a8d174.png)
通过shell命令进入命令模式。
1. 用户按下esc键, 就可以使vi进入命令模式下; 当使用vi打开一个新文件开始也是进入命令模式下.
2. 从命令模式切换到文本输入模式只需输入如下命令: i
3. 从命令模式切换到末行模式, 输入冒号:,存在以下命令
![末行模式的命令](https://img-blog.csdnimg.cn/5b7ec1d194c34fd390dca545d7e5c875.png)

## 2. gcc
### gcc的工作流程
从hello.c到可执行文件实际上有四步
1. **预处理:	gcc -E test.c -o test.i //-o就是output到某文件**
- .i结尾
- 头文件展开----不检查语法错误，可以展开任意文件
- 宏定义替换。 --- 将宏名替换回宏值。

2. **编译:gcc -S test.i -o test.s**
- .s结尾
- 逐行检查语法错误【重点】，耗时最长
- 将c翻译成汇编指令
	
3. **汇编:gcc -c test.s -o test.o**
- .o结尾
- 将汇编指令翻译成二进制编码

5. **链接:gcc -o test test.o**
**or** gcc test.c -o test（直接）
- 无结尾
- 链接test.c代码中调用的库函数
### gcc常用命令
- -v  查看gcc版本号, --version也可以
- -E  生成预处理文件
- -S  生成汇编文件
- -c  只编译, 生成.o文件, 通常称为目标文件
- -I   指定头文件所在的路径
- -L   指定库文件所在的路径
- -l    指定库的名字
- -o   指定生成的目标文件的名字
- -g   包含调试信息, 使用gdb调试需要添加-g参数

## 3.  静态库和共享库
### 3.1 库的介绍
库是二进制文件, 是源代码文件的另一种表现形式, 是**加了密的源代码**;  是一些功能相近或者是相似的函数的集合体.
### 3.2 库制作完成后, 如何给用户使用
需要两个文件，用户才能使用：
- 头文件(.h文件)---包含了库函数的声明
- 库文件(.a文件或者.so文件)---包含了库函数的代码实现

<font color='red'> 注意: 库不能单独使用, 只能作为其他执行程序的一部分完成某些功能, 也就是说只能被其他程序调用才能使用.
 </font>
###  3.3 静态库（static library）
静态库可以认为是一些目标代码的集合, 是在**可执行程序运行前就已经加入到执行码中**, 成为执行程序的一部分. 按照习惯, 一般以.a做为文件后缀名。静态库文件一般由**多个.o文件制作成一个.a文件**，具体看下一小节**静态库的制作**
- 前缀：lib
- 库名称：自定义即可, 如test
- 后缀：.a

所以最终的静态库的名字应该为：libtest.a
&emsp;&emsp;注意：一个.o文件也能看作是库文件，是库文件的一个特例，库文件一般由多个.o文件构成

#### 3.3.1 静态库的制作
1. 将.c文件制作成.o文件
```
gcc -c fun1.c fun2.c
//可以不加-o默认生成对应的.o文件
```

3. 使用ar命令将.o文件打包成.a文件
```
ar rcs libtest1.a fun1.o fun2.o
```
#### 3.3.2 静态库的使用
1. main.c(调用了库文件的文件)与head.h和libtest1.a**在同一级目录的情况**
```
gcc -o main1 main.c -I./ -L./ -l test1
```
- 注:-I(i的大写)   指定头文件所在的路径
&emsp;-L   指定库文件所在的路径
&emsp;-l    指定库的名字

2. main.c与head.h和libtest1.a在**不同一级目录的情况**
```
gcc -o main1 main.c -I./include -L./lib -l test1
```
### 3.4 动态库

静态库是.a结尾，动态库则是以.so结尾，还没学，略。
# 第三章
## 1.  Makefile

makefile文件中定义了一系列的规则来指定, 哪些文件需要先编译, 哪些文件需要后编译, 哪些文件需要重新编译, 甚至于进行更复杂的功能操作, 因为makefile就像一个Shell脚本一样, 其中也可以执行操作系统的命令.  makefile带来的好处就是——“自动化编译”, 一旦写好, **只需要一个make命令, 整个工程完全自动编译**, 极大的提高了软件开发的效率。 makefile文件是用来管理项目工程文件，通过执行make命令，**make就会自动解析并执行makefile文件**。

### 1.1 makefile的基本规则
makefile由一组规则组成,规则如下:
目标（要生成的目标文件）: 依赖(生成目标所依赖的文件)
（tab）命令

makefile基本规则三要素：
- 目标: 要生成的目标文件
- 依赖: 目标文件由哪些文件生成
- 命令: 通过执行该命令由依赖文件生成目标

例如
```
main: main.c fun1.c fun2.c
	gcc -o main main.c fun1.c fun2.c
```

### 1.2 makefile的工作原理

要想生成目标文件，先要检查依赖条件是否都存在：
- 若都存在，则比较目标时间和依赖的时间，如果依赖的时间比目标的时间新，
则重新生成目标；否则不重新生成。
- 若不存在，则往下找有没有生成依赖的规则，有则生成，没有则报错。
![在这里插入图片描述](https://img-blog.csdnimg.cn/94421db31bc24e81ad56c1abbec1a820.png)
### 1.3 makefile中的变量（不记，只当需要的时候查找使用）
在makefile中使用变量有点类似于C语言中的宏定义, 使用该变量相当于内容替换, 使用变量可以使makefile易于维护, 修改起来变得简单。
makefile有三种类型的变量:
- 普通变量: var=hello, $(var)
- 自带变量:
&emsp;CC = gcc #arm-linux-gcc
&emsp;CPPFLAGS : C预处理的选项 -I
&emsp;CFLAGS:   C编译器的选项 -Wall -g -c
&emsp;LDFLAGS :  链接器选项 -L  -l
- 自动变量(**特别注意：自动变量只能在规则的命令中使用**.)
&emsp; $@: 表示规则中的目标
&emsp; $<: 表示规则中的第一个依赖条件
&emsp; $^: 表示规则中的所有依赖条件, 组成一个列表, 以空格隔开, 如果这个列表中有重复的项则消除重复项。

模式规则:
&emsp; %.0:%.c //前后的%代表字符串，必须相同。
比如：main.o:main.c  fun1.o: fun1.c  fun2.o:fun2.c, 说的简单点就是: xxx.o:xxx.c(%.0:%.c)

### 1.4 makefile函数与清理
标记一下，没学。对应07-makefile的第五个版本。

## 2.  gdb
标记一下，美学。对应08-gdb介绍 到 4-gdb调试综合练习

## 3.  文件IO
C语言使用fopen函数打开一个文件, 返回一个FILE* fp, fp指向堆空间，堆空间存放的不是文件本身，而是文件的描述信息的结构体。
![36a7b770c79198361d.png)](https://img-blog.csdnimg.cn/7e81c82117be4a4f80023554364022a2.png)
这个指针指向的结构体有三个重要的成员：
- 	**文件描述符:** 通过文件描述符可以找到文件的**inode**, 通过inode可以找到对应的数据块
- **文件指针**: 读和写共享一个文件指针, 读或者写都会引起文件指针的变化
- **文件缓冲区**: 读或者写会先通过文件缓冲区, 主要目的是为了减少对磁盘的读写次数, 提高读写磁盘的效率.

## 4.  C库函数和系统函数
![在这里插入图片描述](https://img-blog.csdnimg.cn/2a830427f61146cd8ab42f1a907a811e.png)

## 5. 虚拟地址空间
用户区的内存4区模型：
	
- 代码段：.text段。 程序源代码（二进制形式）。

- 数据段：只读数据段 .rodata段。初始化数据段 .data段。 未初始化数据段 .bss 段。

- stack：栈。 在其之上开辟 栈帧。	windows 1M --- 10M	Linux： 8M --- 16M

- heap：堆。 给用户自定义数据提供空间。 约 1.3G+

进程的虚拟地址空间分为**用户区**和**内核**区, 其中内核区是受保护的, 用户是不能够对其进行读写操作的;

内核区中很重要的一个就是进程管理, 进程管理中有一个区域就是PCB(**本质是一个结构体**)，称进程控制块;
PCB中有文件描述符表, **文件描述符表中存放着打开的文件描述符**, 涉及到文件的IO操作都会用到这个文件描述符.


## 6. PCB与文件描述符
每一个进程有一个PCB。PCB是一个结构体，其中有一项是文件描述符表，文件描述符表中存放着打开的文件描述符。

一个进程默认会打开三个文件：标准输入，标准输出，标准出错，对应index：0，1，2。每当进程每打开一个文件，就会在文件描述符表上记录一个文件描述符。

![在这里插入图片描述](https://img-blog.csdnimg.cn/d499f0ad0f38482e880376b1d1130dfe.png)
&emsp;&emsp;强调：文件描述符的作用：通过文件描述符可以找到文件的**inode**, 通过inode可以找到对应的数据块。

# 第四章（文件IO操作）
## 1. open、close、read、write、lseek、dup、dup2、fcntl

### 1. open函数

头文件
#include <fcntl.h>
#include <sys/stat.h>

- **函数原型:**
	- int open(const char *pathname, int flags);
	- int open(const char *pathname, int flags, mode_t mode);
- **函数描述:**
	 - 打开或者新建一个文件
 -  **函数参数：**
	- pathname参数是要打开或创建的文件名,和fopen一样, pathname既可以是相对路径也可以是绝对路径。
	- flag: 必选项:**以下三个常数中必须指定一个, 且仅允许指定一个。**
		- O_RDONLY 只读打开
		- O_WRONLY 只写打开
		- **O_RDWR** 可读可写打开
	- 以下可选项可以同时指定0个或多个, 和**必选项按位或起来作为flags参数**。可选项有很多, 这里只介绍几个常用选项：
		- **O_APPEND** 表示追加。如果文件已有内容, 这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容。
		- **O_CREAT** 若此文件不存在则创建它。使用此选项时需要提供第三个参数mode, 表示该文件的访问权限。
文件最终权限：mode & ~umask
		- O_EXCL 如果同时指定了O_CREAT,并且文件已存在,则出错返回。
		- O_TRUNC 如果文件已存在, 将其长度截断为为0字节。
		- O_NONBLOCK 对于设备文件, 以O_NONBLOCK方式打开可以做非阻塞I/O(NonblockI/O),非阻塞I/O。

3. mode:一个整数参数，用来指定创建文件的权限，可以为数字，或宏
	直接写0755
~~- S_IRWXU:指定创建文件的用户的权限为可读,可写，可执行，等价00700~~ 
~~- S_IRUSR:指定创建文件的用户的权限为可读，等价00400~~ 
~~- S_IWUSR:指定创建文件的用户的权限为可写，等价00200~~ 
~~- S_IXUSR:指定创建文件的用户的权限为可执行，等价00100~~ 

open函数常这么用：
```cpp
int fd = open("test.log", O_RDWR | OCREAT, 0755)；
```
- **函数返回值:**
	- 成功: 返回一个最小且未被占用的文件描述符
	- 失败: 返回-1, 并设置errno值.

### 2. close函数
- **函数原型:**
	- int close(int fd);
- **函数描述:**
	- 关闭文件
- **函数参数：**
	- fd: 文件描述符
- **函数返回值:**
	- 成功: 返回0.
	- 失败: 返回-1, 并设置errno值.

需要说明的是,当一个进程终止时, 内核对该进程所有尚未关闭的文件描述符调用close关闭,所以即使用户程序不调用close, 在终止时内核也会自动关闭它打开的所有文件。但是对于一个长年累月运行的程序(比如网络服务器), 打开的文件描述符一定要记得关闭, 否则随着打开的文件越来越多, 会占用大量文件描述符和系统资源。

### 3. read函数
- **函数原型:**
	- ssize_t read(int fd, void *buf, size_t count);
- **函数描述:**
	- 从打开的设备或文件中读取数据
- **函数参数：**
	- fd: 文件描述符
	- buf: 读上来的数据保存在缓冲区buf中
	- count: buf缓冲区存放的最大字节数
- **函数返回值:**
	- \>0：读取到的字节数
	- =0：文件读取完毕
	- -1： 出错，并设置errno
### 4. write函数
- **4.1 函数原型:**
	- ssize_t write(int fd, const void *buf, size_t count);
- **4.2 函数描述:**
	- 向打开的设备或文件中写数据
- **4.3 函数参数：**
	- fd: 文件描述符
	- buf: 缓冲区，要写入文件或设备的数据
	- count：buf中数据的长度
- **4.4 函数返回值:**
	- 成功：返回写入的字节数
	- 错误：返回-1并设置errno

### 5. lseek函数--修改文件偏移量
- **5.1 函数原型:**
	- off_t lseek(int fd, off_t offset, int whence);
- **5.2 函数描述:**
	- 所有打开的文件都有一个当前**文件偏移量**(current file offset),以下简称为cfo. cfo通常是一个非负整数, 用于表明文件开始处到文件当前位置的字节数. 读写操作通常开始于 cfo, 并且使 cfo 增大, 增量为读写的字节数. 文件被打开时, cfo 会被初始化为 0, 除非使用了 O_APPEND.
使用 lseek 函数可以改变文件的 cfo.
- **5.3 函数参数：**
	- fd: 文件描述符
	- offset：文件偏移量， offset 的含义取决于参数 whence。
	- whence：
		- SEEK_SET：文件开头位置、
		- SEEK_CUR：当前位置、
		- SEEK_END：文件结尾位置
- **5.4 函数返回值:**
	- 若lseek成功执行, 则返回新的偏移量。
### 6. dup函数---复制文件描述符
头文件 <unistd.h>
- **6.1 函数原型:**
	- int dup(int oldfd);
- **6.2 函数描述:**
	- 复制的文件描述符。当调用dup函数后，oldfd和返回的int变量都指向同一个文件。
```cpp
int newfd = dup(fd);//此时newfd和fd这两个文件描述符都指向同一个文件
```
- **6.3 函数参数：**
	- oldfd -要复制的文件描述符
- **6.4 函数返回值:**
	- 成功：返回最小且没被占用的文件描述符
	- 错误：返回-1并设置errno

<font color='red'> 注意：当调用dup后，内核会维护**描述符计数**，close一个文件描述符时，计数-1，只有减到0时，文件才真正的关闭。 </font>


### 7. dup2函数 ---指定一个文件描述符去复制成新的文件描述符
- **7.1 函数原型:**
	- int dup2(int oldfd, int newfd);
- **7.2 函数描述:**
	- 复制文件描述符
- **7.3 函数参数：**
	- oldfd-原来的文件描述符
	- newfd-复制成的新的文件描述符
- **7.4 函数返回值:**
	- 成功：返回0。并将oldfd复制给newfd, 两个文件描述符指向同一个文件
	- 错误：返回-1并设置errno

假设newfd已经指向了一个文件，首先close原来打开的文件，然后newfd指向oldfd指向的文件.
若newfd没有被占用，newfd指向oldfd指向的文件.

调用dup2后，内核会修改内部的描述符计数为2。

**7.5 重定向操作:**

dup2可以**实现重定向操作**：意思就是printf的时候可以不把内容输出到终端，而是输出到指定的文件上
```cpp
//dup2实现文件重定向操作
int main(int argc, char * argv[])
{
    //打开文件
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    //调用dup2实现文件重定向
    dup2(fd, STDOUT_FILENO);
    printf("你好宝贝");
    return 0;
}
```

实现文件重定向原理
![在这里插入图片描述](https://img-blog.csdnimg.cn/b5cec653bfe14da29f251ca2cfc20f98.png)
所以当调用dup2时，会把STDOUT_FILENO指向其他文件，所以printf会把内容输出到对应文件。

### 8. fcntl----改变打开文件的属性
头文件 <fcntl.h>
- **函数原型:**
	- int fcntl(int fd, int cmd, ... /* arg */ );
- **函数描述:**
	- 改变已经打开的文件的属性。用法比较多
- **函数参数：**
	- fd: 文件描述符
	- cmd: 
		- 若cmd为F_DUPFD, 复制文件描述符, 与dup相同
		- 若cmd为F_GETFL, 获取文件描述符的flag属性值
		- 若cmd为 F_SETFL, 设置文件描述符的flag属性
	- /* arg */:可变参数
- **函数返回值:**
- **函数返回值**: 返回值取决于cmd
	- 成功:
		- 若cmd为F_DUPFD, 返回一个新的文件描述符
		- 若cmd为F_GETFL, 返回文件描述符的flags值
		-  cmd为 F_SETFL, 返回0
	- 失败：返回-1, 并设置errno值.

fcntl函数常用的操作:
1.  复制一个新的文件描述符:
int newfd = fcntl(fd, F_DUPFD, 0);
2. 获取文件的属性标志
int flag = fcntl(fd, F_GETFL, 0)
3. 设置文件状态标志
flag = flag | O_APPEND;
fcntl(fd, F_SETFL, flag)
4.  常用的属性标志
O_APPEND-----设置文件打开为末尾添加
O_NONBLOCK-----设置打开的文件描述符为非阻塞

fcntl常用函数:
```cpp
//获得和设置文件描述符的flag属性: 
int flag = fcntl(fd, F_GETFL, 0);
flag |= O_APPEND;//末尾追加
fcntl(fd, F_SETFL, flag);

//获取和设置文件描述符为非阻塞
int flags = fcntl(fd[0], F_GETFL, 0); 
flag |= O_NONBLOCK;//非阻塞
fcntl(fd[0], F_SETFL, flags);

```

# 第五章（进程）
## 1. 程序与进程
- 程序，是指编译好的二进制文件，在磁盘上，占用磁盘空间, 是一个**静态**的概念.
- 进程，一个启动的程序， 进程占用的是系统资源，如：物理内存，CPU，终端等，是一个**动态**的概念

## 2. 并行和并发
- 并发：在一个时间段内, 是在同一个cpu上, 同时运行多个程序。（时间片等）
- 并行：并行性指两个或两个以上的程序在同一时刻发生(需要有多颗cpu)。

## 3. PCB-进程控制块
每个进程在内核中都有一个进程控制块（PCB）来维护进程相关的信息，Linux内核的进程控制块是task_struct结构体。

PCB中有很多信息：主要有
- 进程id。系统中每个进程有唯一的id，在C语言中用pid_t类型表示，其实就是一个非负整数。
- 进程的状态，有初始、就绪、运行、阻塞（挂起）、停止五个状态。
- 进程切换时需要保存和恢复的一些CPU寄存器。
- 描述虚拟地址空间的信息。
- 描述控制终端的信息。
- 当前工作目录（Current Working Directory）。
- umask掩码。（用于获取文件的权限信息）
- 文件描述符表，包含很多指向file结构体的指针。
- 和信号相关的信息。
- 用户id和组id。
- 会话（Session）和进程组。
- 进程可以使用的资源上限（Resource Limit）。ulimit -a


## 4. 进程状态的转化
![](https://img-blog.csdnimg.cn/b0e5bddfa12f4c40b37d34991f0ddcef.png)

## 5. fork()
头文件 <unistd.h>
### 5.1 fork()原理
fork函数的作用：产生子进程

fork函数一次调用，两次返回：父进程返回子进程的ID，子进程返回0

fork函数的执行逻辑
![在这里插入图片描述](https://img-blog.csdnimg.cn/b83bb96bec6a4a3eb790107373339e37.png)


调用fork函数的内核实现原理：除了修改内核区中PCB的进程id外，其他全部复制一份到子进程.(**最重要的是，文件描述符也会一同复制过去**)

![在这里插入图片描述](https://img-blog.csdnimg.cn/267b29efac2d4ab2a19bd586d67e5a0b.png)
fork函数总结：
1.fork函数的返回值：
- 父进程返回子进程的PID，是一个大于0数;
- 子进程返回0；
- 特别需要注意的是：**不是fork函数在一个进程中返回2个值，而是在父子进程各自返回一个值。**
2. 子进程创建成功后，代码的执行位置：父进程执行到什么位置，子进程就从哪里执行
3. 区分父子进程：通过fork函数的返回值
4. 父子进程的执行顺序：不一定，哪个进程先抢到CPU，哪个进程就先执行

### 5.2 fork()代码
```cpp
pid_t pid=fork()
if(pid<0)
{
	printf("创建失败");
	exit(1);
}
if(pid==0)//子进程执行的片段
{
	pid=getpid();//获取当前进程的pid
	pid=getppid();//获取当前进程的父进程pid
}
if(pid>0)//父进程执行的片段
{
	yyy;
}
```
当父进程比子进程先结束时，子进程的父进程pid会变成1，由init进程进行领养。

### 5.3 父子进程不能共享全局变量
如标题所示，原因是：会把父进程的用户区完全copy一份给子进程，所以子进程也有自己的全局变量。而且，如果共享的话，还要**进程间通信**干嘛？

写时复制，读时共享：
- 如果父子进程仅仅是对变量做“读”操作，这时候该变量在物理内存中是共享的（节省空间）。
- 如果说父子进程中的某进程对变量进行修改，则会复制一份该数据的副本，在副本上对该数据进行修改，并映射回虚拟地址。如图所示
![看图中间的箭头](https://img-blog.csdnimg.cn/d0ba96fc080f46588c3c8b6405d199ce.png)


所以父子进程不能通过全局变量进程通信！如果父子进程想要通信，必须借助其他工具。

## 6. ps命令和kill命令



### 6.1 ps----查看进程相关信息
ps：查看进程相关信息。

ps aux | grep "xxx"//grep 关键字:可以查看启动信息中包含关键字的进程
ps ajx | grep "xxx"
- -a：（all）当前系统所有用户的进程
- -u：查看进程所有者及其他一些信息
- -x：显示没有控制终端的进程 -- 不能与用户进行交互的进程【输入、输出】
- -j: 列出与作业控制相关的信息
### 6.2 kill----给进程发送信号
kill：**用于向运行中的进程发送信号**，默认发送的信号是终止信号，会请求进程退出。不一定是杀死进程，也可以发送其他信号。

kill -l 查看系统有哪些信号
kill -9 pid 杀死某个线程


## 7. exec----在进程中执行命令或程序
exec：想在一个进程中：**执行一个应用程序**或者想**执行一个系统命令**，应先fork，再在子进程中执行execl拉起可执行程序或者命令。
```c
pid = fork()
if(pid==0)//让子进程执行
{
	int excel (path,"ls", "-l", NULL)
	//调用execl函数后，子进程的代码段会被ls命令代码段替换。
	//注意：子进程的地址空间没有变化 ，子进程的PID	也没有变化。
}
```

execl实现原理：
&emsp;&emsp;当调用子进程使用execl时，子进程的地址空间中的代码段(.txt)、数据段，堆和栈等将被替换成execl中所执行的新进程。原有的进程空间没有发生变化，**并没有创建新的进程**，进程PID没有发生变化。
![在这里插入图片描述](https://img-blog.csdnimg.cn/aae110cbc8384be98e7df75c64e52ae8.png)

exec有六大函数，记住两个主要的函数：
### 7.1  execl()  ----用于自己写的应用程序

一般execl用于**自己写的一个应用程序**

函数原型：
&emsp;&emsp;int execl("绝对路径", "标识符",  "参数1","参数2" ,NULL);
```cpp
excel("./test", "test", "Hello","world", NULL);
excel("/bin/ls", "ls", "-l", NULL);
```
参数介绍：
- 绝对路径	const char* ，文件存储的绝对路径，可通过pwd命令查看
- 标识符(占位符)	const char* ，取个别名
- 参数	------	执行该程序/命令所需要的参数(例如ls的-l)
- NULL	------	最后这个必须传NULL，否则函数会报错

返回值：**若是成功，则不返回，不会再执行exec函数后面的代码；**
若是失败，会执行execl 后面的代码，可以用perror打印错误原因。

### 7.2 execlp()  ----执行系统命令

一般execlp用于**执行系统命令**

函数原型:
&emsp;&emsp;int execlp("命令名/程序名"，"标识符", "参数1","参数2", NULL);
```cpp
execlp("ls", "ls", "-l", NULL);
```
参数介绍：
- 命令名/程序名：执行命令的名字(例如ls)，根据**PATH 环境变量**来搜索该命令(加上路径也可以，如果执行命令就不需要加路径，如果执行程序，就把路径加上)
- 标识符(占位符)	const char* ，取个别名
- 参数	------	执行该程序/命令所需要的参数(例如ls的-l)
- NULL	------	最后这个必须传NULL，否则函数会报错

返回值：**若是成功，则不返回，不会再执行exec函数后面的代码**；若是失败，会执行


## 8. 子进程资源回收
当一个子进程退出之后，子进程只能**回收自己的用户区的资源**，但是不能回收内核空间的PCB资源，必须由它的父进程调用**wait**或者**waitpid函数**完成对子进程资源的回收，避免造成系统资源的浪费。

## 9. 孤儿进程
若父进程先退出，子进程就变成孤儿进程。

并且为了保证每个进程都有一个父进程，孤儿进程会被init进程领养，init进程成为了孤儿进程的养父进程，当孤儿进程退出之后，由init进程完成对孤儿进程的回收。
## 10. 僵尸进程
子进程先退出，父进程没有完成对子进程的回收（回收内核区资源），此时子进程变成僵尸进程。
如何解决僵尸进程：
&emsp;&emsp;使用杀死僵尸进程父进程的方法来解决僵尸进程。原因是：杀死其父进程可以让init进程领养僵尸进程,最后由init进程回收僵尸进程.

## 11. 子进程回收函数
头文件 <sys/wait.h>
　<font color='red'>所有的进程回收函数都必须由**父进程**完成！！ </font>
### 11.1 wait函数

**函数原型**：pid_t wait(int *status);
**函数作用：**
- **阻塞**并等待子进程退出：会阻塞父进程！父进程会等到子进程执行完退出才接着运行父进程。
- 回收子进程残留资源 
- 获取子进程结束状态(退出原因)。

**返回值：**
- 成功：清理掉的子进程ID；
- 失败：-1 (没有子进程)

**status参数**:传出参数(有点像值-结果参数)
- WIFEXITED(status)：为非0        → 进程正常结束
- WEXITSTATUS(status)：获取进程退出状态 
- WIFSIGNALED(status)：为非0 → 进程被信号杀死的
- WTERMSIG(status)：取得进程终止的信号编号。

如果对状态不关心，就传NULL就行。

```cpp
pid_t pid=fork()

if(pid>0)//父进程执行
{
	printf("this is father pid=%d", getpid());
	pid_t w_pid=wait(NULL);//在父进程执行wait函数进行子进程的资源回收
}
if(pid==0)//子进程执行
{
	printf("this is son pid=%d", getpid());
}
```

### 11.2 waitpid函数
**函数原型**：pid_t waitpid(pid_t pid, int *status, in options);
**函数作用：**
同wait函数。但是比wait函数灵活，**可以控制是否阻塞父进程**。
**函数参数:**
- pid：
 &emsp;&emsp;pid = -1 等待任一子进程。与wait同效果。
 &emsp;&emsp;pid > 0 等待其进程ID与pid相等的子进程。
 - status: 子进程的退出状态，用法同wait函数。
 - options：设置父进程是否阻塞，默认阻塞。设置为WNOHANG，函数非阻塞; 设置为0，函数阻塞。
 
**返回值：**
- \>0：返回回收掉的子进程ID；
- -1：无子进程
- =0：参3为WNOHANG情况下，且目前没有子进程退出。


# 第六章（进程间通信）
## 1. 进程间通信的基本概念
Linux环境下，**进程地址空间相互独立**，**每个进程各自有不同的用户地址空间**。**任何一个进程的全局变量在另一个进程中都看不到**，**所以进程和进程之间不能相互访问**，<font color='red'> 要交换数据必须通过**内核** </font>。具体来说，在内核中开辟一块缓冲区，进程1把数据从用户空间拷到内核缓冲区，进程2再从内核缓冲区把数据读走，内核提供的这种机制称为**进程间通信**（**IPC**，InterProcess Communication）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/596a8238029e44cead74021b739fb82e.png)
## 2. 进程间通信的方式
在进程间完成数据传递需要借助操作系统提供特殊的方法,现今常用的进程间通信方式有：
- **管道: PIPE、FIFO** (使用最简单)
- **信号** (开销最小)
- **共享映射区** (无血缘关系)
- **本地套接字** (最稳定)

## 3. 利用管道的方式进行进程间通信（父子进程之间）
**管道应用于有血缘关系的进程之间。**
### 3.1 管道的概念
管道是一种最基本的IPC机制，也称匿名管道，应用于**有血缘关系的进程之间**，进行数据传递。调用pipe函数即可创建一个管道。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2fc610dd709649758664f9ae932d1f9a.png)



管道的特点
- 管道的本质是一块内核缓冲区 ，**数据只要读就没了。**（防止管道满）
- 由两个**文件描述符引用**，一个表示读端，一个表示写端。
- 规定数据从管道的写端流入管道，从读端流出。
- 当两个进程都终止的时候，管道也自动消失。
- <font color='red'> 管道的读端和写端默认都是**阻塞**的</font>

注意：父进程创建管道，子进程会继承。

**原理：**
- 管道内部使用**环形队列**实现。
- 默认缓冲区大小为4K，可以使用ulimit -a命令获取大小。
- 实际操作过程中缓冲区会根据数据压力做适当调整。

**局限性**：
- 数据一旦被读走，便不在管道中存在，**不可反复读取**。
- **数据只能在一个方向上流动**，若要实现双向流动，必须使用两个管道
- 只能在**有血缘关系的进程间**使用管道。

为什么管道只能在有血缘关系的进程间使用：
&emsp;&emsp;管道的两端是通过文件描述符表示的，如果是两个毫不相关的进程，在一个进程内创建了管道，没办法让另一个进程获得该管道的文件描述符。但是在父子进程中就可以。

### 3.2 pipe( fd )---创建管道
头文件： <unistd.h3>

- **函数原型**:
	- int pipe(int fd[2]);
- **函数作用:** 创建一个管道
- **函数参数:** 
	- 若函数调用成功，**fd[0]存放管道的读端，fd[1]存放管道的写端**
- **返回值:**
	- 成功返回0；
	- 失败返回-1，并设置errno值。

用法：
```cpp
int fd[2];
int result=pipe(fd);
```
函数调用成功返回读端和写端的文件描述符，**向管道读写数据是通过使用这两个文件描述符进行**的，**读写管道的实质是操作内核缓冲区**。对管道的读写操作其实就有点像对文件的操作。

管道创建成功以后，创建该管道的进程（父进程）同时掌握着管道的读端和写端。如何实现父子进程间通信呢？

### 3.3 代码实现父子进程使用管道通信
创建管道通信的步骤：
1. 父进程**调用pipe函数创建管道**，得到两个文件描述符fd[0]和fd[1]，分别指向管道的读端和写端。
2. 父进程**调用fork创建子进程**，那么子进程也有两个文件描述符指向同一管。
3. **父进程关闭管道读端**，**子进程关闭管道写端**。父进程可以向管道中写入数据，子进程将管道中的数据读出，这样就实现了父子进程间通信。

注意 **管道的读端是阻塞的，如果没有数据，会阻塞到有数据才运行。 
管道的写端是阻塞的，如果数据满了，会阻塞到可以写数据时才运行。**

```cpp
//管道通信的简单案例
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//利用管道实现父子间的进程通信


int main() {
    int fd[2];
    int result= pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }
    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork error");
        return -1;
    }
    else if(pid>0)//父进程
    {
        //关闭管道读端
        close(fd[0]);
        //向管道写入数据
        write(fd[1],"Hello world", sizeof("Hello world"));

        wait(NULL);
//        waitpid(-1,NULL,0);
    }
    else//pid==0，子进程
    {
        //关闭管道写端
        close(fd[1]);
        //向管道读取数据
        char buf[100];
        //初始化buf
        memset(buf,0x00,sizeof(buf));
        int n = read(fd[0], buf, sizeof(buf));
        printf("read over，n==[%d], text=[%s]", n, buf);

    }
    return -1;
}
```

### 3.4 进程的管道通信执行命令ps aux | grep bash
在父子进程间完成ps aux | grep bash命令，ps aux在父进程执行，grep bash在子进程执行。

这个代码需要运用到不同的知识点。
具体需求分析如下：
- 父子的进程通信，利用pipe
- 创建进程，利用fork
- 在父进程中将标准输出重定向管道写端，利用dup2重定向
- 在子进程中将标准输入重定向管道读端，利用dup2重定向
- 在父子进程中调用系统命令，利用execlp
- 父进程回收子进程资源，利用wait

具体重定向分析：
1. 父进程中执行ps aux，那么命令将会输出到标准输出，但是我们想让他输出到管道的写端，**dup2(fd[1], STDOUT_FILENO)**。**当调用这个函数后，ps aux将不会往标准输出写，而是往管道写端写。**
2. 子进程执行grep bash，那么该命令将会从标准输入中读数据，但是我们向让他从管道的读端读数据，**dup(fd[0], STDOUT_FILENO)**。

整体流程：
1. 创建管道pipe
2. 创建子进程fork
3. 在父进程中关闭读端fd[0]
4. 在子进程中关闭写端fd1]
5. 在父进程中将标准输出重定向到管道的写端
6. 在子进程中将标准输入重定向到管道的读端
7. 在父进程中调用execl函数执行ps aux命令
8. 在子进程中调用execl函数执行grep bash命令
9. 在父进程中回收子进程wait函致

```cpp
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>

//实现ps aux|grep bash
int main() {
    // 1. 开通管道
    int fd[2];
    int result=pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }
    //2. 创建子进程
    pid_t pid=fork();

    if(pid<0)
    {
        perror("pipe error");
        return -1;
    }
    if(pid>0)//父进程
    {
        //父进程关闭读端
        close(fd[0]);
        //标准输出重定向到管道写端
        dup2(fd[1],STDOUT_FILENO);
        execlp("ps", "ps", "aux", NULL);

        perror("execlp error");//当execlp执行成功，则不会执行后续代码。若执行失败，打印错误
        wait(NULL);
    }
    if(pid==0)//子进程
    {
        //子进程关闭写端
        close(fd[1]);
        //标准输入重定向到管道读端
        dup2(fd[0],STDIN_FILENO);
        execlp("grep","grep","bash", NULL);
        perror("execlp error");
    }
    return 0;
}
```

### 3.5 管道的读写行为详解
**对于读端而言：**
- 如果有数据:
--read正常读，返回读出的数据
- 如果无数据:
--若写端 ( close(fd[1]) ) 全部关闭了，read解除阻塞，返回0。
--若写端没有全部关闭，read阻塞

**对于写端而言：**
- 若读端全部关闭：管道破裂，进程直接终止，内核给当前进程发送SIGPIPE信号。
- 若读端没有全部关闭:
-- 缓冲区如果写满了，write阻塞。
--缓冲区如果没有满，继续write。

### 3.6 设置管道为非阻塞
由之前学的可知，**管道读写两端是默认阻塞的**，可以通过fcntl(fd[0], F_SETFL, flags)函数，将管道的读写两端设置成为非阻塞
```cpp
int test1()
{
    int fd[2];
    int result= pipe(fd);
    if(result<0)
    {
        perror("pipe error");
        return -1;
    }

    //设置为非阻塞
    int flags=fcntl(fd[0], F_GETFL,0);
    flags = flags | O_NONBLOCK;
    fcntl(fd[0], F_SETFL, flags);

    char buf[100];
    memset(buf,0x00,sizeof(buf));
    //此时管道没有写入数据，如果没有设置成非阻塞，进程将一直阻塞在这
    read(fd[0],buf, sizeof(buf));

    return -1;
}
```
## 4. 利用FIFO的方式进行进程间通信
头文件：
#include <sys/types.h>
#include <sys/stat.h>
### 4.1 FIFO的概念
FIFO常被称为**命名管道**。管道(pipe)只能用于“有血缘关系”的进程间通信。**但通过FIFO，不相关的进程也能交换数据。**


FIFO是Linux基础文件类型中的一种（文件类型为p，可通过ls -l查看文件类型）。但FIFO文件在磁盘上没有数据块，文件大小为0，**仅仅用来标识内核中一条通道**。进程可以打开这个文件进行read/write，实际上是在读写内核缓冲区，这样就实现了进程间通信。

**FIFO 不同于PIPE的地方在于，他是双向的，两端都可读可写。**

### 4.2 创建管道
1. 方式1-使用**终端命令** mkfifo
命令格式： mkfifo 管道名
例如：mkfifo myfifo
2. 方式2-使用**函数**
int mkfifo(const char *pathname, mode_t mode);
```cpp
int result=mkfifo("./myfifo", 0777);
```

当创建了一个FIFO，就可以使用open函数打开它，**常见的文件I/O函数都可用于FIFO**。如：close、read、write、unlink等。

FIFO严格遵循**先进先出**（first in first out），对FIFO的读总是从开始处返回数据，对它们的写则把数据添加到末尾。**FIFO不支持诸如lseek()等文件定位操作。**


### 4.3 使用FIFO完成两个不同进程间的通信

![在这里插入图片描述](https://img-blog.csdnimg.cn/62437c148fc44016a04864d8946517db.png)
创建完FIFO后，就和普通文件一样操作就行：一个进程写，另一个进程读。

具体代码案例:

FIFO_write.c:
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <sys/stat.h>
//写入FIFO
int main()
{
    //判断FIFO是否存在，如果不存在则创建
    int result= access("./myfifo1", F_OK);
    //access函数用来判断文件是否存在
    if(result!=0)//不存在该文件
    {
        result= mkfifo("./myfifo1",0777);
        if(result<0)
        {
            perror("mkfifo error");
            return -1;
        }
    }
    int fd= open("./myfifo1", O_RDWR);
    //写fifo文件
    write(fd,"Hello world", strlen("Hello world"));
    sleep(20);
    //sleep是因为要让read进程读到FIFO数据后在关闭，不然读不到。
    close(fd);
}
```

FIFO_read.c:
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <sys/stat.h>
//从FIFO中读
int main()
{
    int fd= open("./myfifo1", O_RDWR);
    if(fd<0)
    {
        perror("open error");
        return -1;
    }
    //读fifo文件
    char buf[100];
    memset(buf,0x00,sizeof(buf));
    read(fd,buf, sizeof (buf));//如果没有数据会阻塞到有数据
    printf("buf=[%s]", buf);
    close(fd);
}
```

## 5. 内存映射区
**存储映射I/O (Memory-mapped I/O) 使一个磁盘文件与内存中的一个缓冲区相映射。**从缓冲区中取数据，就相当于读文件中的相应字节；将数据写入缓冲区，则会将数据写入文件。这样，就可在不使用read和write函数的情况下，使用地址（指针）完成I/O操作。**即操作内存=操作文件**

使用存储映射这种方法，首先应通知内核，将一个指定文件映射到内存区域中。**这个映射工作可以通过mmap函数来实现。**

<font color='red'> **通过借助文件来实现进程间的通信** </font>
![在这里插入图片描述](https://img-blog.csdnimg.cn/f7ebfbc8be6c457f9524669268c2216e.png)
### 5.1 mmap----建立存储映射区
头文件 <sys/mman.h>
- **函数原型**
	- void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
- **函数作用**:
	- 建立存储映射区
- **5.1. 3参数**
	- addr: 	指定内存的起始地址, **设为NULL, 由系统指定内存其实地址**
	- length：映射到内存的文件长度。获取文件大小的方式：lseek(), stat()
	- prot：	映射区的保护方式, 最常用的:
读：PROT_READ
写：PROT_WRITE
读写：PROT_READ | PROT_WRITE
	- flags：	映射区的特性, 可以是
		- MAP_SHARED: **对映射区的修改会反映到文件本身**（可以对文件进行修改）, 且允许其他映射该文件的进程共享。
		- MAP_PRIVATE: 对映射区的写入操作会产生一个映射区的复制(copy-on-write), **对此映射区所做的修改不会对文件产生影响**。所以如果只是对文件进行读操作，用此参数。
	- fd：由open返回的文件描述符, 代表要映射的文件。只能是普通文件
	- offset：以文件开始处的偏移量, 必须是4k的整数倍, **通常为0**, 表示从文件头开始映射。
- **函数返回值**
	- 成功：返回创建的映射区首地址；
	- 失败：MAP_FAILED宏

### 5.2 munmap函数----释放存储映射区
头文件  <sys/mman.h>
 
- **函数原型**
	- int munmap(void *addr, size_t length);
- **函数作用**:
	- 释放由mmap函数建立的存储映射区
- **参数**
	- addr：调用mmap函数成功返回的参数（映射区首地址）
	- length：映射区大小（mmap函数的第二个参数）
- **函数返回值**
	- 成功：返回0
	- 失败：返回-1，设置errno值

具体代码案例看我的github中linux区的issues。
附上网址：https://github.com/jiong1998/Linux-.github.io/issues/7

### 5.3 利用mmap函数建立匿名映射
匿名映射：不需要文件，但是只能用于父子间进程通信。（悄悄问：那为什么不用PIPE？）

mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | 
<font color='red'> MAP_ANONYMOUS </font>, -1, 0);

不需要指定fd和偏移量。
# 第七章（信号）
## 1. 信号的基本介绍
进程A给进程B发送信号，进程B收到信号之前执行自己的代码，收到信号后，**不管执行到程序的什么位置，都要暂停运行，去处理信号**，处理完毕后再继续执行。与硬件中断类似——异步模式。但信号是软件层面上实现的中断，早期常被称为“软中断”。

<font color='red'> 每个进程收到的所有信号，都是由内核负责发送的 </font>

![请添加图片描述](https://img-blog.csdnimg.cn/a52f2175c6514841a89b1ffefdc5a783.png)

注意：**信号相关的内容都保存在内核区当中的PCB中**

几个常用到的信号：
SIGINT、SIGQUIT、SIGKILL、SIGSEGV、SIGUSR1、SIGUSR2、SIGPIPE、SIGALRM、SIGTERM、SIGCHLD、SIGSTOP、SIGCONT

2）SiGINT：Ctrl-C发出
3）SIGQUIT：Ctrl-\发出
10）SIGUSR1：用户自定义信号1
12）SIGUSR2：用户自定义信号2
14）SIGALRM：定时器到点后发送的信号
17）SIGCHLD：子进程退出后，内核会给父进程发送该信号。

### 1.1 信号的状态
信号有三种状态：产生、未决和递达。
- 信号的产生：
	- 按键产生，如：Ctrl+c、Ctrl+z、Ctrl+\
	- 系统调用产生，如：kill、raise、abort
	- 软件条件产生，如：定时器alarm
	- 硬件异常产生，如：非法访问内存(段错误)、除0(浮点数例外)、内存对齐出错(总线错误)
	-  命令产生，如：kill命令
- 未决：产生和递达之间的状态。主要由于阻塞(屏蔽)导致该状态。 **没有被处理的信号**
- 递达：递送并且到达进程。**信号已经被处理了**
### 1.2 信号的处理方式
信号的处理方式有三种：
- 执行默认动作 （绝大多数都是终止，因为终止对系统产生影响最小）：
	- Term：终止进程
	- Ign：忽略信号 (默认即时对该种信号忽略操作)
	- Core：终止进程，生成Core文件。(查验死亡原因，用于gdb调试)
	- Stop：停止（暂停）进程
	- Cont：继续运行进程
- 忽略信号(丢弃不处理)
- **捕捉信号**(调用用户的自定义的处理函数)：需要告诉内核，用户希望如何处理某一种信号，说白了就是**用户写一个信号处理函数**，然后将这个函数告诉内核。当该信号产生时，由**内核来调用用户自定义的函数**，以此来实现某种信号的处理。

注意：**SIGKILL和SIGSTOP**不能捕获，不能阻塞，不能忽略。只能执行他们的默认动作。SIGKILL的默认动作是终止，SIGSTOP的默认动作是暂停。


**不要使用信号来完成进程间的通信！！**

## 2. 信号相关函数
### 2.1 signal函数

看下面第四节
### 2.2 kill函数---给指定进程发送信号
头文件 #include <signal.h>

kill分为kill命令和kill函数，这里讲的是kill函数。

- **kill函数原型**：
	- int kill(pid_t pid, int sig);	
- **作用**：给指定的进程(pid)发送指定的信号
- **函数参数**：
	- sig信号参数：不推荐直接使用数字，应使用宏名，因为不同操作系统信号编号可能不同，但名称一致。
	- pid参数：
		- **pid > 0: 发送信号给指定的进程。一般只用pid>0**
		- pid = 0: 发送信号给与调用kill函数进程属于同一进程组的所有进程。
		- pid < -1:  取|pid|发给对应进程组。
		- pid = -1：发送给进程有权限发送的系统中所有进程。
- **函数返回值**：
	- 成功：0；
	- 失败：-1，设置errno

注意：父进程可以给子进程发送信号，子进程也可以给父进程发送信号，因此子进程可以杀死父进程( kill(getppid(), SIGKILL) )。

### 2.3 abort函数raise函数（用的不多）
- **raise函数**
	- 函说描述：给当前进程发送指定信号(**自己给自己发**)	
	- 函数原型：int raise(int sig);
	- 函数返回值：成功：0，失败非0值
	- 函数等同于：raise(signo) == kill(getpid(), signo);
- **abort函数**
	- 函数描述：给自己发送异常终止信号 **6) SIGABRT**，并产生core文件
	- 函数原型：void abort(void);  
	- 函数等同于：abort() == kill(getpid(), SIGABRT);

### 2.4 alarm ----计时器
头文件 #include <unistd.h>

- **函数原型**：unsigned int alarm(unsigned int seconds); 
- **函数描述**：设置定时器(闹钟)。在指定seconds后，内核会给当前进程发送**14）SIGALRM信号**。进程收到该信号，**默认动作终止**。<font color='red'> 每个进程都有且只有唯一的一个定时器 </font>。
- **函数返回值**：返回0或剩余的秒数，无失败。当第一次调用时，返回0。后面再调用时返回旧闹钟剩余的秒数，并重置闹钟

注意：alarm(0)取消闹钟，返回旧闹钟剩下的秒数

### 2.5 setitimer----周期性触发计时器
#include <sys/time.h>

 - **函数描述**
 	- alarm函数调用一次只能触发一次SIGABRT信号，如果我们希望周期性触发，可以使用setitimer函数。精度微秒us。
- **函数原型**       
	- int setitimer(int which, const struct itimerval *new_value,
                     struct itimerval *old_value);
 - **函数参数**
	- which：填ITIMER_REAL，计算自然时间
	- new_value是一个结构体，该结构体包含了两个相同的结构体。

具体使用看代码案例：

```cpp
//设置一个周期性2s发送一次时钟信号
struct itimerval tm;
//触发周期赋值
tm.it_interval.tv_sec=2;//每隔两秒触发一次
tm.it_interval.tv_usec=0;//微秒，不管他
//第一次触发事件赋值
tm.it_value.tv_sec=3;//三秒后第一次触发
tm.it_value.tv_usec=;//微秒，不管他

setitimer(ITIMER_REAL, &tm, NULL);
```


## 3. 信号集相关
### 3.1 未决信号集与阻塞信号集的关系
**未决信号集**：没有被处理的信号的集合（假设产生了某信号，如果此时该信号存在于阻塞信号集中，则会被放入未决信号集）
**阻塞信号集**：被当前进程阻塞的信号的集合
这两个集合都存储在内核的PCB。

**未决信号集是否应该被处理取决于阻塞信号集对应标志位是1还是0**，当这个信号被处理之前，先检查阻塞信号集对应标志位：
如果是1，说明该信号被阻塞，暂不处理；
如果是0说明该信号没有被阻塞，可以处理。 处理完后未决信号机的这个标识位从1变为0；表示这个信号已经被处理了（抵达）。
处理有三种方式：1默认 2被当前进程捕获 3忽略。

![在这里插入图片描述](https://img-blog.csdnimg.cn/1498366f26be48c0852d648358cf89e6.png)
一个标志位代表序号为x的信号，比如标志位为9代表SIGKILL信号。

注意：只有当阻塞信号集中某个信号x置为1了，当进程在运行过程中碰到x信号，该信号才会进未决信号集。否则会直接处理。

### 3.2信号集的相关函数
头文件 <signal.h>
信号集变量 sigset_t set;

由于信号集属于内核的一块区域，用户不能直接操作内核空间，为此，内核提供了一些信号集相关的接口函数，使用这些函数用户就可以完成对信号集的相关操作。
**信号集是一个能表示多个信号的数据类型**，sigset_t set，set即一个信号集。既然是一个集合，就需要对集进行添加、删除等操作。


- int sigemptyset(sigset_t *set);
函数说明：将某个信号集清0
函数返回值：成功：0；失败：-1，设置errno
- int sigfillset(sigset_t *set);
函数说明：将某个信号集置1		  		
函数返回值：成功：0；失败：-1，设置errno
- int sigaddset(sigset_t *set, int signum);	
函数说明：将某个信号加入信号集合中（将某个信号标志位置1）
函数返回值：成功：0；失败：-1，设置errno
- int sigdelset(sigset_t *set, int signum);		
函数说明：将某信号从信号清出信号集（将某个信号标志位置0）
函数返回值：成功：0；失败：-1，设置errno
- int sigismember(const sigset_t *set, int signum);
函数说明：判断某个信号是否在信号集中
函数返回值：在：1；不在：0；出错：-1，设置errno

**注意前面这些函数都没有操作内核的信号集**，只是在进程自己的栈上定义了一个信号集，并对自己定义的信号集的处理，处理完要通过下面两个函数才能对内核区的信号集进行真正的处理

sigprocmask函数(操作内核中的阻塞信号集)
- 函数说明：用来阻塞信号、解除阻塞也使用该函数。其本质，读取或修改内核中进程控制块(PCB)中的信号阻塞字（阻塞信号集）。**特别注意，阻塞信号只是将信号处理延后执行(延至解除阻塞)；而忽略表示将信号丢弃处理。**
- 函数原型：int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
- 函数返回值：成功：0；失败：-1，设置errno
- 函数参数：
	- how参数取值：假设当前的内核信号阻塞字为mask
		- **SIG_BLOCK**: 当how设置为此值，set表示需要阻塞的信号。相当于 mask = mask|set
		- **SIG_UNBLOCK**: 当how设置为此，set表示需要解除阻塞的信号。**set中置为1的，在内核的阻塞信号集都将置0**。相当于 mask = mask & ~set。并且当某个信号解除阻塞后，若未决信号集中有对应信号，则会马上处理。 ~~一般慎用~~ 
		- **SIG_SETMASK**: 当how设置为此，set表示用于替代原始屏蔽及的新阻塞集。相当于mask = set若，调用sigprocmask解除了对当前若干个信号的阻塞，则在sigprocmask返回前，至少将其中一个信号递达。
	- set：传入参数，是一个自定义信号集合。由参数how来指示如何修改当前信号屏蔽字。
	- oldset：传出参数，保存旧的信号阻塞字。

sigpending函数 (读取当前进程的未决信号集)
- 函数原型：int sigpending(sigset_t *set);	   
- 函数参数：set传出参数
- 函数返回值：成功：0；失败：-1，设置errno
如何判断某个信号是否在未决信号集中：先调用sigpending函数将未决信号集获取到本地栈上，在调用sigismember判断。


```cpp
//sigprocmask简单示范:
//需求：将SIGINT和SIGQUIT信号的加入到阻塞信号集

//创建信号集
sigset_t set
//初始化信号集
sigemptyset(&set);
//将SIGINT和SIGQUIT加入set集合中
sigaddset(&set, SIGINT);
sigaddset(&set, SIGQUIT);
//将信号集set中SIGINT和SIGQUIT信号的加入到阻塞信号集中
sigprocmask(SIG_BLOCK,&set, NULL);
```
信号集代码相关案例请移步github：设置阻塞信号集并把所有常规信号的未决状态打印至屏幕 #9------https://github.com/jiong1998/Linux-.github.io/issues/9

## 4. 信号捕捉函数
### 4.1 signal----简单的注册信号处理函数
头文件 #include <signal.h>

- **函数原型**
```cpp
void handler(int signum);//信号处理函数
sighandler_t signal(int signum, sighandler_t handler);//注册信号处理函数
```
- **函数作用**：
	- signal()：注册 信号处理函数。**当遇到该信号时，内核就会调用对应的信道处理函数。**
	- handler()：信号处理函数,作为参数传入signal中。
- **函数参数**
	- signum：信号编号。用宏定义，不要用数字
	- handler：信号处理函数


注意：**内核执行的信号处理函数，不是进程执行。**

### 4.2 sigaction----复杂的注册信号处理函数
signal不同的unix版本动作会不同，用sigaction代替
- 函数说明：
	- 注册一个信号处理函数
- 函数原型：
	- int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
- 函数参数：
	- signum：捕捉的信号
	- act：    结构体，传入参数，新的处理方式。
	- oldact： 结构体，传出参数，旧的处理方式

```
sigaction结构体介绍：
struct sigaction 
{
	   // 信号处理函数。可赋值为SIG_IGN表忽略或SIG_DFL表执行默认动作
       void  (*sa_handler)(int);
       //信号处理函数（不用，不管他）
       void  (*sa_sigaction)(int, siginfo_t *, void *); 
       sigset_t  sa_mask; //信号处理函数执行期间需要阻塞的信号
       int      sa_flags; //通常为0，表示使用默认标识
       void     (*sa_restorer)(void);//废弃
};
```
sigaction函数结论：
<font color='red'> 1. 若a信号处理函数执行期间，又产生了多次a信号，信号a本身会被阻塞，信号处理函数不会被打断，当信号处理函数执行完后，后面产生的多次信号a也只会被处理一次，即**信号不支持排队**。
2. 在a信号处理函数执行期间（并且sa_mask中阻塞了b信号），若此时收到了b的信号，则b信号会被阻塞，当a信号处理函数执行完后，才会转去执行b信号处理函数（若收到多次b信号，也只会执行一次）。
 </font>

sigaction用法示例：
```cpp
//sigaction函数测试：完成信号的注册
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void sighandle(int signum)
{
    printf("signum=%d", signum);
}
int main()
{
    struct sigaction act;
    act.sa_handler=sighandle;//信号处理函数
    //设置sa_mask：在执行信号处理函数时需要阻塞的信号
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGQUIT);//在信号处理函数执行期间，阻塞SIGQUIT信号
    act.sa_flags=0;
    //利用sigaction注册信号捕捉函数
    sigaction(SIGINT,&act,NULL);
    //利用signal注册信号捕捉函数
    signal(SIGINT, sighandle);
    return 0;
}
```

### 4.3 SIGCHLD信号
当子进程退出时，内核会给父进程发送SIGCHLD信号。（或者子进程收到SIGSTOP信号；又或者子进程停止时，收到SIGCONT信号）

SIGCHLD信号作用：
- 子进程退出后，内核会给父进程发送SIGCHLD信号，**提醒父进程对子进程进行回收。**
- 使用SIGCHLD信号完成对子进程的回收可以避免父进程阻塞等待而不能执行其他操作，**只有当父进程收到SIGCHLD信号之后才去调用信号捕捉函数完成对子进程的回收**，未收到SIGCHLD信号之前可以处理其他操作。

**SIGCHLD信号的默认动作是忽略。若父进程没有捕获该信号，则会使得子进程变成僵尸进程。**

### 4.4父进程回收子进程案例分析
需求分析：
父进程创建出三个子进程，并且，父进程通过接收SIGCHLD信号完成对子进程的回收。

原始的版本不放出来，需要的移步去github：https://github.com/jiong1998/Linux-.github.io/issues/11

原始的版本存在的问题：
问题1：
&emsp;&emsp;在信号函数注册之前，子进程可能已经开始运行并退出了。此时由于信号函数还没有注册，会导致退出的进程成为僵尸进程。
解决办法：
&emsp;&emsp; 在fork()之前，先将SIGCHLD信号阻塞，完成信号处理函数的注册后，在解除阻塞。此时如果有SIGCHLD信号产生，会放入未决信号集。(更简单的方法是把信号注册函数放在fork函数之前，但是这会让子进程继承，既然子进程没有子子进程，就没有必要继承)
问题2：
&emsp;&emsp;前面提到过，**信号不支持排队**，所以在执行信号处理函数回收子进程的时候，如果有2个及以上的子进程同时给父进程发送SIGCHLD信号，父进程也只会回收一次！会导致存在僵尸进程。
解决办法：
&emsp;&emsp;可以在信号处理函数里面使用while(1)循环回收, 这样就有可能出现捕获一次SIGCHLD信号但是回收了多个子进程的情况，从而可以避免产生僵尸进程。

```cpp
//改进后的父进程通过SIGCHLD回收子进程
//SIGCHLD信号处理函数
void waitchild_new(int signum)
{
    //当收到SIGCHLD信号时，使用waitpid函数回收进程
    pid_t wpid ;//
    while(1)//利用循环回收解决僵尸进程问题
    {
        wpid=waitpid(-1,NULL, WNOHANG);
        if(wpid>0)
        {
            printf("已利用信号SIGCHLD回收子进程资源,pid=[%d]\n", wpid);
        }
        if(wpid==0)//目前没有子进程退出
        {
            printf("还有子进程存活\n");
            break;
        }
        if(wpid==-1)
        {
            printf("子进程已经全部回收\n");
            break;
        }
    }
}

//创造出三个进程，每个进程执行自己的事。
//当父进程接收到子进程的SIGCHLD信号时，转向执行信号处理函数，对子进程进程资源的释放
int main()
{
    int i;

    //将SIGCHLD信号阻塞。
    //创建信号集
    sigset_t set;
    //初始化信号集
    sigemptyset(&set);
    //将SIGCHLD加入set集合中
    sigaddset(&set, SIGCHLD);
    //将set集合加入到阻塞信号集
    sigprocmask(SIG_BLOCK, &set,NULL);

    for(i=0;i<3;++i)//循环创建三个子进程
    {
        pid_t pid=fork();
        if(pid < 0)
        {
            perror("fork error");
            return -1;
        }
        if(pid > 0)//父进程
        {
            printf("子进程创建成功,子进程pid=[%d]\n", pid);
        }
        if(pid==0)//子进程
            break;
    }
    if(i==0)//第一个子进程
    {
        printf("the first child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(1);//模拟子进程在执行其他事
    }
    if(i==1)//第二个子进程
    {
        printf("the second child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(2);//模拟子进程在执行其他事
    }
    if(i==2)//第三个子进程
    {
        printf("the third child, pid=[%d],ppid=[%d]\n", getpid(), getppid());
        sleep(2);//模拟子进程在执行其他事
    }
    if(i==3)//父进程
    {
        printf("the father, pid=[%d]\n", getpid());
        //父进程注册SIGCHLD信号处理函数
        struct sigaction act;
        act.sa_handler=waitchild_new;//信号处理函数
        //设置sa_mask：在执行信号处理函数时需要阻塞的信号
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        //注册信号处理函数
        sigaction(SIGCHLD,&act, NULL);
        //SIGCHLD解除阻塞
        sigprocmask(SIG_UNBLOCK, &set, NULL);
        while(1)//这个while函数模拟父进程在执行其他事情。
        {
            sleep(5);//模拟父进程在执行其他事情。
            printf("asd");
        }

    }
}
```

# 第八章（线程）
## 1. 守护进程
### 1.1 守护进程的概念
Daemon(精灵)进程，是**Linux中的后台服务进程**，**通常独立于控制终端并且周期性地执行某种任务或等待处理某些发生的事件**。一般采用以d结尾的名字，如vsftpd

Linux后台的一些系统服务进程，没有控制终端，不能直接和用户交互。不受用户登录、注销的影响，一直在运行着，他们都是守护进程。如：预读入缓输出机制的实现；ftp服务器；nfs服务器等。

总结守护进程的特点：
- Linux后台服务进程
- 独立于控制终端(不受终端控制)
- 周期性的执行某种任务
- 不受用户登陆和注销的影响
- 一般采用以d结尾的名字（不是绝对）

### 1.2 进程组和会话
#### 进程组：
一个进程组包含多个进程

每个进程都属于一个进程组，**引入进程组是为了简化对进程的管理。** 当父进程创建子进程的时候，默认子进程与父进程属于同一个进程组。

进程组的特点：
1. **进程组ID\=\=第一个进程ID**（组长进程）。如父进程创建了多个子进程，父进程和多个子进程同属于一个组，而由于父进程是进程组里的第一个进程，所以父进程就是这个组的组长, 组长ID==父进程ID。
2. 可以使用kill -SIGKILL -进程组ID(负的)来将整个进程组内的进程全部杀死。
3. 只要进程组中有一个进程存在，进程组就存在，与组长进程是否终止无关。
4. 进程组生存期：从进程组创建到最后一个进程离开。

#### 会话：
多个进程组组成一个会话

会话的特点：
1. 重要！！<font color='red'> **创建会话的进程不能是进程组组长**（所以父进程不能创建会话） </font>
2. 创建会话的进程成为一个进程组的组长进程，同时也成为会话的会长。
3. 新创建的会话丢弃原有的控制终端
4.  建立新会话时，先调用fork, 父进程终止，子进程调用setsid函数

进程组和会话的关系图
![在这里插入图片描述](https://img-blog.csdnimg.cn/49030911141f474e8c6041b9c7c86bb1.png)

### 1.3 创建守护进程的流程
1. **父进程fork子进程, 然后父进程退出.**
	- 目的是: 让子进程不成为进程组长, 为后续调用setsid函数提供了条件.
2. **子进程调用setsid函数创建一个新的会话.**
	- 该子进程成了该会话的会长
	-  该子进程成了该组的组长进程.
	-  不再受控制终端的影响了
3. 改变当前的工作目录, chdir  -----不是必须的
4. 重设文件掩码, umask(0000)  -----不是必须的
	- 增加进程的操作的灵活性
5. 关闭STDIN_FILENO  STDOUT_FILENO STDERR_FILENO   ----不是必须的
	- 节省资源
6. **执行核心操作**

### 1.4创建守护进程具体代码实现
编写一个守护进程，每隔2S钟获取一次系统时间，并将这个时间写入磁盘文件。

需求分析：
- 首先要创建一个守护进程.
- 每隔2S钟: 使用setitimer函数设置时钟, 该时钟发送的是SIGALRM信号,
- 信号操作: 注册信号处理函数,signal或者sigaction, 还有一个信号处理函数
- 获取一次系统时间: time函数的使用, ctime函数的使用
- 文件IO操作：写入磁盘文件: 文件操作函数: open write close 

```cpp
//编写一个守护进程，每隔2S钟获取一次系统时间，并将这个时间写入磁盘文件。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
int flags=0;
void handler()
{
    //文件只需要打开一次，设置flag来判断是否第一次打开
    int fd;
    if(flags==0)
    {
        fd= open("./time.log",O_RDWR | O_APPEND | O_CREAT, 0755);
        if(fd<0)
            return;
    }
    //获取时间
    time_t t;
    time(&t);
    char * p = ctime(&t);
    //写入文件
    write(fd, p, strlen(p));
    return;
}
int main()
{
    pid_t pid= fork();
    if(pid < 0 || pid>0)//父进程退出
    {
        exit(1);
    }
    //子进程创建会话
    setsid();
    //改变当前工作目录
    chdir("./");//这里不该
    //重设文件掩码
    umask(0000);
    //核心操作:

    //注册信号
    struct sigaction act;
    act.sa_handler= handler;
    act.sa_flags=0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
//    signal(SIGALRM, handler);

    //设置时钟信号
    struct itimerval tm;
    //触发周期赋值
    tm.it_interval.tv_sec=2;//每隔两秒触发一次
    tm.it_interval.tv_usec=0;//微秒，不管他
    //第一次触发事件赋值
    tm.it_value.tv_sec=3;//三秒后第一次触发
    tm.it_value.tv_usec=0;//微秒，不管他
    setitimer(ITIMER_REAL, &tm, NULL);//该函数每隔2s发送一次SIGALRM信号
    while (1)//让守护进程持续运行
    {
        sleep(1);
        //注意：结束程序要先用命令：ps ajx | grep day8_Daemo查找进程号，然后kill -9 进程号 来结束进程 
    }
}
```

## 2. 线程

### 2.1 线程的概念
线程----轻量级的进程

- 进程：分配资源的基本单位
- 线程：系统调度的基本单位

进程：**拥有独立的地址空间**，拥有PCB，相当于独居。
线程：有PCB，**但没有独立的地址空间**，**多个线程共享进程空间**，相当于合租。


![在这里插入图片描述](https://img-blog.csdnimg.cn/f2f8a3a73cee450bbbbc149b16be516a.png)
由图可知：在使用pthread_create创造线程后，会在内核区复制pcb来创造线程（也没有完全复制，比如文件描述符。所有线程共享同一个文件描述符），共享除了栈以外的所有资源

特点：
- 不同的线程有不同的线程号，但是pid是同一个。
- **除了栈空间外**，其余资源全部共享
- 线程执行顺序不一定
- 从内核里看进程和线程是一样的，底层实现都是调用同一个内核函数 clone。进程和线程都有各自不同的PCB。内核只看pcb，有几个pcb就有几个进程。**在内核看来，线程只是没有独立地址空间的进程。**

所以由于线程共享地址空间，所以当线程读全局变量时，需要加锁。

- 线程共享资源
	- 文件描述符表
	- 每种信号的处理方式
	- 当前工作目录
	- 用户ID和组ID
	- 内存地址空间 (.text/.data/.bss/heap/共享库) 
- 线程非共享资源
	- 线程id
	- 处理器现场和栈指针(内核栈)
	- 独立的栈空间(用户空间栈)
	- errno变量
	- 信号屏蔽字（线程里不使用信号）
	- 调度优先级

从经验来说：一般业务处理用进程，一般网络通信用线程

### 2.2 pthread_create----创建线程
<pthread.h>
要链接-pthread库
- **函数原型**：
```cpp
int pthread_create(pthread_t *thread, 
				const pthread_attr_t *attr,
                void *(*start_routine) (void *),
				  void *arg);
```
-  函数参数：
	- pthread_t：线程id。传出参数，保存系统为我们分配好的线程ID
	- attr：线程属性。通常传NULL，表示使用线程默认属性。若想使用具体属性也可以修改该参数。
	- start_routine：回调函数。函数指针，指向线程主函数(线程体)，该函数运行结束，则线程结束。
	- arg：线程主函数执行期间所使用的参数
- 返回值：
	- 成功，返回0
	- 失败，返回错误号(不要用perror，用**strerror**打印错误)


简单创建子线程案例
```cpp
//创建子线程接收参数并执行printf
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
void *mythread(void *arg)
{
    printf("child thread, pid=%d, id=%ld\n", getpid(),pthread_self());
    pthread_exit(NULL);//子线程退出
}
int main()
{
    //创建子进程
    pthread_t pthread_id;
    int ret = pthread_create(&pthread_id, NULL, mythread,NULL);
    if(ret!=0)
    {
        printf("pthread_create error", strerror(ret));
        return -1;
    }
    printf("father thread, pid=%d, id=%ld\n", getpid(),pthread_self());
    pthread_join(pthread_id, NULL);//线程回收
    return 1;
}
```

#### 2.2.1 循环创建多个子线程
在原始版本中（原版本代码可以移步至https://github.com/jiong1998/Linux-.github.io/issues/13中看），最后每个子线程打印出来的值并不是想象中的值，比如都是5。

为什么？
原因是：由于主线程可能会在一个cpu时间片内连续创建了5个子线程，此时变量i的值变成了5，并且传入的值是int*地址, 当主线程失去cpu的时间片后，子线程得到cpu的时间片，**子线程访问的是变量i的内存空间的值**，所以打印出来值为5.

解决办法：
可以在主线程定义一个数组：int arr[5];，然后创建线程的时候分别传递不同的数组元素，这样每个子线程访问的就是互不相同的内存空间，这样就可以打印正确的值。

分析的如图所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/3b1aad743c7a480daed0318db049534e.png)
左图是修改前的代码所示（所有线程共享一个i的内存空间），右边图是修改后的代码所示（每个线程访问互不相同的内存空间）

代码修改后如下：
```cpp
//子线程执行函数
void *mythread(void * arg)
{
    int i= *(int *)arg;
    printf("i=[%d],thread_id=[%ld]\n", i, pthread_self());
}

int main()
{
    pthread_t thread[5];
    int i,ret;
    int arr[5];
    for(i=0;i<5;++i)
    {
        arr[i]=i;
        ret = pthread_create(&thread[i],NULL,mythread,&arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error", strerror(ret));
            return -1;
        }
    }
    sleep(1);
    return 0;
}
```
```
输出结果：
i=[2],thread_id=[6167113728]
i=[0],thread_id=[6165966848]
i=[1],thread_id=[6166540288]
i=[3],thread_id=[6167687168]
i=[4],thread_id=[6168260608]
```
### 2.3 pthread_exit---线程退出
**在线程中禁止调用exit函数**，否则会导致整个进程退出，取而代之的是调用pthread_exit函数，这个函数是使一个线程退出，<font color='red'> 如果主线程调用pthread_exit函数也不会使整个进程退出，不影响其他线程的执行。</font>

- 函数描述
	- 将单个线程退出
- 函数原型
	-	void pthread_exit(void *retval);	
- 函数参数
	- retval表示线程退出状态，通常传NULL

另注意，pthread_exit或者return返回的指针所指向的内存单元必须是**全局的或者是用malloc分配的**，不能在线程函数的栈上分配，因为当其它线程得到这个返回指针时线程函数已经退出了，栈空间就会被回收。

**当主线程提前退出，并不会影响子线程的运行，而此时子线程还没有退出，会导致整个进程变成僵尸进程**。

只有最后一个退出的是主线程，整个进程空间才能回收，才能避免僵尸进程。因此需要线程回收--pthread_join

### 2.4 pthread_join---线程回收
类似进程的wait()函数

- 函数描述：**阻塞**等待线程退出，获取线程退出状态。其作用，对应进程中的waitpid() 函数。
- 函数原型：int pthread_join(pthread_t thread, void **retval); 
- 函数返回值：
	- 成功：0；
	- 失败：错误号
- 函数参数： 
	- thread：线程ID
	- retval：存储线程结束状态，整个指针和pthread_exit的参数是同一块内存地址。


### 2.5 pthread_detach---设置线程分离状态
- 线程的分离状态决定一个线程以什么样的方式来终止自己，有两种状态：
	- 非分离状态：线程的默认属性是非分离状态，这种情况下，原有的线程等待创建的线程结束。只有当pthread_join()函数返回时，创建的线程才算终止，才能释放自己占用的系统资源。
	- 分离状态：指定该状态，线程主动与主控线程断开关系。**子线程运行结束了，子线程也就终止了，马上自己释放系统资源**。将不会产生僵尸进程。网络、多线程服务器常用。


也可使用 pthread_create函数参2(线程属性)来设置线程分离。pthread_detach函数是在创建线程之后调用的。

- 函数描述
	- 实现线程分离
- 函数原型
	- int pthread_detach(pthread_t thread);	
- 函数返回值
	- 成功：0；
	- 失败：错误号



### 2.6 pthread_cancel---取消线程
- 函数描述
	- 杀死(取消)线程。其作用，对应进程中 kill() 函数。
- 函数原型
	- int pthread_cancel(pthread_t thread);	
- 函数返回值
	- 成功：0；
	- 失败：错误号

**注意：线程的取消并不是实时的，而有一定的延时。需要等待线程到达某个取消点(检查点)。**

取消点：是线程检查是否被取消，并按请求进行动作的一个位置。通常是一些系统调用creat，open，pause，close，read，write..... 。**可粗略认为一个系统调用(进入内核)即为一个取消点（可以让进程阻塞的一般都取消点）**。还以通过调用 **pthread_testcancel()** 函数设置一个取消点。

### 2.7 进程函数和线程函数的比较
进程    | 线程
------ | -----
fork  | pthread_create
exit  | pthread_exit
wait/waitpid | pthread_join
kill  | pthread_cancel
getpid| pthread_self


## 3. 线程属性
linux下线程的属性是可以根据实际项目需要，进行设置，之前讨论的线程都是采用线程的默认属性，默认属性已经可以解决绝大多数开发时遇到的问题，如果对程序的性能提出更高的要求，则需要设置线程属性，

### 3.1 创建线程的时候设置分离属性
本节以设置线程的分离属性为例讲解设置线程属性。

设置线程属性分为以下步骤：

第1步：定义线程属性类型的变量
pthread_attr_t  attr;	

第2步：对线程属性变量进行初始化
int pthread_attr_init (pthread_attr_t* attr);

第3步：设置线程为分离属性
```cpp
int pthread_attr_setdetachstate(
pthread_attr_t *attr, int detachstate);
```
- 参数:
	- attr: 线程属性
	- detachstate: PTHREAD_CREATE_DETACHED(分离属性)


注意：这一步完成之后调用pthread_create函数创建线程，则创建出来的线程就是分离线程；其实上述三步就是pthread_create的第二个参数做准备工作。

第4步：释放线程属性资源
int pthread_attr_destroy(pthread_attr_t *attr);
参数：线程属性

创建子线程的时候设置分离属性的简单案例：
```cpp
//在创建子线程的时候设置分离属性
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

void * mythread(void * arg)
{
    printf("chlid pthread[%ld]\n", pthread_self());
}

int main()
{
    pthread_t pthread_id;
    //1 定义线程属性类型的变量
    pthread_attr_t attr;
    //2 对线程属性变量进行初始化
    pthread_attr_init(&attr);
    //3 设置线程为分离属性
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int ret = pthread_create(&pthread_id, &attr, mythread, NULL);
    if(ret!=0)
    {
        printf("pthread_create error", strerror(ret));
        return -1;
    }
    //4 释放线程属性
    pthread_attr_destroy(&attr);
    printf("father pthread[%ld]\n", pthread_self());
    return 1;
}
```

## 4 互斥锁
同步：对运行次序的一种制约关系。
互斥：一种对共享资源的制约关系（信号灯）

### 4.1 两个线程数数模型（问题引出）
创建两个线程，让两个线程共享一个全局变量int number， 然后让每个线程数5000次数，最后打印出来的数不是10000。

造成这样的原因：
- **多个线程访问共享资源**
- 调度随机（线程操作共享资源的先后顺序不确定）	
- 线程间缺乏必要的互斥机制。

解决办法：
&emsp;&emsp;提供互斥机制，即**加互斥锁**。
![在这里插入图片描述](https://img-blog.csdnimg.cn/7a51b9ea7c1847cd9d625a9d5190b22a.png)
当某个线程加锁，**另一个线程也想访问该资源发现上锁了，就会阻塞等待，直到解锁**。

使用互斥锁之后，两个线程由并行操作变成了串行操作，效率降低了，但是数据不一致的问题得到解决了。
### 4.2 pthread_mutex_init----初始化锁

pthread_mutex_t 类型
- 其本质是一个结构体，为简化理解，应用时可忽略其实现细节，简单当成整数看待。
- thread_mutex_t mutex; 变量mutex只有两种取值1、0。

pthread_mutex_init函数：
- 函数描述：
	- 初始化一个互斥锁(互斥量) ---> 初值可看作1
- 函数原型：
```cpp
int pthread_mutex_init(pthread_mutex_t *restrict mutex, 
		const pthread_mutexattr_t *restrict attr);
```
- 函数参数
	- mutex：传出参数，调用时应传 &mutex	
	- attr：互斥锁属性。是一个传入参数，通常传NULL，选用默认属性(线程间共享)。
	
互斥量mutex的两种初始化方式：
- 静态初始化：如果互斥锁 mutex 是静态分配的（定义在全局，或加了static关键字修饰），可以直接使用宏进行初始化。
pthead_mutex_t muetx = PTHREAD_MUTEX_INITIALIZER;
- 动态初始化：局部变量应采用动态初始化。
pthread_mutex_init(&mutex, NULL)

### 4.3 pthread_mutex_destroy----销毁锁
- 函数描述
	- 销毁一个互斥锁
- 函数原型
	- int pthread_mutex_destroy(pthread_mutex_t *mutex);
- 函数参数
	- mutex—互斥锁变量
### 4.4 pthread_mutex_lock----加锁操作
- 函数描述
	- 对互斥所加锁，可理解为将mutex- -。这是一个阻塞函数，如果加锁不成功（mutex减后<0），则调用该函数的线程将被阻塞。
- 函数原型
	- int pthread_mutex_lock(pthread_mutex_t *mutex);
- 函数参数
	- mutex—互斥锁变量

### 4.5 pthread_mutex_unlock----解锁操作
- 函数描述
	- 对互斥所解锁，可理解为将mutex ++
- 函数原型
	- int pthread_mutex_unlock(pthread_mutex_t *mutex);

### 4.6 pthread_mutex_trylock----尝试加锁
ead_mutex_trylock函数
- 函数描述
	- 尝试加锁。不会阻塞，如果发现加不了锁，直接返回。
- 函数原型
	- int pthread_mutex_trylock(pthread_mutex_t *mutex);
- 函数参数
	- mutex—互斥锁变量

### 4.7 加锁和解锁
加锁和解锁
- lock尝试加锁，如果加锁不成功，线程阻塞，阻塞到持有该互斥量的其他线程解锁为止。
- unlock主动解锁函数，**同时将阻塞在该锁上的所有线程全部唤醒**，至于哪个线程先被唤醒，取决于优先级、调度。默认：先阻塞、先唤醒。

### 4.8 互斥锁的使用步骤
互斥锁步骤：
- 第一步：全局创建一把互斥锁
	- pthread_mutex_t mutex;
- 第二步：在main函数中初始化互斥锁
	- pthread_mutex_init(&mutex, NULL);---相当于mutex=1
- 第三步：在代码中寻找共享资源（也称为临界区）
	- pthread_mutex_lock(&mutex);  -- mutex = 0
	- [临界区代码]
	- pthread_mutex_unlock(&mutex); -- mutex = 1
- 第四步：在main函数中释放互斥锁
	- pthread_mutex_destroy(&mutex);

### 4.9 互斥锁代码示例：利用互斥锁完成两个线程数数
```cpp
//需求：定义一个全局变量number，利用互斥锁让每个线程数5000个数,
//最后全局变量number应该输出10000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

//定义一个互斥锁
pthread_mutex_t mutex;

int number=0;
void * mythread(void * arg)//线程调用函数
{
    int i;
    int j;
    for(i=0;i<5000;++i)
    {
        pthread_mutex_lock(&mutex);//加锁
        j=number;
        j++;
        number=j;
        pthread_mutex_unlock(&mutex);//解锁
    }
    pthread_exit(NULL);
}
int main()
{
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //创建子进程
    pthread_t pthread_id[2];
    for (int j=0;j<2;++j)
    {
        int ret = pthread_create(&pthread_id[j], NULL, mythread,NULL);
        if(ret!=0)
        {
            printf("pthread_create error", strerror(ret));
            return -1;
        }
//        pthread_detach(pthread_id[j]);
    }
    pthread_join(pthread_id[0],NULL);
    pthread_join(pthread_id[1],NULL);
    printf("number=%d", number);
    //销毁锁
    pthread_mutex_destroy(&mutex);
    return 1;
}
```
## 5. 死锁
![在这里插入图片描述](https://img-blog.csdnimg.cn/02c06a2d2f9b406a99ea5d8485880450.png)
死锁是指两个或两个以上的进程在执行过程中，由于竞争资源或者由于彼此通信而造成的一种阻塞的现象。

如何解决死锁：
- 让线程按照一定的顺序去访问共享资源
- 在访问其他锁的时候，需要先将自己的锁解开
- 调用pthread_mutex_trylock，如果加锁不成功会立刻返回



## 6. 读写锁

### 6.1 读写锁的基本概念
读写锁也叫共享-独占锁。当读写锁以读模式锁住时，它是以共享模式锁住的；当它以写模式锁住时，它是以独占模式锁住的。**写独占、读共享。**

读写锁是“读模式加锁”时，此时如果既有试图以写模式加锁的线程，也有试图以读模式加锁的线程。那么读写锁就会阻塞读模式锁请求。**优先满足写模式锁**。读锁、写锁并行阻塞，写锁优先级高

总结就是：读并行，写独占，当读写同时等待锁的时候写的优先级高。

读写锁使用场合
&emsp;&emsp;读写锁非常适合于对数据结构读的次数远大于写的情况。

### 6.2 读写锁的相关函数
定义一把读写锁: pthread_rwlock_t rwlock;
1. 初始化读写锁
```cpp
int pthread_rwlock_init(                                
pthread_rwlock_t *restrict rwlock, 
const pthread_rwlockattr_t *restrict attr);
```
- 函数参数
	- wlock-读写锁
	- attr-读写锁属性，传NULL为默认属性

2. 销毁读写锁
	- int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);        
3. 加读锁
	- int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);              
4. 尝试加读锁
	- int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
5. 加写锁
	- int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
6. 尝试加写锁
	- int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
7. 解锁
	- int pthread_rwlock_unlock(&pthread_rwlock_t *rwlock);




### 6.3 读写锁测试程序
3个线程不定时写同一全局资源，5个线程不定时读同一全局资源
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//定义一把锁
pthread_rwlock_t rwlock;

//定义全局变量number
int number=0;

void* write_function(void * arg)
{
    int temp;
    while(1)
    {
        //加写锁
        pthread_rwlock_wrlock(&rwlock);

        temp=number;
        temp++;
        number=temp;

        //解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(rand()%3);
    }
}
void* read_function(void * arg)
{
    int i=*(int *) arg;
    int temp;
    while(1)
    {
        //加写锁
        pthread_rwlock_rdlock(&rwlock);

        temp=number;
        printf("[%ld]---read：[%d]\n", i,temp);

        //解锁
        pthread_rwlock_unlock(&rwlock);
        sleep(rand()%3);
    }
}
int main()
{
    int i;
    int n=8;
    int arr[8];
    pthread_t thread[8];
    //初始化读写锁
    pthread_rwlock_init(&rwlock,NULL);

    //创建3个写线程
    for(i=0;i<3;++i)
    {
        arr[i]=i;
        pthread_create(&thread[i],NULL,write_function, &arr[i]);
    }
    //创建5个读线程
    for(i=3;i<n;++i)
    {
        arr[i]=i;
        pthread_create(&thread[i],NULL,read_function, &arr[i]);
    }
    for(int j=0;j<n;++j)
    {
        pthread_join(thread[j], NULL);
    }
    //销毁读写锁
    pthread_rwlock_destroy(&rwlock);
    return 0;
}
```

## 7. 条件变量
### 7.1 条件变量的概念
条件本身不是锁！但它也可以造成线程阻塞。通常与互斥锁配合使用。给多线程提供一个会合的场所。

- **使用条件变量可以使线程阻塞, 等待某个条件的发生, 当条件满足的时候解除阻塞.**

条件变量的两个动作:
- 条件不满足, 阻塞线程
- 条件满足, 通知阻塞的线程解除阻塞, 开始工作.

### 7.2  pthread_cond_init----初始化条件变量
定义一个条件变量：pthread_cond_t  cond;

初始化条件变量：
```cpp
int pthread_cond_init(pthread_cond_t *restrict cond,
               const pthread_condattr_t *restrict attr);
```
- 函数描述:初始化条件变量
- 函数参数: 
	- cond: 条件变量
	- attr: 条件变量属性, 通常传NULL
- 函数返回值:成功返回0, 失败返回错误号


### 7.3  pthread_cond_destroy----销毁条件变量
- 函数描述: 销毁条件变量
- 函数参数: 条件变量
- 返回值: 成功返回0, 失败返回错误号


### 7.4  pthread_cond_wait----判断条件是否阻塞
- 函数描述:
 	- **若收到pthread_cond_signal发送的信号, 解除线程阻塞, 并试图加互斥锁**
	-  **若未收到pthread_cond_signal发送的信号, 引起线程阻塞并解互斥锁**
- 函数参数:
	- cond: 条件变量
	- mutex: 互斥锁变量
- 函数返回值: 成功返回0, 失败返回错误号

### 7.5  pthread_cond_signal----唤起阻塞线程
- 函数描述: 唤醒至少一个阻塞在该条件变量上的线程
- 函数参数: 条件变量
- 函数返回值: 成功返回0, 失败返回错误号

### 7.6 条件变量的使用步骤
互斥锁步骤：
- 第一步：全局创建一个条件变量
	- pthread_cond_t cond;
- 第二步：在main函数中初始化条件变量
	- pthread_cond_init(&cond, NULL);
- 第三步：在生产者线程中调用: 
	- pthread_cond_wait(&cond);---解除消费者的阻塞
- 第四部：在消费者线程中调用
	- pthread_cond_wait(&cond);---将消费者阻塞
- 第四步：在main函数中释放条件变量
	- pthread_cond_destroy(&mutex);

### 7.7 利用条件变量解决单生产者和单消费者代码案例
生产者每次生产一个链表，消费者每次消费一个链表。（链表不带头节点）
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

//定义一把锁
pthread_mutex_t mutex;
//定义条件变量
pthread_cond_t cond;
//定义链表
typedef struct node
{
    int data;
    struct node * next;

}NODE;

NODE *head = NULL;

void * producer(void*arg)
{
    NODE * pnode= NULL;
    while(1)
    {
        //生产一个链表
        pnode = (NODE *) malloc(sizeof(NODE));
        pnode->data=rand()%1000;
        //加锁
        pthread_mutex_lock(&mutex);
        pnode->next=head;
        head=pnode;
        printf("Producing---[%d]\n", head->data);
        //解锁
        pthread_mutex_unlock(&mutex);

        //通知消费者线程解除阻塞
        pthread_cond_signal(&cond);
        sleep(rand()%2);

    }
}

void * consumer(void*arg)
{
    NODE * pnode = NULL;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        if(head==NULL)
        {
            //如果头节点为空，则阻塞在此并解锁，直到生产者给出信号(被生成者线程调用pthread_cond_signal函数通知)，才唤醒并重新加锁
            pthread_cond_wait(&cond, &mutex);
        }
        pnode=head;
        head=head->next;
        printf("Consuming---[%d]\n", pnode->data);
        //解锁
        pthread_mutex_unlock(&mutex);
        free(pnode);
        pnode=NULL;
        sleep(rand()%2);
    }
}

int main()
{
    int ret;
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);

    pthread_t pthread[2];

    ret=pthread_create(&pthread[0], NULL, producer,NULL);
    if(ret!=0)
    {
        printf("pthread_create error, [%s]\n", strerror(ret));
        return -1;
    }
    ret=pthread_create(&pthread[1], NULL, consumer,NULL);
    if(ret!=0)
    {
        printf("pthread_create error, [%s]\n", strerror(ret));
        return -1;
    }

    //等待线程结束
    pthread_join(pthread[0],NULL);
    pthread_join(pthread[1],NULL);

    //销毁锁
    pthread_mutex_destroy(&mutex);
    //销毁条件变量
    pthread_cond_destroy(&cond);
    return 1;
}
```

### 7.8 利用条件变量解决多个生产者和消费者代码案例
分析：多个生产者和多个消费者如果还是使用上面的代码改成多线程，程序会崩掉。原因是：
&emsp;&emsp; 假设生产者只生产了一个节点。通过pthread_cond_signal唤醒了两个消费者，于是两个消费者同时解除阻塞，但是此时只有一个消费者加锁成功，而另一个消费者此时又会被阻塞，但是这次的阻塞时阻塞在互斥锁上，而不是条件变量。当第一个消费者消费完并解锁后，head=NULL。此时第二个消费者就会加锁并获取head->data，但是这时head=NULL，所以系统会崩。

解决办法 ：
```
在消费者中的这行代码把if改成while
if(head==NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
修改后：
while(head==NULL)
        {
            pthread_cond_wait(&cond, &mutex);
        }
```
具体代码如下：

需求：生产者每次生产一个链表，消费者每次消费一个链表。（链表不带头节点）
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

//定义一把锁
pthread_mutex_t mutex;
//定义条件变量
pthread_cond_t cond;
//定义链表
typedef struct node
{
    int data;
    struct node * next;

}NODE;

NODE *head = NULL;

void * producer(void * arg)
{
    NODE * pnode= NULL;
    int i=*(int *)arg;
    while(1)
    {
        //生产一个链表
        pnode = (NODE *) malloc(sizeof(NODE));
        pnode->data=rand()%1000;
        //加锁
        pthread_mutex_lock(&mutex);
        pnode->next=head;
        head=pnode;
        printf("[%d]Producing---[%d]\n", i, head->data);
        //解锁
        pthread_mutex_unlock(&mutex);

        //通知消费者线程解除阻塞
        pthread_cond_signal(&cond);
        sleep(rand()%2);

    }
}

void * consumer(void * arg)
{
    NODE * pnode = NULL;
    int i=*(int *)arg;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        while(head==NULL)//通常只需要一部分线程去做执行任务，所以其它的线程需要继续wait。所以这里改成while不要用if
        {
            //如果头节点为空，则阻塞在此并解锁，直到生产者给出信号(被生成者线程调用pthread_cond_signal函数通知)，才唤醒并重新加锁
            pthread_cond_wait(&cond, &mutex);
        }
        pnode=head;
        head=head->next;
        printf("[%d]Consuming---[%d]\n", i, pnode->data);
        //解锁
        pthread_mutex_unlock(&mutex);
        free(pnode);
        pnode=NULL;
        sleep(rand()%3);
    }
}

int main()
{
    int ret;
    int i;
    int P_arr[5];
    int C_arr[5];
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    //初始化条件变量
    pthread_cond_init(&cond, NULL);

    pthread_t P_pthread[3];
    pthread_t C_pthread[5];
    //创造生产者
    for(i=0;i<5;++i)
    {
        P_arr[i]=i;
        ret=pthread_create(&P_pthread[i], NULL, producer,&P_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }
    //创造消费者
    for(i=0;i<5;++i)
    {
        C_arr[i]=i;
        ret=pthread_create(&C_pthread[i], NULL, consumer,&C_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }

    //等待线程结束
    for(i=0;i<5;++i)
    {
        pthread_join(P_pthread[i],NULL);
    }
    for(i=0;i<5;++i)
    {
        pthread_join(C_pthread[i],NULL);
    }


    //销毁锁
    pthread_mutex_destroy(&mutex);
    //销毁条件变量
    pthread_cond_destroy(&cond);
    return 1;
}
```
## 8. 信号量

### 8.1 信号量的概念
信号量是先做加/减法，再判断是否进入。

- 加锁：S先减1，如果S=0还是可以进入(此时没空位)，如果S<0，则阻塞
- 解锁：S先加1，如果S=0，说明有一个线程在等待，放一个进去。如果S<0，说明有多个线程再等待，也是放一个进去。

### 8.2 sem_init----初始化信号量
定义一个信号量 sem_t sem

int sem_init(sem_t *sem, int pshared, unsigned int value);	
- 函数描述: 初始化信号量
- 函数参数:
	- sem: 信号量变量
	- pshared: 0表示线程同步, 1表示进程同步
	- value: 最多有几个线程操作共享数据
- 函数返回值:成功返回0, 失败返回-1, 并设置errno值


### 8.3 sem_wait----信号量-1（加锁）
- 函数描述: 调用该函数一次, 相当于sem--, 当sem为0的时候, 引起阻塞
- 函数参数: 信号量变量
- 函数返回值: 成功返回0, 失败返回-1, 并设置errno值


### 8.4 sem_post----信号量+1（解锁）
- 函数描述: 调用一次, 相当于sem++
- 函数参数: 信号量变量
- 函数返回值: 成功返回0, 失败返回-1, 并设置errno值

### 8.5 sem_trywait----尝试-1
- 函数描述: 尝试加锁, 若失败直接返回, 不阻塞
- 函数参数: 信号量变量
- 函数返回值: 成功返回0, 失败返回-1, 并设置errno值

### 8.6 sem_destroy----初始化信号量

- 函数描述: 销毁信号量
- 函数参数: 信号量变量
- 函数返回值: 成功返回0, 失败返回-1, 并设置errno值

### 8.7 信号量的使用步骤
	1 定义信号量变量
		sem_t sem1;
		sem_t sem2;
	2 初始化信号量
		sem_init(&sem1, 0, 5);
		sem_init(&sem2, 0, 0);
	3 加锁
		sem_wait(&sem1);
		//共享资源
		sem_post(&sem2);
		
		sem_wait(&sem2);
		//共享资源
		sem_post(&sem1);
	4 释放资源
		sem_destroy(sem1);
		sem_destroy(sem2);

### 8.8 利用信号量解决多线程间的生产者和消费者问题


案例：利用信号量解决多线程间的生产者和消费者问题

需求：假设有5个生产者，4个消费者，生产者每次生产一个苹果到桌子上，消费者每次从桌子上消费一个苹果，假设桌子最多放3个苹果,利用信号量解决该问题
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

sem_t empty;//剩余空间信号量
sem_t full;//占用空间信号量
pthread_mutex_t mutex;//互斥锁
int apple_number=0;//苹果的数量

void * producer(void * arg)
{
    int i = *(int *)arg;
    while(1)//生产
    {
        sem_wait(&empty);//生产苹果，剩余空间-1
        pthread_mutex_lock(&mutex);
        sleep(1);
        apple_number++;
        printf("序号[%d]生产了一个苹果,目前桌子上有[%d]个水果\n", i, apple_number);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);//生产完，占用空间+1
        sleep(rand()%2);
    }
}

void * consumer(void * arg)
{
    int i = *(int *)arg;
    while(1)//消费
    {
        sem_wait(&full);//消费苹果，占用空间-1
        pthread_mutex_lock(&mutex);
        sleep(1);
        apple_number--;
        printf("序号[%d]消费了一个苹果,目前桌子上有[%d]个水果\n", i, apple_number);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);//消费完，剩余空间+1
        sleep(rand()%3);
    }
}

int main()
{
    int i,ret;
    int P_arr[5];
    int C_arr[4];
    pthread_mutex_init(&mutex,NULL);
    sem_init(&empty,0,3);//空间多大就设多少
    sem_init(&full,0,0);

    pthread_t P_pthread[5];
    pthread_t C_pthread[4];
    //创造生产者
    for(i=0;i<5;++i)
    {
        P_arr[i]=i;
        ret=pthread_create(&P_pthread[i], NULL, producer,&P_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }
    //创造消费者
    for(i=0;i<4;++i)
    {
        C_arr[i]=i;
        ret=pthread_create(&C_pthread[i], NULL, consumer,&C_arr[i]);
        if(ret!=0)
        {
            printf("pthread_create error, [%s]\n", strerror(ret));
            return -1;
        }
    }
    //等待线程结束
    for(i=0;i<5;++i)
    {
        pthread_join(P_pthread[i],NULL);
    }
    for(i=0;i<5;++i)
    {
        pthread_join(C_pthread[i],NULL);
    }
    //销毁锁
    pthread_mutex_destroy(&mutex);
    //销毁信号量
    sem_destroy(&empty);
    sem_destroy(&full);
    return 1;
}
```
