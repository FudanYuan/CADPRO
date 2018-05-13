#include "GA.h"
#include <QTime>
#include <QDebug>

Genome::Genome() :
    fitness(0)
{

}

Genome::Genome(QVector<double> vec, double f) :
    genome(vec),
    fitness(f)
{

}

void Genome::setGenome(QVector<double> g)
{
    genome = g;
}

QVector<double> Genome::getGenome()
{
    return genome;
}

QVector<double> Genome::getGenome(int start, int end)
{
    QVector<double> res;
    for(int i=start; i<=end; i++){
        res.append(genome[i]);
    }
    return res;
}

void Genome::setFitness(double f)
{
    fitness = f;
}

double Genome::getFitness()
{
    return fitness;
}

void Genome::swap(int index1, int index2)
{
    double temp = genome[index1];
    genome[index1] = genome[index2];
    genome[index2] = temp;
}

bool Genome::isNull()
{
    return genome.length() == 0 && fitness == 0;
}

GA::GA(int size, int gLenght, int cf, double distance, double p,
       double (*fitnessFunc)(Genome &genemo), double score,
       double cRate, double mRate) :
    calculateFitness(fitnessFunc),
    popSize(size),
    genLength(gLenght),
    CF(cf),
    similarity(distance),
    punish(p),
    fitnessMax(score),
    crossoverRate(cRate),
    mutationRate(mRate),
    totalFitness(0),
    bestFitness(0),
    worstFitness(__LONG_MAX__),
    averageFitness(0),
    generation(0),
    crossoverCount(0),
    mutationCount(0),
    stop(false),
    seed(time(0))
{
    qsrand(seed);
    // 单点交叉概率与双点交叉概率平分
    crossoverOneRate = crossoverTwoRate = 0.5;
    // 旋转变异与位置变异相同
    mutationLocationRate = mutationRotateRate = mutationRate;
}

GA::~GA()
{

}

void GA::initPopulation()
{
    // 随机生成popSize大小的种群
    // 首先，生成基因初始化序列，即[1, 2, 3, ... , genLength]
    double genome[genLength];
    for(int n=0; n<genLength; n++){
        genome[n] = n + 1;
    }
    // 为增加多样性，随机确定某一位置location，在其前后分别随机排列
    int location = randT<int>(0, genLength-1);
    std::random_shuffle(genome, genome+location);
    std::random_shuffle(genome+location+1, genome+genLength);
    for(int i=0; i<popSize;i++){
        std::random_shuffle(genome, genome+genLength);
        QVector<double> genomeVector;
        for(int n=0; n<genLength; n++){
            // 随机生成旋转方向
            // 随机数为奇数，则方向为-1，否则为1;
            int direction = qrand() % 2 == 0 ? 1 : -1;
            genomeVector.append(direction * genome[n]);
        }
        //qDebug() << genomeVector;
        Genome g(genomeVector, 0);
        population.append(g);
    }
    //qDebug() << "  ";
}

void GA::evaluateFitness()
{
    totalFitness = 0;
    averageFitness = 0;
    for (int i=0; i<popSize; i++) {
        double fitness = calculateFitness(population[i]);
        // 累计适应性分数
        totalFitness += fitness;
        // 得到最好的适应度分数，并更新最优个体
        if(fitness >= bestFitness)
        {
            bestFitness = fitness;
            fittestGenome = population[i];
        }
        // 得到最差的适应度分数
        if(fitness <= worstFitness)
            worstFitness = fitness;
    }
    // 计算平均适应度分数
    averageFitness = totalFitness / popSize;
#ifdef NESTDEBUG
    qDebug() << "适应度累计分数: " << totalFitness;
    qDebug() << "最好适应度分数: " << bestFitness;
    qDebug() << "最坏适应度分数: " << worstFitness;
    qDebug() << "平均适应度分数: " << averageFitness;
    qDebug() << "";
#endif
}

void GA::sortPopulation(QVector<Genome> &vector)
{
    // 根据适应度值对原始种群的个体降序排列，
    qSort(vector.begin(), vector.end(), qGreater<Genome>());
}

void GA::calculateHammingDistance(QVector<Genome> &vector)
{
//    qDebug() << "计算海明距离：";
//    qDebug() << "长度：" << vector.length();
//    for(int i=0; i<vector.length();i++){
//        qDebug() << "基因组：(个体" << i << ")" << vector[i].getGenome();
//    }
//    qDebug() << "";
    // 计算两两之间的距离
    int count = 0;
    for(int i=0; i<vector.length()-1;i++){
        for(int j=i+1; j<vector.length();j++){
            double res = 0;
            for(int n=0;n<genLength;n++){
                res += qPow((vector[i].getGenome()[n] - vector[j].getGenome()[n]), 2);
            }
            // 若距离小于阈值，则惩罚其中适应度值较差的个体。
            double distance = qSqrt(res);
            if(distance < similarity){
                count++;
                if(vector[i].fitness <= vector[j].fitness){
                    //qDebug() << "基因组被惩罚：(个体" << i << ")" << vector[i].getGenome();
                    //qDebug() << "被惩罚前分数：(个体" << i << ")" << vector[i].getFitness();
                    vector[i].fitness = punish;
                } else{
                    //qDebug() << "基因组被惩罚：(个体" << j << ")" << vector[i].getGenome();
                    //qDebug() << "被惩罚前分数：(个体" << j << ")" << vector[j].getFitness();
                    vector[j].fitness = punish;
                }
            }
        }
    }
    //qDebug() << "惩罚因子：" << count;
}

