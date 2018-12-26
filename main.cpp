//头文件
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

/*利用结构体定义多项式的项 */


typedef struct Polynomial     //多项式 
{
	float coef;               //系数 
	int expn;                 //指数 
	struct Polynomial *next;  //递归算法的应用
}*Polyn, Polynomial;

/*设计思路 利用insert函数，将指数插入到存储空间。若指数相同相合并 ；
若系数为0的话释放结点；若指数为新将结点插入*/

void Insert(Polyn p, Polyn h)  // 插入，它并不是C语言的一部分，用在函数名中可以表示插入元素。
{
	if (p->coef == 0)
		free(p);                  //系数为0的话释放结点
	else
	{
		Polyn q1, q2;
		q1 = h;
		q2 = h->next;
		while (q2&&p->expn < q2->expn)
		{//查找插入位置
			q1 = q2;
			q2 = q2->next;
		}
		if (q2&&p->expn == q2->expn)
		{//将指数相同相合并
			q2->coef += p->coef;
			free(p);
			if (!q2->coef)
			{//系数为0的话释放结点
				q1->next = q2->next;
				free(q2);
			}
		}
		else
		{//指数为新时将结点插入
			p->next = q2;
			q1->next = p;
		}
	}
}

Polyn CreatePolyn(Polyn head, int m)//建立一个头指针为head、项数为m的一元多项式（一个多项式最多不超过20项） 
{
	int i;
	Polyn p;
	head = (Polyn)malloc(sizeof(struct Polynomial));//动态分配内存
	head->next = NULL;
	for (i = 0; i < m; i++)
	{
		p = (Polyn)malloc(sizeof(struct Polynomial));//建立新结点以接收数据
		printf("请输入第%d项的系数与指数:", i + 1);
		scanf("%f %d", &p->coef, &p->expn);
		Insert(p, head);                            //调用Insert函数插入结点
	}
	return head;
}

void PrintPolyn(Polyn P) //输出多项式，设置一个标志flag
{
	Polyn q = P->next;
	int flag = 1;                              //项数计数器
	if (!q)                                   //若多项式为空，输出0
	{
		putchar('0');
		printf("\n");
		return;
	}
	while (q)
	{
		if (q->coef > 0 && flag != 1) putchar('+'); //系数大于0且不是第一项
		if (q->coef != 1 && q->coef != -1)          //系数非1或-1的普通情况
		{
			printf("%g", q->coef);            //%g用来输出实数,它根据数值的大小,自动选f格式或e格式(选择输出时占宽度较小的一种),且不输出无意义的0。
			if (q->expn == 1)
				putchar('X');
			else if (q->expn)
				printf("X^%d", q->expn);
		}
		else
		{
			if (q->coef == 1)                  //系数为1的时候 
			{
				if (!q->expn)
					putchar('1');
				else if (q->expn == 1)
					putchar('X');
				else printf("X^%d", q->expn);
			}
			if (q->coef == -1)                //系数为-1的时候
			{
				if (!q->expn)
					printf("-1");
				else if (q->expn == 1)
					printf("-X");
				else printf("-X^%d", q->expn);
			}
		}
		q = q->next;
		flag++;
	}
	printf("\n");
}

int compare(Polyn a, Polyn b)//比较a,b值 
{
	if (a&&b)                //a和b同时为true, 才返回 true, 否则返回false
	{
		if (!b || a->expn > b->expn) // 俩者任意一个为true 就返回true , 否则返回false
			return 1;
		else if (!a || a->expn < b->expn)
			return -1;
		else return 0;
	}
	else if (!a&&b)
		return -1;             //a多项式已空，但b多项式非空
	else return 1;         //b多项式已空，但a多项式非空
}

Polyn AddPolyn(Polyn pa, Polyn pb)//多项式加法
{//求解并建立多项式a+b，返回其头指针
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	Polyn headc, hc, qc;
	hc = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hc->next = NULL;
	headc = hc;
	while (qa || qb)
	{
		qc = (Polyn)malloc(sizeof(struct Polynomial));
		switch (compare(qa, qb)) {
		case 1:
		{
			qc->coef = qa->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			break;
		}
		case 0:
		{
			qc->coef = qa->coef + qb->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			qb = qb->next;
			break;
		}
		case -1:
		{
			qc->coef = qb->coef;
			qc->expn = qb->expn;
			qb = qb->next;
			break;
		}
		}
		if (qc->coef != 0)
		{
			qc->next = hc->next;
			hc->next = qc;
			hc = qc;
		}
		else free(qc);//当相加系数为0时，释放该结点
	}
	return headc;
}

