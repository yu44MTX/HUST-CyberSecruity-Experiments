I = imread('inputgray.bmp');
O = imread('output.bmp');
I = I(1:numel(I));
O = O(1:numel(I));
x = 30;

subplot(1, 2, 1);  % ��һ����ͼ
histogram(I, 0:1:x);
set(gca, 'xtick', 0:2:x);
grid on;
title('Original Image');

subplot(1, 2, 2);  % �ڶ�����ͼ
histogram(O, 0:1:x);
set(gca, 'xtick', 0:2:x);
grid on;
title('Processed Image');