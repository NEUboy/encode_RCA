function [ board ] = state_to_board( state )
[row,col]= size(state);
board=zeros(row*2,col*2);
for i=1:row
    for j=1:col
        if state(i,j)==1
           board(((i-1)*2+1):i*2,((j-1)*2+1):j*2)=255;
         else
           board(((i-1)*2+1):i*2 ,((j-1)*2+1):j*2)=0;
        end
    end
end
end

