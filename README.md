# MyFrame
easy coding on linux 


///////////////////////////////////////
该项目使用qmake进行管理
    qmake
    make
    make clean
使用QT工具时：
    编译命令选择：
        temp.sh
    清理命令选择：
        make clean
运行时文件选择：
    TEMP/main.bin
如何使用Qt Creator打开此项目：
    使用*.pro文件打开此项目
/////////////////////////////////////////
文件的添加：
    在temp.pro文件中添加要添加的头文件以及源文件路径
    并在src文件夹中相应的文件

//////////////////////////////////////////
git管理：
SSH:
    git@github.com:lkpworkspace/my_cplusplus_master.git
push an existing repo:
    git remote add origin git@github.com:lkpworkspace/my_cplusplus_master.git
    git push -u origin master


Event类：           block            说明                    是否让MyAllEvent接收此事件
MyEvent               no         抽象事件类                    no
MyKeyEvent            yes        监听键盘消息的事件类           yes
MyMouseEvent          yes        监听鼠标消息的事件类           yes
MyNormalEvent         no         通用的事件类                   yes
MySock                           抽象套接字类，继承MyEvent      yes
MyUdp                 yes        Udp事件类，继承MySock类        yes      可通过继承该类，重写CallBackFunc，并不调用父类CallBackFunc让MyAllEvent接收不到此事件（实际并没有必要）
MyTcpServer           no         Tcp事件类，继承MySock类        yes      可通过继承该类，重写CallBackFunc，并不调用父类CallBackFunc让MyAllEvent接收不到此事件
MyTcpClient           no         Tcp事件类，继承MySock类        yes      可通过继承该类，重写CallBackFunc，并不调用父类CallBackFunc让MyAllEvent接收不到此事件
MyTcpSocket           no         Tcp事件类，继承MyEvent类       yes      可通过继承该类，重写CallBackFunc，并不调用父类CallBackFunc让MyAllEvent接收不到此事件
MyTFTP                yes        TFTP事件类，继承MyUdp类        no
MyRawSock                        // TODO...


数据结构类：
MyList                           双向循环链表
MyVec                            动态数组
MyHeap                           堆


OS:
	Ubuntu
IDE:
	QtCreator

调试笔记：
03月27号
1. 调试链表程序，发现老是段错误？
	原因是我把同一个节点同时加入两个链表中，第二个链表修改该节点的prev,next指针，导致
    第一个链表段错误，切记切记！！！(一个节点在同一时刻只能在一个链表中)

2. 程序加锁后，造成死锁的情况？
    在一个加锁的程序片段中，调用一个使用同样锁的函数调用，导致死锁的情况

03月28号
1. 在将事件加入任务队列后没有删除导致，后续该事件有没有触发都会调用此回调函数？
    在执行过的回调函数后删除事件，是否再次加入由客户逻辑决定
2. 在某个事件触发执行回调后，是否重新加入监听队列
    此版本不再加入队列，若要加入队列可在回调中加入队列
3. 潜在的问题？？？
    任务有的很忙，有的很闲，需要任务调度策略
4. 创建两个线程后，idle_task进入无限循环？？？
    创建两个线程后加入空闲队列，然后等待后又加入队列，造成链表混乱
    解决： 初始创建后不加入队列
    引申出的问题：
        遍历空队列时，Begin方法返回空，造成与End比较时出现程序崩溃
        解决方法：队列为空就返回 End 的值

04月15号
1. 编写调试输出的可变测宏时，发现编译不过?
	应将MyDebugPrint(format, ...) 改为 MyDebugPrint(...), 这样在只有format参数的时候也不会出错。

04月16号
1. 事件删除操作还没有做，目前事件删除会造成内存泄漏。  mark

04月17号
1. 编写完成MyTFTP类，
	存在的问题： 
		发送读文件请求时后，服务无回应ack(没有什么影响)
		需要修改GetFile,SendFile为阻塞调用 (已经修改为阻塞函数)

04月20号
1. 使用消息队列控制服务进行某些参数的配置，以及可以获得服务的运行状态（use root）    // delay
2. 要编写通用的类，使普通任务也能使用线程池   // 已完成(MyNormalEvent)
3. MyTFTP 潜在的问题： 对于请求的不存在的文件没有进行错误处理    // delay

04月22号
1. 已解决编写通用类
2. 由于加入配置功能(msgget)，以后程序的运行要使用 sudo 启动 // 已定义宏可以开关此功能

04月23号
1. 添加MySelfprotcol类
2. 添加设置udp 广播属性，可以发送广播包 // 未测试

04月28号
1. MyUdp Write方法添加mutex互斥量
2. 设置MyEvent的类以及继承Myevent类的析构函数为虚函数
3. 修复MySqlite3的一个BUG(GetRow判断的bug，修改 < 为 <=)
4. 编写MyTalk项目， 实现了登录的功能

04月30号
1. 编写Mytalk项目，实现传输好友的功能; 需要添加选项ip，在线状态 // mark 
2. 用户登录过程中，服务需要记录用户的ip，并标记在线 //TODO...
05月09号
1. 编写MyTalk项目，实现一对一聊天功能
2. 在界面与聊天逻辑之间添加代理类，使用信号与槽的通信机制，
   使用代理类即保持了界面与聊天逻辑的松耦合，又解决了不同线程间调用界面函数失败的问题
3. 界面要添加在线状态标志, 添加登录超时避免程序卡死状态，消息框消息左右对齐问题 // TODO...
4. 添加 注册 用户功能， 查找用户功能， 添加 删除好友功能， // TODO...

05月11号
1. 完善MyTFTP类的错误处理 // TODO...
2. 完善MyTask类的错误处理 // TODO...
3. 实现RawSocket类       // TODO...

05月20号
1. 已完善MyTFTP的错误处理
2. 已完善MyTask的错误处理
3. 实现了MyApp的退出清理功能







































