
    % ��ȡͼƬ
    img = imread('output.bmp');
    
    % ����Ƿ�Ϊ�Ҷ�ͼ�����������ת��Ϊ�Ҷ�
    if size(img, 3) == 3
        img = rgb2gray(img);
    end
    
    % ��ʼ��ֱ��ͼ����      
    histogram = zeros(1, 256);
    
    % ����ͼ���ֱ��ͼ
    for i = 1:numel(img)
        pixelValue = img(i);
        histogram(pixelValue + 1) = histogram(pixelValue + 1) + 1;
    end
    
    % ����ÿ������ֵ��0��1, 2��3,...,254��255���������Ͳ���
    chiSquareSum = 0;
    for i = 1:2:255
        observedFrequency1 = histogram(i);
        observedFrequency2 = histogram(i + 1);
        expectedFrequency = (observedFrequency1 + observedFrequency2) / 2;
        % �������������
        if expectedFrequency > 0
            chiSquareSum = chiSquareSum + (((observedFrequency1 - expectedFrequency)^2) / expectedFrequency);
            chiSquareSum = chiSquareSum + (((observedFrequency2 - expectedFrequency)^2) / expectedFrequency);
        end
    end
    
    % �������ɶȣ�����pair��ԭ����룩
    dof = 127;
    
    % ����Pֵ
    pValue = 1 - chi2cdf(chiSquareSum, dof);
    
    fprintf('The Chi-square test p-value is: %f\n', pValue);
