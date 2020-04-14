function distance = crowding_distance(values1, values2, front)

distance = zeros(1, length(front)); 
sorted1 = sort_by_values(front, values1);
sorted2 = sort_by_values(front, values2);
distance(1) = 77777777;
distance(length(front)) = 77777777;

for k =2:length(front)-1
    distance(k) = distance(k) + (values1(sorted1(k+1))-values2(sorted1(k-1)))/(max(values1)-min(values1));
end

for k =2:length(front)-1
    distance(k) = distance(k) + (values1(sorted2(k+1)) - values2(sorted2(k-1)))/(max(values2) - min(values2));
    
end

end