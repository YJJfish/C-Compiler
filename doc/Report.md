

 

![img](Images/clip_image002.gif)

 

<h1><center>本科实验报告</center></h1>

 

 

<center><font size="5">课程名称：<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;编译原理&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u></font></center>

<center><font size="5">姓名学号：<u>&nbsp;尤锦江&nbsp;3190102352&nbsp;</u></font></center>

<center><font size="5">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<u>&nbsp;张之昀&nbsp;31901033273&nbsp;</u></font></center>

<center><font size="5">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<u>&nbsp;陈一航&nbsp;319010____&nbsp;</u></font></center>

<center><font size="5">学&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;院：<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;竺可桢学院&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u></font></center>

<center><font size="5">专&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;业：<u>&nbsp;&nbsp;&nbsp;计算机科学与技术&nbsp;&nbsp;&nbsp;</u></font></center>

<center><font size="5">指导老师：<u>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;冯雁&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</u></font></center>

<center><font size="5"><u>&nbsp;&nbsp;2022&nbsp;&nbsp;</u>年<u>&nbsp;&nbsp;5&nbsp;&nbsp;</u>月<u>&nbsp;&nbsp;16&nbsp;&nbsp;</u>日</font></center>

<div style="page-break-after:always"></div>



<h2><center>浙江大学实验报告</center></h2>

## 〇、序言

本次实验完成了一个C语言的编译器，能够分析C语言的语法，并将其编译至LLVM IR，最后再编译至目标代码（`.o`文件）。除了基本数据结构和基本指令以外，我们实现了以下C语言特性：

- **数组、结构体**。以及由基本类型、数组、结构体组合而成的更复杂的数据类型。
- **指针类型**。并且支持`&`, `*`, `->`等指针运算。
- **递归式结构体**。结构体内可以定义指向自己类型的指针，从而实现链表。
- **左值和右值**。我们的编译器可以编译C语言支持的任意表达式，例如`((c)?a:b)[3]=st->x;`。
- **可变长参数列表**。例如`void sum(int n, ...);`
- **函数先声明后定义**。若函数只有声明，则由链接器负责链接外部函数。因此我们的编译器支持调用C标准库的函数。
- **类型转换系统**。我们的编译器严格按照C语言的类型转换机制设计。例如`int+float->float`, `pointer+int->pointer`。
- **符号表作用域**。我们的编译器允许在`for`, `while`,`if`以及其他含有语句块的语句内定义变量，并且可以覆盖外层作用域的重名变量。变量的作用域只在语句块内。
- **编译器优化**。支持`-O0`, `-O1`, `-O2`, `-O3`, `-Os`, `-Oz`优化选项。

### 0.1 依赖项



### 0.2 安装说明



### 0.3 工程搭建 & 编译



### 0.4 使用手册



## 一、词法分析



## 二、语法分析



## 三、语义分析



## 四、优化考虑



## 五、代码生成



## 六、测试案例