#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 9
#define ARRAY_SIZE 10

int array[10] = {6,3,2,1,23,4,7,7,4,1};
pthread_t threads[NUM_THREADS];
pthread_t checker;
pthread_mutex_t mutexes[ARRAY_SIZE];
int sorted;


int arraycheck()
{
	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		if (array[i-1] > array[i])
		{
			return 0;
		}
	}

	return 1;
}

void *check()
{
	while(1)
	{
		if(arraycheck())
		{
			sorted = 1;
			return 0;
		}
	}
}

void *sort(void *arg)
{
	int id;
	id = (int)arg;

	while(1)
	{
		if (sorted == 1)
		{
			pthread_exit(NULL);
		}else{

			if(pthread_mutex_trylock(&mutexes[id]) && pthread_mutex_trylock(&mutexes[id+1]))
			{
				if (array[id]>array[id+1])
				{
					array[id] += array[id+1];
					array[id+1] = array[id]-array[id+1];
					array[id] -= array[id+1];
				}

				pthread_mutex_unlock(&mutexes[id]);
				pthread_mutex_unlock(&mutexes[id]);
			}
		}
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	sorted = 0;
	printf("BEFORE\n");
	for(int i = 0; i < ARRAY_SIZE; i++) 
	{
        printf("%d ", array[i]);
    }
    printf("\n");
    printf("----------------------\n");

	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		if(pthread_mutex_init(&mutexes[i], NULL) != 0)
		{
			return 1;
		}
	}

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		if (pthread_create(&threads[i], NULL, &sort, (void *)i) != 0)
		{
			printf("thread %d failed to initialize\n", i);
			return 1;
		}
	}

	pthread_create(&checker,NULL,&check,NULL);

	for (int i = 0; i < NUM_THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	pthread_join(checker, NULL);

	for (int i = 0; i < ARRAY_SIZE; ++i)
	{
		pthread_mutex_destroy(&mutexes[i]);
	}

	printf("AFTER\n");

	for(int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
	
}



