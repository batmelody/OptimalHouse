function L = f(route,n,l)
    L = 0;
    for i =1:n-1
        L = L + l(route(i),route(i+1));
    end
end
