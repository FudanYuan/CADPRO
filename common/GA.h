#ifndef GA_H
#define GA_H

#include <qmath.h>
#include <QVector>
#include "debug.h"

// 基因组类
class Genome
{
public:
     friend class GA;
     Genome();
     Genome(QVector<double> vec, double f=0);
     void setGenome(QVector<double> g);
     QVector<double> getGenome();
     QVector<double> getGenome(int start, int end);
     void setFitness(double f);
     double getFitness();
     void swap(int index1, int index2);
     bool isNull();
     friend Q_DECL_CONSTEXPR inline bool operator==(const Genome &, const Genome &);
     friend Q_DECL_CONSTEXPR inline bool operator==(const Genome &, const Genome &);
     friend Q_DECL_CONSTEXPR inline bool operator!=(const Genome &, const Genome &);
     friend Q_DECL_CONSTEXPR inline bool operator>(const Genome &, const Genome &);
     friend Q_DECL_CONSTEXPR inline bool operator<(const Genome &, const Genome &);
private:
     QVector<double> genome; // 基因组向量
     double fitness;  // 适应度评分
};

/*****************************************************************************
  Genome inline functions
 *****************************************************************************/

Q_DECL_CONSTEXPR inline bool operator==(const Genome &g1, const Genome &g2){
    return g1.fitness == g2.fitness;
}
Q_DECL_CONSTEXPR inline bool operator!=(const Genome &g1, const Genome &g2){
    return g1.fitness != g2.fitness;
}
Q_DECL_CONSTEXPR inline bool operator>(const Genome &g1, const Genome &g2){
    return g1.fitness > g2.fitness;
}
Q_DECL_CONSTEXPR inline bool operator<(const Genome &g1, const Genome &g2){
    return g1.fitness < g2.fitness;
}
// 遗传算法
class GA
{
public:
    explicit GA(int size, int gLenght, int cf, double distance,
                double p, double (*fitnessFunc)(Genome &genemo),
                double score=0.95,
                double cRate=0.7, double mRate=0.05);
    ~GA();
    void initPopulation();  // 初始化种群
    void evaluateFitness();  // 评估适应度
    void sortPopulation(QVector<Genome> &vector);  // 将原始种群降序排列
    void calculateHammingDistance(QVector<Genome> &vector);  // 计算海明距离
    Genome selection();  // 选择，物竞天择（轮盘赌）
    Genome crossover(Genome parent1, Genome parent2);  // 交叉
    Genome mutationLocation(Genome genome);  // 位置变异
    Genome mutationRotate(Genome genome);  // 旋转变异
    Genome getNewChild();  // 产生新后代
    void getNewGeneration();   // 产生最新一代
    Genome getFittestGenome();  // 获取最优个体

    bool isStop();  // 获取停止位标识
    template<typename T>
    static T randT(T lower, T upper); //产生任意类型随机数函数
private:
    double (*calculateFitness)(Genome &genemo);  // 适应度计算函数
    QVector<Genome> population;  // 种群集合
    QVector<Genome> memoryPop;  // 记忆种群集合
    int popSize;  // 人口(种群)数量
    int genLength;  // 每一条染色体的基因的总数目
    int CF;  // 排挤因子
    double similarity;  // 相似距离
    double punish;  // 惩罚因子
    double fitnessMax;  // 最优值阈值
    double crossoverRate;  // 基因交叉的概率一般设为0.7
    double crossoverOneRate;  // 基因单点交叉的概率
    double crossoverTwoRate;  // 基因双点交叉的概率
    double mutationRate;  // 旋转变异概率
    double mutationRotateRate;  // 旋转变异概率
    double mutationLocationRate;  // 位置变异概率
    Genome fittestGenome;  // 最适应的个体在m_vecPop容器里面的索引号
    double totalFitness;  // 所有个体对应的适应性评分的总和
    double bestFitness;  // 在所有个体当中最适应的个体的适应性评分
    double worstFitness;  // 在所有个体当中最不适应的个体的适应性评分
    double averageFitness;  // 所有个体的适应性评分的平均值
    int generation;  // 代数的记数器
    int crossoverCount;  // 交叉计数器
    int crossoverOneCount;  // 单点交叉计数器
    int crossoverTwoCount;  // 双点交叉计数器
    int mutationCount;  // 变异计数器
    int mutationLocationCount;  // 位置变异计数器
    int mutationRotateCount;  // 旋转变异计数器
    bool stop;  // 终止标识
    int seed;  // 随机值种子
};

#endif // !GA_H
