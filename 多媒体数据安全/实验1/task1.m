

clear  % 清空变量
close all % 关闭打开的图像窗口

Picture = imread('inputgray.bmp');
Double_Picture = Picture;
Double_Picture = double(Double_Picture);

%读取秘密信息文件为二进制数字流，为嵌入图像做准备
wen.txt_id = fopen ('information.txt','r');
[msg, len] = fread(wen.txt_id, 'ubit1');

%根据LSB算法，将秘密信息的二进制数字流隐藏入连续的像素中
[m, n]=size(Double_Picture);
p=1;
for f2 = 1:n
    for f1 = 1:m
        Double_Picture(f1, f2) = Double_Picture(f1, f2)-mod(Double_Picture(f1,f2), 2)+msg(p,1);
        if p == len
            break;
        end
        p = p+1;
    end
    if p == len
        break;
    end
end

%将得到的隐密图像保存为output.bmp，并利用Matlab将载体图与隐密图画在同一对话框中进行比较
Double_Picture=uint8(Double_Picture);
imwrite(Double_Picture, 'output.bmp');
subplot(121);imshow(Picture);title('未嵌入信息的图像');
subplot(122);imshow(Double_Picture);title('嵌入信息的图像');

fclose(wen.txt_id);


