import matplotlib.image as mpimg

def read_image(file_name):
	a = mpimg.imread(file_name)
	col = 28
	row = 28
	hua =1.5

#	print ("type(a): ", type(a))
	ret = [[0 for i in range(row)] for i in range(col)]

	for i in range(row):
		for j in range(col):
#ok			x = (a[i,j,0]) + (a[i,j,0]) + (a[i,j,1]) + (a[i,j,2])
			x =  (a[i,j,0]) + (a[i,j,1]) + (a[i,j,2])
			if (x < hua):
				ret[i][j]=1
	return ret

def show_image(image_array):
	row = 28
	col = 28
	for i in range(row):
		for j in range(col):
			if(image_array[i][j] == 1):
				print ("* ",end='')
			else:
				print ("0 ",end='')

def imageToArray(data):
	ret = [[0 for i in range(784)]]
	row = 28
	col = 28
	k=0
	for i in range(row):
		for j in range(col):
			ret[0][k] = (data[i][j])
			k += 1
	return ret
def show_ndarray(image_array):
	for i in range(784):
		if(image_array[0,i] != 0):
			print ("* ",end='')
		else:
			print ("  ",end='')
		if (i+1)%28==0 :
			print()
