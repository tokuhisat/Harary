#include <stdio.h>
#include <time.h>

int main(void)
{
    time_t start_time, end_time;
    long int max;
    int a,b,i,j,T,number,until,count,div; // 
    
    max = 10000000000000000;
    number = 1000000000;
    div = 0;
    
    long int collatz[number];
    
    printf("何倍して何を足すの？\n");
    scanf("%d",&a);
    printf("倍して\n");
    scanf("%d",&b);
    printf("を足す！\n");
    printf("どこまで計算するの？\n");
    scanf("%d",&until);
    printf("まで計算する！\n");
    
    start_time = time(NULL);
    
    for(i=1;i<until;i+=2){
        collatz[0]=i;
        
        for(count=1;count<number;count+=1){
            if(collatz[count-1]%2==0){
                collatz[count]=collatz[count-1]/2;
            }
            else{
                collatz[count]=(collatz[count-1]*a+b)/2;
            }
            
            if(collatz[count]>max){
                div = div+1;
                printf("%d は発散疑い \n",i);
                break;
            }
                
            if(collatz[count]==collatz[0]){
                printf("%d は巡回 \n",i);
                break;
            }

            if(collatz[count]==1){
                break;
            }
                
            T=0;
            for(j=0;j<count;j+=1){
                if(collatz[j]==collatz[count]){
                    T=1;
                    break;
                }
            }
            
            if(T==1){
                break;
            }

          
        }
    }
    
    if(div!=0){
        printf("発散数は%d個\n",div);
    }

    end_time = time(NULL);
    printf("計算時間は%ld\n",end_time - start_time);
    
    return 0;
}
