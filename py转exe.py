#-*-coding:utf-8-*-
import sys, os, re, time, shutil, base64
from threading import Thread
from random import randint,choice
_VERSION = True if sys.version_info[0]<3 else False
if _VERSION:
    reload(sys)
    sys.setdefaultencoding("utf-8")
    import Tkinter as tk
    from tkMessageBox import askokcancel
    from tkFileDialog import askopenfile, askdirectory
else:
    import tkinter as tk
    from tkinter.messagebox import askokcancel
    from tkinter.filedialog import askopenfile,askdirectory
    from functools import reduce

rootp = os.path.abspath(os.path.dirname(__file__)) + os.sep
Rf = lambda p,m='r':((lambda f:(f.read(),f.close()))(open( p,m)))[0]
Wf = lambda t,p,m='w':((lambda x:(not os.path.exists(x) and os.makedirs(x),((lambda f:((f.write(t),f.close())))(open(p,m)))))(os.path.dirname(p)))
refind = lambda s,data,arg=0:((lambda x:x and x.group(arg) or '')(re.search(s, data, flags= re.DOTALL)))
getcol = lambda:'#{:02x}{:02x}{:02x}'.format(*map(lambda i:randint(0,255),range(3)))
getname = lambda i,j=[chr(i) for i in range(65,91)]:''.join([choice(j) for x in range(i)])


def wrapper(value):
    def twinkle(args,val):
        col = ["#ee9999","#99ee99","#9999ee","#eeee99","#99eeee","#ee99ee"]
        self,text = args
        for i in range(val):
            self.showLabel(text,choice(col),'black')
            time.sleep(0.5)
        else:
            self.showLabel(text,"#eeeeee")

    def schedule(func):
        def target(*args):
            if value>1:
                args = list(args)
                if len(args)==2:
                    Thread(target=twinkle,args=(args,value)).start()
                elif len(args)<2:
                    args += ["Python 转换工具","#eeeeee",'red']
                return func(*args)
            else:
                if len(args)!=5:
                    return func(*args)
                self,path,py,flag,ico = args
                py,ico = self.refilename(py,ico)
                return func(self,path,py,flag,ico)
        return target
    return schedule


