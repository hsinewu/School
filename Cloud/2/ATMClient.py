# Echo client program
import socket, select, sys, threading
from Tkinter import *

class MyApp(Frame):
    def createWidgets(self):
        # left
        self.left = Frame(self)
        self.left.pack(side="left")
        self.textarea = Text(self.left)
        self.textarea.insert(END,"Welcome!!")
        self.textarea.pack()

        # right
        self.right = Frame(self)
        self.right.pack(side="right")

        Button(self.right,text="Inquiry").grid(row=0,column=0)
        # Button(self.right,text="Inquiry",command=self.inq).grid(row=0,column=0)

        Button(self.right,text="Deposit").grid(row=1,column=0)
        # Button(self.right,text="Deposit",command=self.dep).grid(row=1,column=0)
        self.deposit = Entry(self.right)
        self.deposit.insert(0,"0")
        self.deposit.grid(row=1,column=1)

        Button(self.right,text="Withdraw").grid(row=2,column=0)
        # Button(self.right,text="Withdraw",command=self.wit).grid(row=2,column=0)
        self.withdraw = Entry(self.right)
        self.withdraw.insert(0,"0")
        self.withdraw.grid(row=2,column=1)

    def createLogin(self):
        self.popup = Toplevel()
        self.popup.title("Please log in")
        self.popup.attributes("-topmost", True)

        self.e1 = Entry(self.popup)
        self.e1.insert(0,"Your account")
        self.e1.pack()
        self.e2 = Entry(self.popup)
        self.e2.insert(0,"Your password")
        self.e2.pack()

        Button(self.popup, text="Log in").pack()
        # Button(self.popup, text="Log in", command=self.login).pack()
        Button(self.popup, text="Exit", command=self.popup.destroy).pack()

    # def login(self):
    #     global sock
    #     ac = self.e1.get()
    #     pw = self.e2.get()
    #     sock.sendall(ac)
    #     sock.sendall(pw)
    #     data = sock.recv(1024)
    #     if not 'Wel' in data:   # not test
    #         self.e2.delete(0,END)
    #         self.e2.insert(0,"Refused")
    #         return
    #     self.usr = ac
    #     self.popup.destroy()

    # def inq(self):
    #     global sock
    #     sock.sendall('i')
    # def dep(self):
    #     global sock
    #     sock.sendall('d %s' % self.deposit.get())
    # def wit(self):
    #     global sock
    #     sock.sendall('w %s' % self.withdraw.get())

    def __init__(self, master=None):
        Frame.__init__(self, master)
        master.title("4101056014")
        self.pack()
        self.createWidgets()
        self.createLogin()

root = Tk()
app = MyApp(master=root)
# TKINTER IS NOT THREAD SAFE WTF

# a = threading.Thread(target=app.mainloop)
# a.start()

app.mainloop()

# HOST = 'localhost'    # The remote host
# PORT = 54321          # The same port as used by the server
# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# sock.connect((HOST, PORT))

# def receiving():
#     while 1:
#         data = sock.recv(1024)
#         if data=='exit':
#             sock.close()
#             sys.exit()
#         sys.stdout.write(data)
#         if app.ready:
#             app.textarea.insert(END,data)

# r = threading.Thread(target=receiving)
# r.start()

# a.join()
# r.join()
# sock.close()
# root.destroy()
