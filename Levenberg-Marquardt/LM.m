%%  sym variable process
syms x y
Fx = (x-3)^2+(y-2)^2;  %f(x,y) =e^(xy)+cos(x)
Jx = jacobian(Fx);  % Jacobian matrix of f
Ax = Jx'*Jx; %J'J
gx = Jx'*Fx; % J'f(x,y)


%% syms transform into function
F = matlabFunction(Fx); 
J = matlabFunction(Jx);
A = matlabFunction(Ax);
g = matlabFunction(gx);

%% initial value
tau = 1e-10; 
e = 1e-10; %error
k = 0;
v = 2;
I = diag([1 1]);
X = [199; -33]; %initial point
found = norm(g(X(1), X(2)));
mu = tau*max(diag(A(X(1),X(2)))); 



%% Main Loop
while k<10000
    k = k+1;
    h = inv(A(X(1),X(2))+mu.*I)*-g(X(1),X(2)); %it can be changed to (A+mu*I)/-g but I didnt have a try
    if norm(h)<e*(norm(X)+h)
       break;
    else
        X_new = X + h;
        rho = (F(X(1),X(2))-F(X_new(1),X_new(2)))/(0.5*h'*(mu*h-g(X(1),X(2))));
        if rho > 0
            X = X_new;
            A_ = A(X(1),X(2));
            g_ = g(X(1),X(2));
            found = norm(g_);
            mu = mu*max(1/3, 1-(2*rho-1).^3);
            v = 2;
        else
            mu = mu*v;
            v = 2*v;
        end
    end
end



        
                
    
