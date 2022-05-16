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

	printf("[-----[�忹��] [2021041018] -----]");

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


/* recursive ����� Inorder ��ȸ */
void recursiveInorder(Node* ptr)
{
	/* LVR ������ �湮(���) */
	if(ptr) {
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 * iterative ����� Inorder ��ȸ
 */
void iterativeInorder(Node* node)
{
	while (1) {
		/* Ʈ�� ��带 �������� �������鼭 ���ÿ� push */
		for (; node; node = node->left)
			push(node);
		/* ���ÿ��� �ϳ��� pop�ϸ� ���
		 * pop�� ��忡 right �ʵ� �ڽ� ��尡 �ִٸ� ��� �� �������� ����
		 * ���� LVR ������ �湮(���), ��� �湮�Ͽ��ٸ� break */
		node = pop();
		if (!node) break;
		printf(" [%d] ", node->key);
		node = node->right;
	}
}

/**
 * textbook: p 225
 * level ������ ��ȸ
 */
void levelOrder(Node* ptr)
{
	/* BST�� head->left�� NULL�̶�� ���� Ʈ���̹Ƿ� return */
	if (!ptr) return;
	/* ���� Ʈ���� �ƴ϶�� �켱 root�� enqueue */
	enQueue(ptr);
	while (1) {
		/* ť�� �� ������� ���Ұ� ��� */
		ptr = deQueue();
		if (ptr) {
			printf(" [%d] ", ptr->key);
			/* �ڽ� �ʵ尡 �ִٸ� left, right ������ enqueue
			 * ���� ť���� Ʈ�� ��尡 ���� ������ �� */
			if (ptr->left)
				enQueue(ptr->left);
			if (ptr->right)
				enQueue(ptr->right);
		}
		/* ��� ��µǾ��ٸ� break */
		else break;
	}
}

/* Ʈ���� ��� ���� */
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

/* Ʈ������ Ư�� key ���� ��� ���� */
int deleteNode(Node* head, int key)
{
	/* Ʈ���� ���ǵ��� �ʾҰų� ��� �־ �������� ���ϴ� ��� */
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
			/* Leaf Node�� �����ϴ� ���(root ��� �����ϴ� ��쵵 ����) */
			if (findNode->left == NULL && findNode->right == NULL) {
				/* �ش� ����� Parent Node�� ����/������ �ʵ� �� ��� NULL�� ������ Ȯ�� */
				if (parentNode == head || parentNode->key > key)
					parentNode->left = NULL;
				else
					parentNode->right = NULL;
				break;
			}
			/* 2���� �ڽ� ��带 ������ ��� */
			else if (findNode->left != NULL && findNode->right != NULL) {
				/* ������ ����Ʈ�l���� ���� ���� ������ ��ü */
				Node* minNode = findNode->right;
				Node* minNodeparent = findNode;
				/* ���� ���� ���� ���� ���ʿ� �ִ� ���� ���̹Ƿ� �������� ������ */
				while (minNode->left) {
					minNodeparent = minNode;
					minNode = minNode->left;
				}
				/* �ش� ����� left �ʵ带 ���� ���� ����� left �ʵ忡 �Ҵ�
				 * ���� ���� ���� ����� �θ� ����� left �ʵ带 NULL�� ���� */
				minNode->left = findNode->left;
				minNodeparent->left = NULL;
				/* �ش� ��尡 �θ� ����� left �ʵ忡 �־��ٸ� ���� ���� ��带 left �ʵ忡 ����
				 * right �ʵ忡 �־��ٸ� right �ʵ忡 ���� */
				if (parentNode->key > key)
					parentNode->left = minNode;
				else parentNode->right = minNode;
				break;
			}
			/* 1���� �ڽ� ��带 ������ ��� */
			else {
				/* ������ ��尡 �θ� ����� left �ʵ忡 �ִ� ��� */
				if (parentNode->key > key) {
					/* �ش� �ʵ带 ������ ����� �ڽ� ���� ��ü */
					if (findNode->left != NULL)
						parentNode->left = findNode->left;
					else
						parentNode->left = findNode->right;
				}
				/* ������ ��尡 �θ� ����� right �ʵ尡 �ִ� ��� */
				else {
					/* �ش� �ʵ带 ������ ����� �ڽ� ���� ��ü */
					if (findNode->left != NULL)
						parentNode->right = findNode->left;
					else
						parentNode->right = findNode->right;
				}
				break;

			}
		}
		parentNode = findNode;
		/* ��� �������� ������ Ȯ�� */
		if (findNode->key > key)
			findNode = findNode->left;
		else if (findNode->key < key)
			findNode = findNode->right;

	}
	/* �ش��ϴ� ��带 ã�� ���� ��� */
	if (findNode == NULL)
		printf("Cannot find the node [%d]\n", key);

	/* �ش��ϴ� ��� ���� */
	free(findNode);

	return 0;

}

/* recursive ������� ��忡 �Ҵ�� �޸� ���� */
void freeNode(Node* ptr)
{	
	/* recursive ������� free */
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

/* Ʈ���� �Ҵ�� ��ü �޸� ���� */
int freeBST(Node* head)
{
	/* ���� Ʈ���� ��� head ��常 ���� */
	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;
	/* freeNode �Լ� ȣ�� */
	freeNode(p);
	/* head ������ ���� */
	free(head);
	return 1;
}

/* ���ÿ��� ���� ����, ������ ���� ���� */
Node* pop()
{
	/* ������ ����� ��� */
	if (top == -1) {
		return NULL;
	}
	return stack[top--];
}

/* ���ÿ� ���� ����ֱ� */
void push(Node* aNode)
{
	/* ������ ������ ��� */
	if (top == MAX_STACK_SIZE - 1) {
		return;
	}
	stack[++top] = aNode;
}

/* ť���� ���� ����, ������ ���� ���� */
Node* deQueue()
{
	/* ť�� ����� ��� */
	if (front == rear) {
		return NULL;
	}
	front = (front + 1) % MAX_QUEUE_SIZE;
	return queue[front];
}

/* ť�� ���� ����ֱ� */
void enQueue(Node* aNode)
{
	/* ť�� ������ ��� */
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
