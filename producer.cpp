#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>  
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

using namespace std;


void producerHandler (int sig);  
void consumerHandler (int sig);  


const int BUFFER_SIZE = 5;  
struct SharedMemory          
{
	int numItems;            
	int in, out;              
	int buffer[BUFFER_SIZE];  
};

int shmid;                 
SharedMemory *shmptr;      
pid_t child_pid;           

int main()
{
	
	shmid = shmget(IPC_PRIVATE, sizeof(struct SharedMemory), 0666|IPC_CREAT);   
	if (shmid == -1) {
		cerr << "shmget failed\n";
		exit(EXIT_FAILURE);
	}

	
	child_pid = fork();
	if (child_pid < 0) { 
		cerr << "Fork Failed\n";
		exit(-1);
	}

	if (child_pid == 0) 
	{ 
		signal (SIGINT, consumerHandler);  
		cout << "I am the consumer\n"
			<< "Consumer will sleep for a bit to make sure that producer "
			<< "initializes shared memory" << endl;
		sleep(1);

		
		cout << "Consumer: attaching shared memory\n";
		shmptr = (SharedMemory*)shmat(shmid, NULL, 0);
		if (shmptr == (SharedMemory *)-1) {
			cerr << "shmat failed\n";
			exit(EXIT_FAILURE);
		}
		cout << "Consumer: memory attached\n";

		
		while (true)
		{
			cout << "Consumer attempting to read\n";
			
			while (shmptr->numItems == 0)
				;  
			
			shmptr->numItems--; 

			int nextConsumed = shmptr->buffer[shmptr->out];

			cout << "Consumer read: " << nextConsumed	<< " at index " << shmptr->out << endl;

			shmptr->out = (shmptr->out + 1) % BUFFER_SIZE;

			sleep(rand()%3);  // 0-3 seconds

		}  

	}  
	else 
	{ 
		signal (SIGINT, producerHandler);  
		cout << "I am the producer\n";

		
		cout << "Producer: attaching shared memory\n";
		shmptr = (SharedMemory *)shmat(shmid,NULL,0);
		if (shmptr == (SharedMemory *)-1) {
			cerr << "shmat failed\n";
			exit(EXIT_FAILURE);
		}
		cout << "Producer: memory attached\n";

		
		cout << "Producer: initializing shared memory\n";
		shmptr->in = 0;
		shmptr->out = 0;
		shmptr->numItems = 0;
		cout << "Producer: shared memory initialized\n";

		
		while (true)
		{
			cout << "Producer attempting write\n";

			int nextProduced = rand()%10000; 

			while (shmptr->numItems == BUFFER_SIZE)
				;  // do nothing

			shmptr->buffer[shmptr->in] = nextProduced;

			cout << "Producer wrote: " << nextProduced 	<< " at index " << shmptr->in << endl;

			shmptr->in = (shmptr->in + 1) % BUFFER_SIZE;

			shmptr->numItems++;  // CRITICAL section

			sleep(rand()%3);  // 0-3 seconds

		} 
	}  
}  


void consumerHandler (int sig)
{
	
	cout << "Consumer detaching shared memory\n";
	if (shmdt(shmptr) == -1) {
		cerr << "shmdt failed\n";
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}  


void producerHandler (int sig)
{
	
	cout << "Producer killing consumer\n";
	kill(child_pid, SIGINT);
	wait(NULL);

	cout << "Producer detaching and removing shared memory\n";
	
	if (shmdt(shmptr) == -1) {
		cerr << "shmdt failed\n";
		exit(EXIT_FAILURE);
	}
	
	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		cerr << "shmctl(IPC_RMID) failed\n";
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}  


