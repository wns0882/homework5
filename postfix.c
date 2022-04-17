#include<stdio.h> 
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10 //stack�� ũ��.
#define MAX_EXPRESSION_SIZE 20 //

/* stack ������ �켱������ ��������, lparen = 0 ���� ���� */
typedef enum{
	lparen = 0,  /* ( ���� ��ȣ */
	rparen = 9,  /* ) ������ ��ȣ*/
	times = 7,   /* * ���� */
	divide = 6,  /* / ������ */
	plus = 5,    /* + ���� */
	minus = 4,   /* - ���� */
	operand = 1 /* �ǿ����� */
} precedence; //����ü�� ��Ī�� precedence�� ����

char infixExp[MAX_EXPRESSION_SIZE];//ũ�Ⱑ 20�� char �ڷ��� �迭 ���� ->���� ������ �迭
char postfixExp[MAX_EXPRESSION_SIZE];//ũ�Ⱑ 20�� char �ڷ��� �迭 ���� -> ���� ������ �迭
char postfixStack[MAX_STACK_SIZE];//���� ������ �迭�� ����
int evalStack[MAX_STACK_SIZE];//�����ڸ� �����ϴ� ����

int postfixStackTop = -1; //���� �������� Top�� -1�� ����
int evalStackTop = -1;// �����ڸ� �����ϴ� ������ Top�� -1�� ����

int evalResult = 0; //��� ����� �ʱ�ȭ
//�ؿ� �Լ����� �Լ��� ������ ������ �� ������ �߰��߽��ϴ�
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
        printf("[----- [���ؿ�] [2019038010] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //Ŀ�ǵ带 �Է� �޴´�

		switch(command) {
		case 'i': case 'I': //i�� �Է� �޾��� ��� ���� �����ڸ� �Է� ����
			getInfix();
			break;
		case 'p': case 'P'://p�� �Է� �޾��� ��� ���� �����ڷ� ��ȯ
			toPostfix();
			break;
		case 'e': case 'E'://e�� �Է� �޾��� ��� ��� ����
			evaluation();
			break;
		case 'd': case 'D'://d�� �Է� �޾��� ��� debug
			debug();
			break;
		case 'r': case 'R'://r�� �Է� �޾��� ��� �ʱ�ȭ
			reset();
			break;
		case 'q': case 'Q': //q�� �Է� �޾��� ��� ����
			break;
		default://�Է� ���� Ŀ�ǵ尡 ���� ��� �����̶�� �޼��� ��� 
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');//q�� �Է¹ޱ� ������ �ݺ�

	return 1;


}

void postfixPush(char x) //���� �����ڿ��� stack�� push�ϴ� �Լ�
{
    postfixStack[++postfixStackTop] = x; //���� ������ ���ÿ� Top�� �Է¹��� �� ����
}

char postfixPop()//���� �����ڿ��� stack�� pop�� �ϴ� �Լ�
{
	char x;
    if(postfixStackTop == -1) //���ÿ��� Top���� -1�� ���-> ���ÿ� ���� ���� ���� �ʴ� ���
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];//stack�� �����ڰ� ������� Top�� ���� x�� ���Խ�Ű�� Top���� -1 ���ش� 
    }
    return x; //Top�� �ִ� ���� ����

}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x; //���ÿ� �����ڸ� �߰� -> Top���� +1 �� �� ���ÿ� �ִ´�
}

int evalPop()
{
    if(evalStackTop == -1) //Top�� -1�� ��� ����
        return -1;
    else
        return evalStack[evalStackTop--]; //top�� ����Ű�� �����ڰ� ���� ��� ���ÿ��� ���� �����ϰ� Top���� -1
}

/**
 * infix expression�� �Է¹޴´�.
 * infixExp���� �Էµ� ���� �����Ѵ�.
 */

void getInfix() //�����ڿ� �ǿ����ڸ� �Է¹޴� �Լ�
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);//������ or �ǿ����ڸ� �Է� ->������ �Է� �޴´�
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen; // (�� �Է� �޾��� ��� lparen�� ����
	case ')' : return rparen;// )�� �Է� �޾��� ��� rparen�� ����
	case '+' : return plus;// +�� �Է� �޾��� ��� plus�� ����
	case '-' : return minus;// -�� �Է� �޾��� ��� minus�� ����
	case '/' : return divide;// /�� �Է� �޾��� ��� divide�� ����
	case '*' : return times;// *�� �Է� �޾��� ��� times�� ����
	default : return operand; // ���� �ش����� ���� �ܿ� operand�� ���� 
	}// ---> �Է¹��� �����ڿ� �ǿ����ڿ� �켱������ �����ֱ� ���ؼ� ���
}

precedence getPriority(char x) //�켱������ �����ֱ� ���� ���
{
	return getToken(x);// getToken �Լ��� �Է¹��� �����ڸ� ��ȯ�Ͽ� ����
}

/**
 * �����ϳ��� ���޹޾�, postfixExp�� �߰�
 */
