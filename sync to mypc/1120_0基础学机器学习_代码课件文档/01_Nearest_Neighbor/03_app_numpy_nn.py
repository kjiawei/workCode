#coding=utf-8

import numpy as np
from image import image2onebit as it
import sys

from tensorflow.examples.tutorials.mnist import input_data
import math
import datetime



if len(sys.argv) < 2:
	print ('参数不足，请输入要处理的图片路径！')
	sys.exit()
"""
"""
train_sum=5000

def get_index(train_data,test_data, i):
#1	
#	return np.argmin(np.sqrt(np.sum(np.power(test_X[i]-train_X,2),axis=1)))
#2	
#	print("test_Y[i]=",test_Y[i])
	return np.argmin(np.sqrt(np.sum(np.square(test_data[i]-train_data),axis=1)))
#3	return np.argmin(np.linalg.norm(test_X[i]-train_X,axis=1))



#1
#读取训练与测试用的图片数据集
mnist = input_data.read_data_sets("./MNIST_data", one_hot=True)

#取训练数据
train_data, train_label = mnist.train.next_batch(train_sum) 

#2
#准备计算用的数据内存空间
min_index=0

#处理命令行参数传入的图片，转换成numpy.array类型
x1_tmp = it.read_image(sys.argv[1])
x1 = it.imageToArray(x1_tmp)
x1 = np.array(x1)
#用字符矩阵打印图片
it.show_ndarray(x1)

t1=datetime.datetime.now()
#3

min_index = get_index(train_data,x1,0)
print ('predicted: ',np.argmax((train_label[min_index])) )

#4
t2=datetime.datetime.now()
print (t2-t1)
