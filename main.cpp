//ͷ�ļ�
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

/*���ýṹ�嶨�����ʽ���� */


typedef struct Polynomial     //����ʽ 
{
	float coef;               //ϵ�� 
	int expn;                 //ָ�� 
	struct Polynomial *next;  //�ݹ��㷨��Ӧ��
}*Polyn, Polynomial;

/*���˼· ����insert��������ָ�����뵽�洢�ռ䡣��ָ����ͬ��ϲ� ��
��ϵ��Ϊ0�Ļ��ͷŽ�㣻��ָ��Ϊ�½�������*/

void Insert(Polyn p, Polyn h)  // ���룬��������C���Ե�һ���֣����ں������п��Ա�ʾ����Ԫ�ء�
{
	if (p->coef == 0)
		free(p);                  //ϵ��Ϊ0�Ļ��ͷŽ��
	else
	{
		Polyn q1, q2;
		q1 = h;
		q2 = h->next;
		while (q2&&p->expn < q2->expn)
		{//���Ҳ���λ��
			q1 = q2;
			q2 = q2->next;
		}
		if (q2&&p->expn == q2->expn)
		{//��ָ����ͬ��ϲ�
			q2->coef += p->coef;
			free(p);
			if (!q2->coef)
			{//ϵ��Ϊ0�Ļ��ͷŽ��
				q1->next = q2->next;
				free(q2);
			}
		}
		else
		{//ָ��Ϊ��ʱ��������
			p->next = q2;
			q1->next = p;
		}
	}
}

Polyn CreatePolyn(Polyn head, int m)//����һ��ͷָ��Ϊhead������Ϊm��һԪ����ʽ��һ������ʽ��಻����20� 
{
	int i;
	Polyn p;
	head = (Polyn)malloc(sizeof(struct Polynomial));//��̬�����ڴ�
	head->next = NULL;
	for (i = 0; i < m; i++)
	{
		p = (Polyn)malloc(sizeof(struct Polynomial));//�����½���Խ�������
		printf("�������%d���ϵ����ָ��:", i + 1);
		scanf("%f %d", &p->coef, &p->expn);
		Insert(p, head);                            //����Insert����������
	}
	return head;
}

