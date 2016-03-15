from Tkinter import *
from functools import partial
from gen_id import generateID, code_city_pairs

class MyApp(Frame):

    def generate(self):
        self.textarea.insert(END,generateID(self._cityCode,self._gender)+"\n")

    def setGender(self,gender):
        self.gender["text"] = gender
        self._gender = 1 if gender == "Male" else 2

    def setCity(self,city,cityCode):
        self.city["text"] = city
        self._cityCode = cityCode

    def createWidgets(self):
        # top
        self.top = Frame(self)
        self.top.grid(row=0,column=0)
        Label(self.top,text="ID generator").grid(row=0,column=0)
        Button(self.top,text="Generate",command=self.generate,fg="red").grid(row=0,column=1)
        Label(self.top,text="Gender:").grid(row=1,column=0)
        Label(self.top,text="City:").grid(row=2,column=0)
        self.gender = Label(self.top,text="Male")   # default value
        self.gender.grid(row=1,column=1)
        self.city = Label(self.top,text="Tainan")   # default value
        self.city.grid(row=2,column=1)
        # mid
        self.mid = Frame(self)
        self.mid.grid(row=1,column=0)
        Button(self.mid,text="Male",command=partial(self.setGender,gender="Male")).grid(row=0,column=0)
        Button(self.mid,text="Female",command=partial(self.setGender,gender="Female")).grid(row=0,column=1)
        r,c = 1,0
        for code,city in code_city_pairs():
            self[city] = Button(self.mid,text=city,command=partial(self.setCity,city=city,cityCode=code)).grid(row=r,column=c)
            c=c+1
            if c == 3:
                c=0;r=r+1
        self.textarea = Text(self)
        self.textarea.grid(row=2,column=0)


    def __init__(self, master=None):
        Frame.__init__(self, master)
        master.title("4101056014")
        self._cityCode = 'D'   # default value
        self._gender = 1       # default value
        self.pack()
        self.createWidgets()

root = Tk()
app = MyApp(master=root)
app.mainloop()
root.destroy()