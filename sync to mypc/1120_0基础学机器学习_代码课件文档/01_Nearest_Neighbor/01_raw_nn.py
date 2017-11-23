#coding=utf-8

from tensorflow.examples.tutorials.mnist import input_data
import math
import datetime


train_sum=5000
test_sum=20

def get_dist(train_data,test_data, i,j):
	sum=0.
	for k in range(784):
		#每个维度元素求差，然后差值乘2次方，接着乘方求和
		sum += math.pow(train_data[j,k]-test_data[i,k],2)
		#把乘方和再开方，并返回
	return math.sqrt(sum)

def get_number(vector):
	for i in range(len(vector)):
		if vector[i]:
			return i
	

#读取训练与测试用的图片数据集
mnist = input_data.read_data_sets("./MNIST_data", one_hot=True)

#取5000个训练数据
train_data, train_label = mnist.train.next_batch(train_sum) 
#取500个测试数据
test_data, test_label = mnist.test.next_batch(test_sum)   

#准备计算用的数据内存空间
accuracy = 0.
#最小距离值
min_dist=0.
#测试样本与训练样本的真实距离值
real_dist=0.
#最小距离样本的索引
min_index=0


t1=datetime.datetime.now()
#
#计算距离并保存
for i in range(test_sum):
	print('[',i,'] ',end='')
	for j in range(train_sum):
		real_dist = get_dist(train_data,test_data,i,j) 		
#排序求出最小距离
		if(j==0):
			min_dist=real_dist
			min_index=j
		else:
			if(real_dist<min_dist):
				min_dist=real_dist
				min_index=j
#打印每一个距离样本的类别
	print (get_number(test_label[i]),' ',get_number(train_label[min_index]))
	if(get_number(test_label[i]) == get_number(train_label[min_index])):
		accuracy += 1/float(test_sum)

print("accuracy=",accuracy)


t2=datetime.datetime.now()
print(t2-t1)
