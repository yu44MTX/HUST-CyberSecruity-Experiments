
    % 读取图片
    img = imread('output.bmp');
    
    % 检查是否为灰度图像，如果不是则转换为灰度
    if size(img, 3) == 3
        img = rgb2gray(img);
    end
    
    % 初始化直方图数组      
    histogram = zeros(1, 256);
    
    % 计算图像的直方图
    for i = 1:numel(img)
        pixelValue = img(i);
        histogram(pixelValue + 1) = histogram(pixelValue + 1) + 1;
    end
    
    % 计算每对像素值（0和1, 2和3,...,254和255）的总数和差异
    chiSquareSum = 0;
    for i = 1:2:255
        observedFrequency1 = histogram(i);
        observedFrequency2 = histogram(i + 1);
        expectedFrequency = (observedFrequency1 + observedFrequency2) / 2;
        % 避免除以零的情况
        if expectedFrequency > 0
            chiSquareSum = chiSquareSum + (((observedFrequency1 - expectedFrequency)^2) / expectedFrequency);
            chiSquareSum = chiSquareSum + (((observedFrequency2 - expectedFrequency)^2) / expectedFrequency);
        end
    end
    
    % 计算自由度（由于pair的原因减半）
    dof = 127;
    
    % 计算P值
    pValue = 1 - chi2cdf(chiSquareSum, dof);
    
    fprintf('The Chi-square test p-value is: %f\n', pValue);
