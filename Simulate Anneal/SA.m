

f = @(x) x.^2; 
x = -3;
k = 0;
C = 10;
while k <10000
    h = rand(1) - 0.5;
    T = f(x);
    delta_T = f(x+h) - f(x);
    if delta_T < 0
        x = x + h;
        T = f(x);
    else
        r = rand(1)+1;
        if r < exp(-delta_T/C*T)
            x = x + h;
            T = f(x);
        else
            T = (x);
        end
    end
    k = k+1;
end
xx = -5:0.1:5;
yy = f(xx);
plot(xx, yy);
hold on;
plot(x, f(x),'Mo');