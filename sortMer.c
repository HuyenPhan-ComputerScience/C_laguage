#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define SIZE 11
#define NUMBER_OF_THREADS 7
void *sorter(void *params);
void *merger(void *params);
int list[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8, 20};
int result[SIZE];
typedef struct
{
   int from_index;
   int to_index;
} parameters;
typedef struct

{
   int from_index;
   int to_index;
   int size;

} mergeparam;

int main(int argc, const char *argv[])
{
   //divide the list into four
   int i;
   int start1 = 0;
   int end1 = (SIZE / 4) - 1;

   int start2 = (SIZE / 4);
   int end2 = (SIZE / 2) - 1;

   int start3 = (SIZE / 2);
   int end3 = (3 * SIZE / 4) - 1;

   int start4 = (3 * SIZE / 4);
   int end4 = SIZE - 1;

   pthread_t workers[NUMBER_OF_THREADS];

   // fist sorting thread
   parameters *data = (parameters *)malloc(sizeof(parameters));

   data->from_index = start1;

   data->to_index = end1;

   pthread_create(&workers[0], 0, sorter, data);
   // second sorting thread
   data = (parameters *)malloc(sizeof(parameters));

   data->from_index = start2;

   data->to_index = end2;

   pthread_create(&workers[1], 0, sorter, data);
   // third thread
   data = (parameters *)malloc(sizeof(parameters));

   data->from_index = start3;

   data->to_index = end3;

   pthread_create(&workers[2], 0, sorter, data);
   // forth sorting thread
   data = (parameters *)malloc(sizeof(parameters));

   data->from_index = start4;

   data->to_index = end4;

   pthread_create(&workers[3], 0, sorter, data);

   // wait for sorting finish
   for (int i = 0; i < NUMBER_OF_THREADS - 1; i++)

      pthread_join(workers[i], NULL);

   // merge thread
   mergeparam *datam;
   datam = (mergeparam *)malloc(sizeof(mergeparam));
   datam->from_index = 0;
   datam->to_index = (SIZE / 4);
   datam->size = SIZE / 2;
   pthread_create(&workers[4], 0, merger, datam);

   datam = (mergeparam *)malloc(sizeof(mergeparam));
   datam->from_index = start3;
   datam->to_index = start4;
   datam->size = SIZE;
   pthread_create(&workers[5], 0, merger, datam);

   // wait merge finish
   pthread_join(workers[4], NULL);
   pthread_join(workers[5], NULL);
   //copy result to list
    for(i= 0; i < SIZE; i++) {
      list[i] = result[i];
   }

   datam = (mergeparam *)malloc(sizeof(mergeparam));
   datam->from_index = start1;
   datam->to_index = start3;
   datam->size = SIZE;
   //merging
   pthread_create(&workers[6], 0, merger, datam);
   //wait merge finish
   pthread_join(workers[6], NULL);
   //print out the final sorting list
   for(i=0;i<SIZE;i++){
      printf("%d\n",result[i]);
   }

   return 0;
}
//sorting method
void *sorter(void *params)

{

   mergeparam *p = (mergeparam *)params;

   int begin = p->from_index;

   int end = p->to_index;

   int swapped = 1;

   int j = 0;

   int i;

   int temp;

   while (swapped == 1)
   {

      swapped = 0;

      j++;

      for (i = begin; i <= end - j; i++)
      {

         if (list[i] > list[i + 1])
         {

            temp = list[i];

            list[i] = list[i + 1];

            list[i + 1] = temp;

            swapped = 1;
         }
      }
   }

   pthread_exit(0);
}


void *merger(void *params)
{
   mergeparam *p = (mergeparam *)params;
   int i, j, size;
   i = p->from_index;
   j = p->to_index;
   size = p->size;
   int position = p->from_index; /* position being
   inserted into result list */
   while (i < p->to_index && j < size)
   {
      if (list[i] <= list[j])
      {
         result[position++] = list[i];
         i++;
      }
      else
      {
         result[position++] = list[j];
         j++;
      }
   }

   /* copy the remainder */
   if (i < p->to_index)
   {
      while (i < p->to_index)
      {
         result[position] = list[i];
         position++;
         i++;
      }
   }
   else
   {
      while (j < size)
      {
         result[position] = list[j];
         position++;
         j++;
      }
   }
   pthread_exit(0);
}