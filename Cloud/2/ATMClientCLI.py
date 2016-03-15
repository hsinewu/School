# Echo client program
import socket, select, sys, threading
from Tkinter import *

HOST = 'localhost'    # The remote host
PORT = 54321          # The same port as used by the server
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((HOST, PORT))

def sending():
    while 1:
        sock.sendall(raw_input())

def receiving():
    while 1:
        data = sock.recv(1024)
        if data=='exit':
            sock.close()
            sys.exit()
        sys.stdout.write(data)

r = threading.Thread(target=receiving)
s = threading.Thread(target=sending)
r.start()
s.start()

r.join()
s.join()
# sock.close()
