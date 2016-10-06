struct node{
	struct node *next;
	int data;
}

typedef struct node NodeType;

NodeType *headpt;
NodeType *pt;

//Create statically
NodeType theList[3] =  
{{&theList[1], 1}, 
{&theList[2], 10}, 
{0, 100}}

headpt = theList; // &theList[0]

//next
pt = pt->next;

//the search method
int search(int x){
	NodeType *pt;
	pt = headpt;
	while(pt){
		if((pt->data) == x) return 1;
		pt = pt->next;
	}	
	return 0;
}
