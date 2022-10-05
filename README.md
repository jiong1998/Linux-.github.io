
# Linux系统编程
个人通过学习，整理出了一份Linux下系统编程的笔记，包含了文件IO、进程、进程间通信、信号、线程、互斥等知识点，持续更新
## Day2
### 1. Vim三种工作模式
Vi有三种基本工作模式: 命令模式、文本输入模式、末行模式。
![在这里插入图片描述](https://img-blog.csdnimg.cn/c06dc943c1584f27b543875679a8d174.png)
通过shell命令进入命令模式。
1. 用户按下esc键, 就可以使vi进入命令模式下; 当使用vi打开一个新文件开始也是进入命令模式下.
2. 从命令模式切换到文本输入模式只需输入如下命令: i
3. 从命令模式切换到末行模式, 输入冒号:,存在以下命令
![末行模式的命令](https://img-blog.csdnimg.cn/5b7ec1d194c34fd390dca545d7e5c875.png)

### 2. gcc
#### gcc的工作流程
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
#### gcc常用命令
- -v  查看gcc版本号, --version也可以
- -E  生成预处理文件
- -S  生成汇编文件
- -c  只编译, 生成.o文件, 通常称为目标文件
- -I   指定头文件所在的路径
- -L   指定库文件所在的路径
- -l    指定库的名字
- -o   指定生成的目标文件的名字
- -g   包含调试信息, 使用gdb调试需要添加-g参数

### 3.  静态库和共享库
#### 3.1 库的介绍
库是二进制文件, 是源代码文件的另一种表现形式, 是**加了密的源代码**;  是一些功能相近或者是相似的函数的集合体.
#### 3.2 库制作完成后, 如何给用户使用
需要两个文件，用户才能使用：
- 头文件(.h文件)---包含了库函数的声明
- 库文件(.a文件或者.so文件)---包含了库函数的代码实现

<font color='red'> 注意: 库不能单独使用, 只能作为其他执行程序的一部分完成某些功能, 也就是说只能被其他程序调用才能使用.
 </font>
####  3.3 静态库（static library）
静态库可以认为是一些目标代码的集合, 是在**可执行程序运行前就已经加入到执行码中**, 成为执行程序的一部分. 按照习惯, 一般以.a做为文件后缀名。静态库文件一般由**多个.o文件制作成一个.a文件**，具体看下一小节**静态库的制作**
- 前缀：lib
- 库名称：自定义即可, 如test
- 后缀：.a

所以最终的静态库的名字应该为：libtest.a
&emsp;&emsp;注意：一个.o文件也能看作是库文件，是库文件的一个特例，库文件一般由多个.o文件构成

##### 3.3.1 静态库的制作
1. 将.c文件制作成.o文件
```
gcc -c fun1.c fun2.c
//可以不加-o默认生成对应的.o文件
```

3. 使用ar命令将.o文件打包成.a文件
```
ar rcs libtest1.a fun1.o fun2.o
```
##### 3.3.2 静态库的使用
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
#### 3.4 动态库

静态库是.a结尾，动态库则是以.so结尾，还没学，略。
## Day3
### 1.  Makefile

makefile文件中定义了一系列的规则来指定, 哪些文件需要先编译, 哪些文件需要后编译, 哪些文件需要重新编译, 甚至于进行更复杂的功能操作, 因为makefile就像一个Shell脚本一样, 其中也可以执行操作系统的命令.  makefile带来的好处就是——“自动化编译”, 一旦写好, **只需要一个make命令, 整个工程完全自动编译**, 极大的提高了软件开发的效率。 makefile文件是用来管理项目工程文件，通过执行make命令，**make就会自动解析并执行makefile文件**。

#### 1.1 makefile的基本规则
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

#### 1.2 makefile的工作原理

要想生成目标文件，先要检查依赖条件是否都存在：
- 若都存在，则比较目标时间和依赖的时间，如果依赖的时间比目标的时间新，
则重新生成目标；否则不重新生成。
- 若不存在，则往下找有没有生成依赖的规则，有则生成，没有则报错。
![在这里插入图片描述](https://img-blog.csdnimg.cn/94421db31bc24e81ad56c1abbec1a820.png)
#### 1.3 makefile中的变量（不记，只当需要的时候查找使用）
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

#### 1.4 makefile函数与清理
标记一下，没学。对应07-makefile的第五个版本。

### 2.  gdb
标记一下，美学。对应08-gdb介绍 到 4-gdb调试综合练习

### 3.  文件IO
C语言使用fopen函数打开一个文件, 返回一个FILE* fp, fp指向堆空间，堆空间存放的不是文件本身，而是文件的描述信息的结构体。
![36a7b770c79198361d.png)](https://img-blog.csdnimg.cn/7e81c82117be4a4f80023554364022a2.png)
这个指针指向的结构体有三个重要的成员：
- 	**文件描述符:** 通过文件描述符可以找到文件的**inode**, 通过inode可以找到对应的数据块
- **文件指针**: 读和写共享一个文件指针, 读或者写都会引起文件指针的变化
- **文件缓冲区**: 读或者写会先通过文件缓冲区, 主要目的是为了减少对磁盘的读写次数, 提高读写磁盘的效率.

### 4.  C库函数和系统函数
标记一下，没学。对应16-C库函数和系统函数的关系。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2a830427f61146cd8ab42f1a907a811e.png)

### 5. 虚拟地址空间
用户区的内存4区模型：
	
- 代码段：.text段。 程序源代码（二进制形式）。

- 数据段：只读数据段 .rodata段。初始化数据段 .data段。 未初始化数据段 .bss 段。

- stack：栈。 在其之上开辟 栈帧。	windows 1M --- 10M	Linux： 8M --- 16M

- heap：堆。 给用户自定义数据提供空间。 约 1.3G+

进程的虚拟地址空间分为**用户区**和**内核**区, 其中内核区是受保护的, 用户是不能够对其进行读写操作的;

内核区中很重要的一个就是进程管理, 进程管理中有一个区域就是PCB(**本质是一个结构体**)，称进程控制块;
PCB中有文件描述符表, **文件描述符表中存放着打开的文件描述符**, 涉及到文件的IO操作都会用到这个文件描述符.


### 6. PCB与文件描述符
每一个进程有一个PCB。PCB是一个结构体，其中有一项是文件描述符表，文件描述符表中存放着打开的文件描述符。

一个进程默认会打开三个文件：标准输入，标准输出，标准出错，对应index：0，1，2。每当进程每打开一个文件，就会在文件描述符表上记录一个文件描述符。

![在这里插入图片描述](https://img-blog.csdnimg.cn/d499f0ad0f38482e880376b1d1130dfe.png)
&emsp;&emsp;强调：文件描述符的作用：通过文件描述符可以找到文件的**inode**, 通过inode可以找到对应的数据块。

## Day4
### 1. open、close、read、write、lseek、dup、dup2、fcntl

#### 1. open函数
头文件 #include <fcntl.h>

**1.1 函数原型:**
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);

**1.2 函数描述:**
 打开或者新建一个文件
 
**1.3 函数参数：**
pathname参数是要打开或创建的文件名,和fopen一样, pathname既可以是相对路径也可以是绝对路径。
1. 必选项:**以下三个常数中必须指定一个, 且仅允许指定一个。**
- O_RDONLY 只读打开
- O_WRONLY 只写打开
- O_RDWR 可读可写打开
2. 以下可选项可以同时指定0个或多个, 和**必选项按位或起来作为flags参数**。可选项有很多, 这里只介绍几个常用选项：
- O_APPEND 表示追加。如果文件已有内容, 这次打开文件所写的数据附加到文件的末尾而不覆盖原来的内容。
- O_CREAT 若此文件不存在则创建它。使用此选项时需要提供第三个参数mode, 表示该文件的访问权限。
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
**1.4 函数返回值:**
&emsp;&emsp;成功: 返回一个最小且未被占用的文件描述符
&emsp;&emsp;失败: 返回-1, 并设置errno值.

#### 2. close函数
**2.1 函数原型:**
int close(int fd);

**2.2 函数描述:**
关闭文件

**2.3 函数参数：**
fd: 文件描述符

**2.4 函数返回值:**
&emsp;&emsp;成功: 返回0.
&emsp;&emsp;失败: 返回-1, 并设置errno值.

需要说明的是,当一个进程终止时, 内核对该进程所有尚未关闭的文件描述符调用close关闭,所以即使用户程序不调用close, 在终止时内核也会自动关闭它打开的所有文件。但是对于一个长年累月运行的程序(比如网络服务器), 打开的文件描述符一定要记得关闭, 否则随着打开的文件越来越多, 会占用大量文件描述符和系统资源。

#### 3. read函数
**3.1 函数原型:**
ssize_t read(int fd, void *buf, size_t count);

**3.2 函数描述:**
从打开的设备或文件中读取数据

**3.3 函数参数：**
- fd: 文件描述符
- buf: 读上来的数据保存在缓冲区buf中
- count: buf缓冲区存放的最大字节数

**3.4 函数返回值:**
- \>0：读取到的字节数
- =0：文件读取完毕
- -1： 出错，并设置errno
#### 4. write函数
**4.1 函数原型:**
ssize_t write(int fd, const void *buf, size_t count);

**4.2 函数描述:**
向打开的设备或文件中写数据

**4.3 函数参数：**
- fd: 文件描述符
- buf: 缓冲区，要写入文件或设备的数据
- count：buf中数据的长度

**4.4 函数返回值:**
&emsp;&emsp;成功：返回写入的字节数
&emsp;&emsp;错误：返回-1并设置errno

#### 5. lseek函数
**5.1 函数原型:**
off_t lseek(int fd, off_t offset, int whence);

**5.2 函数描述:**
所有打开的文件都有一个当前**文件偏移量**(current file offset),以下简称为cfo. cfo通常是一个非负整数, 用于表明文件开始处到文件当前位置的字节数. 读写操作通常开始于 cfo, 并且使 cfo 增大, 增量为读写的字节数. 文件被打开时, cfo 会被初始化为 0, 除非使用了 O_APPEND.
使用 lseek 函数可以改变文件的 cfo.

**5.3 函数参数：**
- fd: 文件描述符
- offset：文件偏移量， offset 的含义取决于参数 whence。
- whence：SEEK_SET：文件开头位置、SEEK_CUR：当前位置、
SEEK_END：文件结尾位置

**5.4 函数返回值:**
若lseek成功执行, 则返回新的偏移量。
#### 6. dup函数
头文件 <unistd.h>
**6.1 函数原型:**
int dup(int oldfd);

**6.2 函数描述:**
复制的文件描述符。
当调用dup函数后，oldfd和返回的int变量都指向同一个文件。
```cpp
int newfd = dup(fd);//此时newfd和fd这两个文件描述符都指向同一个文件
```
**6.3 函数参数：**
- oldfd -要复制的文件描述符

**6.4 函数返回值:**
&emsp;&emsp;成功：返回最小且没被占用的文件描述符
&emsp;&emsp;错误：返回-1并设置errno

<font color='red'> 注意：当调用dup后，内核会维护**描述符计数**，close一个文件描述符时，计数-1，只有减到0时，文件才真正的关闭。 </font>


#### 7. dup2函数
**7.1 函数原型:**
int dup2(int oldfd, int newfd);

**7.2 函数描述:**
复制文件描述符

**7.3 函数参数：**
- oldfd-原来的文件描述符
- newfd-复制成的新的文件描述符

**7.4 函数返回值:**
&emsp;&emsp;成功：返回0。并将oldfd复制给newfd, 两个文件描述符指向同一个文件
&emsp;&emsp;错误：返回-1并设置errno

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

#### 8. fcntl
头文件 <fcntl.h>
**8.1 函数原型:**
int fcntl(int fd, int cmd, ... /* arg */ );


**8.2 函数描述:**
改变已经打开的文件的属性。用法比较多

**8.3 函数参数：**
- fd: 文件描述符
- cmd: 
-- 若cmd为F_DUPFD, 复制文件描述符, 与dup相同
-- 若cmd为F_GETFL, 获取文件描述符的flag属性值
-- 若cmd为 F_SETFL, 设置文件描述符的flag属性
- /* arg */:可变参数

**8.4 函数返回值:**
函数返回值:返回值取决于cmd
- 成功:
-- 若cmd为F_DUPFD, 返回一个新的文件描述符
-- 若cmd为F_GETFL, 返回文件描述符的flags值
-- cmd为 F_SETFL, 返回0
- 失败返回-1, 并设置errno值.

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

## Day5（进程）
### 1. 程序与进程
- 程序，是指编译好的二进制文件，在磁盘上，占用磁盘空间, 是一个**静态**的概念.
- 进程，一个启动的程序， 进程占用的是系统资源，如：物理内存，CPU，终端等，是一个**动态**的概念

### 2. 并行和并发
- 并发：在一个时间段内, 是在同一个cpu上, 同时运行多个程序。（时间片等）
- 并行：并行性指两个或两个以上的程序在同一时刻发生(需要有多颗cpu)。

### 3. PCB-进程控制块
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


### 4. 进程状态的转化
![](https://img-blog.csdnimg.cn/b0e5bddfa12f4c40b37d34991f0ddcef.png)

### 5. fork()
头文件 <unistd.h>
#### 5.1 fork()原理
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

#### 5.2 fork()代码
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

#### 5.3 父子进程不能共享全局变量
如标题所示，原因是：会把父进程的用户区完全copy一份给子进程，所以子进程也有自己的全局变量。而且，如果共享的话，还要**进程间通信**干嘛？

写时复制，读时共享：
- 如果父子进程仅仅是对变量做“读”操作，这时候该变量在物理内存中是共享的（节省空间）。
- 如果说父子进程中的某进程对变量进行修改，则会复制一份该数据的副本，在副本上对该数据进行修改，并映射回虚拟地址。如图所示
![看图中间的箭头](https://img-blog.csdnimg.cn/d0ba96fc080f46588c3c8b6405d199ce.png)


所以父子进程不能通过全局变量进程通信！如果父子进程想要通信，必须借助其他工具。

### 6. ps命令和kill命令
ps：查看进程相关信息。
kill：**用于向运行中的进程发送信号**，默认发送的信号是终止信号，会请求进程退出。不一定是杀死进程，也可以发送其他信号。

主要用法
1.
ps aux | grep "xxx"//grep 关键字:可以查看启动信息中包含关键字的进程
ps ajx | grep "xxx"
- -a：（all）当前系统所有用户的进程
- -u：查看进程所有者及其他一些信息
- -x：显示没有控制终端的进程 -- 不能与用户进行交互的进程【输入、输出】
- -j: 列出与作业控制相关的信息
2.
kill -l 查看系统有哪些信号
kill -9 pid 杀死某个线程


### 7. exec
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
#### 7.1  execl()  

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

#### 7.2 execlp()  

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


### 8. 子进程资源回收
当一个子进程退出之后，子进程只能**回收自己的用户区的资源**，但是不能回收内核空间的PCB资源，必须由它的父进程调用**wait**或者**waitpid函数**完成对子进程资源的回收，避免造成系统资源的浪费。

### 9. 孤儿进程
若父进程先退出，子进程就变成孤儿进程。

并且为了保证每个进程都有一个父进程，孤儿进程会被init进程领养，init进程成为了孤儿进程的养父进程，当孤儿进程退出之后，由init进程完成对孤儿进程的回收。
### 10. 僵尸进程
子进程先退出，父进程没有完成对子进程的回收（回收内核区资源），此时子进程变成僵尸进程。
如何解决僵尸进程：
&emsp;&emsp;使用杀死僵尸进程父进程的方法来解决僵尸进程。原因是：杀死其父进程可以让init进程领养僵尸进程,最后由init进程回收僵尸进程.

### 11. 子进程回收函数
头文件 <sys/wait.h>
　<font color='red'>所有的进程回收函数都必须由**父进程**完成！！ </font>
#### 11.1 wait函数

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

#### 11.2 waitpid函数
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
- =0：参3为WNOHANG，且目前没有子进程退出。


## Day6（进程间通信）
### 1. 进程间通信的基本概念
Linux环境下，**进程地址空间相互独立**，**每个进程各自有不同的用户地址空间**。**任何一个进程的全局变量在另一个进程中都看不到**，**所以进程和进程之间不能相互访问**，<font color='red'> 要交换数据必须通过**内核** </font>。具体来说，在内核中开辟一块缓冲区，进程1把数据从用户空间拷到内核缓冲区，进程2再从内核缓冲区把数据读走，内核提供的这种机制称为**进程间通信**（**IPC**，InterProcess Communication）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/596a8238029e44cead74021b739fb82e.png)
### 2. 进程间通信的方式
在进程间完成数据传递需要借助操作系统提供特殊的方法,现今常用的进程间通信方式有：
- **管道: PIPE、FIFO** (使用最简单)
- **信号** (开销最小)
- **共享映射区** (无血缘关系)
- **本地套接字** (最稳定)

### 3. 利用管道的方式进行进程间通信（父子进程之间）
**管道应用于有血缘关系的进程之间。**
#### 3.1 管道的概念
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

#### 3.2 创建管道pipe函数
头文件： <unistd.h3>

**函数原型:**
```cpp
int pipe(int fd[2]);
```
**函数作用:** 创建一个管道

**函数参数:** 若函数调用成功，**fd[0]存放管道的读端，fd[1]存放管道的写端**

**返回值:**
&emsp;&emsp;成功返回0；
&emsp;&emsp;失败返回-1，并设置errno值。

函数调用成功返回读端和写端的文件描述符，其中fd[0]是读端， fd[1]是写端，**向管道读写数据是通过使用这两个文件描述符进行**的，**读写管道的实质是操作内核缓冲区**。对管道的读写操作其实就有点像对文件的操作。

管道创建成功以后，创建该管道的进程（父进程）同时掌握着管道的读端和写端。如何实现父子进程间通信呢？

#### 3.3 代码实现父子进程使用管道通信
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

#### 3.4 进程的管道通信执行命令ps aux | grep bash
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

#### 3.5 管道的读写行为详解
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

#### 3.6 设置管道为非阻塞
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
### 4. 利用FIFO的方式进行进程间通信
头文件：
#include <sys/types.h>
#include <sys/stat.h>
#### 4.1 FIFO的概念
FIFO常被称为**命名管道**。管道(pipe)只能用于“有血缘关系”的进程间通信。**但通过FIFO，不相关的进程也能交换数据。**


FIFO是Linux基础文件类型中的一种（文件类型为p，可通过ls -l查看文件类型）。但FIFO文件在磁盘上没有数据块，文件大小为0，仅仅用来标识内核中一条通道。进程可以打开这个文件进行read/write，实际上是在读写内核缓冲区，这样就实现了进程间通信。

**FIFO 不同于PIPE的地方在于，他是双向的，两端都可读可写。**

#### 4.2 创建管道
1. 方式1-使用**命令** mkfifo
命令格式： mkfifo 管道名
例如：mkfifo myfifo
2. 方式2-使用**函数**
int mkfifo(const char *pathname, mode_t mode);
```cpp
int result=mkfifo("./myfifo", 0777);
```

当创建了一个FIFO，就可以使用open函数打开它，**常见的文件I/O函数都可用于FIFO**。如：close、read、write、unlink等。

FIFO严格遵循**先进先出**（first in first out），对FIFO的读总是从开始处返回数据，对它们的写则把数据添加到末尾。**FIFO不支持诸如lseek()等文件定位操作。**


#### 4.3 使用FIFO完成两个不同进程间的通信

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
    read(fd,buf, sizeof (buf));
    printf("buf=[%s]", buf);
    close(fd);
}
```

### 5. 内存映射区
**存储映射I/O (Memory-mapped I/O) 使一个磁盘文件与内存中的一个缓冲区相映射。**从缓冲区中取数据，就相当于读文件中的相应字节；将数据写入缓冲区，则会将数据写入文件。这样，就可在不使用read和write函数的情况下，使用地址（指针）完成I/O操作。**即操作内存=操作文件**

使用存储映射这种方法，首先应通知内核，将一个指定文件映射到内存区域中。**这个映射工作可以通过mmap函数来实现。**

<font color='red'> **通过借助文件来实现进程间的通信** </font>
![在这里插入图片描述](https://img-blog.csdnimg.cn/f7ebfbc8be6c457f9524669268c2216e.png)
#### 5.1 mmap函数
**5.1. 1 函数原型**
   void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
   
**5.1. 2 函数作用**:
建立存储映射区

**5.1. 3参数**
- addr: 	指定内存的起始地址, **设为NULL, 由系统指定内存其实地址**
- length：映射到内存的文件长度。获取文件大小的方式：lseek(), stat()
- prot：	映射区的保护方式, 最常用的:
读：PROT_READ
写：PROT_WRITE
读写：PROT_READ | PROT_WRITE
- flags：	映射区的特性, 可以是
MAP_SHARED: **对映射区的修改会反映到文件本身**（可以对文件进行修改）, 且允许其他映射该文件的进程共享。
MAP_PRIVATE: 对映射区的写入操作会产生一个映射区的复制(copy-on-write), **对此映射区所做的修改不会对文件产生影响**。所以如果只是对文件进行读操作，用此参数。
- fd：由open返回的文件描述符, 代表要映射的文件。
- offset：以文件开始处的偏移量, 必须是4k的整数倍, **通常为0**, 表示从文件头开始映射。

**5.1. 4函数返回值**
- 成功：返回创建的映射区首地址；
- 失败：MAP_FAILED宏

#### 5.2 mmap函数

**5.2. 1 函数原型**
int munmap(void *addr, size_t length);
   
**5.2. 2 函数作用**:
释放由mmap函数建立的存储映射区

**5.2. 3参数**
- addr：调用mmap函数成功返回的参数（映射区首地址）
- length：映射区大小（mmap函数的第二个参数）

**5.2. 4函数返回值**
- 成功：返回0
- 失败：返回-1，设置errno值


## Day7（信号）
### 1. 信号的基本介绍
进程A给进程B发送信号，进程B收到信号之前执行自己的代码，收到信号后，**不管执行到程序的什么位置，都要暂停运行，去处理信号**，处理完毕后再继续执行。与硬件中断类似——异步模式。但信号是软件层面上实现的中断，早期常被称为“软中断”。
<font color='red'> 每个进程收到的所有信号，都是由内核负责发送的 </font>

几个常用到的信号：
SIGINT、SIGQUIT、SIGKILL、SIGSEGV、SIGUSR1、SIGUSR2、SIGPIPE、SIGALRM、SIGTERM、SIGCHLD、SIGSTOP、SIGCONT
SIGCHLD：子进程退出后，内核会给父进程发送该信号。

#### 1.1 信号的状态
信号有三种状态：产生、未决和递达。
1. 信号的产生：
- 按键产生，如：Ctrl+c、Ctrl+z、Ctrl+\
- 系统调用产生，如：kill、raise、abort
- 软件条件产生，如：定时器alarm
- 硬件异常产生，如：非法访问内存(段错误)、除0(浮点数例外)、内存对齐出错(总线错误)
-  命令产生，如：kill命令
2. 未决：产生和递达之间的状态。主要由于阻塞(屏蔽)导致该状态。 
3. 递达：递送并且到达进程。
#### 1.2 信号的处理方式
信号的处理方式有三种：
- 执行默认动作 （绝大多数都是终止，因为终止对系统产生影响最小）：
Term：终止进程
Ign：忽略信号 (默认即时对该种信号忽略操作)
Core：终止进程，生成Core文件。(查验死亡原因，用于gdb调试)
Stop：停止（暂停）进程
Cont：继续运行进程
- 忽略信号(丢弃不处理)
- **捕捉信号**(调用用户的自定义的处理函数)：需要告诉内核，用户希望如何处理某一种信号，说白了就是**用户写一个信号处理函数**，然后将这个函数告诉内核。当该信号产生时，由**内核来调用用户自定义的函数**，以此来实现某种信号的处理。

注意：SIGKILL和SIGSTOP不能捕获，不能阻塞，不能忽略。只能执行他们的默认动作。SIGKILL的默认动作是终止，SIGSTOP的默认动作是暂停。

### 2. 信号相关函数
#### 2.1 signal函数
作用：注册信号捕捉函数
函数原型
```cpp
void handler(int signum);//信号处理函数
sighandler_t signal(int signum, sighandler_t handler);//注册信号处理函数
//signum：信号编号
//handler：信号处理函数
```
注意：内核执行的信号处理函数，不是进程执行。

#### 2.1 kill函数
