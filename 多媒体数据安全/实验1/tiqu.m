clear  %��ձ���

% ����Matlab�Դ�������ȡ����ͼ��output. bmp���õ�����ͼ�����Ϣ����ͼ��ת��Ϊ������
Picture = imread( 'output.bmp' );
Picture = double(Picture);
[m, n] = size(Picture);

% �򿪴��������Ϣ���ļ�����û�����½�һ���ļ���˳����ȡͼ����Ӧ����LSB��������Ϣ���洢�ڴ򿪵��ļ��в�����
frr = fopen('tiqu.txt', 'w');
len = 227680;
%test = [];
p = 1;
for f2 = 1:n
    for f1 = 1:m
        %lowbit = bitand(Picture(f1, f2), 1);
        %test = [test lowbit]; % ע�⣡���������cat������������������ȡ��������ʱ��
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
