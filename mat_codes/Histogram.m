clear all;
clc;
fid1=fopen('lenna_decrypted.raw','r');%打开原始图像
pix1=fread(fid1,[256 256],'uint8');
imhist(pix1/256);

% num=zeros(256,1);
% for i=1:256
%     for j= 1:256
%         num(pix1(i,j)+1)=num(pix1(i,j)+1)+1;
%     end
% end
% plot(num);


