function m = crossover(a, b, min_x, max_x)
r = rand(1);
if r >0.5
    m = mutation((a+b)/2, min_x, max_x);
    
else
    m = mutation((a-b)/2, min_x, max_x);
end
end