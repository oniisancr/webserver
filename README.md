# webserver

Try to create a web server using C++.

- 结构：
  - EventHandler类，处理客户端业务逻辑。由ThreadPool线程池管理。
  - Simpoller类，抽象出epoll、kqueue的共同接口，经过条件编译，选择epoll或kqueue，实现跨平台。
  - sockets文件夹下类，是对socket不同阶段的封装。

- 特点：
  - 使用Epoll、kqueue实现IO复用
  - 使用边缘触发、非阻塞socket
  - 使用线程池处理业务逻辑
  - 对socket进行封装，可复用代码

- 问题：
  - 仅支持GET方法，可对simpleweb类重新编写
  - 使用tcp的短链接，即每一次处理完就关闭该连接，不能多次交互。
    - 可删除EventHandler类中关闭连接，另外在线程池中增加对socket超时的关闭连接。
  - 项目出于对方法的学习，未从顶层设计。结构稍显混乱。

- reference:
  - https://www.youtube.com/watch?v=YwHErWJIh6Y
  - https://www.bilibili.com/video/BV1JB4y1q773
  - https://www.bilibili.com/video/BV1qJ411w7du
  - 《TCP/IP网络编程》--尹圣雨
