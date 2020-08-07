#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fnmatch.h>
#include <pthread.h> 
#include <signal.h>

int counter = 0;
/*----------------Error Handling------------------*/
void arguments_error()
{
	fprintf(stderr,"invalid number of arguments for Parallel File Find\n");
	exit(1);
}

void directory_search_error()
{
	fprintf(stderr, "invalid search directory\n");
	exit(1);
}

void bad_alloc_error()
{
	fprintf(stderr, "error allocating memory\n");
	exit(1);
}

void mutex_error()
{
	fprintf(stderr, "mutex init failed\n");
    	exit(1);
}

void cond_error()
{
	fprintf(stderr, "cond init failed\n");
    	exit(1);
}
/*------------------------------------------------*/


/*---------------------Queue----------------------*/
typedef struct QNode
{ 
    char key[100]; 
    struct QNode *next;
 
}QNode;


typedef struct Queue
{ 
   QNode *front, *rear;

}Queue; 

QNode* newNode(char *k) 
{ 
	QNode* temp = (QNode*)malloc(sizeof(QNode));
	if(!temp)
		bad_alloc_error();
		 
    	strcpy(temp->key, k); 
    	temp->next = NULL;

    	return temp; 
} 
   
Queue* createQueue() 
{ 
    	Queue* q = (Queue*)malloc(sizeof(Queue)); 
    	if(!q)
    		bad_alloc_error();
    		
    	q->front = q->rear = NULL;
 
    	return q; 
} 
  
void enQueue(Queue* q, char k[100]) 
{ 
    	QNode *temp = newNode(k); 
  
    	if (q->rear == NULL)
	{ 
        	q->front = q->rear = temp; 
        	return; 
    	} 
  
    	q->rear->next = temp; 
    	q->rear = temp; 
} 

void deQueue(Queue *q) 
{ 
	if (q->front == NULL) 
		return; 
	  
	QNode *temp = q->front; 
	  
	q->front = q->front->next; 
	  
	if (q->front == NULL) 
		q->rear = NULL; 

	free(temp);
}
/*------------------------------------------------*/


/*------------------Declarations------------------*/
Queue *myQueue;
pthread_mutex_t mutex1, mutex2;
pthread_cond_t condition;
pthread_t *thread_id;
int numThreads;
struct sigaction s;
/*------------------------------------------------*/



/*----------------Signal Handling-----------------*/
void handler(int s)
{
	int i;
	while(myQueue->front != NULL)
	{
		Queue *tmp = (Queue *)myQueue->front;
		myQueue->front = myQueue->front->next;
		free(tmp);
		
	}
	for(i = 0; i < numThreads; i++)
	{
		pthread_cancel(thread_id[i]);
	}
	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_cond_destroy(&condition);
	free(thread_id);
	printf("Search stopped, found %d files\n", counter);

	exit(0);

}

/*------------------------------------------------*/



/*---------------Search Functions-----------------*/
int name_corresponds(const char *path, const char *str)
{
	return fnmatch(str, strrchr(path,'/') + 1 ,0) == 0;
}

void treat_file(const char* path, const char *exp)
{
	if (name_corresponds(path, exp))
	{
		counter++;
		printf("%s\n", path);
	}		
}

