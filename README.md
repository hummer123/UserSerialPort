# UserSerialPort
A serial debugging assistant with waveform display

如上标题，这是一个可以绘制数据波形的串口调试助手，基于Qt开发，完成在2016年9月份，最近空闲了才决定整理一下并上传到github.
OK,开始进入正题。本软件实现的功能有如下:</br>
> * 1. 基本的串口收发功能,(使用线程及环形缓冲区接收并处理数据):
> * 2. 发送文件和保存接收的数据:
> * 3. 接收特定格式数据并绘制波形:

**主界面:** <br>
![image](https://github.com/hummer123/UserSerialPort/raw/master/README-PIC/major.png)

**次界面:** <br>
![image](https://github.com/hummer123/UserSerialPort/raw/master/README-PIC/minor.png)

## 串口的数据接收
Qt中由于串口类使用的是外带的，**这个类在串口接收时可能将一帧数据会分几次接收**，有两种方法可以解决：
> * 1. 定时接收，当发现串口缓存中有可用数据时，打开定时器，一般定时100ms左右，当然也是根据实际情况来确定的;
> * 2. 使用线程接收数据，这个就比较高效了，本程序就是基于线程的数据接收;

**在用线程接收数据时为了在提高一帧数据的完整接收，程序使用环形数组来接收数据并分析出一帧完整数据后，再显示出来**

##绘制数据波形
由于Qt的界面更新只能在主线程中刷新，所以在另一个窗口中不能开启线程来刷新绘图，程序中使用painEvent来更新绘图，
同时实现历史记录的鼠标拖动查看。

##效果图
> **测试绘图效果**
![image](https://github.com/hummer123/UserSerialPort/raw/master/README-PIC/sin.png)

> **公司控温的效果图**
![image](https://github.com/hummer123/UserSerialPort/raw/master/README-PIC/temp.bmp)

> (注: 为了适应公司系统的调试，所以做了两个发送缓冲区，其中下面一个不可以自动发送哦！！！)
