#include<stdio.h> 
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //stack의 크기
#define MAX_EXPRESSION_SIZE 20 //

/* stack 내에서 우선순위는 내림차순, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence; //구조체의 별칭을 precedence로 지정

char infixExp[MAX_EXPRESSION_SIZE];//크기가 20인 char 자료형 배열 선언 ->중위 연산자 배열
char postfixExp[MAX_EXPRESSION_SIZE];//크기가 20인 char 자료형 배열 선언 -> 후위 연산자 배열
char postfixStack[MAX_STACK_SIZE];//후위 연산자 배열의 스택
int evalStack[MAX_STACK_SIZE];//연산자를 저장하는 스택

int postfixStackTop = -1; //후위 연산자의 Top을 -1로 선언
int evalStackTop = -1;// 연산자를 저장하는 스택의 Top을 -1로 선언

int evalResult = 0; //계산 결과값 초기화
//밑에 함수들은 함수의 원형을 정의할 때 설명을 추가했습니다
void postfixPush(char x); 
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
        printf("[----- [박준용] [2019038010] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //커맨드를 입력 받는다

		switch(command) {
		case 'i': case 'I': //i를 입력 받았을 경우 중위 연산자를 입력 받음
			getInfix();
			break;
		case 'p': case 'P'://p를 입력 받았을 경우 후위 연산자로 변환
			toPostfix();
			break;
		case 'e': case 'E'://e를 입력 받았을 경우 계산 실행
			evaluation();
			break;
		case 'd': case 'D'://d를 입력 받았을 경우 debug
			debug();
			break;
		case 'r': case 'R'://r를 입력 받았을 경우 초기화
			reset();
			break;
		case 'q': case 'Q': //q를 입력 받았을 경우 종료
			break;
		default://입력 받은 커맨드가 없을 경우 집중이라는 메세지 출력 
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');//q를 입력받기 전까지 반복

	return 1;


}

void postfixPush(char x) //후위 연산자에서 stack에 push하는 함수
{
    postfixStack[++postfixStackTop] = x; //후위 연산자 스택에 Top에 입력받은 값 저장
}

char postfixPop()//후위 연산자에서 stack에 pop을 하는 함수
{
	char x;
    if(postfixStackTop == -1) //스택에서 Top값이 -1일 경우-> 스택에 값이 존재 하지 않는 경우
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];//stack에 연산자가 있을경우 Top에 값을 x에 대입시키고 Top값을 -1 해준다 
    }
    return x; //Top에 있는 값을 리턴

}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; //스택에 연산자를 추가 -> Top값을 +1 한 후 스택에 넣는다
}

int evalPop()
{
    if(evalStackTop == -1) //Top이 -1일 경우 종료
        return -1;
    else
        return evalStack[evalStackTop--]; //top이 가리키는 연산자가 있을 경우 스택에서 값을 리턴하고 Top값을 -1
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */

void getInfix() //연산자와 피연산자를 입력받는 함수
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);//연산자 or 피연산자를 입력 ->수식을 입력 받는다
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen; // (를 입력 받았을 경우 lparen로 리턴
	case ')' : return rparen;// )를 입력 받았을 경우 rparen로 리턴
	case '+' : return plus;// +를 입력 받았을 경우 plus로 리턴
	case '-' : return minus;// -를 입력 받았을 경우 minus로 리턴
	case '/' : return divide;// /를 입력 받았을 경우 divide로 리턴
	case '*' : return times;// *를 입력 받았을 경우 times로 리턴
	default : return operand; // 위에 해당하지 않을 겨우 operand로 리턴 
	}// ---> 입력받은 연산자와 피연산자에 우선순위를 정해주기 위해서 사용
}

