from socket import *
import threading


con = threading.Condition()
# host = '127.0.0.1'
port = 8888

s = socket(AF_INET,SOCK_STREAM)
s.bind(('', port))
s.listen(5)
print("监听开始")
data = ""


def NotifyAll(socket):
    global data
    if con.acquire():  # 获取锁
        data = socket.encode('utf-8')
        con.notify_all()  # 表示当线程放弃对资源占用 通知其他可以执行
        con.release()  # 释放锁


def threadOut(conn, content):  # 发送消息
    global data
    while True:
        if con.acquire():
            con.wait()  # 放弃资源占用
            if data:
                try:
                    # print(data)
                    conn.send(data)
                    con.release()  # 解锁
                except:
                    con.release()
                    return


def threadIn(conn, content):  # 接收消息
    global data
    while True:
        try:
            temp = conn.recv(1024).decode('utf-8')
        # print(type(temp))
            if not temp:
                conn.close()
                return
            NotifyAll(temp)
            print(data)
        except:
            temp = content + ':exit'
            NotifyAll(temp.decode('utf-8'))

            # print(error)
while True:
    conn, address = s.accept()  # 返回的是元组
    print("connector:"+str(address[0])+':'+str(address[1]))
    content = conn.recv(1024).decode('utf-8')  # 传入的姓名
    NotifyAll('welcome '+content+' join in the room!!!')
    print(data.decode().strip())
    conn.send(data)
    threading.Thread(target=threadOut, args=(conn, content)).start()
    threading.Thread(target=threadIn, args=(conn, content)).start()
