function y=CostFunction(x)
%% 要求解的函数
    y =(10.*sin(5*x) + 7*abs(x - 5) + 10);
end