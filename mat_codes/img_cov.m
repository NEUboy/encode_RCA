% 计算图像像素相关性
% img 为待求图像，随机取n对像素点
function img_cov(img,n)

% M,N 存放图像大小
[M,N]=size(img);

% gray_val_base,gray_val_hor,gray_val_ver,gray_val_dia分别存储基点，水平相邻点，垂直相邻点，斜相邻点的像素灰度值
gray_val_base = zeros(1,n);
gray_val_hor = zeros(1,n);
gray_val_ver = zeros(1,n);
gray_val_dia = zeros(1,n);

% 计算相关性，随机取n对点，水平相邻点间的关系为(x,y)-(x+1,y)，垂直相邻点间的关系为(x,y)-(x,y+1)，斜相邻点间的关系为(x
% ,y)-(x+1,y+1)
% 循环 n 次
for i = 1:n
    % 产生一个随机的(x,y)坐标
    x_coor = randint(1,1,[1,N-1]); % 取N-1，M-1,这样相邻点不会超界
    y_coor = randint(1,1,[1,M-1]);
    % 取灰度值，放入相应数组中
    gray_val_base(i) = img(x_coor,y_coor);
    gray_val_hor(i) = img(x_coor+1,y_coor);
    gray_val_ver(i) = img(x_coor,y_coor+1);
    gray_val_dia(i) = img(x_coor+1,y_coor+1);
end
% 按照相关度公式计算
sum_base = 0;
sum_hor = 0;
sum_ver = 0;
sum_dia = 0;
for i=1:n
    sum_base = sum_base + gray_val_base(i);
    sum_hor = sum_hor + gray_val_hor(i);
    sum_ver = sum_ver + gray_val_ver(i);
    sum_dia = sum_dia + gray_val_dia(i);
end
mean_base = sum_base/n;
mean_hor = sum_hor/n;
mean_ver = sum_ver/n;
mean_dia = sum_dia/n;

sum_base_var = 0;
sum_hor_var = 0;
sum_ver_var = 0;
sum_dia_var = 0;
for i=1:n
    sum_base_var = sum_base_var + (gray_val_base(i) - mean_base)^2;
    sum_hor_var = sum_hor_var + (gray_val_hor(i) - mean_hor)^2;
    sum_ver_var = sum_ver_var + (gray_val_ver(i) - mean_ver)^2;
    sum_dia_var = sum_dia_var + (gray_val_dia(i) - mean_dia)^2;
end
variance_base = sum_base_var/n;
variance_hor = sum_hor_var/n;
variance_ver = sum_ver_var/n;
variance_dia = sum_dia_var/n;

sum_cov_base_hor = 0;
sum_cov_base_ver = 0;
sum_cov_base_dia = 0;
for i=1:n
    sum_cov_base_hor = sum_cov_base_hor + (gray_val_base(i) - mean_base)*(gray_val_hor(i) - mean_hor);
    sum_cov_base_ver = sum_cov_base_ver + (gray_val_base(i) - mean_base)*(gray_val_ver(i) - mean_ver);
    sum_cov_base_dia = sum_cov_base_dia + (gray_val_base(i) - mean_base)*(gray_val_dia(i) - mean_dia);
end
cov_base_hor = sum_cov_base_hor/n;
cov_base_ver = sum_cov_base_ver/n;
cov_base_dia = sum_cov_base_dia/n;

r_base_hor = cov_base_hor/(sqrt(variance_base)*sqrt(variance_hor));
r_base_ver = cov_base_ver/(sqrt(variance_base)*sqrt(variance_ver));
r_base_dia = cov_base_dia/(sqrt(variance_base)*sqrt(variance_dia));

% 输出结果
r_base_hor
r_base_ver
r_base_dia

% 绘图
figure;
plot(gray_val_base,gray_val_hor,'.','Markersize',3);
figure;
plot(gray_val_base,gray_val_ver,'.','Markersize',3);
figure;
plot(gray_val_base,gray_val_dia,'.','Markersize',3);





 