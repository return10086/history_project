'''
Author: 程佳乐 &&cjl2646289864@gmail.com
Date: 2023-12-01 20:00:36
LastEditors: 程佳乐 &&cjl2646289864@gmail.com
LastEditTime: 2023-12-05 12:58:03
FilePath: \基础学习\test.py
Description: 

Copyright (c) 2023 by cjl2646289864@gmail.com, All Rights Reserved. 
'''
import pandas as pd
import numpy as np
from scipy.fft import fft, ifft
import matplotlib.pyplot as plt

# 读取Excel文件
file_path = "Z:/新建 Microsoft Excel 工作表.xlsx"

df = pd.read_excel(file_path)


# 将离散数据转换为numpy数组
data = pd.DataFrame(pd.read_excel(file_path))
a=data['ax']
accel_data=np.array(a)

#accael_data只取前256个数据
accel_data=accel_data[:512]
print(accel_data)

# 对离散数据进行傅里叶变换
time_interval = 0.01 # 采样周期10ms
frequencies = np.fft.fftfreq(len(accel_data), time_interval)
accel_fft = fft(accel_data)


# 计算频率分量的幅值
amplitudes = np.abs(accel_fft)

# 过滤出高频和低频成分
high_frequency = amplitudes[:int(len(amplitudes)/2)]
low_frequency = amplitudes[int(len(amplitudes)/2):]

# 对过滤后的频谱进行逆傅里叶变换，得到时域信号
filtered_signal = ifft(high_frequency + low_frequency)
time_vector = np.arange(len(filtered_signal)) * time_interval




# 检测低频成分并生成输出波形
low_freq_threshold = 10
output_waveform = np.zeros_like(filtered_signal)
output_waveform[np.where(np.abs(filtered_signal) < low_freq_threshold)[0]] = 1000 # 输出高电平波形，阈值设为10
output_waveform[np.where(np.abs(filtered_signal) >= low_freq_threshold)[0]] = -1000 # 输出低电平波形，阈值设为10
output_signal = ifft(output_waveform)
time_vector = np.arange(len(output_signal)) * time_interval

#去除傅里叶变换直流分量

accel_fft[0]=0

# 绘制输入信号、输出波形和频谱图
plt.figure()
plt.plot(time_vector, filtered_signal.real, label='Filtered High-Frequency')
plt.plot(time_vector, filtered_signal.imag, label='Filtered Low-Frequency')
plt.plot(time_vector, output_signal.real, label='Output Waveform') # 绘制输出波形
plt.xlabel('Time (ms)')
plt.ylabel('Amplitude')
plt.legend()
plt.show()

from scipy.signal import butter, lfilter

#指定滤波的频率为10Hz
cutoff_frequency = 10
nyquist_frequency = 0.5 * len(accel_data) / time_interval
normalized_cutoff = cutoff_frequency / nyquist_frequency
b, a = butter(4, normalized_cutoff, btype='low', analog=False)
filtered_signal = lfilter(b, a, filtered_signal)

#绘制输入信号、输出波形和频谱图
plt.figure()
plt.plot( accel_data.real, label='Output Waveform') # 绘制输出波形
plt.xlabel('Time (ms)')
plt.ylabel('Amplitude')
plt.legend()
plt.show()
