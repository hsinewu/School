config = {'w':10,'h':10,'win':5}
table = []
turn = True
def init():
	global table, turn
	table = []
	for i in range(config['w']):
		table.append([' ']*config['h'])
	turn = True

def checkDirection(x,y,dx,dy):
	ret = 0
	newx, newy = x+dx, y+dy
	while 0<= newx < config['w'] and 0<= newy < config['h'] and table[newx][newy]==table[x][y]:
		ret += 1
		newx, newy = newx+dx, newy+dy
	return ret

def getConfig():
	return config['w'], config['h'], config['win']

def checkWin(x,y):
	ver = checkDirection(x,y,1,0) + checkDirection(x,y,-1,0) + 1 >= config['win']
	hor = checkDirection(x,y,0,1) + checkDirection(x,y,0,-1) + 1 >= config['win']
	dia = checkDirection(x,y,1,1) + checkDirection(x,y,-1,-1) + 1 >= config['win']
	dia2= checkDirection(x,y,-1,1) + checkDirection(x,y,1,-1) + 1 >= config['win']
	return ver or hor or dia or dia2

def set(x,y):
	global turn
	v = 'o' if turn else 'x'
	table[x][y] = v
	turn = not turn
	return checkWin(x,y), v