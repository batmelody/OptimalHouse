function output = index_of(a, list_ya)
for i =1:length(list_ya)
    if list_ya(i) == a
        output = i ;
    end
end
end