clear  %清空变量

% 利用Matlab自带函数读取隐密图像output. bmp，得到隐密图像的信息并将图像转化为二进制
Picture = imread( 'output.bmp' );
Picture = double(Picture);
[m, n] = size(Picture);

% 打开存放秘密信息的文件，若没有则新建一个文件。顺序提取图像相应像素LSB的秘密信息，存储在打开的文件中并保存
frr = fopen('tiqu.txt', 'w');
len = 227680;
%test = [];
p = 1;
for f2 = 1:n
    for f1 = 1:m
        %lowbit = bitand(Picture(f1, f2), 1);
        %test = [test lowbit]; % 注意！如果做数组cat操作，会严重增加提取程序运行时长
        fwrite(frr, bitand(Picture(f1, f2), 1), 'ubit1');
        if p == len
            break;
        end
        p=p+1;
    end
    if p == len
        break;
    end
end
%fwrite(frr, test);
fclose(frr);