Genome GA::selection()
{
    Genome g;
    // 产生0到（适配值之和）之间的任何一个实数
    double fitness = randT<double>(0, totalFitness);
    for(int i=0; i<popSize; i++){
        fitness -= population[i].fitness;
        if(fitness <= 0){
            g = population[i];
            break;
        }
    }
    return g;
}

Genome GA::crossover(Genome parent1, Genome parent2)
{
    // 随机生成一个概率
    double rate = randT<double>(0, 1);
    // 若该概率小于单点交叉概率，则进行单点交叉，否则，进行双点交叉
    QVector<double> newGene;
    if(rate <= crossoverOneRate){
        // 单点交叉
        // 随机生成一个交叉位p
        int index = randT<int>(0, genLength-1);
        //qDebug() << "单点交叉";
        //qDebug() << "位置：" << index;
        // 生成新的基因
        // 首先得到要提到最前面的基因片段
        newGene = parent1.getGenome(0, index);
        crossoverOneCount += 1;
    } else{
        // 双点交叉
        // 首先随机生成两个交叉位p, q
        int index1 = randT<int>(0, genLength-1);
        int index2 = randT<int>(index1+1, genLength-1);
        //qDebug() << "双点交叉";
        //qDebug() <<  "位置：" << index1 << "   " << index2;
        // 生成新的基因
        // 首先得到要提到最前面的基因片段
        newGene = parent1.getGenome(index1, index2);
        crossoverTwoCount += 1;
    }

    // 将剩余基因按parent2出现的先后顺序添加至最新的基因片段后
    for(int i=0;i<genLength;i++){
        double g = parent2.genome[i];
        if(!newGene.contains(g) && !newGene.contains(-g)){
            newGene.append(g);
        }
    }
    crossoverCount += 1;
    return Genome(newGene);
}

Genome GA::mutationLocation(Genome genome)
{
    // 随机取得0到genLength-1之间的2个数，包括0和genLength-1
    int index1 = randT<int>(0, genLength-1);
    int index2 = randT<int>(0, genLength-1);
    // 将两个基因互换
    if(index1 != index2){
        genome.swap(index1, index2);
    }
    mutationLocationCount += 1;
    mutationCount += 1;
    return genome;
}

Genome GA::mutationRotate(Genome genome)
{
    // 随机取得0到genLength-1之间的一个数，包括0和genLength-1
    int index = randT<int>(0, genLength-1);
    //qDebug() << "旋转变异位： " << index;
    // 该位置之前的基因全部进行旋转变异
    for(int i = 0; i<=index; i++){
        genome.genome[i] = -genome.genome[i];
    }
    mutationRotateCount += 1;
    mutationCount += 1;
    return genome;
}

Genome GA::getNewChild()
{
    Genome child;
    Genome parent1 = selection();
    double rate = randT<double>(0, 1);
    // 按概率交叉
    if(rate < crossoverRate){
        Genome parent2 = selection();
        child = crossover(parent1, parent2);
    } else{
        child = parent1;
    }
    // 按概率位置突变
    rate = randT<double>(0, 1);
    if(rate < mutationLocationRate){
        child = mutationLocation(child);
    }
    // 按概率旋转突变
    rate = randT<double>(0, 1);
    if(rate < mutationRotateRate){
        child = mutationRotate(child);
    }
    return child;
}

void GA::getNewGeneration()
{
    // 评估，计算每一个个体的适应度值
    evaluateFitness();

    // 如果适应度值足够高，可提前结束
    if(bestFitness >= fitnessMax){
        stop = true;
    }

    // 根据适应度值对种群的个体降序排列
    sortPopulation(population);

    // 记忆前 popSize / CF 个个体。
    memoryPop.clear();
    for(int i=0; i<popSize/CF; i++){
        memoryPop.append(population[i]);
    }

    // 更新种群
    // 合并父代记忆的个体集合与子代个体组成新的种群,
    // 大小为popSize + popSize / CF
    QVector<Genome> newPopulation;
    for(int i=0; i<memoryPop.length(); i++){
        newPopulation.append(memoryPop[i]);
    }
    while(newPopulation.length() - popSize / CF< popSize){
        newPopulation.append(getNewChild());
    }

    // 进行小生境淘汰，
    // 即对最新种群内的两两个体计算海明距离，进行相似比较，惩罚较差个体
    calculateHammingDistance(newPopulation);

    /*
    for(int i=0;i<popSize+popSize/CF;i++){
        qDebug() << "惩罚后的基因组：" << newPopulation[i].getGenome();
        qDebug() << "惩罚后的fitness：" << newPopulation[i].getFitness();
    }
    */

    // 根据适应度值对最新种群的个体降序排列
    sortPopulation(newPopulation);

    // 更新种群，大小为popSize
    population.clear();
    for(int i=0; i<popSize; i++){
        population.append(newPopulation[i]);
    }
    generation += 1;
}

Genome GA::getFittestGenome()
{
    return fittestGenome;
}

bool GA::isStop()
{
    return stop;
}

template<typename T>
T GA::randT(T lower, T upper)
{
    double r = qrand() % 1000 / (double)1001;
    return lower + r * (upper - lower);
}
