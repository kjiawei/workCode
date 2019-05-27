 # -- coding: UTF-8 --
import os,sys
import chardet

#批量转换目录下所有文件格式为utf-8
#usage:fileName.py documentPath
 
def codeswith( filename,out_enc="UTF8"):#其它格式
    try:#在下行添加格式 filename[1]不一定是最后一位小数点
        if os.path.splitext(filename)[1] == '.css' or os.path.splitext(filename)[1] == '.cs' or os.path.splitext(filename)[1] == '.h' or os.path.splitext(filename)[1] == '.java' or os.path.splitext(filename)[1] == '.c' or os.path.splitext(filename)[1] == '.h' or os.path.splitext(filename)[1] == '.cpp':
            content = open(filename).read()
            result = chardet.detect(content)#通过chardet.detect获取当前文件的编码格式串，返回类型为字典类型
            coding = result.get('encoding')#获取encoding的值[编码格式]
            if coding != 'utf-8':#文件格式如果不是utf-8的时候，才进行转码
                new_content = content.decode(coding, 'ignore').encode(out_enc) #decode第二参数忽略非gb2312编码的字符，这样就不会报错
                open(filename, 'w').write(new_content)
                print filename +"->"+coding + "<===> UTF8! \n", 
            else:
                print filename + " Is UTF8\n",
    except IOError,e:
        print " error"
 
def explore(dir):
    for root, dirs, files in os.walk(dir):
        for file in files:
            path = os.path.join(root, file)
            codeswith(path)
 
def main():
    for path in sys.argv[1:]:
        if os.path.isfile(path):
            codeswith(path)
        elif os.path.isdir(path):
            explore(path)
 
if __name__ == "__main__":
    main()