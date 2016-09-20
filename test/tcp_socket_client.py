import socket
import time

BUF_SIZE = 1024
server_addr = ('127.0.0.1', 6789)
#  server_addr = ('127.0.0.1', 6379)

MSG_SIGLE_TAG = '*'
MSG_BATCH_TAG = '$'
MSG_CRLF = '\r\n'

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(server_addr)


def build_cmd(string):
    args = [arg for arg in string.split(' ') if len(arg) > 0]
    if len(args) > 0:
        cmd = MSG_SIGLE_TAG + str(len(args)) + MSG_CRLF

        for arg in args:
            cmd += MSG_BATCH_TAG + str(len(arg)) + MSG_CRLF + arg + MSG_CRLF

        return cmd

    return ''


while True:
    data = input('>>> ')
    cmd = build_cmd(data)
    if len(cmd) > 0:
        client.send(cmd.encode())
        data = client.recv(BUF_SIZE)
        print(data.decode('utf-8'))

client.close()