Polyn SubtractPolyn(Polyn pa, Polyn pb) //求解并建立多项式a-b，返回其头指针
{
	Polyn h = pb;
	Polyn p = pb->next;
	Polyn pd;
	while (p)
	{           //将pb的系数取反
		p->coef *= -1;
		p = p->next;
	}
	pd = AddPolyn(pa, h);
	for (p = h->next; p; p = p->next)    //恢复pb的系数
		p->coef *= -1;
	return pd;
}

float ValuePolyn(Polyn head, float x) //输入x值，计算并返回多项式的值
{
	Polyn p;
	int i;
	float sum = 0, t;
	for (p = head->next; p; p = p->next)
	{
		t = 1;
		for (i = p->expn; i != 0;)
		{
			if (i < 0) { t /= x; i++; }//指数小于0，进行除法
			else { t *= x; i--; }//指数大于0，进行乘法
		}
		sum += p->coef*t;
	}
	return sum;
}

Polyn Derivative(Polyn head) //求解并建立导函数多项式，并返回其头指针
{
	Polyn q = head->next, p1, p2, hd;
	hd = p1 = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hd->next = NULL;
	while (q)
	{
		if (q->expn != 0)
		{               //该项不是常数项时
			p2 = (Polyn)malloc(sizeof(struct Polynomial));
			p2->coef = q->coef*q->expn;
			p2->expn = q->expn - 1;
			p2->next = p1->next;//连接结点
			p1->next = p2;
			p1 = p2;
		}
		q = q->next;
	}
	return hd;
}

Polyn MultiplyPolyn(Polyn pa, Polyn pb) //求解并建立多项式a*b，返回其头指针
{
	Polyn hf, pf;
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	hf = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hf->next = NULL;
	for (; qa; qa = qa->next)
	{
		for (qb = pb->next; qb; qb = qb->next)
		{
			pf = (Polyn)malloc(sizeof(struct Polynomial));
			pf->coef = qa->coef*qb->coef;
			pf->expn = qa->expn + qb->expn;
			Insert(pf, hf);//调用Insert函数以合并指数相同的项
		}
	}
	return hf;
}

Polyn WriteFile(Polyn pa,Polyn pb)
{
	FILE *fp;
	Polyn qa = pa->next;//定义qa和qb两个新结构体变量来暂时存储pa->next和pb->next的链表
	Polyn qb = pb->next;
	int i = 1, j = 1;
	if ((fp = fopen("data.txt", "w")) == NULL)//打开文件并判断是否打开成功
	{
		printf("文件打开失败");
		exit(0);
	}
	for (; qa; qa = qa->next)
	{
		printf("写入文件的多项式a第%d项系数和指数分别为：%.2f  %d\n", i, qa->coef, qa->expn);//先输出要写入的多项式的系数
		fprintf(fp, "(%.2f, %d)", qa->coef, qa->expn);//向文件输出格式化的系数和指数
		i++;
	}
	fprintf(fp, "\n");
	for (; qb; qb = qb->next)
	{
		printf("写入文件的多项式b第%d项系数和指数分别为：%.2f  %d\n", j, qb->coef, qb->expn);
		fprintf(fp, "(%.2f, %d)", qb->coef, qb->expn);
		j++;
	}
	fclose(fp);
}

Polyn ReadFile()
{
	//int i,m=2;
	//Polyn p;
	//p == (Polyn)malloc(sizeof(struct Polynomial));//动态分配内存 
	//p->next = NULL;
	FILE *fp;
	Polyn pr=0;
	Polyn head1;
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("文件打开失败");
		exit(0);
	}
	pr = head1 = (Polyn)malloc(sizeof(struct Polynomial));//动态分配内存 
	head1->next = NULL;
	//for (i = 0; i < m; i++)
	//{
	pr = (Polyn)malloc(sizeof(struct Polynomial));//建立新结点以接收数据
	while (getchar() != '\n');
	fscanf(fp, "(%.2f,%d)", pr->coef, pr->expn);
	Insert(pr, head1);//调用Insert函数插入结点
	printf("%.2f  %d", pr->coef, pr->expn);
	fclose(fp);
	//}
}

