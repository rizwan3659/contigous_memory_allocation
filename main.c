
#include "main.h" 


struct Node *head;           //dummy node keeps track of avaliable main memory
struct Node *block_of_space; //initial block of main memory
struct Node *temp;           // Pointer used to traverse throught contingous memory

char request[3];
char process[3];
char algo_type[2];
int last_address_space;
int space_requested;

struct Node
{
    int available_space; //space in that section / fragment
    int start_address;
    int end_address;
    struct Node *next;
    char process_id[MAX_ID_NUMBER];
};

// Allocate the first hole that is big enough.
void first_fit(char process_id[3], int space_requested)
{
    temp = head;
    int left_over_space = 0; //helps calculate if there was space left over

    while (temp->next != NULL)
    {
        //if the block is being unused and big enough place the process in that memory space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the unused node
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested -1  ;

            //if the space_requested is smaller then the space found.
            //make new node with leftover space, if left_over > 0
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space -1 ;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }
            printf("Successfully allocated %d bytes to process %s using first fit\n",space_requested, process_id);
            return; //halt the function, when space is found and inserted
        }
        else //else continue traversing linkedlist
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb\n", process_id, space_requested);
}

// Allocate the smallest hole that is big enough
void best_fit(char process_id[3], int space_requested)
{
    
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    int smallest_space = 999999; //tracks the smallest space
    int left_over_space = 0;
    temp = head;

    //this loop will find the smallest possible avaliable space
    while (temp->next != NULL)
    {

        //if space is unused and big enough to fit, track min space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //track min space
            if (temp->next->available_space <= smallest_space)
                smallest_space = temp->next->available_space;
              temp=temp->next;
        }
        else //keep traversing
            temp = temp->next;
    }

    temp = head;
    //this loop will place the new processor in the space
    while (temp->next != NULL)
    {
        //travserse till you find the smallest spot found
      //  if (temp->next->available_space == smallest_space)
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space == smallest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested-1;

            //if theres space left over, make a new node representing unused left over space
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space-1;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }
            printf("Successfully allocated %d bytes to process %s using best fit\n",space_requested, process_id);

            return; //halt the function, when space is found and inserted
        }
        else
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb\n", process_id, space_requested);
}

// Allocate the largest hole
void worst_fit(char process_id[3], int space_requested)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    int largest_space = -999999; //tracks the smallest space
    int left_over_space = 0;
    temp = head;

    //this loop will find the smallest possible avaliable space
    while (temp->next != NULL)
    {

        //if space is unused and big enough to fit, track min space
        if (strcmp(temp->next->process_id, "Unused") == 0 && temp->next->available_space >= space_requested)
        {
            //track max space
            if (temp->next->available_space >= largest_space)
                largest_space = temp->next->available_space;
            temp = temp->next;
        }
        else //keep traversing
            temp = temp->next;
    }

    temp = head;
    //this loop will place the new processor in the space
    while (temp->next != NULL)
    {
        //travserse till you find the largest spot found
        if (temp->next->available_space == largest_space)
        {
            //calculating avalible space left over
            head->available_space = head->available_space - space_requested;

            //writing over the space
            strcpy(temp->next->process_id, process_id);
            temp->next->end_address = temp->next->start_address + space_requested-1;

            //if theres space left over, make a new node representing unused left over space
            left_over_space = temp->next->available_space - space_requested;
            if (left_over_space > 0)
            {
                struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
                strcpy(newNode->process_id, "Unused");
                newNode->available_space = left_over_space;
                temp->next->available_space = space_requested;
                newNode->start_address = temp->next->end_address + 1; //starts at the end of prev node
                newNode->end_address = newNode->start_address + left_over_space-1;
                if (newNode->end_address > last_address_space) //if end_address is bigger than inital space
                    newNode->end_address = last_address_space;
                newNode->next = temp->next->next;
                temp->next->next = newNode;
            }
            printf("Successfully allocated %d bytes to process %s using worst fit\n",space_requested, process_id);

            return; //halt the function, when space is found and inserted
        }
        else
            temp = temp->next;
    }

    //if temp pointer reached the end of the LL without inserting & space is still avaliable
    printf("There is no space to place process %s, of %dkb \n", process_id, space_requested);
}

//this method handles all 3 algorithms for finding memeory spaces
void request_memory(char process_id[3], int space_requested, char algo[2])
{
    if (strcmp("W", algo) == 0)
        worst_fit(process_id, space_requested);
    else if (strcmp("B", algo) == 0)
     {
        best_fit(process_id, space_requested);
     }
    else if (strcmp("F", algo) == 0)
        first_fit(process_id, space_requested);
    else
    {
        printf("Choose between best_fit, worst_fit, and first_fit. Try again\n");
        return;
    }
}

