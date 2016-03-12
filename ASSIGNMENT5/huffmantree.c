#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define SIZE 128



//Define struct node* to Link to improve readability

typedef struct node* Link;



struct node  //This is the node structure
{

    int priority;

    int child; //left/right child: 0 = left, 1 = right

    char c;

    Link parent;

    char code[SIZE]; //Huffman code

};



typedef struct   //Defines the priority queue structure
{

    int size;

    Link *array;

} queue;

queue q;



//QUEUE FUNCTIONS

void initialize();
int parent(int child);
int lchild(int parent);
int rchild(int parent);
int is_empty();
void swap(int x, int y);
void heapify(int ind);
void drop (int ind);
void insert (Link z);
Link dequeue();



int main(int argc, char * argv[])

{

    //initialize variables

    initialize(SIZE);

    int chars[SIZE], i;

    char c;

    Link huffman[SIZE];

    for (i=0; i<SIZE; i++) //Initialize array ‘chars’ to all 0s
    {

        chars[i] = 0;

    }



    //Step 1 – Read in while counting frequency. Also opens file.

    FILE * f;

    if((f = fopen (argv[1],"r")))
    {

    }
    else
    {

        printf("Sorry, file cannot be opened or is not present.");

        return 1;

    }

    do
    {

        c = fgetc(f);

        chars[(int)c]++;

    }
    while(c != EOF);

    fclose(f);



    //Step 2 – Creating Huffman tree nodes and inserting them into priority queue

    for (i = 0; i< SIZE; i++)
    {

        if (chars[i]!=0)
        {

            huffman[i] = malloc(sizeof(Link));

            huffman[i]->priority = chars[i];

            huffman[i]->c = i;

            huffman[i]->parent = NULL;

            insert(huffman[i]);

        }

    }



    //Step 3 – The Huffman Tree is created

    Link l, r;

    while(q.size>1)
    {

        l = dequeue();

        l->parent = malloc(sizeof(Link));

        r = dequeue();

        r->parent = l->parent;

        l->child = 0;

        r->child = 1;

        (r->parent)->priority = l->priority + r->priority;

        insert(r->parent);

    }



    //Step 4 – The root node is created

    Link root = dequeue();



    //Step 5 – Each node is assigned a code

    Link leaf;

    int j;

    for (j = 0; j<SIZE; j++)
    {

        if (chars[j]!=0)
        {

            leaf = huffman[j];

            char temp[128];

            huffman[j]->code[0] = '\0';

            temp[j] = '\0';

            while(leaf!=root)
            {

                strcpy(temp, huffman[j]->code);

                sprintf(huffman[j]->code, "%d", leaf->child);

                strcat(huffman[j]->code, temp);

                leaf = leaf->parent;

            }

            printf("%c: %s ", j, huffman[j]->code);



        }

    }



    //Step 6 – File is read again and the Huffman code for each character is printed

    printf("\n");

    f = fopen (argv[1],"r");

    do
    {

        c = fgetc(f);

        if (c==EOF) continue;

        printf("%s", huffman[(int)c]->code);



    }
    while(c != EOF);

    fclose(f);




    printf("\n");


    return 0;

}

void initialize()  //initialize priority queue
{

    int i;

    q.size = 0;

    q.array = malloc(sizeof(Link)*SIZE);

    for(i = 0; i<SIZE; i++)
    {

        q.array[i] = NULL;

    }

}

int parent(int child)  //This function finds the parent of a node
{

    if (child == 0) return -1;

    return (child+1)/2 -1;

}

int lchild(int parent)  //This function finds the left child of a node
{

    int child = (parent+1)*2-1;

    if (child >= q.size) return -1;

    return child;

}



int rchild(int parent)  //This function finds the right child of a node
{

    int child = (parent+1)*2;

    if (child >= q.size) return -1;

    return child;

}

int is_empty()  //Checks if priority queue is empty, returns 1 for true and 0 for false
{

    if (q.array[0] == NULL)

        return 1;

    return 0;

}



void swap(int x, int y)  //swap two nodes in a priority queue
{

    Link temp = q.array[x];

    q.array[x] = q.array[y];

    q.array[y] = temp;

}



void heapify(int ind)  //floats newly inserted node up to correct position
{

    if (parent(ind) == -1) return;

    if (q.array[parent(ind)]->priority>q.array[ind]->priority)
    {

        swap(ind, parent(ind));

        heapify(parent(ind));

    }

    return;

}

void drop(int ind) //drops new root node to correct position
{

    if(rchild(ind)==-1&&lchild(ind)==-1) return;

    else if(rchild(ind) == -1)
    {

        if(q.array[lchild(ind)]->priority<q.array[ind]->priority)

            swap(lchild(ind), ind);

        return;

    }

    else if(lchild(ind) == -1)
    {

        if(q.array[rchild(ind)]->priority<q.array[ind]->priority)

            swap(rchild(ind), ind);

        return;

    }

    else
    {

        if(q.array[ind]->priority>q.array[rchild(ind)]->priority&&q.array[ind]->priority>q.array[lchild(ind)]->priority)
        {

            if(q.array[rchild(ind)]->priority<q.array[lchild(ind)]->priority)
            {

                swap(ind, rchild(ind));

                drop(rchild(ind));

            }

            else
            {

                swap(ind, lchild(ind));

                drop(lchild(ind));

            }

        }
        else if(q.array[ind]->priority>q.array[rchild(ind)]->priority)
        {

            swap(ind, rchild(ind));

            drop(rchild(ind));

        }
        else
        {

            swap(ind, lchild(ind));

            drop(lchild(ind));

        }

    }

}

 void insert(Link z)  //inserts new node
{

    q.array[q.size] = z;

    heapify(q.size);

    q.size++;

}



Link dequeue()  //dequeues node
{

    if (is_empty())

        return NULL;

    Link temp = q.array[0];

    q.array[0] = q.array[q.size-1];

    q.array[q.size-1] = NULL;

    q.size--;

    drop(0);

    return temp;

}