void PrintPolyn(Polyn P) //�������ʽ������һ����־flag
{
	Polyn q = P->next;
	int flag = 1;                              //����������
	if (!q)                                   //������ʽΪ�գ����0
	{
		putchar('0');
		printf("\n");
		return;
	}
	while (q)
	{
		if (q->coef > 0 && flag != 1) putchar('+'); //ϵ������0�Ҳ��ǵ�һ��
		if (q->coef != 1 && q->coef != -1)          //ϵ����1��-1����ͨ���
		{
			printf("%g", q->coef);            //%g�������ʵ��,��������ֵ�Ĵ�С,�Զ�ѡf��ʽ��e��ʽ(ѡ�����ʱռ��Ƚ�С��һ��),�Ҳ�����������0��
			if (q->expn == 1)
				putchar('X');
			else if (q->expn)
				printf("X^%d", q->expn);
		}
		else
		{
			if (q->coef == 1)                  //ϵ��Ϊ1��ʱ�� 
			{
				if (!q->expn)
					putchar('1');
				else if (q->expn == 1)
					putchar('X');
				else printf("X^%d", q->expn);
			}
			if (q->coef == -1)                //ϵ��Ϊ-1��ʱ��
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

int compare(Polyn a, Polyn b)//�Ƚ�a,bֵ 
{
	if (a&&b)                //a��bͬʱΪtrue, �ŷ��� true, ���򷵻�false
	{
		if (!b || a->expn > b->expn) // ��������һ��Ϊtrue �ͷ���true , ���򷵻�false
			return 1;
		else if (!a || a->expn < b->expn)
			return -1;
		else return 0;
	}
	else if (!a&&b)
		return -1;             //a����ʽ�ѿգ���b����ʽ�ǿ�
	else return 1;         //b����ʽ�ѿգ���a����ʽ�ǿ�
}

Polyn AddPolyn(Polyn pa, Polyn pb)//����ʽ�ӷ�
{//��Ⲣ��������ʽa+b��������ͷָ��
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	Polyn headc, hc, qc;
	hc = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
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
		else free(qc);//�����ϵ��Ϊ0ʱ���ͷŸý��
	}
	return headc;
}

Polyn SubtractPolyn(Polyn pa, Polyn pb) //��Ⲣ��������ʽa-b��������ͷָ��
{
	Polyn h = pb;
	Polyn p = pb->next;
	Polyn pd;
	while (p)
	{           //��pb��ϵ��ȡ��
		p->coef *= -1;
		p = p->next;
	}
	pd = AddPolyn(pa, h);
	for (p = h->next; p; p = p->next)    //�ָ�pb��ϵ��
		p->coef *= -1;
	return pd;
}

float ValuePolyn(Polyn head, float x) //����xֵ�����㲢���ض���ʽ��ֵ
{
	Polyn p;
	int i;
	float sum = 0, t;
	for (p = head->next; p; p = p->next)
	{
		t = 1;
		for (i = p->expn; i != 0;)
		{
			if (i < 0) { t /= x; i++; }//ָ��С��0�����г���
			else { t *= x; i--; }//ָ������0�����г˷�
		}
		sum += p->coef*t;
	}
	return sum;
}

Polyn Derivative(Polyn head) //��Ⲣ��������������ʽ����������ͷָ��
{
	Polyn q = head->next, p1, p2, hd;
	hd = p1 = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
	hd->next = NULL;
	while (q)
	{
		if (q->expn != 0)
		{               //����ǳ�����ʱ
			p2 = (Polyn)malloc(sizeof(struct Polynomial));
			p2->coef = q->coef*q->expn;
			p2->expn = q->expn - 1;
			p2->next = p1->next;//���ӽ��
			p1->next = p2;
			p1 = p2;
		}
		q = q->next;
	}
	return hd;
}

Polyn MultiplyPolyn(Polyn pa, Polyn pb) //��Ⲣ��������ʽa*b��������ͷָ��
{
	Polyn hf, pf;
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	hf = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
	hf->next = NULL;
	for (; qa; qa = qa->next)
	{
		for (qb = pb->next; qb; qb = qb->next)
		{
			pf = (Polyn)malloc(sizeof(struct Polynomial));
			pf->coef = qa->coef*qb->coef;
			pf->expn = qa->expn + qb->expn;
			Insert(pf, hf);//����Insert�����Ժϲ�ָ����ͬ����
		}
	}
	return hf;
}

Polyn WriteFile(Polyn pa,Polyn pb)
{
	FILE *fp;
	Polyn qa = pa->next;//����qa��qb�����½ṹ���������ʱ�洢pa->next��pb->next������
	Polyn qb = pb->next;
	int i = 1, j = 1;
	if ((fp = fopen("data.txt", "w")) == NULL)//���ļ����ж��Ƿ�򿪳ɹ�
	{
		printf("�ļ���ʧ��");
		exit(0);
	}
	for (; qa; qa = qa->next)
	{
		printf("д���ļ��Ķ���ʽa��%d��ϵ����ָ���ֱ�Ϊ��%.2f  %d\n", i, qa->coef, qa->expn);//�����Ҫд��Ķ���ʽ��ϵ��
		fprintf(fp, "(%.2f, %d)", qa->coef, qa->expn);//���ļ������ʽ����ϵ����ָ��
		i++;
	}
	fprintf(fp, "\n");
	for (; qb; qb = qb->next)
	{
		printf("д���ļ��Ķ���ʽb��%d��ϵ����ָ���ֱ�Ϊ��%.2f  %d\n", j, qb->coef, qb->expn);
		fprintf(fp, "(%.2f, %d)", qb->coef, qb->expn);
		j++;
	}
	fclose(fp);
}

Polyn ReadFile()
{
	//int i,m=2;
	//Polyn p;
	//p == (Polyn)malloc(sizeof(struct Polynomial));//��̬�����ڴ� 
	//p->next = NULL;
	FILE *fp;
	Polyn pr=0;
	Polyn head1;
	if ((fp = fopen("data.txt", "r")) == NULL)
	{
		printf("�ļ���ʧ��");
		exit(0);
	}
	pr = head1 = (Polyn)malloc(sizeof(struct Polynomial));//��̬�����ڴ� 
	head1->next = NULL;
	//for (i = 0; i < m; i++)
	//{
	pr = (Polyn)malloc(sizeof(struct Polynomial));//�����½���Խ�������
	while (getchar() != '\n');
	fscanf(fp, "(%.2f,%d)", pr->coef, pr->expn);
	Insert(pr, head1);//����Insert����������
	printf("%.2f  %d", pr->coef, pr->expn);
	fclose(fp);
	//}
}

void DestroyPolyn(Polyn p) //���ٶ���ʽp
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

/*���������˵�����*/
void main()
{
	int m, n, o, a = 1;
	float x;
	char flag;
	Polyn pa = 0, pb = 0, pc;
	system("color 70");//�趨������������ɫ������16λ
	printf("         ��ӭʹ�ö���ʽ��������\n\n");
	printf("������a������:");
	o = scanf("%d", &m);//�����벻��Ҫ���������͵����ݣ�o�Ͳ�����1
	while (0 >= m || m > 20 || o != 1)//�ж��û������ֵ�Ƿ���������
	{
		printf("��������������");
		while (getchar() != '\n');//����������еĻس���
		o = scanf("%d", &m);
	};
	pa = CreatePolyn(pa, m);//��������ʽa
	printf("������b������:");
	o = scanf("%d", &n);
	while (0 >= n || n > 20 || o != 1)
	{
		printf("��������������");
		while (getchar() != '\n');//����������еĻس���
		o = scanf("%d", &n);
	};
	pb = CreatePolyn(pb, n);//��������ʽb

	//����˵�
	printf("   ********************************************************************\n");
	printf("   *                          ����ʽ��������                          *\n");
	printf("   *                                                                  *\n");
	printf("   *             A:�������ʽa            B:�������ʽb               *\n");
	printf("   *                                                                  *\n");
	printf("   *             C:���a�ĵ���            D:���b�ĵ���               *\n");
	printf("   *                                                                  *\n");
	printf("   *             E:����x��ֵ����a         F:����x��ֵ����b            *\n");
	printf("   *                                                                  *\n");
	printf("   *             G:���a+b                H:���a-b                   *\n");
	printf("   *                                                                  *\n");
	printf("   *             I:���a*b                J:�Ѷ���ʽд���ļ�          *\n");
	printf("   *                                                                  *\n");
	printf("   *             K:��ȡ�ļ��еĶ���ʽ     L:�˳�����                  *\n");
	printf("   *                                                                  *\n");
	printf("   ********************************************************************\n");

	while (a)
	{
		printf("\n��ѡ�������");
		scanf(" %c", &flag);//�ո����һ��Ҫע��,�ɽ��������ڵĿո�ȷ���Ӱ������
		switch (flag)
		{

		case'A':
		case'a':
		{
			printf("\n       ����ʽa=");
			PrintPolyn(pa);
			break;
		}
		case'B':
		case'b':
		{
			printf("\n       ����ʽb=");
			PrintPolyn(pb);
			break;
		}
		case'C':
		case'c':
		{
			pc = Derivative(pa);
			printf("\n       ����ʽa�ĵ�����Ϊ��a'=");
			PrintPolyn(pc);
			break;
		}
		case'D':
		case'd':
		{
			pc = Derivative(pb);
			printf("\n       ����ʽb�ĵ�����Ϊ��b'=");
			PrintPolyn(pc);
			break;
		}
		case'E':
		case'e':
		{
			printf("����x��ֵ��x=");
			scanf("%f", &x);
			printf("\n       x=%.2fʱ��a=%.2f\n", x, ValuePolyn(pa, x));
			break;
		}
		case'F':
		case'f':
		{
			printf("����x��ֵ��x=");
			scanf("%f", &x);
			printf("\n       x=%.2fʱ��b=%.2f\n", x, ValuePolyn(pb, x));
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
			ReadFile();//����������
			break;
		}
		case'L':
		case'l':
		{
			printf("\n       ��лʹ�ô˳���\n");
			DestroyPolyn(pa);
			DestroyPolyn(pb);
			a = 0;
			break;
		}
		default:
			printf("\n       ����ѡ�����������ѡ��!\n");
		}
	}
}