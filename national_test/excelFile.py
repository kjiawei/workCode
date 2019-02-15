#!/usr/bin/python3
# -*- coding:utf-8 -*-

'''
Create by:jkCodic 2018.3.19
Use:to deal excel file    strengthen nationalExaming join speed
Version:1.00.0
'''

import sys
import xlrd

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QWidget, QApplication, QDesktopWidget, QTableWidget, QHBoxLayout, QTableWidgetItem, \
QComboBox, QFrame
from PyQt5.QtGui import QFont, QColor, QBrush, QPixmap, QIcon

'''
if __name__ == '__main__':
    #每一pyqt5应用程序必须创建一个应用程序对象。sys.argv参数是一个列表，从命令行输入参数。
    app = QApplication(sys.argv)
    #QWidget部件是pyqt5所有用户界面对象的基类。他为QWidget提供默认构造函数。默认构造函数没有父类。
    w = QWidget()
    #resize()方法调整窗口的大小。这离是250px宽150px高
    w.resize(800, 600)
    #move()方法移动窗口在屏幕上的位置到x = 300，y = 300坐标。
    w.move(300, 80)
    #设置窗口的标题
    w.setWindowTitle('公务员报考助手')
    #显示在屏幕上
    w.show()
    
    #系统exit()方法确保应用程序干净的退出
    #的exec_()方法有下划线。因为执行是一个Python关键词。因此，exec_()代替
    sys.exit(app.exec_())
	
class ExcelFile(QWidget):
	def __init__(self):
		super().__init__()
		self.initUi() #界面绘制交给InitUi方法

	def initUi(self):
		self.setGeometry(800, 600, 300, 80) #设置窗口的位置和大小
        self.setWindowTitle('公务员报考助手')
        self.setWindowIcon(QIcon('panda_16px.png')) #设置窗口的图标，引用当前目录下的panda_16px.png图片
        self.show()#显示窗口
		#horizontalHeader = ["编号", "姓名", "性别", "年龄", "职业"]

if __name__ == '__main__':
    #创建应用程序和对象
    app = QApplication(sys.argv)
    ex = ExcelFile()
    sys.exit(app.exec_()) 

table = QTableWidget()
table.setColumnCount(5)
table.setRowCount(2)
table.setHorizontalHeaderLabels(horizontalHeader)
table.setEditTriggers(QTableWidget.NoEditTriggers)
table.setSelectionBehavior(QTableWidget.SelectColumns)
table.setSelectionMode(QTableWidget.SingleSelection)

for index in range(table.columnCount()):
	headItem = table.horizontalHeaderItem(index)
	headItem.setFont(QFont("song", 12, QFont.Bold))
	headItem.setForeground(QBrush(Qt.gray))
	headItem.setTextAlignment(Qt.AlignLeft | Qt.AlignVCenter)

table.setColumnWidth(4, 100)
table.setRowHeight(0, 40)
# self.table.setFrameShape(QFrame.HLine)#设定样式
# self.table.setShowGrid(False) #取消网格线
# self.table.verticalHeader().setVisible(False) #隐藏垂直表头

table.setItem(0, 0, QTableWidgetItem("001"))
table.setItem(0, 1, QTableWidgetItem("刘亦菲"))
genderComb = QComboBox()
genderComb.addItem("男")
genderComb.addItem("女")
genderComb.setCurrentIndex(1)
table.setCellWidget(0, 2, genderComb)
table.setItem(0, 3, QTableWidgetItem("30"))
table.setItem(0, 4, QTableWidgetItem("演员"))

table.setItem(1, 0, QTableWidgetItem("002"))
table.setItem(1, 1, QTableWidgetItem("马云"))
genderComb = QComboBox()
genderComb.addItem("男")
genderComb.addItem("女")
genderComb.setCurrentIndex(0)
table.setCellWidget(1, 2, genderComb)
table.setItem(1, 3, QTableWidgetItem("50"))
table.setItem(1, 4, QTableWidgetItem("企业家"))

row_count = table.rowCount()
table.insertRow(row_count)
mainLayout = QHBoxLayout()
mainLayout.addWidget(table)
self.setLayout(mainLayout)

self.resize(600, 280)
self.center()
self.setWindowTitle('TableWidget Excempt')

def center(self):
	qr = self.frameGeometry()
	cp = QDesktopWidget().availableGeometry().center()
	qr.moveCenter(cp)
	self.move(qr.topLeft())

if __name__ == '__main__':
	#每一pyqt5应用程序必须创建一个应用程序对象。sys.argv参数是一个列表，从命令行输入参数
	app = QApplication(sys.argv)
	w = QWidget()
	w.resize(250,150)
	w.move(300,300)
	w.setWindowTitle('Excel File')
	w.show()
	#table = ExcelFile()
	#table.show()
	sys.exit(app.exec_())
'''
	
#path='E:\share\python\test1.xlsx'
data = xlrd.open_workbook(r'E:\share\python\test3.xls','w')
sheets=data.sheets()

#通过函数、索引、名称获得工作表。
sheet_1_by_function = data.sheets()[3] #获得第一个工作表
#sheet_1_by_index=data.sheet_by_index(0)
#sheet_1_by_name=data.sheet_by_name(u'Sheet1')

#通过方法获得某一列或者某一行的数值。
#print(sheet_1_by_function.row_values(1))
#sheet_1_by_name.col_values(1)

#通过工作表的属性获得行数和列数。
n_of_rows = sheet_1_by_function.nrows #行数 1035
n_of_cols = sheet_1_by_function.ncols #列数 18
#print(n_of_cols)

for i in range(n_of_rows):
	#print(sheet_1_by_function.row(i)[i].value)
	#一级参数 学历学位 硬性条件
	if sheet_1_by_function.col(8)[i].value == '学士' and \
		sheet_1_by_function.col(7)[i].value == '本科' and \
		sheet_1_by_function.col(12)[i].value == '否' and \
		'不限' in sheet_1_by_function.col(10)[i].value: #  or   公共事业管理  市场营销  不限(除了专业 学历 学位 )
					print(i) 	
					print(sheet_1_by_function.row_values(i))
	pass
#print(sheet_1_by_function.row_values(3))

#严格查询(精准)   放宽查询(包括无限条件)
#输入框(先提供招考专业目录比对表) 输入专业 学历 地区 性别     注意:招聘的特殊要求显示
#在第三行插入0开头的序列行(不固定在这行)  第四行插入筛选按钮(整行)
#查询记录输出到新的表格

#最新招考信息滚动显示在最上方中间(附件功能)
#相关名词解读功能
#趣题复习-类似百词斩
'''

cell_A1 = sheet_1_by_function.cell(0,0).value
cell_A1 = sheet_1_by_function.row(0)[0].value
cell_A1 = sheet_1_by_function.col(0)[0].value

#通过以下的方法对单元格的数值进行修改。
row=0
col=0
#ctype 0:empty,1:string,2:number,3:date,4:boolean,5:error
cell_type = 1
value='Hello,Excel'
cell_A1 = sheet_1_by_function.cell(0,0).value
format = 0
sheet_1_by_function.put_cell(row,col,cell_type,value,format)
cell_A1 = sheet_1_by_function.cell(0,0).value
'''


''' import panda as pd

#another teching  https://www.cnblogs.com/snow-backup/p/4021554.html

data = pd.read_excel('./test1.xlsx')
data[data.招考职位 == '宣传教育处主任科员以下'].录用人数

'''