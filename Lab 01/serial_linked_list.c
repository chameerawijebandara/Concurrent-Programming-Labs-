/* File:     serial_linked_list .c
 * Purpose:  Implement a linked list as a Serial program
 * 			 Implementation should support Member( ), Insert( ), and Delete( ) functions. 
 *			 Populate the linked list with n random, but unique values. 
 * 			 Each value should be between 0 and 2^16 - 1.
 * 			 Then perform m random Member, Insert, and Delete operations on the link list.
 * 			 Let mMember, mInsert, and mDelete be the fractions of operations of each type.
 * 			 You may use any values within 0 and 2^16 – 1 while performing these three operations.
 * 			 However, to simplify the implementation, a new value inserted into the list cannot be a 
 * 			 value already in the list (it may be a value that was initially added to the list, but later removed).
 *
 * Compile:  gcc -g -Wall -o serial_linked_list serial_linked_list.c -pthread
 *           
 * Run:      ./serial_linked_list <n> <m> <mMember> <mInsert> <mDelete> 
 *           n is the number of initial unique values in the Link List.
 *           m is number of random Member, Insert, and Delete operations on the link list.
 *			 mMember is the fractions of operations of Member operation.	
 *			 mInsert is the fractions of operations of Insert operation.	
 *			 mDelete is the fractions of operations of Delete operation.	
 *
 * Input:    none
 * 
 * Output:   This version prints the elapsed time required for
 *           single-threaded calculations.
 *
 * Notes:
 *      	CS4532 Concurrent Programming 
 *			Take Home Lab 1 
 *			Due – Jun 28 before 11:55 PM  
 *
 * Date:	07/25/2014
 *
 * Author : Wijebandara WMNC
 * 			100598M
 *			Computer Science and Engineering
 *
 * Email : chameerawijebandara@gmail.com 
 * 				
 */        

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

struct list_node_s
{
	int data;
	struct list_node_s* next;
};


int n;
int m;
float mMember;
float mInsert;
float mDelete;


int member( int value, struct  list_node_s* head_p );
int insert(int value, struct list_node_s** head_pp);
int delete (int value, struct list_node_s** head_pp);
int printList( struct  list_node_s* head_p ); 


/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

/* Main function */
int main(int argc, char* argv[])
{
	struct list_node_s* head = NULL;	
	int i=0;
	double start, finish, elapsed;
	
	/* read command line arguments */
	Get_args(argc, argv); 
	 
	/* initially populating the link list */ 
	for(;i<n;i++)
	{	
		int r = rand()%65536;
		if(!insert(r,&head))
		{
			i--;
		}
	}
	
	int count_member=0;
	int count_insert=0;
	int count_delete=0;
	//printf("%f\n",mMember);
	//printf("%f\n",mInsert);
		
	start = clock();	
	for(i=0;i<m;i++)
	{
	
		float prob = (rand()%100/100.0);
		//printf("%f\n",prob);
		
		
		int r = rand()%65536;
		if(prob<mMember)
		{
			member(r,head);
			count_member++;
		}
		else if(prob < mMember + mInsert )
		{
			insert(r,&head);
			count_insert++;
		}
		else
		{			
			delete(r,&head);
			count_delete++;
		}
		
	}
	finish = clock();
   	elapsed = (finish - start)/CLOCKS_PER_SEC;
   	printf("Elapsed time = %e seconds\n", elapsed);
	printf("Member operation count = %d\n",count_member);
	printf("Insert operation count = %d\n",count_insert);
	printf("Delete operation count = %d\n",count_delete);
	//printList(head);
	return 0;
}/*main*/	



/*------------------------------------------------------------------
 * Function:       member
 * Purpose:        Check if the given values is in the link list
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:   
 * Return val: Return 1 if value exist otherwise 0
 */
int member( int value, struct  list_node_s* head_p )
{
	struct list_node_s* curr_p = head_p;
	
	while( curr_p != NULL && curr_p->data < value )
	{
		curr_p = curr_p->next;
	}

	if(curr_p == NULL || curr_p->data > value)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}/* member */

/*------------------------------------------------------------------
 * Function:       insert
 * Purpose:        Add new values in to link list
 * In arg:         value, head_p
 * Globals in:  
 * Global in/out:  
 * Return val: Return 1 if value successfully add to the list otherwise 0
 */
int insert(int value, struct list_node_s** head_pp)
{
	struct list_node_s* curr_p = *head_pp;			
	struct list_node_s* pred_p = NULL;
	struct list_node_s* temp_p = NULL;

	while(curr_p !=NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->next;
	}
	
	if(curr_p == NULL || curr_p->data > value)
	{
		temp_p = malloc(sizeof(struct list_node_s));		
		temp_p->data = value;
		temp_p->next = curr_p;
		
		if(pred_p == NULL)
		{
			*head_pp = temp_p;
		}
		else
		{
			pred_p->next = temp_p;
		}
		return 1;
 
	}
	else
	{
		return 0;
	}
}	/*insert*/


/*------------------------------------------------------------------
 * Function:       delete
 * Purpose:        remove values from the link list 
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:  
 * Return val: Return 1 if value successfully remove from the list otherwise 0
 */
int delete (int value, struct list_node_s** head_pp)
{
	struct list_node_s* curr_p = *head_pp;
	struct list_node_s* pred_p = NULL;
	
	while(curr_p != NULL && curr_p->data < value)
	{
		pred_p = curr_p;
		curr_p = curr_p->next;
	}	
	
	if(curr_p != NULL && curr_p -> data < value)
	{
		if(pred_p == NULL){
			*head_pp = curr_p->next;
			free(curr_p);
		}
		else
		{
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
		
	}
	else
	{
		return 0;
	}

}	/*delete*/


/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: n, m, mMember, mInsert, mDelete
 */
void Get_args(int argc, char* argv[]) {
	if (argc != 6)
   	{
		Usage(argv[0]);
	}
   	n = (int) strtol(argv[1], (char **)NULL, 10);
   	m = (int) strtol(argv[2], (char **)NULL, 10);
   	
   	mMember = (float) atof(argv[3]);
   	mInsert = (float) atof(argv[4]);
	mDelete = (float) atof(argv[5]);
	
   if (n <= 0 || m <= 0 || mMember + mInsert + mDelete!=1.0) Usage(argv[0]);
   
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <n> <m> <mMember> <mInsert> <mDelete>\n", prog_name);
   fprintf(stderr,"n is the number of initial unique values in the Link List.");
   fprintf(stderr,"m is number of random Member, Insert, and Delete operations on the link list.");
   fprintf(stderr,"mMember is the fractions of operations of Member operation.");
   fprintf(stderr,"mInsert is the fractions of operations of Insert operation.");
   fprintf(stderr,"mDelete is the fractions of operations of Delete operation.");
   			 
   exit(0);
}  /* Usage */

/*------------------------------------------------------------------
 * Function:       printList
 * Purpose:        Add in the terms computed by the thread running this 
 * In arg:         value, head_p
 * Globals in:     
 * Global in/out:   
 * Return val: Estimate of pi using n terms of Maclaurin series
 */
int printList( struct  list_node_s* head_p ) 
{
	struct list_node_s* curr_p = head_p;
	
	while(curr_p != NULL)
	{
		printf("%d ",curr_p->data);
		curr_p = curr_p->next;
	}
	printf("\n");
}	/*printList */


