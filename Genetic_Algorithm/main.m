clear;clc;close all;

%%遗传参数设置
NUMPOP=100;%初始种群大小
irange_l=-1; %问题解区间
irange_r=2;
LENGTH=22; %二进制编码长度
ITERATION = 10000;%迭代次数
CROSSOVERRATE = 0.7;%杂交率
SELECTRATE = 0.6;%选择率
VARIATIONRATE = 0.001;%变异率

%初始化种群
pop=Initial_pop(NUMPOP,irange_l,irange_r);
pop_save=pop;
%绘制初始种群分布
x=linspace(-1,2,1000);
y=CostFunction(x);
plot(x,y);
hold on
for i=1:size(pop,2)
    plot(pop(i),CostFunction(pop(i)),'ro');
end
hold off
title('初始种群');

%开始迭代
for time=1:ITERATION
    %计算初始种群的适应度
    fitness=m_Fitness(pop);
    %选择
    pop = Selection(fitness,pop,SELECTRATE);
    %编码
    binpop = Coding(pop,LENGTH,irange_l);
    %交叉
    kidsPop = Crossover(binpop,NUMPOP,CROSSOVERRATE);
    %变异
    kidsPop = Variation(kidsPop,VARIATIONRATE);
    %解码
    kidsPop = Decoding(kidsPop,irange_l);
    %更新种群
    pop=[pop kidsPop];
end
figure
x=linspace(-1,2,1000);
y=CostFunction(x);
plot(x,y);
hold on
for i=1:size(pop,2)
    plot(pop(i),CostFunction(pop(i)),'ro');
end
hold off
title('终止种群');

disp(['最优解：' num2str(max(CostFunction(pop)))]);
disp(['最大适应度：' num2str(max(m_Fitness(pop)))]);   
    
    


