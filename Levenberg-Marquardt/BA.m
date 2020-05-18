function X = BA(RayIn1, RayIn2, label1, label2)

syms x x1 r1 r2 r3 r4 r5 r6 y1 y2 y3 y4 y5 y6
X = [cosd(66)*cosd(66) -sind(66) cosd(66)*sind(66); cosd(66)*sind(66) cosd(66) sind(66)*sind(66); -sind(66) 0 cosd(66)];Y = [cosd(x1)*cosd(x) -sind(x) cosd(x)*sind(x1); cosd(x1)*sind(x) cosd(x) sind(x)*sind(x1); -sind(x1) 0 cosd(x1)];
R1 = [r1 r2 r3]'; R2 = [r4 r5 r6]';
Y1 = [y1 y2 y3]'; Y2 = [y4 y5 y6]';

%%
Fx = norm(X* Y * R1 - Y1) + norm(X * Y * R2 - Y2);
Jx = jacobian(Fx, [x x1]);
Ax = Jx'*Jx;
gx = Jx'*Fx;

%%
F = matlabFunction(Fx);
A = matlabFunction(Ax);
g = matlabFunction(gx);


%%
r = [RayIn1' RayIn2'];
y = [label1' label2'];
X = [33 23]';%initial weight
tau = 1e-5; %initial parameter
e = 1e-13; %deformate error
k = 0;
v = 2;
I = diag([1 1]); %(r1,r2,r3,r4,r5,r6,x1,x2,x3,x4,x5,x6,x7,x8,x9,y1,y2,y3)
mu = tau*max(diag(A(r(1),r(2),r(3),r(4),r(5),r(6),X(1),X(2),y(1),y(2),y(3),y(4),y(5),y(6))));

while k<1000
    k = k+1;
    h = (A(r(1),r(2),r(3),r(4),r(5),r(6),X(1),X(2),y(1),y(2),y(3),y(4),y(5),y(6))+mu.*I)\-g(r(1),r(2),r(3),r(4),r(5),r(6),X(1),X(2),y(1),y(2),y(3),y(4),y(5),y(6));
    if norm(h) <e*(norm(X)+h)
        continue
    else
        X_new = X + h;
        rho = (F(r(1),r(2),r(3),r(4),r(5),r(6),X(1),X(2),y(1),y(2),y(3),y(4),y(5),y(6)) - ...
            F(r(1),r(2),r(3),r(4),r(5),r(6),X_new(1),X_new(2),y(1),y(2),y(3),y(4),y(5),y(6)))/...
            (0.5*h'*(mu*h-g(r(1),r(2),r(3),r(4),r(5),r(6),X_new(1),X_new(2),y(1),y(2),y(3),y(4),y(5),y(6))));
        if rho > 0 
            X = X_new;
            mu = mu*max(1/3, 1-(2*rho -1).^2);
            v = 2;
        else
            mu = mu*v;
            v = 2*v;
        end
    end
end
end

