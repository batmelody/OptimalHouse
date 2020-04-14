function solution = mutation(solution, min_x, max_x)
 mutation_prob = rand(1);
if mutation_prob < 1
    solution =  min_x + (max_x - min_x)* rand(1);
end
end