void* browse(void *exp)
{

	char *expression = (char*)exp;

	while(1)
	{	
		pthread_mutex_lock( &mutex1 );
		numThreads--;
		while(myQueue->front == NULL)
		{
			if(numThreads == 0)
			{
				pthread_mutex_unlock( &mutex1 );
				pthread_cond_signal(&condition);
				pthread_exit(NULL);
			}
			pthread_testcancel();
			pthread_cond_wait(&condition, &mutex1);
		}
		numThreads++;

		char path[strlen(myQueue->front->key)];
		strcpy(path, myQueue->front->key);
		deQueue(myQueue);

		pthread_mutex_unlock( &mutex1 );

		DIR *dir = opendir(path);
		struct dirent *entry;
		struct stat dir_stat;

		if (! dir)
		{ 
			fprintf(stderr, "opending dir failed");
			pthread_exit(NULL);
		}
		
		pthread_mutex_lock( &mutex1 );
		while ((entry = readdir(dir)) != NULL)
		{	
			pthread_mutex_unlock( &mutex1 );
			char buff[strlen(path)+ strlen(entry->d_name)+ 2];
			sprintf(buff,"%s/%s", path, entry->d_name);
			int s = stat(buff, &dir_stat);
			if(s == -1)
			{
				fprintf(stderr, "Unable to stat");
				pthread_exit(NULL);
			}

			if (strcmp(entry->d_name,"..") != 0)
			{
				if (((dir_stat.st_mode & __S_IFMT) == __S_IFDIR)  && strcmp(entry->d_name, ".") != 0)
				{
					pthread_mutex_lock( &mutex2 );
					
					pthread_cond_signal(&condition);
					
				    	if (myQueue->rear == NULL)
					{ 
						myQueue->front = malloc(sizeof(QNode));
						if(!myQueue->front)
						{
							fprintf(stderr, "error allocating memory\n");
							pthread_exit(NULL);
						}
						strcpy(myQueue->front->key, buff);
						myQueue->front->next = NULL;
						myQueue->rear = myQueue->front;
				    	} 
				  	else
					{
						myQueue->rear->next = malloc(sizeof(QNode));
						if(!myQueue->rear->next)
						{
							fprintf(stderr, "error allocating memory\n");
							pthread_exit(NULL);
						}
						myQueue->rear = myQueue->rear->next;
						strcpy(myQueue->rear->key, buff);
						myQueue->rear->next = NULL;
						
					}

					pthread_mutex_unlock( &mutex2 );
				}
				else
				{
					treat_file(buff, expression);
				}
				
			}
			pthread_mutex_lock( &mutex1 );
			
		 }
		pthread_mutex_unlock( &mutex1 );
		closedir(dir);
	}
}
/*------------------------------------------------*/


int main(int argc, char **argv)
{ 	
	s.sa_handler = handler;
	sigaction(SIGINT, &s, NULL);
	
	char *exp;
	int i, j, error_;
	struct stat info;

	//invalid number of arguments were sent
	if (argc < 4) 
	{	
		arguments_error();
	}
	
	//validating argv[1] is a searchable directory

	if (stat( argv[1], &info ) != 0 ) //cannot access directory
	{
		directory_search_error();
	}
	if ( info.st_mode & (__S_IFDIR == 0)) //is not a directory
	{
	    	directory_search_error();
	}

	numThreads = atoi(argv[3]);
	
	//initializing queue
	myQueue = createQueue();
	enQueue(myQueue, argv[1]);

	exp = malloc(strlen(argv[2]) + 3);
	if(!exp)
		bad_alloc_error();
		
	exp[0] = '*';
	strcat(exp, argv[2]);
	strcat(exp, "*");
	
	thread_id = malloc(sizeof(pthread_t) * (*argv[3]));
	if(!thread_id)
		bad_alloc_error();
		
	int m1 = pthread_mutex_init(&mutex1, NULL);
	int m2 = pthread_mutex_init(&mutex2, NULL);
	if(m1 != 0 || m2 != 0)
		mutex_error();
		
	int c = pthread_cond_init(&condition, NULL);
	if(c != 0)
		cond_error();
		
	//creating threads
	for (i = 0; i < numThreads; i++)
	{
		error_ = pthread_create(&thread_id[i], NULL, browse, exp);

		if (error_ != 0) 
            	{
            		fprintf(stderr, "\nThread can't be created\n");
            		exit(1);
            	}
	}

	for (j = 0; j < numThreads; j++)
	{
		pthread_join(thread_id[j], NULL);
	}

	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_cond_destroy(&condition);
	free(thread_id);
	printf("Done searching, found %d files\n", counter);

	return 0;
}
