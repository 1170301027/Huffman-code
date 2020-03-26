
/*
哈夫曼编码与译码<1170301027fs>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 1100
#define min 50
#define element char
//Huffman树类型
typedef struct Huffman
{
    element data;
    int count;//权值
    int parent;
    int lchild;
    int rchild;
}HuffmanTree,*btpointer;
//编码树
typedef struct Code
{
    element ch;
    element data[min];
}CodeTree;
//数据链表
typedef struct List
{
    element data;//文章中出现的字符
    int count;//出现次数
    struct List *next;
    element code[min];//编码
}*listpointer;
//新建二叉树
btpointer newbt()
{
    btpointer a;
    a = (btpointer)malloc(sizeof(struct Huffman));
    a->lchild = a->rchild = NULL;
    return a;
}
//新建权重元素链
void newnode(listpointer ls,element data)
{
    listpointer a;
    a = (listpointer)malloc(sizeof(struct List));
    a->count = 1;//出现次数初始化
    a->data = data;
    a->next = NULL;
    ls->next = a;
}
//添加元素
void Attch(listpointer a,element data)
{
    int flag = 1;
    listpointer temp;
    while(a!=NULL)
    {
        if(a->data == data)
        {
            flag = 0;
            a->count++;
            break;
        }
        temp = a;
        a = a->next;
    }
    if(flag)
    {
        newnode(temp,data);
    }
}
//字符串处理及权值分配，
void DealWeight(listpointer a,element data[])
{
    int i;
    for(i = 1;data[i]!='\0';i++)
    {
        Attch(a,data[i]);
    }
}
//读文件,
void ReadFile(element data[])
{
	FILE *fp;
	int n = 0;
	if ((fp = fopen("C:\\Users\\冯帅\\desktop\\1.txt", "r")) != NULL)//以只读方式打开文件并判断是否打开,并输出相应语句
	{
		printf("the file 1 was opened\n");
		while(!feof(fp)){
		fscanf(fp,"%c",&data[n]);
		n++;
		}
		data[n] = '\0';
		printf("succeed to read to array\n\n");
	}
	else printf("the file 1 was not opened\n\n");
	fclose(fp);
}

//显示各字符和权值和编码
void Display(listpointer a)
{
    while(a!=NULL)
    {
        printf("\n%c\t%d\t%s\n",a->data,a->count,a->code);
        a = a->next;
    }
}
//从大到小排序链表中的元素
void Sort(listpointer a)
{
    listpointer b;//替代品
    element data;
    int count;
    while(a!=NULL)
    {
        b = a->next;
        while(b!=NULL){
            if(a->count < b->count)
            {
                data = b->data;
                b->data = a->data;
                a->data = data;
                count = b->count;
                b->count = a->count;
                a->count = count;
            }
            b = b->next;
        }
        a = a->next;
    }
}
//选最小两个数
void SelectMin(HuffmanTree T[],int n,int *min1,int *min2)
{
    int temp;
    int i = 0;
    while(T[i].parent!=-1) i++;
    *min1 = i++;
    while(T[i].parent!=-1) i++;
    *min2 = i++;
    for(;i<=n;i++)
    {
        if(T[i].parent!=-1) continue;
        if(T[*min1].count<T[*min2].count){ temp = *min1; *min1 = *min2; *min2 = temp;}
        if(T[*min2].count>T[i].count)//T[i]三个数中最小
        {
            *min1 = *min2;
            *min2 = i;
        }
        else if(T[*min1].count>T[i].count)//T[i]在三个数中间
        {
            *min1 = i;
        }
        else continue;//T[i]在三个数中最大
    }
}
//建立哈夫曼树
void CreateHT(HuffmanTree T[],int n)
{
    int p1=0,p2=1;//存储最小权值下标
    for(int i = n;i<2*n-1;i++)
    {
        SelectMin(T,i-1,&p1,&p2);
        T[p1].parent = T[p2].parent = i;
        T[i].lchild = p1;
        T[i].rchild = p2;
        T[i].count = T[p1].count + T[p2].count;
    }
}
void CodeCalculate(HuffmanTree T[],listpointer ls,int n)
{
    int i = 0,start,c,p;
    element code[n+1];
    code[n] = '\0';
    while(ls!=NULL){
        i =0;
        start = n;
        while(i<n){
            if(T[i].data == ls->data)
            {
                c = i;
                while((p = T[c].parent)!=-1){
                    code[--start] = (T[p].lchild==c)?'0':'1';
                    c = p;
                }
                strcpy(ls->code,&code[start]);//从编码末尾位置开始复制到编码链表中
                break;
            }
            i++;
        }
        ls = ls->next;
    }
}
//哈弗曼编码后写入文件
void WriteFile(element data[],listpointer ls)
{
	FILE *fp;
	int n = 0;
	listpointer temp;
	if ((fp = fopen("C:\\Users\\冯帅\\desktop\\2.txt", "w+")) != NULL)//以只读方式打开文件并判断是否打开,并输出相应语句
	{
		printf("the file 1 was opened\n");
		while(data[n]!='\0'){
		    temp = ls;
            while(temp!=NULL){
                if(temp->data == data[n])
                {
                    fprintf(fp,"%s",temp->code);
                    break;
                }
                temp = temp->next;
            }
            n++;
		}
		printf("succeed to write to file\n\n");
	}
	else printf("the file 1 was not opened\n\n");
	fclose(fp);
}
//译码
void Coding(HuffmanTree T[],int n,element passage[])
{
	FILE *fp;
	int i = 0,c,count = 0,l = 0,length;//压缩率计算
	element ch;
	if ((fp = fopen("C:\\Users\\冯帅\\desktop\\2.txt", "r")) != NULL)//以只读方式打开文件并判断是否打开,并输出相应语句
    {
        printf("the file 2 was opened\n");
        while(!feof(fp)){
            c = 2*n-2;//根节点
            /*T[c].lchild!=-1 && T[c].rchild!=-1*/
            while(c>=n && !feof(fp)){
                ch = fgetc(fp);
                count++;
                if(count == 8) {l++;count = 0;}//八位二进制算一个字节
                if(ch == '0') c = T[c].lchild;
                else c = T[c].rchild;
            }
            if(!feof(fp)) {
                passage[i] = T[c].data;
                i++;
            }
        }
        passage[i] = '\0';
        length = strlen(passage);
        printf("\n压缩率为：%f%%\n\n",(float)l/length*100);
        printf("succeed to read to array\n\n");
	}
	else printf("the file 2 was not opened\n");
	fclose(fp);
}
int main()
{
    element data[max];//用于存储录入的字符
    int n=0;//用于存储不同字符数量
    ReadFile(data);//读入
    printf("%s",data);//显示
    listpointer a,temp;
    a = (listpointer)malloc(sizeof(struct List));
    a->next = NULL;
    a->data = data[0];
    a->count = 1;//初始化
    DealWeight(a,data);//权值处理
    Sort(a);//排序(大到小)
    temp = a;
    while(temp!=NULL)//遍历算不同元素个数
    {
        n++;
        temp = temp->next;
    }
    HuffmanTree huffmanT[2*n-1];//huffman树初始化
    temp = a;
    int i;
    for(i=0;temp!=NULL;i++)//数组初始化
    {
        huffmanT[i].data = temp->data;
        huffmanT[i].count = temp->count;
        huffmanT[i].lchild = huffmanT[i].rchild = huffmanT[i].parent = -1;
        temp = temp->next;
    }
    while( i<2*n-1){ huffmanT[i].lchild = huffmanT[i].rchild = huffmanT[i].parent = -1;i++;}
    CreateHT(huffmanT,n);
    CodeCalculate(huffmanT,a,n);
    Display(a);//展示
    WriteFile(data,a);//编码后写入文件
    element passage[max];//用于存储译码后文章
    Coding(huffmanT,n,passage);//译码
    printf("%s",passage);
    return 0;

}

