#!/bin/bash
# 以上一行声明脚本使用bash解释器

# 提示用户输入文件名
echo "Please enter a filename:"
count=0 # 初始化行数计数器
read File # 读取用户输入的文件名

# 逐行读取文件内容并输出
while read Line
do
  echo $Line    # 输出当前行内容
  count=$(($count+1))    # 行数加1
done < $File  # 从用户输入的文件中读取内容

# 输出文件的总行数
echo "There are $count lines." # 输出总行数
