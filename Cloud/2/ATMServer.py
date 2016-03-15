import socket, threading
 
HOST = ''
PORT = 54321
 
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((HOST, PORT))
sock.listen(4)
# bank is a dictionary associating account and deposit(int)
bank = {
	'tony': 100,
}
# accounts is a dictionary associating username and password
accounts = {
	'tony':'123',
}
lock = threading.Lock()

def deposit(usr,cmd):
	if len(cmd)!=2:
		return 'Too less/many arguments'
	try:
		cash = int(cmd[1])
	except:
		return 'Must be intergral input'
	if usr in bank:
		lock.acquire()
		bank[usr] += cash
		lock.release()
		print '%s save %d' %(usr,cash)
	else:
		bank[usr] = cash

def withdraw(usr,cmd):
	if len(cmd)!=2:
		return 'Too less/many arguments'
	try:
		cash = int(cmd[1])
	except:
		return 'Must be intergral input'
	if usr in bank and bank[usr]>=cash:
		lock.acquire()
		bank[usr] -= cash
		lock.release()
		print '%s take out %d' %(usr,cash)
	else:
		return "You don't have enough deposit!!"

def inquiry(usr,cmd):
	if len(cmd)!=1:
		return 'Too less/many arguments'
	if usr in bank:
		return "You have %s in bank" % bank[usr]
	else:
		return "You have 0 in bank"

def transfer(usr,cmd):
	# cmd = ['t', 100, 'usr2']
	if len(cmd)!=3:
		return 'Too less/many arguments'
	try:
		cash = int(cmd[1])
	except:
		return 'Must be intergral input'
	usr2 = cmd[2]
	if usr in bank and bank[usr]>=cash:
		lock.acquire()
		if usr2 in bank:
			bank[usr2] += cash
		else:
			bank[usr2] = cash
		bank[usr] -= cash
		lock.release()
		return "You transfer %d to %s" % (cash,usr2)
	else:
		return "You don't have enough deposit!!"

# commands define list of operations avaliable to clients
commands = {
	'd':deposit,
	'w':withdraw,
	'i':inquiry,
	't':transfer,
	# exit is defined in 'run', maybe move this to client?
}

# main method handling client request
def run(socket,address):
    print 'Client %s:%s connected.' % address
    usr = asklogin(socket)
    while usr:
        data = socket.recv(1024)
        cmd = data.split()
        if cmd[0] in commands:
        	m = commands[cmd[0]](usr,cmd)
        	if m:
        		socket.sendall(m+'\r\n')
        	else:
        		socket.sendall("Transcation finished\r\n")
        else:
        	if cmd[0]=='exit':
        		break
        	socket.sendall("Undefined operation\r\n")
    socket.close()
    print 'Client %s:%s disconnected.' % address

def asklogin(socket):
	global accounts 
	chance = 3
	socket.sendall('Please login.\r\n')
	while True:
		socket.sendall('Your account?(%d)\r\n' % chance)
		ac = socket.recv(1024).strip()
		print 'Receive %s' % ac
		if not ac in accounts:
			socket.sendall('New account, set you password\r\n')
			pw = socket.recv(1024).strip()
			accounts[ac] = pw
			return ac
		socket.sendall('Your password?\r\n')
		pw = socket.recv(1024).strip()
		print 'Receive %s' % pw
		if accounts[ac] == pw:
			socket.sendall('Welcome %s!!!\r\n' % ac)
			return ac
		chance -= 1
		if chance == 0:
			socket.sendall('Connection closed\r\n')
			return None
		socket.sendall('Invalid account, please try again\r\n')

# print bank/accounts in server side
def maintain():
	while 1:
		d = raw_input()
		if d == 'bank':
			for k in bank:
				print '%s: %s' % (k,bank[k])
		elif d == 'acc':
			for k in accounts:
				print '%s: %s' % (k,accounts[k])
		else:
			print 'what you mean?'
t = threading.Thread(target=maintain)
t.start()

if __name__ == "__main__":
    while True:
        threading.Thread(target=run,args=sock.accept()).start()
t.join()