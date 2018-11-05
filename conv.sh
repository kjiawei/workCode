#!/bin/bash -   
#===============================================================================  
#  
#          FILE: conv.sh  
#   
#         USAGE: ./conv.sh   
#   
#   DESCRIPTION:   
#   
#       OPTIONS: ---  
#  REQUIREMENTS: ---  
#          BUGS: 目前不支持传入参数中含有空格；  
#         NOTES: ---  
#        AUTHOR: linkscue (scue), linkscue@gmail.com  
#       CREATED: 2013年03月06日 22时52分31秒 HKT  
#     COPYRIGHT: Copyright (c) 2013, linkscue  
#      REVISION: 0.1  
#  ORGANIZATION: ---  
#===============================================================================  
  
__ScriptVersion="0.1"  
  
#===  FUNCTION  ================================================================  
#         NAME:  usage  
#  DESCRIPTION:  Display usage information.  
#===============================================================================  
function usage ()  
{  
        cat <<- EOT  
  
  Usage :  $0 -s suffix1 -s suffix2 -d dir1 -d dir2 -f file1 -f file2  
  
  Options:  
  -h|help       Display this message  
  -v|version    Display script version  
  -s suffix     Setting suffix  
  -d directory  Convert all file encoding to UTF-8  
  -f file       Convert a file encoding to UTF-8  
  
EOT  
}    # ----------  end of function usage  ----------  
  
#-----------------------------------------------------------------------  
#  Handle command line arguments  
#-----------------------------------------------------------------------  
  
suffixs=()  
directorys=()  
files=()  
while getopts ":hvd:f:s:" opt  
do  
  case $opt in  
  
    h|help     )  usage; exit 0   ;;  
  
    v|version  )  echo "$0 -- Version $__ScriptVersion"; exit 0   ;;  
  
    f  )  files+=("$OPTARG")   ;;  
  
    d  )  directorys+=("$OPTARG")   ;;  
  
    s  )  suffixs+=("$OPTARG")   ;;  
  
    \? )  echo -e "\n  Option does not exist : $OPTARG\n"  
          usage; exit 1   ;;  
  
  esac    # --- end of case ---  
done  
shift $(($OPTIND-1))  
  
# 检查输入  
if [[ ${#files} -lt 1 ]] && [[ ${#directorys} -lt 1 ]]; then  
    usage  
    exit  
fi  
  
TMPFILE="$(mktemp -t convXXXXXX)"  
trap "rm -f '$TMPFILE'" 0               # EXIT  
trap "rm -f '$TMPFILE'; exit 1" 2       # INT  
trap "rm -f '$TMPFILE'; exit 1" 1 15    # HUP TERM  
  
#-------------------------------------------------------------------------------  
#  转换编码函数  
#-------------------------------------------------------------------------------  
conv_utf8(){  
    file="$1"  
    echo "处理文件: '$file' ..."  
    iconv -f gb2312 -t UTF-8 "$file" -o $TMPFILE 2> /dev/null &&\  
        mv -f $TMPFILE "$f" || {  
        echo "转换失败: '${file}'"  
    }  
}  
  
# 转换文件  
for f in "${files[@]}"; do  
    conv_utf8 "$f"  
done  
  
# 转换目录文件  
if [[ ${#directorys} -gt 1 ]]; then  
    if [[ ${#suffixs} -lt 1 ]]; then  
        echo  
        echo "请指定需转换编码的文件后缀，如 '-s txt -s java'"  
        echo  
        usage  
        exit  
    else  
        for s in "${suffixs[@]}"; do  
            for f in $(find $directorys -type f -name "*.${s#.}"); do  
                conv_utf8 "$f"  
            done  
        done  
    fi  
fi