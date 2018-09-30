# -*- coding: utf-8 -*-

from PyQt5.QtCore import pyqtSlot, QTimer
from PyQt5.QtWidgets import QMainWindow, QMessageBox
from Ui_main import Ui_MainWindow

import socket
import os
import time
import sys
import platform

class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        global timerInterval
        timerInterval=0
        self.timer = QTimer(self)
        self.timer2 = QTimer(self)
        self.timer.timeout.connect(self.dataSendFunc) 
        self.timer2.timeout.connect(self.listDataSendFunc) #不能加()
        self.setWindowTitle("6068PC版命令收发测试工具")
        self.ipAddress.setPlaceholderText("请输入IP地址")
        self.connectPort.setPlaceholderText("请输入端口号")
        self.dataSend.setEnabled(False)
        self.dataTimerSend.setEnabled(False)
        self.dataSend_list.setEnabled(False)
        self.stopTimer.setEnabled(False)
     
    @pyqtSlot()#不加装饰器,会执行多次
    def on_dataSend_clicked(self):#单次发送
        tmpData = self.readyData.text()
        tcpSocket.send(bytes(tmpData, 'utf-8'))
    
    @pyqtSlot()
    def on_dataTimerSend_clicked(self):#"定时发"按钮
        self.stopTimer.setEnabled(True)
        self.readyTimerData.setEnabled(False)
        self.timer.start(1000)
    
    @pyqtSlot()
    def on_dataSend_list_clicked(self):#队列发送 "生效并开始"按钮
        global timerInterval
        global listDataTimerInterval
        self.stopTimer.setEnabled(True)
        timerInterval=0
        listDataTimerInterval = 0
        self.timer2.start(1000)
    
    @pyqtSlot()
    def on_startConnect_clicked(self):#开始连接  192.168.20.245
        global tcpSocket
        ip_address=self.ipAddress.text()
        port=self.connectPort.text()
        tcpSocket = socket.socket(family=2,type=socket.SOCK_STREAM)
        #tcpSocket.waitForConnected(50000)
        tcpSocket.connect((ip_address,int(port)))
        self.dataSend.setEnabled(True)
        self.dataTimerSend.setEnabled(True)
        self.dataSend_list.setEnabled(True)
        '''if tcpSocket.connect((ip_address,port)) == 0:
            QMessageBox.information(self, "链接成功", "确定,重设", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            self.dataSend.setEnabled(True)
            self.dataTimerSend.setEnabled(True)
        elif int(tcpSocket.connect((ip_address,port)) )!= 0:
            QMessageBox.information(self, "链接失败", "确定,重试", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
        '''
    
    @pyqtSlot()
    def on_stopTimer_clicked(self):#关定时器 定时/队列
        global timerInterval
        timerInterval=0
        self.readyTimerData.setEnabled(True)
        self.timer.stop()
        self.timer2.stop()

    def createSocket(self):
        '''global tcpSocket
        global timerInterval
        global listDataTimerInterval
        timerInterval=0
        listDataTimerInterval = 0
        ip_address='192.168.20.7'
        port = 8001
        tcpSocket = socket.socket(family=2,type=socket.SOCK_STREAM)
        #tcpSocket.waitForConnected(50000)
        tcpSocket.connect((ip_address,port))'''
       
    def dataSendFunc(self):#定时发调用
        global timerInterval
        timerInterval += 1
        self.runTimeShow.setNum(timerInterval)
        tmpData2 = self.readyTimerData.text()
        tcpSocket.send(bytes(tmpData2, 'utf-8'))
        #终止链接或未链接上的处理
    
    def listDataSendFunc(self):#timer2调用 队列发送  #192.168.20.245
        global listDataTimerInterval
        listDataTimerInterval += 1
        self.runTimeShow.setNum(listDataTimerInterval)
		
        tmpTimeData1 = self.data1.text()#输入验证器 独立获得整形 判断是否为空,然后是否获取
        tmpTimeData2 = self.data1_2.text()
        tmpTimeData3 = self.data1_3.text()
        tmpTimeData4 = self.data1_4.text()
        tmpTimeData5 = self.data1_5.text()
        tmpTime1 = int(self.time1.text()) if self.time1.text() == '' else 0 
        if tmpTime1 == '':
            QMessageBox.information(self, "输入有误", "重输,放弃", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            #print(reply)
        elif tmpTime1 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData1, 'utf-8'))

        tmpTime2 = int(self.time1.text())
        if tmpTime2 == '':
            QMessageBox.information(self, "输入有误", "重输,放弃", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            #print(reply)
        elif tmpTime2 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData2, 'utf-8'))
			
        tmpTime3 = int(self.time1.text())
        if tmpTime3 == '':
            QMessageBox.information(self, "输入有误", "重输,放弃", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            #print(reply)
        elif tmpTime3 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData2, 'utf-8'))
			
        tmpTime4 = int(self.time1.text())
        if tmpTime4 == '':
            QMessageBox.information(self, "输入有误", "重输,放弃", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            #print(reply)
        elif tmpTime4 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData2, 'utf-8'))
			
        tmpTime5 = int(self.time1.text())
        if tmpTime5 == '':
            QMessageBox.information(self, "输入有误", "重输,放弃", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            #print(reply)
        elif tmpTime5 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData2, 'utf-8'))
			
    def print_ts(message):
        print("[%s] %s"%(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()), message))
    
    def run(interval, command):
        print_ts("Command:%s"%command)
        print_ts("Starting every %s seconds."%interval)
        while True:
            try:
                time_remaining = interval-time.time()%interval
                print_ts("Sleeping until %s (%s seconds)..."%((time.ctime(time.time()+time_remaining)), time_remaining))
                time.sleep(time_remaining)
                print_ts("Starting command.")
                status = os.system(command)
                print_ts("Command status = %s."%status)
            except Exception as e:
                print(e)
        
if __name__ == '__main__':
    from PyQt5.QtWidgets import QApplication
    app = QApplication(sys.argv)
    dlg = MainWindow()#Ui_MainWindow()
    dlg.show()
    global interval
    interval = 1
    global command
    command = r'dir'
    sys.exit(app.exec_())
    
    @pyqtSlot()
    def on_protocolData_clicked(self):
        
        raise NotImplementedError
    
    @pyqtSlot()
    def on_readCanData_clicked(self):
        
        raise NotImplementedError
    
    @pyqtSlot()
    def on_setOrReset_clicked(self):
        
        raise NotImplementedError
    
    @pyqtSlot()
    def on_rebot_clicked(self):
        
        raise NotImplementedError
    
    @pyqtSlot()
    def on_shutDown_clicked(self):
        
        raise NotImplementedError
    
    @pyqtSlot()
    def on_other_clicked(self):
        
        raise NotImplementedError
    
    