void DestroyPolyn(Polyn p) //销毁多项式p
{
	Polyn q1, q2;
	q1 = p->next;
	q2 = q1->next;
	while (q1->next)
	{
		free(q1);
		q1 = q2;
		q2 = q2->next;
	}
}

/*主函数，菜单功能*/
void main()
{
	int m, n, o, a = 1;
	float x;
	char flag;
	Polyn pa = 0, pb = 0, pc;
	system("color 70");//设定背景和字体颜色，仅有16位
	printf("         欢迎使用多项式操作程序\n\n");
	printf("请输入a的项数:");
	o = scanf("%d", &m);//当输入不需要的数据类型的数据，o就不等于1
	while (0 >= m || m > 20 || o != 1)//判断用户输入的值是否满足条件
	{
		printf("请重新输入项数");
		while (getchar() != '\n');//清除缓存区中的回车符
		o = scanf("%d", &m);
	};
	pa = CreatePolyn(pa, m);//建立多项式a
	printf("请输入b的项数:");
	o = scanf("%d", &n);
	while (0 >= n || n > 20 || o != 1)
	{
		printf("请重新输入项数");
		while (getchar() != '\n');//清除缓存区中的回车符
		o = scanf("%d", &n);
	};
	pb = CreatePolyn(pb, n);//建立多项式b

	//输出菜单
	printf("   ********************************************************************\n");
	printf("   *                          多项式操作程序                          *\n");
	printf("   *                                                                  *\n");
	printf("   *             A:输出多项式a            B:输出多项式b               *\n");
	printf("   *                                                                  *\n");
	printf("   *             C:输出a的导数            D:输出b的导数               *\n");
	printf("   *                                                                  *\n");
	printf("   *             E:代入x的值计算a         F:代入x的值计算b            *\n");
	printf("   *                                                                  *\n");
	printf("   *             G:输出a+b                H:输出a-b                   *\n");
	printf("   *                                                                  *\n");
	printf("   *             I:输出a*b                J:把多项式写入文件          *\n");
	printf("   *                                                                  *\n");
	printf("   *             K:读取文件中的多项式     L:退出程序                  *\n");
	printf("   *                                                                  *\n");
	printf("   ********************************************************************\n");

	while (a)
	{
		printf("\n请选择操作：");
		scanf(" %c", &flag);//空格符号一定要注意,可将缓冲区内的空格等符号影响消除
		switch (flag)
		{

		case'A':
		case'a':
		{
			printf("\n       多项式a=");
			PrintPolyn(pa);
			break;
		}
		case'B':
		case'b':
		{
			printf("\n       多项式b=");
			PrintPolyn(pb);
			break;
		}
		case'C':
		case'c':
		{
			pc = Derivative(pa);
			printf("\n       多项式a的导函数为：a'=");
			PrintPolyn(pc);
			break;
		}
		case'D':
		case'd':
		{
			pc = Derivative(pb);
			printf("\n       多项式b的导函数为：b'=");
			PrintPolyn(pc);
			break;
		}
		case'E':
		case'e':
		{
			printf("输入x的值：x=");
			scanf("%f", &x);
			printf("\n       x=%.2f时，a=%.2f\n", x, ValuePolyn(pa, x));
			break;
		}
		case'F':
		case'f':
		{
			printf("输入x的值：x=");
			scanf("%f", &x);
			printf("\n       x=%.2f时，b=%.2f\n", x, ValuePolyn(pb, x));
			break;
		}
		case'G':
		case'g':
		{
			pc = AddPolyn(pa, pb);
			printf("\n       a+b=");
			PrintPolyn(pc);
			break;
		}
		case'H':
		case'h':
		{
			pc = SubtractPolyn(pa, pb);
			printf("\n       a-b=");
			PrintPolyn(pc);
			break;
		}
		case'I':
		case'i':
		{
			pc = MultiplyPolyn(pa, pb);
			printf("\n      a*b=");
			PrintPolyn(pc);
			break;
		}
		case'J':
		case'j':
		{
			
			WriteFile(pa, pb);
			break;
		}
		case'K':
		case'k':
		{
			ReadFile();//这里有问题
			break;
		}
		case'L':
		case'l':
		{
			printf("\n       感谢使用此程序！\n");
			DestroyPolyn(pa);
			DestroyPolyn(pb);
			a = 0;
			break;
		}
		default:
			printf("\n       您的选择错误，请重新选择!\n");
		}
	}
}