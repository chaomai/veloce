import socket
import time

BUF_SIZE = 1024

server_addr = ('127.0.0.1', 6789)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(server_addr)

count = 0
while True:
    count += 1

    #  data = input('>>>')
    #  data = '*3\r\n$5\r\nlpush\r\n$7\r\nmy_list\r\n$2\r\n12\r\n'
    #  data = '*1\r\n$4\r\nping\r\n'
    data = '*2\r\n$4\r\necho\r\n$8\r\nqewrasdf\r\n'
    client.send(data.encode())
    data = client.recv(BUF_SIZE)
    print(data.decode('utf-8'))
    break

    if count % 10000 == 0:
        print(count)

client.close()
