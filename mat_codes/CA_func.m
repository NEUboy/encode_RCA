function [ state,board] = CA_func(state)
% 迭代一次CA的函数
[row,col]= size(state);
newstate = state;
board=zeros(row*2,col*2);
for i=1:row
    for j=1:col
        cnt=0;
        up=i-1;down=i+1;
        left=j-1;right=j+1;
        if i==1
            up=row;
        end
        if i==row
            down=1;
        end
        if j==1
            left=col;
        end
        if j==col
            right=1;
        end
        cnt= state(up,left)+state(up,j)+state(up,right)...
            +state(i,left)+state(i,right)...
            +state(down,left)+state(down,j)+state(down,right);
        
        if newstate(i,j)==0
            if cnt==8||cnt==5||cnt==3||cnt==1
                newstate(i,j)=1;
            end
        elseif newstate(i,j)==1
            if cnt==8||cnt==7||cnt==6||cnt==3||cnt==2
                newstate(i,j)=0;
            end
        end
        if newstate(i,j)==1
            board(((i-1)*2+1):i*2,((j-1)*2+1):j*2)=255;
        else
            board(((i-1)*2+1):i*2 ,((j-1)*2+1):j*2)=0;
        end
    end
end
state=newstate;
end