precedence getPriority(char x) //우선순위를 정해주기 위해 사용
{
	return getToken(x);// getToken 함수에 입력받은 연산자를 변환하여 리턴
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c) //입력받은 문자를 postfixExp에 입력 
{
	if (postfixExp == '\0') 
		strncpy(postfixExp, c, 1); //문자가 없을경우 c를 복사해서 입력
	else
		strncat(postfixExp, c, 1);//문자가 존재할 경우 c를 이어서 붙인다
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp; //포인터변수에 중위연산자 배열을 입력
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0') //exp가 공백일 때까지 반복
	{
		if(getPriority(*exp) == operand) //*exp가 operand에 속하는 경우
		{
			x = *exp; //*exp값을 저장 후 
        	charCat(&x); //x의 주소를 넘겨 문자를 postfixEXP에 입력
		}
        else if(getPriority(*exp) == lparen) // ( 왼쪽 괄호일 경우 postfixStack에 푸시
        {
         	postfixPush(*exp);
        }
        else if(getPriority(*exp) == rparen)// ) 오른쪽 괄호일 경우
        {
        	while((x = postfixPop()) != '(') // 왼쪽 괄호가 나오기 전까지 팝을 실행하고 -> x의 문자들을 postfixEXP에 넣는다
            {
        		charCat(&x);
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))/*postfixstack Top의 우선순위가 *exp보다 클 경우
            반복*/
            {
            	x = postfixPop(); //x에 postfixStack의 Top값을 대입
            	charCat(&x);//x의 주소를 넘겨줘서 postfixEXP배열에 값을 저장
            }
            postfixPush(*exp);//*exp에 값을 postfixStack에 푸시
        }
        exp++;//exp + 1을해서 다음 값에 접근
	}

    while(postfixStackTop != -1)//스택의 모든 값이 pop이 되기전까지 반복
    {
    	x = postfixPop(); //x에 postfixStack의 Top값을 대입
    	charCat(&x); //x의 주소를 넘겨 postfixEXP배열에 값을 저장 
    }

}

void debug() //입력한 중위연산자를 후위 연산자로 변환한 식을 출력하고 계산한 값을 출력하는 함수
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);//중위 연산자 출력
	printf("postExp =  %s\n", postfixExp);//후위 연산자 출력
	printf("eval result = %d\n", evalResult);//계산값 출력 

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]); //stack에 있는 값들을 출력

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0'; //infixExp 배열의 인덱스 0의 값을 초기화
	postfixExp[0] = '\0';//postifx 배열의 인덱스 0의 값을 초기화

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; //배열의 모든 인덱스의 값을 초기화
         
	postfixStackTop = -1; //스택에 Top 위치를  -1 초기 상태로 만든다
	evalStackTop = -1;// 스택에 Top 위치를 -1 초기 상태로 만든다
	evalResult = 0;// 계산 결과값을 0으로 초기화
}
void evaluation()
{
	int opr1, opr2, i; //입력받은 수를 저장 할 변수 선언 

	int length = strlen(postfixExp); //postfixExp 문자열의 길이를 구해서 length에 입력
	char symbol; 
	evalStackTop = -1; //stackTop을 -1로 지정 

	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i]; 
		if(getToken(symbol) == operand) //피연산자일 경우 실행
        {
			evalPush(symbol - '0');//symbol 값에 '0'을 뺀다-> 아스키코드 48을 빼서 문자가 실제 정수값을 넣어주기 위함
		}
		else {
			opr2 = evalPop();//evalExp에 Top값을 oper2에 입력
			opr1 = evalPop();//evalExp에 Top값을 oper1에 입력
			switch(getToken(symbol)) 
            {
			case plus: evalPush(opr1 + opr2); break; //plus를 입력 받았을 경우 더하기 계산
			case minus: evalPush(opr1 - opr2); break;//minus를 입력 받았을 경우 빼기 계산
			case times: evalPush(opr1 * opr2); break;//times를 입력 받았을 경우 곱하기 계산
			case divide: evalPush(opr1 / opr2); break;//divide를 입력 받았을 경우 나누기 계산
			default: break;
			}
		}
	}
	evalResult = evalPop();//결과에 계산한 값을 pop에서 넣어준다
}