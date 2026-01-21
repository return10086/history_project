'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-10-18 16:16:45
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-10-22 01:31:02
FilePath: \设计与建造\test.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
import socket

# 创建 TCP/IP 套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# 绑定套接字到端口
server_address = ('localhost', 10000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# 监听传入的连接
sock.listen(1)

while True:
    # 等待连接
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)

        # 向客户端发送欢迎消息
        message = 'Welcome to the server!'
        connection.sendall(message.encode())

    finally:
        # 清理连接
        connection.close()
