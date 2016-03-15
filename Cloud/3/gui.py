from Tkinter import *
from functools import partial
import five

def click(x,y):
    global count, status
    if 'win' in status['text'] or btngrid[x][y]['text']:
        return
    count += 1
    isEnd ,btngrid[x][y]['text'] = five.set(x,y)
    if isEnd:
        btngrid[x][y]['bg'] = 'red'
        status['text'] = '%s win' % btngrid[x][y]['text']
    elif count == gridWidth*gridHeight:
        status['text'] = 'Draw'

gridWidth, gridHeight, toWin = five.getConfig()
root = Tk()
root.title("4101056014")
mainframe = Frame(root)
mainframe.grid(row=0,column=0)
status = Label(root,height=3)
status.grid(row=1,column=0)
def init():
    five.init()
    status['text'] = "Make %d in a row" % toWin
    global count, btngrid
    count = 0
    btngrid=[]
    for r in range(gridWidth):
        btns = []
        for c in range(gridHeight):
            btns.append(Button(
                mainframe,
                command=partial(click,x=r,y=c),
                width=6,
                height=3
                ))
            btns[c].grid(row=r,column=c)
        btngrid.append(btns)
Button(root,command=init,text='Reset',width=10).grid(row=2,column=0)

init()
root.mainloop()
# root.destroy()