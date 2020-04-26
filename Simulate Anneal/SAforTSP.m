n = 20;
point = 100.*rand(n,2);
for i =1:n
    for j = 1:n
        l(i,j) = sqrt((point(i,1)-point(j,1))^2 +(point(i,2)-point(j,2))^2);
    end
end
route = randperm(n);
T = 10000;
k = 0;
Tmin = 1;
a = 0.99;
plot(point(route(:),1),point(route(:),2));
hold on;
plot(point(route(:),1),point(route(:),2),'Mo');




while T>Tmin
    r1 = round(rand(1)*19)+1;
    r2 = round(rand(1)*19)+1;
    route_ = route;
    route_([r2 r1]) = route([r1 r2]);
    delta_L = f(route_,n,l) - f(route,n,l);
    if delta_L < 0
        route = route_;
        T = a*T;
    else 
        r3 = rand(1);
        if r3< exp(-delta_L/T)
            route = route_;
        end
    end

end
plot(point(route(:),1),point(route(:),2));
hold on;
plot(point(route(:),1),point(route(:),2),'Mo');











