function sorted_list = sort_by_values(list1, values)
%%  
     %values = [0 0 0 1 1 0 0 0 0 -1];
     %list1 = [4 9];
     %=>sorted_list = [9 4];
     
sorted_list = [];
while length(sorted_list)~=length(list1)
    if sum(index_of(min(values), values) == list1)~=0
        sorted_list(end+1) = index_of(min(values), values);
    end
    values(index_of(min(values), values)) = Inf;
end  
end
