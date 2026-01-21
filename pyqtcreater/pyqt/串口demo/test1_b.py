from PyQt5 import QtSerialPort
from PyQt5.QtWidgets import QComboBox
import serial.tools.list_ports

class QSerialComboBox(QComboBox):

    def __init__(self, parent=None):
        super().__init__(parent)

            
    def showPopup(self):
        name = self.currentText()
        self.clear()
        plist = list(serial.tools.list_ports.comports())#获取口列表
        if len(plist) > 0:#如果有串口
            for i in range(0, len(plist)):#依次把串口放进列表
                plist_0 = list(plist[i])
                self.addItem(str(plist_0[0]))
        super().showPopup()
        index = self.findText(name)
        if index >= 0:
            self.setCurrentIndex(index)