class PythonTool(tk.Frame):
    __doc__ = """作者: Obsession    邮箱: 2608135678@qq.com"""
    def __init__(self,):
        tk.Frame.__init__(self)
        self.pack(side=tk.TOP,fill=tk.BOTH,expand='yes')
        self.log = ''
        self.rename = {}
        self.create_dir()
        self.font = "Times 10 normal roman"
        self.master.title("Python 工具")
        self.master.iconbitmap(self.getIcon())
        self.master.geometry('%dx%d+%d+%d' % self.getSeat())
        self.py_temp = self.py_path[0]+':\\py_temp\\'
        self.mainWindow()
        self.mainloop()

    def mainWindow(self):
        attr = lambda i,j:getattr(self.myset.main,j,0)
        self.key = ['test','icon','pypath','uipath','uiname','black','single','desktop','implement']
        self.var = [(tk.StringVar(value=(attr(i,j) or '')) if i<5 else tk.IntVar(value=(attr(i,j) or 0))) for i,j in enumerate(self.key)]
        [self.var[i].set('') for i in (1,2,3,4) if self.var[i].get() and not os.path.exists((self.var[i].get()))]

        self.label = tk.Label(self,font="Times 12 normal roman",justify='left')
        self.label.pack(side='top',fill='both',pady=2)
        tk.Button(self.label,fg='red',text="关于",command=self.about).pack(side='right',anchor='ne',padx=8)
        self.showLabel()
        mainframe = tk.Frame(self)
        mainframe.pack(side='top',fill='both',expand='yes',pady=5)

        frame = [tk.Frame(mainframe) for i in range(9)]
        for obj in frame:obj.pack(side='top',fill='both',expand='yes',pady=5)
        tk.Label(frame[0],font="Times 12 bold",fg='red',text="py 转 exe").pack(side='left')
        tk.Checkbutton(frame[0],fg='#ff00ff', text="添加到桌面", variable=self.var[7], command=lambda i=7:self.bt_call(i)).pack(side='right')

        if not self.var[1].get():
            self.var[1].set(self.getIcon())
        tk.Label(frame[1],text="图标 ( .ioc ):").pack(side='left')
        entry = tk.Entry(frame[1], textvariable=self.var[1])
        entry.pack(side='left',fill='both',expand='yes')
        tk.Button(frame[1], text="浏览",command=lambda i=1:self.bt_call(i)).pack(side='left')
        self.prompt(entry, "选择输入ico图标文件路径")

        tk.Label(frame[2], text="脚本 ( .py ) :").pack(side='left')
        entry = tk.Entry(frame[2], textvariable=self.var[2])
        entry.pack(side='left',fill='both',expand='yes')
        entry.focus()
        self.prompt(entry, "选择输入py脚本文件路径")
        tk.Button(frame[2], text="浏览",command=lambda i=2:self.bt_call(i)).pack(side='left')

        tk.Checkbutton(frame[3], text="无黑底控制台", variable=self.var[5], command=lambda i=5:self.bt_call(i)).pack(side='left')
        bt = tk.Button(frame[3], bg="#ff9999",font="Times 11 bold", text="转换为 exe",command=self.py_exe)
        bt.pack(side='right')

        tk.Checkbutton(frame[3], text="打包为exe单文件", variable=self.var[6], command=lambda i=6:self.bt_call(i)).pack(side='left')

        tk.Label(frame[4],font="Times 12 bold",fg='blue',text="pyQT5 - Designer 界面 ui 转 py").pack(side='left')

        tk.Label(frame[5],text="文件 ( .ui ):").pack(side='left')
        entry = tk.Entry(frame[5], textvariable=self.var[3])
        entry.pack(side='left',fill='both',expand='yes')
        self.prompt(entry, "选择输入ui文件路径")
        tk.Button(frame[5], text="浏览",command=lambda i=3:self.bt_call(i)).pack(side='left')

        tk.Label(frame[6],text="名称( .py ):").pack(side='left')
        entry = tk.Entry(frame[6], textvariable=self.var[4])
        entry.pack(side='left',fill='both',expand='yes')
        self.prompt(entry, "转换后保存名称")
        tk.Button(frame[6], text="浏览",command=lambda i=4:self.bt_call(i)).pack(side='left')

        tk.Label(frame[7],text="转换完成后:").pack(side='left')
        tk.Checkbutton(frame[7], text="立即执行", variable=self.var[8], command=lambda i=8:self.bt_call(i)).pack(side='left')
        tk.Button(frame[7], bg="#9999ff",font="Times 11 bold", text="转换为 py",command=self.ui_py).pack(side='right')
        for child in mainframe.winfo_children():
            child.pack_configure(padx=10)

    def py_exe(self):
        if os.path.exists(self.py_temp):shutil.rmtree(self.py_temp), os.makedirs(self.py_temp)
        ico_file,py_file = [self.var[i].get() for i in (1,2)]
        if not (os.path.exists(self.py_temp) or os.path.exists(py_file)):return
        ico_file,ico_flag = os.path.exists(ico_file) and [ico_file,'-i'] or ['','-D']
        self.var[1].set(ico_file)
        if not self.install_pyinstaller():return
        ori_name = os.path.basename(py_file).split('.')[0]
        self.showLabel("正在打包 {}".format(ori_name))
        self.master.update()
        self.rename = {}
        try:
            self.runcmd(self.py_temp, py_file, ico_flag, ico_file)
            res_dir = self.py_temp+'dist'
            res_name = self.rename and self.rename.get(py_file) or ori_name
            exe_name = self.var[6].get() and res_name+'.exe' or res_name
            if exe_name in os.listdir(res_dir):
                move_path = os.path.join(self.desktop if self.var[7].get() else os.path.dirname(py_file), exe_name)
                if self.rename:move_path = move_path.replace(self.rename.get(py_file),ori_name)
                self.move_file(os.path.join(res_dir, exe_name), move_path, True)
            self.showLabel("{} 打包成功!".format(ori_name))
        except OSError:
            self.showLabel("{} 打包失败 !".format(ori_name))

    @wrapper(_VERSION)
    def runcmd(self, run_path, command, ico_flag='', ico_file=''):
        """pyinstaller -F -w -i ico py
           from subprocess import Popen,PIPE
           info = Popen(cmd,shell=True,stdout=PIPE)
           print(info.read())
        """
        if ico_flag:
            black,single = self.var[5].get() and '-w' or '', self.var[6].get() and '-F' or '-D'
            cmd = ' '.join([i for i in (self.pyinstaler,single,black,ico_flag,ico_file,command) if i])
        else:
            cmd = command
        os.chdir(run_path)
        info = os.popen(cmd)
        self.log += '\n{}'.format(info.read())

    def ui_py(self):
        ui,name = [self.var[i].get() for i in (3,4)]
        desk,impl = [self.var[i].get() for i in (7,8)]
        if not os.path.exists(ui):return
        ui_name = ui.split('\\')[-1]
        ui_dir = ui.replace(ui_name,'')
        if not name:name = ui_name[:-2]+'py'
        elif not name.lower().endswith('.py'):name=name+'.py'
        self.var[4].set(name)
        pyuic5 = os.path.join(self.py_path+'Scripts\\pyuic5.exe')
        if not os.path.exists(pyuic5):
            return self.showLabel("缺少 pyuic5 ,请先安装 pyQT5 !")
        self.rename = {}
        self.runcmd(ui_dir, "pyuic5 -o %s %s"%(name,ui_name))
        py_file = os.path.join(ui_dir,name)
        if not os.path.exists(py_file):
            return self.showLabel("%s 无法转换!"%ui_name)
        try:
            pyfile,execute = Rf(py_file),self.execute()
            class_ = refind("class\s(\w+)\(",pyfile,1)
            pyfile += (execute%class_)
            Wf(pyfile,py_file)
            if desk:Wf(pyfile,os.path.join(self.desktop,name))
            if impl:
                os.system('%s %s'%(sys.executable,name))
            if desk and py_file!=os.path.join(self.desktop,name):
                os.remove(py_file)
        except:
            self.showLabel("%s 无法打开!"%name)

    def install_pyinstaller(self):
        self.pyinstaler = self.py_path+"Scripts\\pyinstaller.exe"
        if not os.path.exists(self.pyinstaler):
            self.showLabel("正在下载安装所必需模块, 请耐心等待...")
            self.update()
            for cmd in ["pip%s install pywin32", "pip%s install pyinstaller"]:
                self.runcmd(self.py_path+"Scripts", cmd%(not _VERSION and 3 or ''))
            if not os.path.exists(self.pyinstaler):
                return self.showLabel("pyinstaller 模块安装失败,需手动安装!")
            else:
                self.showLabel("pyinstaller 模块安装成功!")
        return True

    def bt_call(self, n):
        if n<5:
            mode = {1:'*.ico',3:'*.ui'}.get(n,'*.py')
            val = askopenfile(filetypes=[("Python Files",mode,'TEXT')])
            if not val:return
            val = val.name.replace('/','\\')
            if n==4:val=val.split(os.sep)[-1]
            self.var[n].set(val)
        else:
            val = self.var[n].get()
        exec('self.myset.main.{}=val'.format(self.key[n]))
        self.myset.main.seat = (self.winfo_width(), self.winfo_height(),self.winfo_rootx(), self.winfo_rooty())
        self.myset.save(True)

    @wrapper(10)
    def showLabel(self,info=None,bgcol=None,fgcol=None):
        self.label.configure(text=info,bg=bgcol,fg=fgcol)

    def prompt(self,widget, text):
        toolTip = ToolTip(widget)
        widget.bind('<Enter>', lambda event:toolTip.showtip(text))
        widget.bind('<Leave>', lambda event:toolTip.hidetip())

    def move_file(self,prec,cur,move=False):
        if not os.path.exists(prec):return
        try:
            if os.path.isfile(prec):
                Wf(Rf(prec,'rb'),cur,'wb')
                if move:os.remove(prec)
            elif os.path.isdir(prec):
                if os.path.exists(cur):
                    shutil.rmtree(cur)
                shutil.move(prec,cur)
        except IOError:
            pass

    def refilename(self,*args):
        obj = list(args)
        for i,x in enumerate(args):
            if x.count('\t')+x.count(' ')>0 or [j for j in x if ord(j)>256]:
                newname = getname(10)
                mo_path = os.path.join(self.py_temp,newname+os.path.splitext(x)[-1])
                self.move_file(x, mo_path)
                self.rename[x] = newname
                obj[i] = mo_path
        return tuple(obj)

    def create_dir(self):
        self.myset = SettingsGroup(rootp+"user_settings.ini")
        self.desktop = "C:\\Users\\Administrator\\Desktop\\" or self.myset.main.desktop
        self.py_path = os.path.dirname(sys.executable)+os.sep or self.myset.main.py_path
        for i,j,k in [(self.py_path,'python.exe','py_path'),(self.desktop,'桌面','desktop')]:
            if not os.path.exists(i) and not getattr(self.myset.main,k,0):
                res = askokcancel(i,"无法找到{}目录,需手动设置".format(j))
                val = askdirectory()
                if not(res or val):self.close()
                if k=='desktop':
                    self.myset.main.desktop = self.desktop = val
                else:
                    self.myset.main.py_path = self.py_path = val
                self.myset.save()

    def getSeat(self,):
        if not self.myset.main.seat:
            w,h = 500,500
            x,y = self.winfo_screenwidth()/2-w/2, self.winfo_screenheight()/2-h/2
            self.myset.main.seat = w,h,x,y
        return self.myset.main.seat

    def execute(self):
        return "\nclass MyMainWindow(QtWidgets.QMainWindow,%s):\n    def __init__(self, parent=None):\n        super(self.__class__, self).__init__(parent)\n        self.setupUi(self)\n\nif __name__ == '__main__':\n    import sys\n    app = QtWidgets.QApplication(sys.argv)\n    win = MyMainWindow()\n    win.show()\n    sys.exit(app.exec_())\n"

    def about(self):
        text = """目前只支持Python-2.7, 其余自测, 支持中文转换.\n"""+self.__doc__
        askokcancel("关于",text)

    def getIcon(self):
        icon = b"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" # base64.b64encode(b'')
        path = (os.path.dirname(sys.executable)+os.sep if _VERSION else rootp)+"Icon.ico"
        if not os.path.exists(path):
            Wf(base64.b64decode(icon),path,"wb")
            time.sleep(0.1)
        return path