void release_memory(char process_id[3])
{
    temp = head;

    while (temp->next != NULL)
    {

        if (temp->next->next != NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  == 0) && (strcmp(temp->next->next->process_id, "Unused") == 0 ) )
        {
          head->available_space = head->available_space + temp->next->available_space;
           temp-> end_address = temp->next->next->end_address;
           temp-> available_space = temp->next->next->end_address - temp->start_address+1;
           temp->next = temp->next->next->next;
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
      else   if(temp->next->next != NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  != 0) && (strcmp(temp->next->next->process_id, "Unused") == 0 ) )
        {
          head->available_space = head->available_space + temp->next->available_space;
           temp-> next->end_address = temp->next->next->end_address;
           temp-> next->available_space = temp->next->end_address - temp->next->start_address+1;
                strcpy(temp->next->process_id, "Unused");
           temp-> next->next = temp->next->next->next;
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
      else   if(temp->next->next != NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  != 0) && (strcmp(temp->next->next->process_id, "Unused") != 0 ) )
        {
          head->available_space = head->available_space + temp->next->available_space;
                strcpy(temp->next->process_id, "Unused");
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
      else   if(temp->next->next != NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  == 0) && (strcmp(temp->next->next->process_id, "Unused") != 0 ) )
        {
          head->available_space = head->available_space + temp->next->available_space;
           temp-> end_address = temp->next->end_address;
           temp-> available_space = temp->end_address - temp->start_address+1;
           temp-> next = temp->next->next;
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
      else   if(temp->next->next == NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  != 0)  )
        {
          head->available_space = head->available_space + temp->next->available_space;
                strcpy(temp->next->process_id, "Unused");
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
      else   if(temp->next->next == NULL && (strcmp(temp->next->process_id, process_id) == 0) && (strcmp(temp->process_id, "Unused")  == 0)  )
        {
          head->available_space = head->available_space + temp->next->available_space;
           temp-> end_address = temp->next->end_address;
           temp-> available_space = temp->end_address - temp->start_address+1;
           temp-> next = temp->next->next;
            printf("Releasing mempry for process  %s \n", process_id);
            printf("Successfully released memory for process %s \n", process_id);
        }
        else
         {
             printf("There is no process with process id %s \n",process_id);
          }
      temp=temp->next;
  }
}


void compact()
{
    temp = head;
    int a1;

    struct Node *p=NULL; 
    while (temp->next != NULL)
    {
        
        if (strcmp(temp->next->process_id, "Unused") == 0 )
        {
           p = temp->next;
            if ((p->next != NULL) && strcmp(p->next->process_id, "Unused") != 0 )
                   {
                        strcpy(p->process_id, p->next->process_id);
                        strcpy(p->next->process_id, "Unused");
                         a1 = p->next->available_space;
                         p->next->available_space= p->next->next->available_space + p->available_space;
                         p->available_space = a1;
                         p->next->end_address= p->next->start_address + p->next->available_space -1;
                        while(p->next->next != NULL && strcmp(p->next->next->process_id,"Unused")==0)
                             { p->next->next = NULL;}
                   }
        }
       temp = temp->next;
    }
}

void status_report()
{
    temp = head;
    printf("avaliable space left: %d\n", head->available_space);
    while (temp->next != NULL)
    {
        
        if (strcmp(temp->next->process_id, "Unused") == 0 )
             printf("Addresses [%d : %d]  %s\n", temp->next->start_address, temp->next->end_address, temp->next->process_id);
          else 
             printf("Addresses [%d : %d] Process %s\n", temp->next->start_address, temp->next->end_address, temp->next->process_id);
        temp = temp->next;
    }
}

int main(int argc, char *argv[])
{

   

    int inital_memory;

   printf("Memory allocation (byte):");;
  scanf("%d",&inital_memory );

    printf("Allocated %d bytes of memory \n", inital_memory);
    char user_input[128];

    //read the user if, if return value is negative, syscall error
    printf("allocator>");
    fflush(stdout);

    if (read(0, user_input, 128) < 0)
        write(2, "An error occurred in the read.\n", 31);

    //the head node is a dummy node
    head = (struct Node *)malloc(sizeof(struct Node));
    //block_of_space is the amount of space in memory
    block_of_space = (struct Node *)malloc(sizeof(struct Node));

    strcpy(head->process_id, "Dummy Node");
    head->start_address = -1;
    head->end_address = -1;
    head->available_space = inital_memory;
    head->next = block_of_space;

    strcpy(block_of_space->process_id, "Unused");
    block_of_space->start_address = 0;
    block_of_space->end_address = inital_memory -1 ;
    block_of_space->available_space = block_of_space->end_address - block_of_space->start_address + 1;
    block_of_space->next = NULL;

    last_address_space = inital_memory;

    while (user_input[0] != 'X')
    {
        if ((sscanf(user_input, "%s %s", request, process)) < 0)
            write(2, "An error occurred in the read.\n", 31);

        //TODO: find a way to lowercase the user input
        // char requested = tolower(request);
        // printf("%s\n", &requested);

        if (strcmp("RQ", request) == 0) //request new process command
        {

            sscanf(user_input, "%s %s %d %s", request, process, &space_requested, algo_type);
            request_memory(process, space_requested, algo_type);
        }
        else if (strcmp("RL", request) == 0) //release memory command
        {

            sscanf(user_input, "%s %s", request, process);
            release_memory(process);
        }
        else if (strcmp("C", request) == 0) //compact
        {
            compact();
        }
        else if (strcmp("STAT", request) == 0) //status of memory
        {
            status_report();
        }
        else //the command is not reconiged
        {
            printf("This command is not recognized, try again\n");
        }


        printf("allocator>");
        fflush(stdout);

        if (read(0, user_input, 128) < 0)
            write(2, "An error occurred in the read.\n", 31);
    }
  printf("Exiting Bye\n");
}

