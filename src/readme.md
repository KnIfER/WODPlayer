### MyTabControl32
Fluent and elegant.  
\
![](screenshots/me.gif)

### SysTabControl32
Flickering and confusing  
\
![](screenshots/you.gif)

|  File   | Description  | Licence  |
|  ----  | ----  | ----  |
| tab.cpp  | the tab control implementation from [WINE](https://github.com/wine-mirror/wine/blob/72186db5fe5ac509d5404b73a5c6590d6fbbca00/dlls/comctl32/tab.c) | LGPL
| demo\res\\*  | svg and bmp icons from [Fluent-UI](https://github.com/microsoft/fluentui-system-icons/blob/master/icons.md) |  MIT 
| demo\src\\*  | Demo application |  Appache 2.0  


## 改进 tab.c:
- 改进了多行模式下平均每行子标签数目的方法。原方式仅考虑标签个数，引入每行平均宽度后效果更好。

<br/>
<br/>

## Win32 自定义控件开发之由 Wine 入道


此 Wine 非彼 Wine，Wine( Is Not a Emulator )是一个软件启动器，用于在linux或其他支持 poxis 标准的操作系统上运行windows的exe应用程序。

可以说 Wine 项目模仿了 windows 的各种行为，其中就包括一些 Win32 控件，比如选项卡、标签页的“标准控件”:SysTabControl32，其实现就位于一个C语言代码中，tab.c。

SysTabControl32 作为“标准控件”实在名不副实，有太多可以改进的地方，这就需要完全可以改写的“开源控件”，开源控件安卓有很多，纯 Win32 开发就少了。幸运的是 Wine 三千多行的 tab.c，稍加改写就可以在纯 Win32 项目中运行。

新的开源 Tab 控件实现位于一个代码文件 tab.c 中，没有头文件( 或者说头文件与原空间一致 )。调用机制是外部用一个 extern 申明的函数调用其中的 Tab_Register，注册窗口类，类名就是"SysTabControl32"。然后使用的时候创建一个SysTabControl32窗口，响应各种不同的窗口样式，运行的时候就通过发送消息（宏）控制，与标准的 SysTabControl32 使用方法一致。

将 tab.c 中的窗口类名改为 MyTabControl32 就可以与原控件共存，将源代码中创建 SysTabControl32（宏名 WC_TABCONTROL） 窗口的地方改写为创建 MyTabControl32 窗口，就获得了完全可以控制的新 Tab 控件了！

### 改进一：消除闪烁
原控件在多行模式下会有严重的闪烁问题，主要通过双重缓冲解决。

参考：https://stackoverflow.com/questions/14153387/double-buffering-win32-c

http://www.catch22.net/tuts/win32/flicker-free-drawing#

单单双重缓冲是不够的，在创建、显隐子窗口的时候仍然有闪烁问题。于是第一个链接中的两个方案（双重缓冲+WM_ERASEBKGND返回1）都要采用。

### 改进二：固定位置
原控件在多行模式下点击标签会将标签所在行移至最底部，完全无用的功能且令人费解。在原本的方案中，只能通过应用 TCS_BUTTON 样式解决，此样式较为简陋。在新的开源方案中，只需修改一处源码即可。

### 改进三：拖拽排序

todo……




