#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //����Q�� ����� 4�� ����
typedef char element; //char�ڷ����� ��Ī�� element�� ����
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //queue��� ũ�Ⱑ 4�� char�� �迭�� ����
	int front, rear; //front �� rear�� ���� ���� ����
}QueueType; //����ü�� ��Ī�� QueueType�� ����


QueueType *createQueue(); //ť�� ����� �Լ�
int freeQueue(QueueType *cQ);//�޸����� �Լ�
int isEmpty(QueueType *cQ);//ť�� ����� ��츦 ����ٰ� �˷��ִ� �Լ�
int isFull(QueueType *cQ); //ť�� ���� ������ ��츦 �˷��ִ� �Լ� 
void enQueue(QueueType *cQ, element item);//ť�� item�� �־��ִ� �Լ�
void deQueue(QueueType *cQ, element* item);//ť�� item�� ���� �Լ�
void printQ(QueueType *cQ);//ť�� ����ϴ� �Լ�
void debugQ(QueueType *cQ);//����� �Լ�
element getElement();//����ü getelement �Լ�


int main(void)
{
	QueueType *cQ = createQueue();//����ü�� ť�� ����
	element data; //char�� data���� ����

	char command;

	do{
		printf("[----- [���ؿ�] [2019038010] -----]\n");
        printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //Ŀ�ǵ带 �Է� �޴´�

		switch(command) {
		case 'i': case 'I': //i�� �Է¹޾��� ��� ����ť�� �� �߰�
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D'://d�� �Է¹޾��� ��� ����ť�� �� ����
			deQueue(cQ,&data);
			break;
		case 'p': case 'P'://p�� �Է¹޾��� ��� ����ť�� ������ ���
			printQ(cQ);
			break;
		case 'b': case 'B'://b�� �Է¹޾��� ��� ����ť�� �� �ε����� ���� front , rear�� ��ġ�� ���
			debugQ(cQ);
			break;
		case 'q': case 'Q'://q�� �Է¹޾��� ��� ���α׷� ����
			break;
		default: //�ٸ� ���ڸ� �Է¹޾��� ��� ����!!�̶�� ��� �޼��� ���
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');//q�� �Է¹ޱ� ������ �ݺ��� ����


	return 1;
}

QueueType *createQueue() //ť�� �����ϴ� �Լ�
{
	QueueType *cQ; //������ ���� ����
	cQ = (QueueType *)malloc(sizeof(QueueType));//cQ��  QueueType�� ũ�⸸ŭ �����Ҵ�
	cQ->front = 0; //cQ�� front �� 0���� �ʱ�ȭ
	cQ->rear = 0; //cQ�� rear �� 0���� �ʱ�ȭ
	return cQ; //cQ�� ����
}

int freeQueue(QueueType *cQ) //cQ �޸� ���� �Լ�
{
    if(cQ == NULL) return 1; //cQ�� ���� �������� ������ 1����
    free(cQ);//�����Ҵ��� cQ�� �޸� ����
    return 1;
}

element getElement() //����ť�� item ���� �־��ִ� �Լ�
{
	element item; //item ���� ����
	printf("Input element = ");
	scanf(" %c", &item); //����ť�� ���� ���� �Է¹޴´� 
	return item;//�Է¹��� ���� ����
}


int isEmpty(QueueType *cQ) //����ť���� ���� ����� ��� �˷��ִ� �Լ�
{
	if (cQ->front == cQ->rear) /*front���� rear���� ������� ����ť�� �ڸ��� ����ٰ� �˷��ش� ->����ť�� maxsize���� 1ĭ�� �ٿ���
                                 save�س��� ������ ���� ���� ĭ�� ����ִ� ���� */
    {
		printf("Circular Queue is empty!"); //����ť�� ����ٰ� �޼��� ���
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ) //����ť�� ���������� ��� �˷��ִ� �Լ�
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) //rear+1�� ���� front���̶� ���ٴ� ���� ����ť�� ��ȭ���¸� �ǹ�
    //����ť�� ũ��� ����� ������ ���� ����ϹǷ� 0~3�� ���� ������ �� ->�ε����� 0~3�̹Ƿ� 
    {
		printf(" Circular Queue is full!"); //����ť�� ����á�ٰ� �޼��� ���
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item) //����ť�� �Է¹��� item���� �־��ִ� �Լ�
{
	if(isFull(cQ)) return; //����ť�� ����á�� ��츦 �˻� ������������� ���� �־��ش�
	else 
    {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear�� +1 ��ŭ ��ĭ �̵��� �� rear�� ����Ű�� ���� ����
		cQ->queue[cQ->rear] = item;//����ť�� rear�� ����Ű�� ���� item�� �߰�
	}
}

void deQueue(QueueType *cQ, element *item)//����ť�� �����ϴ� item���� �����ϴ� �Լ�
{
	if(isEmpty(cQ)) return; //����ť�� ����� ��� ����
	else 
    {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;//����ť�� FIFO����̱� ������ front�� ����Ű�� ������ ��ĭ �̵��� ��
		*item = cQ->queue[cQ->front];//front�� ����Ű�� ���� item���� ����
		return;
	}
}


void printQ(QueueType *cQ)//����ť�� ����ϴ� �Լ�
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;//ù��° �ε����� �� ->front+1�� ���� ä�����ֱ⶧���� 
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;//����ť���� ������ �ε��� ��

	printf("Circular Queue : [");

	i = first;
	while(i != last)//first���� last���� ������ �� ���� �ݺ��� ����->����ť�� ä���� ��� �� ���
    {
		printf("%3c", cQ->queue[i]);//����ť �ε��� 1~3���� ��� ���
		i = (i+1)%MAX_QUEUE_SIZE;//i�� ���� ��ĭ�� ����

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)//����ť���� front,rear�� ����Ű�� �ε�����, �� �ε������� item���� ������ִ� �Լ�
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) //0~3���� �� front���� �����ϴ� ���� ã�Ƽ� front�� ����Ű�� ���� ��ġ�� ���
        {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);//������ �ε����� �ִ� item���� ���� �ε������� ���

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//front,rear�� �����ϴ� ���� ��� ->�ε��� ��ȣ
}