void charCat(char* c) //�Է¹��� ���ڸ� postfixExp�� �Է� 
{
	if (postfixExp == '\0') 
		strncpy(postfixExp, c, 1); //���ڰ� ������� c�� �����ؼ� �Է�
	else
		strncat(postfixExp, c, 1);//���ڰ� ������ ��� c�� �̾ ���δ�
}

/**
 * infixExp�� ���ڸ� �ϳ��� �о�鼭 stack�� �̿��Ͽ� postfix�� �����Ѵ�.
 * ����� postfix�� postFixExp�� ����ȴ�.
 */
void toPostfix()
{
	/* infixExp�� ���� �ϳ����� �б����� ������ */
	char *exp = infixExp; //�����ͺ����� ���������� �迭�� �Է�
	char x; /* �����ϳ��� �ӽ÷� �����ϱ� ���� ���� */

	/* exp�� �������Ѱ��鼭, ���ڸ� �а� postfix�� ���� */
	while(*exp != '\0') //exp�� ������ ������ �ݺ�
	{
		if(getPriority(*exp) == operand) //*exp�� operand�� ���ϴ� ���
		{
			x = *exp; //*exp���� ���� �� 
        	charCat(&x); //x�� �ּҸ� �Ѱ� ���ڸ� postfixEXP�� �Է�
		}
        else if(getPriority(*exp) == lparen) // ( ���� ��ȣ�� ��� postfixStack�� Ǫ��
        {
         	postfixPush(*exp);
        }
        else if(getPriority(*exp) == rparen)// ) ������ ��ȣ�� ���
        {
        	while((x = postfixPop()) != '(') // ���� ��ȣ�� ������ ������ ���� �����ϰ� -> x�� ���ڵ��� postfixEXP�� �ִ´�
            {
        		charCat(&x);
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))/*postfixstack Top�� �켱������ *exp���� Ŭ ���
            �ݺ�*/
            {
            	x = postfixPop(); //x�� postfixStack�� Top���� ����
            	charCat(&x);//x�� �ּҸ� �Ѱ��༭ postfixEXP�迭�� ���� ����
            }
            postfixPush(*exp);//*exp�� ���� postfixStack�� Ǫ��
        }
        exp++;//exp + 1���ؼ� ���� ���� ����
	}

    while(postfixStackTop != -1)//������ ��� ���� pop�� �Ǳ������� �ݺ�
    {
    	x = postfixPop(); //x�� postfixStack�� Top���� ����
    	charCat(&x); //x�� �ּҸ� �Ѱ� postfixEXP�迭�� ���� ���� 
    }

}

void debug() //�Է��� ���������ڸ� ���� �����ڷ� ��ȯ�� ���� ����ϰ� ����� ���� ����ϴ� �Լ�
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);//���� ������ ���
	printf("postExp =  %s\n", postfixExp);//���� ������ ���
	printf("eval result = %d\n", evalResult);//��갪 ��� 

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]); //stack�� �ִ� ������ ���

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0'; //infixExp �迭�� �ε��� 0�� ���� �ʱ�ȭ
	postfixExp[0] = '\0';//postifx �迭�� �ε��� 0�� ���� �ʱ�ȭ

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0'; //�迭�� ��� �ε����� ���� �ʱ�ȭ
         
	postfixStackTop = -1; //���ÿ� Top ��ġ��  -1 �ʱ� ���·� �����
	evalStackTop = -1;// ���ÿ� Top ��ġ�� -1 �ʱ� ���·� �����
	evalResult = 0;// ��� ������� 0���� �ʱ�ȭ
}
void evaluation()
{
	int opr1, opr2, i; //�Է¹��� ���� ���� �� ���� ���� 

	int length = strlen(postfixExp); //postfixExp ���ڿ��� ���̸� ���ؼ� length�� �Է�
	char symbol; 
	evalStackTop = -1; //stackTop�� -1�� ���� 

	for(i = 0; i < length; i++)
	{
		symbol = postfixExp[i]; 
		if(getToken(symbol) == operand) //�ǿ������� ��� ����
        {
			evalPush(symbol - '0');//symbol ���� '0'�� ����-> �ƽ�Ű�ڵ� 48�� ���� ���ڰ� ���� �������� �־��ֱ� ����
		}
		else {
			opr2 = evalPop();//evalExp�� Top���� oper2�� �Է�
			opr1 = evalPop();//evalExp�� Top���� oper1�� �Է�
			switch(getToken(symbol)) 
            {
			case plus: evalPush(opr1 + opr2); break; //plus�� �Է� �޾��� ��� ���ϱ� ���
			case minus: evalPush(opr1 - opr2); break;//minus�� �Է� �޾��� ��� ���� ���
			case times: evalPush(opr1 * opr2); break;//times�� �Է� �޾��� ��� ���ϱ� ���
			case divide: evalPush(opr1 / opr2); break;//divide�� �Է� �޾��� ��� ������ ���
			default: break;
			}
		}
	}
	evalResult = evalPop();//����� ����� ���� pop���� �־��ش�
}