pop_size = 20;
max_gen = 7777;
min_x=-77;
max_x=77;
gen_no=0;


for i =1:pop_size
solution(i) = min_x + (max_x-min_x) * rand(1);
end


while(gen_no <max_gen )
    for i =1:length(solution)
        function1_values(i) = function1(solution(i));
        function2_values(i) = function2(solution(i));
    end

    non_dominated_sorted_solution = fast_non_dominated_sort(function1_values(:),function2_values(:));
    

   

    crowding_distance_values ={};
    for i =1:length(non_dominated_sorted_solution)
            crowding_distance_values{end+1} = crowding_distance(function1_values(:),function2_values(:),non_dominated_sorted_solution{i}(:));
    end

    solution2 = solution;

 


    while(length(solution2)~=2*pop_size)

        a1 = unidrnd(pop_size-1);

        b1 = unidrnd(pop_size-1);
        
        solution2(end+1) = crossover(solution(a1),solution(b1), min_x, max_x);
    end


        
    for i = 1:2*pop_size        
        function1_values2(i) = function1(solution2(i));
        function2_values2(i) = function2(solution2(i));        
    end

    non_dominated_sorted_solution2 = fast_non_dominated_sort(function1_values2,function2_values2);
    crowding_distance_values2 = {};
    for i =1:length(non_dominated_sorted_solution2)
        crowding_distance_values2{end+1} = crowding_distance(function1_values2,function2_values2,non_dominated_sorted_solution2{i});
    end


    new_solution = [];
    for i = 1:length(non_dominated_sorted_solution2)
        for j = 1:length(non_dominated_sorted_solution2{i})
            non_dominated_sorted_solution2_1(j) = index_of(non_dominated_sorted_solution2{i}(j),non_dominated_sorted_solution2{i});
        end
        
        front22 = sort_by_values(non_dominated_sorted_solution2_1, crowding_distance_values2{i});
        for j =1:length(non_dominated_sorted_solution2{i})
            front(j) = non_dominated_sorted_solution2{i}(front22(j));
        end
        front = flip(front);
       
        for num =1 :length(front)
            new_solution(end+1)= front(num);
            if length(new_solution)==pop_size 
                    break
            end
        end
        if  length(new_solution) == pop_size
           break
        end
    for num2 =1: length(new_solution)
        solution(num2) = solution2(new_solution(num2)); 
    end
   end
gen_no = gen_no + 1;
end