class ToolTip(object):
    def __init__(self, widget):
        self.widget = widget
        self.topwin = None

    def showtip(self, text):
        ws = self.widget.bbox("insert")
        if self.topwin or not(text or ws):
            return
        self.topwin = tw = tk.Toplevel(self.widget)
        tw.wm_overrideredirect(True)
        tw.wm_attributes('-topmost',True)
        tw.wm_geometry("+%d+%d" % (self.widget.winfo_rootx()+20, self.widget.winfo_rooty()+40+ws[-1]))
        tk.Label(tw, text=text, justify=tk.LEFT,background="#ffffe0", relief=tk.SOLID, borderwidth=1,font="tahoma 10").pack(ipadx=1)
        self.widget.after(3000, self.hidetip)

    def hidetip(self):
        tw = self.topwin
        self.topwin = None
        tw and tw.destroy()


class SettingsGroup(object):
    __dct__ = """# 四个空格后加注释"""
    def __init__(self,filename=None):
        self.__objs={}
        self.__order=[]
        self.__notes={}
        self.__filename=filename
        if filename and os.path.exists(filename):
            self.load()

    def append(self,name,value,notes=''):
        if not isinstance(value,SettingsGroup):
            try:
                value = eval(value)
            except:
                pass
        self.__objs[name]=value
        self.__notes[name]=notes
        if name in self.__order:
            self.__order.remove(name)
        self.__order.append(name)
        object.__setattr__(self,name,value)

    def load(self):
        from re import findall
        with open(self.__filename,'r') as buf:
            buf,val = buf.read(),None
        for line in buf.splitlines():
            item = findall("\[([a-z_A-Z\u4e00-\u9fa5]\w*?)\](.*)", line)
            if item:
                group, val = list(*item), SettingsGroup()
                group.insert(1,val)
                self.append(*group)
                continue
            item = findall("([a-z_A-Z\u4e00-\u9fa5]\w*?)\s*\=\s*(.*)", line)
            if item and val is not None:
                name,value = item[0]
                obj = value.split(' '*4)
                value,notes = len(obj)>1 and (obj[0],reduce(lambda i,j:i+j,obj[1:])) or (value,'')
                val.append(name,value,notes.lstrip(' '))

    def save(self,space=False):
        if (self.__filename is None):
            raise ValueError("filename not set")
        def func(val):
            if val and isinstance(val,str):
                return val
            elif isinstance(val,(tuple,list)) and len(val)>1:
                item = ', '.join(['{!r}'.format(i) for i in val])
                return isinstance(val,list) and '[%s]'%item or item
            else:
                return repr(val)
        tmp, space = [], ('%s = %s%s' if space else '%s=%s%s')
        for group in self.__order:
            obj = self.__objs[group]
            if not hasattr(obj,'items'):
                continue
            tmp.append('\n[%s]%s'%(group,self.__notes.get(group,'')))
            for name,value,notes in obj.items():
                if not isinstance(value,SettingsGroup):
                    tmp.append(space%(name,func(value),notes and (' '*4+notes) or notes))
        if tmp:
            with open(self.__filename,'w') as buf:
                buf.write('\n'.join([self.__dct__]+tmp+['\n']))

    def get(self):
        return self

    def items(self):
        return [(x,self.__objs[x],self.__notes[x]) for x in self.__order]

    def __getattr__(self,name):
        try:
            obj=self.__objs[name]
        except KeyError:
            value = SettingsGroup()
            self.append(name,value)
            return value
        else:
            if isinstance(obj,SettingsGroup):
                return obj.get()
            else:
                raise TypeError("'obj' must be a SettingsGroup object")

    def __setattr__(self,name,value):
        object.__setattr__(self,name,value)
        if not name.startswith('_SettingsGroup__'):
            self.append(name,value,self.__notes.get(name,''))

    def __getitem__(self,name):
        return self.__objs[name]

    def __len__(self):
        return len(self.__objs)

    def __contains__(self,name):
        return (name in self.__objs)

    def __nonzero__(self):
        return (not (not self.__objs))



if __name__ == '__main__':
    try:
        PythonTool()
    except:
        import traceback
        error = traceback.format_exception(*sys.exc_info())
        print ('\n'.join(error))


