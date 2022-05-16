/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	printf("[-----[장예서] [2021041018] -----]");

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}


/* recursive 방식의 Inorder 순회 */
void recursiveInorder(Node* ptr)
{
	/* LVR 순으로 방문(출력) */
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 * iterative 방식의 Inorder 순회
 */
void iterativeInorder(Node* node)
{
	while (1) {
		/* 트리 노드를 왼쪽으로 내려가면서 스택에 push */
		for (; node; node = node->left)
			push(node);
		/* 스택에서 하나씩 pop하며 출력
		 * pop한 노드에 right 필드 자식 노드가 있다면 출력 후 그쪽으로 향함
		 * 따라서 LVR 순으로 방문(출력), 모두 방문하였다면 break */
		node = pop();
		if (!node) break;
		printf(" [%d] ", node->key);
		node = node->right;
	}
}

/**
 * textbook: p 225
 * level 순서로 순회
 */
void levelOrder(Node* ptr)
{
	/* BST의 head->left가 NULL이라면 공백 트리이므로 return */
	if (!ptr) return;
	/* 공백 트리가 아니라면 우선 root를 enqueue */
	enQueue(ptr);
	while (1) {
		/* 큐에 들어간 순서대로 원소가 출력 */
		ptr = deQueue();
		if (ptr) {
			printf(" [%d] ", ptr->key);
			/* 자식 필드가 있다면 left, right 순으로 enqueue
			 * 따라서 큐에는 트리 노드가 레벨 순서로 들어감 */
			if (ptr->left)
				enQueue(ptr->left);
			if (ptr->right)
				enQueue(ptr->right);
		}
		/* 모두 출력되었다면 break */
		else break;
	}
}

/* 트리에 노드 삽입 */
int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}

/* 트리에서 특정 key 가진 노드 삭제 */
int deleteNode(Node* head, int key)
{
	/* 트리가 정의되지 않았거나 비어 있어서 삭제하지 못하는 경우 */
	if (head == NULL) {
		printf("Please initialize first.\n");
		return 1;
	}
	else if (head->left == NULL) {
		printf("Nothing to delete.\n");
		return 1;
	}

	Node* findNode = head->left;
	Node* parentNode = head;

	while (findNode != NULL) {
		if (findNode->key == key) {
			/* Leaf Node를 삭제하는 경우(root 노드 삭제하는 경우도 포함) */
			if (findNode->left == NULL && findNode->right == NULL) {
				/* 해당 노드의 Parent Node의 왼쪽/오른쪽 필드 중 어디를 NULL로 만들지 확인 */
				if (parentNode == head || parentNode->key > key)
					parentNode->left = NULL;
				else
					parentNode->right = NULL;
				break;
			}
			/* 2개의 자식 노드를 가지는 경우 */
			else if (findNode->left != NULL && findNode->right != NULL) {
				/* 오른쪽 서브트릐에서 가장 작은 값으로 대체 */
				Node* minNode = findNode->right;
				Node* minNodeparent = findNode;
				/* 가장 작은 값은 제일 왼쪽에 있는 값일 것이므로 왼쪽으로 내려감 */
				while (minNode->left) {
					minNodeparent = minNode;
					minNode = minNode->left;
				}
				/* 해당 노드의 left 필드를 가장 작은 노드의 left 필드에 할당
				 * 기존 가장 작은 노드의 부모 노드의 left 필드를 NULL로 만듦 */
				minNode->left = findNode->left;
				minNodeparent->left = NULL;
				/* 해당 노드가 부모 노드의 left 필드에 있었다면 가장 작은 노드를 left 필드에 연결
				 * right 필드에 있었다면 right 필드에 연결 */
				if (parentNode->key > key)
					parentNode->left = minNode;
				else parentNode->right = minNode;
				break;
			}
			/* 1개의 자식 노드를 가지는 경우 */
			else {
				/* 삭제할 노드가 부모 노드의 left 필드에 있는 경우 */
				if (parentNode->key > key) {
					/* 해당 필드를 삭제할 노드의 자식 노드로 대체 */
					if (findNode->left != NULL)
						parentNode->left = findNode->left;
					else
						parentNode->left = findNode->right;
				}
				/* 삭제할 노드가 부모 노드의 right 필드가 있는 경우 */
				else {
					/* 해당 필드를 삭제할 노드의 자식 노드로 대체 */
					if (findNode->left != NULL)
						parentNode->right = findNode->left;
					else
						parentNode->right = findNode->right;
				}
				break;

			}
		}
		parentNode = findNode;
		/* 어느 방향으로 향할지 확인 */
		if (findNode->key > key)
			findNode = findNode->left;
		else if (findNode->key < key)
			findNode = findNode->right;

	}
	/* 해당하는 노드를 찾지 못한 경우 */
	if (findNode == NULL)
		printf("Cannot find the node [%d]\n", key);

	/* 해당하는 노드 삭제 */
	free(findNode);

	return 0;

}

/* recursive 방식으로 노드에 할당된 메모리 해제 */
void freeNode(Node* ptr)
{	
	/* recursive 방식으로 free */
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

/* 트리에 할당된 전체 메모리 해제 */
int freeBST(Node* head)
{
	/* 공백 트리인 경우 head 노드만 해제 */
	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;
	/* freeNode 함수 호출 */
	freeNode(p);
	/* head 노드까지 해제 */
	free(head);
	return 1;
}

/* 스택에서 원소 삭제, 삭제한 원소 리턴 */
Node* pop()
{
	/* 스택이 비었을 경우 */
	if (top == -1) {
		return NULL;
	}
	return stack[top--];
}

/* 스택에 원소 집어넣기 */
void push(Node* aNode)
{
	/* 스택이 가득찬 경우 */
	if (top == MAX_STACK_SIZE - 1) {
		return;
	}
	stack[++top] = aNode;
}

/* 큐에서 원소 삭제, 삭제한 원소 리턴 */
Node* deQueue()
{
	/* 큐가 비었을 경우 */
	if (front == rear) {
		return NULL;
	}
	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];
}

/* 큐에 원소 집어넣기 */
void enQueue(Node* aNode)
{
	/* 큐가 가득찬 경우 */
	if (front == ((rear + 1) % MAX_QUEUE_SIZE)) {
		return;
	}
	rear = (rear + 1) % MAX_QUEUE_SIZE;
	queue[rear] = aNode;
}

void printStack()
{
	int i = 0;
	printf("--- stack ---\n");
	while (i <= top)
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}
