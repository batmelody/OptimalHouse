function front = fast_non_dominated_sort(values1, values2)
S = {};
front = {[]};
n = zeros(1, length(values1));
rank = zeros(1, length(values1));


for i = 1:length(values1)
    S{end+1} = [];
end

for p = 1:length(values1)
    S{p} = [];
    n(p) = 0;
    for q = 1:length(values1)
        if (values1(p) > values1(q) & values2(p) > values2(q)) || (values1(p) >= values1(q) & values2(p) > values2(q))||(values1(p) > values1(q) & values2(p) >= values2(q))
            if sum(q == S{p})==0
                S{p}(end+1) = q;
            end
        elseif (values1(q) > values1(p) & values2(q) > values2(p)) || (values1(q) >= values1(p) & values2(q) > values2(p))||(values1(q) > values1(p) & values2(q) >= values2(p))
            n(p) = n(p) + 1;
        end
    end
    if n(p)==0
        rank(p)=0;
        if sum(p==front{1})==0
            front{1}(end+1) = p;
        end
    end
end

i = 1;
while sum(front{i})~=0  
    Q = [];
    for ii = 1:length(front{i})
        p = front{i}(ii);
        for jj = 1:length(S{p})
            q = S{p}(jj);
            n(q) = n(q) - 1;
            if n(q) == 0
                rank(q)=i+1;
                if sum(q==Q)==0
                    Q(end+1)=q;
                end
            end
        end
    end
    i = i+1;
    front{end+1} = Q;
end
front(cellfun(@isempty, front)) = [];
end           