#coding=utf-8

import numpy as np

from tensorflow.examples.tutorials.mnist import input_data
import math
import datetime



"""
"""
train_sum=5000
test_sum=20

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

#因为这是一个demo，此处实验我们只取一部分数据，可以节约点时间
#取5000个训练数据
train_X, train_Y = mnist.train.next_batch(train_sum) 
#取500个测试数据
test_X, test_Y = mnist.test.next_batch(test_sum)   

#2
#准备计算用的数据内存空间
accuracy = 0.
min_dist=0.
real_dist=0.
min_index=0

t1=datetime.datetime.now()
#3
#计算距离并保存
for i in range(test_sum):
	min_index = get_index(train_X,test_X,i)
	print('[',i,'] ',end='')
	print (np.argmax(test_Y[i]),' ',np.argmax((train_Y[min_index])) )
	if(np.argmax(test_Y[i]) == np.argmax(train_Y[min_index])):
		accuracy += 1/float(test_sum)

#4
print ("accuracy=",accuracy)
t2=datetime.datetime.now()
print (t2-t1)
