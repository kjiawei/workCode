# -*- coding: utf-8 -*-

"""
Module implementing MainWindow.
"""

from PyQt5.QtCore import pyqtSlot, QTimer
from PyQt5.QtWidgets import QMainWindow

from Ui_main import Ui_MainWindow
import socket
import os
import time
import sys
import platform
#

class MainWindow(QMainWindow, Ui_MainWindow):
    #global tcpSocket
    
#    QTimer sendDataTimer
    def __init__(self, parent=None):
        """
        Constructor
        
        @param parent reference to the parent widget
        @type QWidget
        """
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        #self.createSocket()
        self.timer = QTimer(self)
        self.timer2 = QTimer(self)
        self.timer.timeout.connect(self.dataSendFunc) 
        self.timer2.timeout.connect(self.listDataSendFunc) #不能加()
        #self.startConnect.connect()
        self.setWindowTitle("6068PC版命令收发测试工具")
        self.ipAddress.setPlaceholderText("请输入IP地址")
        self.connectPort.setPlaceholderText("请输入端口号")
        self.dataSend.setEnabled(False)
        self.dataTimerSend.setEnabled(False)
     
    @pyqtSlot()#不加装饰器,会执行多次
    def on_dataSend_clicked(self):
        tmpData = self.readyData.text()
        if tcpSocket != NULL:
            tcpSocket.send(bytes(tmpData, 'utf-8'))
        #run(interval, command)
        #tmpData = self.readyData.text()
        #tcpSocket.send(bytes(tmpData, 'utf-8'))#b''
        
    @pyqtSlot()
    def on_dataSend_list_clicked(self):
        global timerInterval
        global listDataTimerInterval
        timerInterval=0
        listDataTimerInterval = 0
        self.timer2.start(1000)
    
    @pyqtSlot()
    def on_startConnect_clicked(self):
        global tcpSocket
        ip_address=self.ipAddress.text()#='192.168.20.7'
        port=self.connectPort.text()# = 8001
        tcpSocket = socket.socket(family=2,type=socket.SOCK_STREAM)
        #tcpSocket.waitForConnected(50000)
        tcpSocket.connect(('192.168.123.50',int(port)))
        self.dataSend.setEnabled(True)
        self.dataTimerSend.setEnabled(True)
        '''if tcpSocket.connect((ip_address,port)) == 0:
            QMessageBox.information(self, "链接成功", "确定,重设", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
            self.dataSend.setEnabled(True)
            self.dataTimerSend.setEnabled(True)
        elif int(tcpSocket.connect((ip_address,port)) )!= 0:
            QMessageBox.information(self, "链接失败", "确定,重试", QMessageBox.No | QMessageBox.Yes ,  QMessageBox.No)
    '''
    @pyqtSlot()
    def on_dataTimerSend_clicked(self):
        #self.readyData.text()
        self.timer.start(1000)
    
    @pyqtSlot()
    def on_stopTimer_clicked(self):
        global timerInterval
        timerInterval=0
        self.runTimeShow.setNum(timerInterval)
        self.timer.stop()

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
       
    def dataSendFunc(self):
        global timerInterval
        timerInterval += 1
        self.runTimeShow.setNum(timerInterval)
        tmpData2 = self.readyTimerData.text()
        tcpSocket.send(bytes(tmpData2, 'utf-8'))
        #终止链接或未链接上的处理
    
    def listDataSendFunc(self):
        global listDataTimerInterval
        listDataTimerInterval += 1
        
        tmpTimeData1 = self.data1.text()
        tmpTimeData2 = self.data1_2.text()
        tmpTimeData3 = self.data1_3.text()
        tmpTimeData4 = self.data1_4.text()
        tmpTimeData5 = self.data1_5.text()
        tmpTime1 = int(self.time1.text())
        tmpTime2 = int(self.time1_2.text())
        tmpTime3 = int(self.time1_3.text())
        tmpTime4 = int(self.time1_4.text())
        tmpTime5 = int(self.time1_5.text())
        print(listDataTimerInterval)
        #tcpSocket.send(bytes(tmpTimeData, 'utf-8'))
        if tmpTime1 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData1, 'utf-8'))
        elif tmpTime2 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData2, 'utf-8'))
        elif tmpTime3 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData3, 'utf-8'))
        elif tmpTime4 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData4, 'utf-8'))
        elif tmpTime5 == listDataTimerInterval:
            tcpSocket.send(bytes(tmpTimeData5, 'utf-8'))
       
    def print_ts(message):
        print("[%s] %s"%(time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()), message))
    
    def run(interval, command):
    #print_ts("-"*100)
    #dataSend()
        print_ts("Command:%s"%command)
        print_ts("Starting every %s seconds."%interval)
    #print_ts("-"*100)
        while True:
            try:
            # sleep for the remaining seconds of interval
                time_remaining = interval-time.time()%interval
                print_ts("Sleeping until %s (%s seconds)..."%((time.ctime(time.time()+time_remaining)), time_remaining))
                time.sleep(time_remaining)
                print_ts("Starting command.")
            # execute the command
                status = os.system(command)
            #print_ts("-"*100)
                print_ts("Command status = %s."%status)
            except Exception as e:
                print(e)
        
if __name__ == '__main__':
    #import sys
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
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSlot()
    def on_readCanData_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSlot()
    def on_setOrReset_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSlot()
    def on_rebot_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSlot()
    def on_shutDown_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSlot()
    def on_other_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    
