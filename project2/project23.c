#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
int n;
int* list;
int max, min, range, sum, mode;
double artMean, harMean, standartDev, median, interRange;
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
void* findMin(){
    min = list[0];
}

void* findMax(){
    max = list[n-1];
}
void* findRange(){
    range = list[n-1]- list[0];
}
void* findMedian(){
    if(n % 2 == 0){
        median = (list[n/2]+list[n/2-1])/(double)2;
    }else{
        median = (double)list[n/2];
    }
}
void* findSum(){
    for(int i = 0; i < n; i++)
    {    
        int temp = list[i];
        sum +=temp;
    }
}
void* findArithmeticMean(){
    int sum = 0;
    for(int i = 0; i < n; i++)
    {
        sum += list[i];
        }
    artMean = sum/(double)n;
}
void* findHarmonicMean(){
    double sum ;
    for(int i = 0; i < n; i++)
    {    
        int temp = list[i];
        sum += 1.0/(double)temp;
    }
    harMean = n/sum;
}
void* findMode(){
    mode = list[0];
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
}
void* findStandartDeviation(){
    double sum = 0.0, mean, standardDeviation = 0.0;
    int i;
    for(i=0; i<n; ++i)
    {
        sum += list[i];
    }
    mean = sum/n;
    for(i=0; i<n; ++i)
        standardDeviation += pow(list[i] - mean, 2);
    standartDev = sqrt(standardDeviation/(n-1));
}
void* findInterRange(){
    double q1, q3;
    if(n % 2 == 0){
        q1 = (list[n/4]+list[n/4-1])/(double)2;
        q3 = (list[n/4*3]+list[n/4*3-1])/(double)2;
    }else{
        q1 = (double)list[n/4];
        q3 = (double)list[n/4*3];
    }
    interRange = q3-q1;
}
void* firstThread(){
    findMin();
    findStandartDeviation();
    
}
void* secondThread(){
    findRange();
    findSum();
}
void* thirdThread(){
    findMedian();
    findArithmeticMean();
}
void* fourthThread(){
    findHarmonicMean();
    findMode();
}
void* fifthThread(){
    findMax();
    findInterRange();
}

int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("output3.txt", "w");
    n = atoi(argv[1]);
    int temp;
    list =(int*)calloc(n, sizeof(int));
    srand(time(0));
    for (int i = 0; i < n; i++)
    {    
        temp = rand()%9001 + 1000;
        list[i] = temp;        
    }
    quicksort(0, n-1);
    clock_t start = clock();
    pthread_t tid[5];
    pthread_create(&tid[0], NULL, firstThread, NULL);
    pthread_create(&tid[1], NULL, secondThread, NULL);
    pthread_create(&tid[2], NULL, thirdThread, NULL);
    pthread_create(&tid[3], NULL, fourthThread, NULL);
    pthread_create(&tid[4], NULL, fifthThread, NULL);
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);
    clock_t end = clock();
    fprintf(fp,"%d\n%d\n%d\n%d\n%.5f\n%d\n%.5f\n%.5f\n%.5f\n%.5f\n%.5f",  
              min, max, range, mode,median, sum, artMean, harMean,standartDev,interRange, (double)(end-start)/CLOCKS_PER_SEC);
    fclose(fp);
   return 0;
}