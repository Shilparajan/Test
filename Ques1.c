#include <stdio.h>
main()
{

    int small, large,i;
    FILE *myFile;
    myFile = fopen("somenumbers.txt", "r");


    int numberArray[200];


    for (i = 0; i < 200; i++)
    {
        fscanf(myFile, "%d", &numberArray[i]);
    }

fclose(myFile);

large=small=numberArray[0];


    for(i=1;i<200;++i)
    {

   if(numberArray[i]>large)

   large=numberArray[i];

   if(numberArray[i]<small)

   small=numberArray[i];

    }

    printf("The largest element is %d",large);
    printf("\nThe smallest element is %d",small);
int j=0 ,mul3[200],mul7[200];
for(i=0;i<200;i++)
{
 if(numberArray[i]%3==0)
   {
     mul3[j]=numberArray[i];
     j++;
   }
 if(numberArray[i]%7==0)
   {
     mul7[j]=numberArray[i];
     j++;
   }
}

    printf("multiples of 3 is:");
for(i=0;i<j;i++)
  {
    printf("%d ",mul3[i]);
  }
   printf("multiples of 7 is:");
for(i=0;i<j;i++)
  {
    printf("%d ",mul7[i]);
  }


int temp;
    for(i=0; i<200; i++)
      {
        for(j=i+1; j<200; j++)
          {
             if(numberArray[i] >numberArray[j])
              {
                temp     = numberArray[i];
                numberArray[i] =numberArray[j];
                numberArray[j] = temp;
            }
        }
    }

    printf("\nElements of array in sorted ascending order: ");
 for(i=0; i<200; i++)
    {
        printf("%d", numberArray[i]);
    }

 for(i=0; i<200; i++)
      {
        for(j=i+1; j<200; j++)
          {
             if(numberArray[i] < numberArray[j])
              {
                temp     = numberArray[i];
                numberArray[i] =numberArray[j];
                numberArray[j] = temp;
            }
        }
    }

    printf("\nElements of array in sorted ascending order: ");
    for(i=0; i<200; i++)
    {
        printf("%d", numberArray[i]);
    }


    return 0;
}
