#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 //원형Q에 사이즈를 4로 지정
typedef char element; //char자료형의 별칭을 element로 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE]; //queue라는 크기가 4인 char형 배열을 선언
	int front, rear; //front 와 rear에 대한 변수 선언
}QueueType; //구조체의 별칭을 QueueType로 지정


QueueType *createQueue(); //큐를 만드는 함수
int freeQueue(QueueType *cQ);//메모리해제 함수
int isEmpty(QueueType *cQ);//큐가 비었을 경우를 비었다고 알려주는 함수
int isFull(QueueType *cQ); //큐가 가득 차있을 경우를 알려주는 함수 
void enQueue(QueueType *cQ, element item);//큐에 item을 넣어주는 함수
void deQueue(QueueType *cQ, element* item);//큐에 item을 빼는 함수
void printQ(QueueType *cQ);//큐를 출력하는 함수
void debugQ(QueueType *cQ);//디버깅 함수
element getElement();//구조체 getelement 함수


int main(void)
{
	QueueType *cQ = createQueue();//구조체에 큐를 생성
	element data; //char형 data변수 선언

	char command;

	do{
		printf("[----- [박준용] [2019038010] -----]\n");
        printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //커맨드를 입력 받는다

		switch(command) {
		case 'i': case 'I': //i를 입력받았을 경우 원형큐에 값 추가
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D'://d를 입력받았을 경우 원형큐에 값 삭제
			deQueue(cQ,&data);
			break;
		case 'p': case 'P'://p를 입력받았을 경우 원형큐에 값들을 출력
			printQ(cQ);
			break;
		case 'b': case 'B'://b를 입력받았을 경우 원형큐에 각 인덱스의 값과 front , rear에 위치를 출력
			debugQ(cQ);
			break;
		case 'q': case 'Q'://q를 입력받았을 경우 프로그램 종료
			break;
		default: //다른 문자를 입력받았을 경우 집중!!이라는 경고 메세지 출력
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');//q를 입력받기 전까지 반복문 실행


	return 1;
}

QueueType *createQueue() //큐를 생성하는 함수
{
	QueueType *cQ; //포인터 변수 선언
	cQ = (QueueType *)malloc(sizeof(QueueType));//cQ에  QueueType의 크기만큼 동적할당
	cQ->front = 0; //cQ에 front 값 0으로 초기화
	cQ->rear = 0; //cQ에 rear 값 0으로 초기화
	return cQ; //cQ를 리턴
}

int freeQueue(QueueType *cQ) //cQ 메모리 해제 함수
{
    if(cQ == NULL) return 1; //cQ에 값이 존재하지 않으면 1리턴
    free(cQ);//동적할당한 cQ를 메모리 해제
    return 1;
}

element getElement() //원형큐에 item 값을 넣어주는 함수
{
	element item; //item 변수 선언
	printf("Input element = ");
	scanf(" %c", &item); //원형큐에 넣을 값을 입력받는다 
	return item;//입력받은 값을 리턴
}


int isEmpty(QueueType *cQ) //원형큐에서 값이 비었을 경우 알려주는 함수
{
	if (cQ->front == cQ->rear) /*front값과 rear값이 같은경우 원형큐의 자리가 비었다고 알려준다 ->원형큐는 maxsize보다 1칸을 줄여서
                                 save해놨기 때문에 같은 경우는 칸이 비어있는 경우다 */
    {
		printf("Circular Queue is empty!"); //원형큐가 비었다고 메세지 출력
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ) //원형큐가 가득차있을 경우 알려주는 함수
{
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) //rear+1의 값이 front값이랑 같다는 것은 원형큐가 포화상태를 의미
    //원형큐의 크기로 나누어서 나머지 값을 출력하므로 0~3에 값이 나오게 됌 ->인덱스가 0~3이므로 
    {
		printf(" Circular Queue is full!"); //원형큐가 가득찼다고 메세지 출력
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item) //원형큐에 입력받은 item값을 넣어주는 함수
{
	if(isFull(cQ)) return; //원형큐가 가득찼을 경우를 검사 차있지않은경우 값을 넣어준다
	else 
    {
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; //rear가 +1 만큼 한칸 이동한 후 rear가 가르키는 값을 변경
		cQ->queue[cQ->rear] = item;//원형큐에 rear가 가르키는 곳에 item값 추가
	}
}

void deQueue(QueueType *cQ, element *item)//원형큐에 존재하는 item값을 삭제하는 함수
{
	if(isEmpty(cQ)) return; //원형큐가 비었을 경우 종료
	else 
    {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;//원형큐는 FIFO방식이기 때문에 front가 가르키는 곳에서 한칸 이동한 후
		*item = cQ->queue[cQ->front];//front가 가르키는 곳에 item값을 삭제
		return;
	}
}


void printQ(QueueType *cQ)//원형큐를 출력하는 함수
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;//첫번째 인덱스에 값 ->front+1에 값이 채워져있기때문에 
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;//원형큐에서 마지막 인덱스 값

	printf("Circular Queue : [");

	i = first;
	while(i != last)//first값과 last값이 같아질 때 까지 반복문 실행->원형큐에 채워진 모든 값 출력
    {
		printf("%3c", cQ->queue[i]);//원형큐 인덱스 1~3까지 모두 출력
		i = (i+1)%MAX_QUEUE_SIZE;//i의 값을 한칸씩 증가

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)//원형큐에서 front,rear가 가리키는 인덱스값, 각 인덱스별로 item값을 출력해주는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) //0~3까지 중 front값이 존재하는 곳을 찾아서 front가 가리키는 곳에 위치를 출력
        {
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);//각각의 인덱스에 있는 item들의 값을 인덱스별로 출력

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);//front,rear가 존재하는 곳을 출력 ->인덱스 번호
}