#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
int n;
int* list;

void quicksort(int first,int last){
int i, j, pivot, temp;
if(first<last){
    pivot=first;
    i=first;
    j=last;
    while(i<j){
        while(list[i]<=list[pivot]&&i<last) i++;
        while(list[j]>list[pivot]) j--;
        if(i<j){
            temp=list[i];
            list[i]=list[j];
            list[j]=temp;
            }
        }
    temp=list[pivot];
    list[pivot]=list[j];
    list[j]=temp;
    quicksort(first,j-1);
    quicksort(j+1,last);
    }
}
int findMin(){
    return list[0];
}
int findMax(){
    return list[n-1];
}
int findRange(){
    return list[n-1]- list[0];
}
double findMedian(){
    if(n % 2 == 0){
        return (list[n/2]+list[n/2-1])/(double)2;
    }else{
        return (double)list[n/2];
    }
}
int findSum(){
    int sum = 0;
    for(int i = 0; i < n; i++)
    {    
        int temp = list[i];
        sum +=temp;
    }
    return sum;
}
double findArithmeticMean(){
    int sum = 0;
    for(int i = 0; i < n; i++)
    {
        sum += list[i];
        }
    return  sum/(double)n;
}
double findHarmonicMean(){
    double sum;
    for(int i = 0; i < n; i++)
    {    
        int temp = list[i];
        sum += 1.0/(double)temp;
    }
    return n/sum;
}
int findMode(){
    int mode = list[0];
    int maxCount = 1;
    for(int i = 0; i < n; i++)
    {
        int count = 0;
        for(int j = i; j < n; j++)
        {
            if(list[j] == list[i])
            {
                count++;
            }else break;
        }
        if(count > maxCount)
        {
            maxCount = count;
            mode = list[i];
        }
    }
    return mode;
}
double findStandartDeviation() {
    double standartDev,mean,sum;
    for (int i = 0; i < n; ++i) {
        sum += list[i];
    }
    mean = sum/n;
    for (int i = 0; i < n; ++i) {
        standartDev += pow(list[i] - mean, 2);
    }
    return sqrt(standartDev / (double)(n-1));
}
double findInterRange(){
    int q1, q3;
    if(n % 2 == 0){
        q1 = (list[n/4]+list[n/4-1])/(double)2;
        q3 = (list[n/4*3]+list[n/4*3-1])/(double)2;
    }else{
        q1 = (double)list[n/4];
        q3 = (double)list[n/4*3];
    }
    return q3-q1;
}

int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("output1.txt", "w");
    n = atoi(argv[1]);
    list =(int*)calloc(n, sizeof(int));
    int temp, max, min, range, sum, mode;
    double artMean, harMean, standartDev, median, interRange;
    srand(time(0));
    for (int i = 0; i < n; i++)
    {    
        temp = rand()%9001 + 1000;
        list[i] = temp;    
    }
    quicksort(0,n-1);
    clock_t start = clock();
    max = findMax();
    min = findMin();
    range = findRange();
    mode = findMode();
    median = findMedian();
    sum = findSum();
    artMean = findArithmeticMean();
    harMean = findHarmonicMean();
    standartDev = findStandartDeviation();
    interRange = findInterRange();
    clock_t end = clock();
    fprintf(fp,"%d\n%d\n%d\n%d\n%.5f\n%d\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f",  
              min, max, range, mode,median, sum, artMean, harMean,standartDev,interRange, (double)(end-start)/CLOCKS_PER_SEC);
    fclose(fp);
    return 0;
}