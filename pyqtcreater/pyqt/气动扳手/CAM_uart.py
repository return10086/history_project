# UART Control
#
# This example shows how to use the serial port on your OpenMV Cam. Attach pin
# P4 to the serial input of a serial LCD screen to see "Hello World!" printed
# on the serial LCD display.

import time
import serial
import struct

RX_NUM = 6
DataType = "<HHH"
#接收的三个数据分别为 检测类型、颜色种类、Debug模式
#0为不检测， 1为颜色检测， 2为圆心检测

class CAM_uart():
    def __init__(self, bode):
        self.DataCount = 0
        self.Recv_pack = bytearray()
        self.uart = serial.Serial("/dev/ttyS1", bode)
        #self.uart = serial.Serial("COM1", bode)
    def sending_data(self, data1, data2, data3, data4):
        #B unsigned char
        #H unsigned short
        #h signed short
        #L unsigned long
        # 发送的数据类型为 颜色种类，坐标，面积
        data = struct.pack("<BBhhhl",
                            0xAA,
                            0xFE,
                            data1,
                            data2,
                            data3,
                            data4
                            )
        self.uart.write(data)

    def recving_data(self):
        while self.uart.inWaiting():
            self.TempData = self.uart.read(1)
            #print(self.TempData)
            if self.DataCount == 0 and not(self.TempData == b'\xFE' and self.LastData == b'\xAA'):  #当检测到标头后，开始接收数据
                self.LastData = self.TempData
                continue

            #DataCount=0时，采集的是标头，所以不接收这一个字节的数据
            if self.DataCount != 0:
                self.Recv_pack += bytearray(self.TempData)

            if self.DataCount == RX_NUM:        #数据接收完成
                self.DataCount = 0
                #print(self.Recv_pack)
                self.Recv_Data = struct.unpack(DataType, self.Recv_pack)       #解包
                self.Recv_pack = bytearray()
                return self.Recv_Data
            else:
                self.DataCount += 1
        return None

if __name__ == '__main__':
    cam_uart = CAM_uart(115200)

    while(True):
        time.sleep(0.3)
        #recv_data = cam_uart.recving_data()
        #if recv_data != None:
        #    print(recv_data)
        cam_uart.sending_data(26, 568, 654, 5685)
