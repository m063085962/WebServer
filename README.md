# WebServer
本项目是基于C++编写的Web服务器，整个服务器是以事件驱动模式为核心。  
封装了Socket的绑定，监听，读写等操作，并利用线程池实现one loop per thread，从而构建出Reactor模式的网络库。  
支持解析HTTP头部，响应HTTP GET请求，可支持服务器长连接。  

## 系统环境
编写环境于Ubuntu 20.04.3 LTS g++ 9.4.0

## 开发工具
`g++ = 9.4.0`  
`cmake`  
`make`  
`vim git netcat`

## 项目核心
* 采用Epoll I/O多路复用工作函数，工作模式采用ET边缘触发模式
* 采用线程池管理线程，并在程序开始时产生线程，避免程序运行中产生影响性能，并具有可预测性的性能
* 采用One Loop Per Thread + Thread Pool编程模式，其内部实现了一个主从Reactor多线程模式的网络库  
* 采用有限状态机分析TCP字节流是否为HTTP包，高效的解析HTTP请求中的URI以及HTTP头部参数

## 参考
[30dayMakeCppServer](https://github.com/yuesong-feng/30dayMakeCppServer.git)
