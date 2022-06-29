clear;clc;
row=128;col=128;
board=zeros(row*2,col*2);
state0= rand(row,col)>0.7;
state1= rand(row,col)>0.7;

%存一下初始的图
board=state_to_board(state0);
imshow(board);
imwrite(board,'E:\matlab_2016\my_code\cryptography\celluar automata\ state0.bmp');

board=state_to_board(state1);
imwrite(board,'E:\matlab_2016\my_code\cryptography\celluar automata\ state1.bmp');

%正向演化
for i=1:100
    temp=state1;
    [state1,board]=CA_func(state1);
    imshow(board);
    state1=mod(state1+state0,2);
    state0=temp;
end
board=state_to_board(state0);
imshow(board);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ final_state0.bmp');
board=state_to_board(state1);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ final_state1.bmp');
d_state0 = state0;
d_state1 = state1;
tabulate(state0(:))
disp('reverse');

% 逆向演化
for i=1:100
    temp=state0;
    [state0,board]=CA_func(state0);
    imshow(board);
    state0=mod(state1+state0,2);
    state1=temp;
end
board=state_to_board(state0);
imshow(board);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ reverse_state0.bmp');
board=state_to_board(state1);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ reverse_state1.bmp');

% 假如给最终的state做一个状态的改变
disp('robust test')
pos=rand(1,2);
x=mod(floor(pos(1)*10000),col);
y=mod(floor(pos(2)*10000),row);
d_state1(x,y)=mod(d_state1(x,y)+1,2);

for i=1:100
    temp=d_state0;
    [d_state0,board]=CA_func(d_state0);
    imshow(board);
    d_state0=mod(d_state1+d_state0,2);
    d_state1=temp;
end
board=state_to_board(d_state0);
imshow(board);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ reverse_dstate0.bmp');
board=state_to_board(d_state1);
imwrite(board, 'E:\matlab_2016\my_code\cryptography\celluar automata\ reverse_dstate1.bmp');