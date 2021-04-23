#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Data {
	char *no;
	int num;
};
Data* datas = NULL;
int len = 0;

void Trans(FILE *);
void Multiple(FILE *);
void ReadFile();
Data *QuickSort(int, int);
char *BinarySearch(Data *, int, int, int);

void main()
{	
	//Read Vaule
	ReadFile();

	//QuickSort
	Data *qs = QuickSort(0, len - 1);

	int q;
	printf("請輸入要尋找的數字：");
	scanf("%d", &q);

	//BinarySearch
	char *no = BinarySearch(qs, q, 0, len - 1);

	if (no != NULL)
		printf("Find %s %d\n", no, q);
	else
		printf("Cannot Find %d\n", q);

	free(qs);
	free(datas); //若不需使用該陣列，需釋放記憶體
}

char *BinarySearch(Data* qs, int q, int s, int e)
{
	if (s == e)
		return qs[s].num == q ? qs[s].no : NULL;
	else if (q <= qs[(e + s) / 2].num)
		return BinarySearch(qs, q, s, (e + s) / 2);
	else
		return BinarySearch(qs, q, (e + s) / 2 + 1, e);
}

Data *QuickSort(int s, int e)
{
	if (e - s >= 2)
	{
		int A_Len = ((e + s) / 2) - s + 1;
		int B_Len = e - ((e + s) / 2 + 1) + 1;
		int C_Len = A_Len + B_Len;
		Data* A = QuickSort(s, s + A_Len - 1);
		Data* B = QuickSort(e - B_Len + 1, e);
		Data* C = (Data *)malloc(C_Len * sizeof(Data));
		int a_idx = 0, b_idx = 0, c_idx = 0;
		do
		{
			if (b_idx == -1 || (a_idx != -1 && A[a_idx].num <= B[b_idx].num))
			{
				C[c_idx].no = A[a_idx].no;
				C[c_idx++].num = A[a_idx].num;
				a_idx = (a_idx + 1 < A_Len ? a_idx + 1 : -1);
			}
			else if (a_idx == -1 || (b_idx != -1 && A[a_idx].num > B[b_idx].num))
			{
				C[c_idx].no = B[b_idx].no;
				C[c_idx++].num = B[b_idx].num;
				b_idx = (b_idx + 1 < B_Len ? b_idx + 1 : -1);
			}
		} while (a_idx != -1 || b_idx != -1);

		free(A);
		free(B);
		return C;
	}
	else if (e - s == 1)
	{
		Data* tmp = (Data *)malloc(2 * sizeof(Data));
		tmp[0].no = datas[datas[s].num < datas[e].num ? s : e].no;
		tmp[0].num = datas[datas[s].num < datas[e].num ? s : e].num;
		tmp[1].no = datas[datas[s].num < datas[e].num ? e : s].no;
		tmp[1].num = datas[datas[s].num < datas[e].num ? e : s].num;
		return tmp;
	}
	else
	{
		Data* tmp = (Data *)malloc(1 * sizeof(Data));
		tmp[0].no = datas[s].no;
		tmp[0].num = datas[s].num;
		return tmp;
	}
	return NULL;
}

void ReadFile()
{
	//開啟檔案
	FILE* fptr = fopen("C:\\Users\\user\\Downloads\\03(HW3)-Sort_BSearch\\Input4.txt", "r");

	//動態配置記憶體並賦值
	char no[100], num[100];
	while (fscanf(fptr, "%s %s", no, num) != -1) //讀取每行序號及數值
	{
		len++;
		if (len != 1)
		{
			//第二次以上進來再重新配置記憶體
			datas = (Data *)realloc(datas, len * sizeof(Data));
		}
		else
		{
			//第一次進來先配置記憶體
			datas = (Data *)malloc(len * sizeof(Data));
		}

		//因序號為字串，故須宣告字元陣列紀錄序號
		char *ptr = (char *)malloc((strlen(no) + 1) * sizeof(char));
		int i = 0;
		for (i = 0; i <= strlen(no); i++)
			ptr[i] = no[i];

		datas[len - 1].no = ptr; //將序號的字元陣列指標設給nos第len-1個位置
		datas[len - 1].num = atoi(num); //將數值設給nums第len-1個位置
	}

	fclose(fptr); //若不需使用該檔案，需釋放記憶體
}

void Mat()
{
	//開啟指定路徑之檔案
	FILE* fptr = fopen("C:\\Users\\user\\Desktop\\C++\\input04.txt", "r");

	char type[100];
	fscanf(fptr, "%s", type); //讀取第一行字串

	//判斷是要做何種動作
	if (strcmp(type, "Matrix_Transpose") == 0)
	{
		Trans(fptr); //如果是Matrix_Transpose，則執行Trans
	}
	else if (strcmp(type, "Matrix_Multiplication") == 0)
	{
		Multiple(fptr); //如果是Matrix_Multiplication，則執行Multiple
	}

	fclose(fptr);
}

void Multiple(FILE* fptr)
{
	int m[100][100], mr = 0, mc = 0, mri = 0, mci = 0; //定義第一個矩陣、及列數、行數之變數
	int n[100][100], nr = 0, nc = 0, nri = 0, nci = 0; //定義第二個矩陣、及列數、行數之變數
	char temp[100];

	fscanf(fptr, "%s %s %s", temp, temp, temp); //將第二行字吃掉，使檔案指標跑到第三行
	//讀取矩陣數字，並存到第一個矩陣m
	while (!feof(fptr))
	{
		fscanf(fptr, "%s", temp);
		if (temp[0] == ';') //判斷是否讀到列的尾巴
		{
			mri++;
			mc = mci;
			mci = 0;
		}
		else if (temp[0] == 'N') //判斷是否讀到Name...
		{
			fscanf(fptr, "%s %s", temp, temp); //吃掉剩餘的字，使檔案指標跑到第一列數字的起始
			break;
		}
		else if (temp[0] != 'E') //判斷是否讀到End_...
		{
			m[mri][mci] = atoi(temp);
			mci++;
		}
	}
	mr = mri;

	//讀取矩陣數字，並存到第二個矩陣m
	while (!feof(fptr))
	{
		fscanf(fptr, "%s", temp);
		if (temp[0] == ';') //判斷是否讀到列的尾巴
		{
			nri++;
			nc = nci;
			nci = 0;
		}
		else if (temp[0] != 'E') //判斷是否讀到End_...
		{
			n[nri][nci] = atoi(temp);
			nci++;
		}
		else if (temp[0] == 'E') //判斷是否讀到End_...
		{
			break;
		}
	}
	nr = nri;

	//矩陣相乘
	for (int i = 0; i < mr; i++)
	{
		for (int j = 0; j < nc; j++)
		{
			int sum = 0;
			for (int k = 0; k < mc; k++)
			{
				sum += m[i][k] * n[k][j];
			}
			printf("%4d ", sum);
		}
		printf("\n");
	}
}

void Trans(FILE* fptr)
{
	int m[100][100], r = 0, c = 0, ri = 0, ci = 0; //定義矩陣、及列數、行數之變數
	char temp[100];

	fscanf(fptr, "%s %s %s", temp, temp, temp); //將第二行字吃掉，使檔案指標跑到第三行
	while (!feof(fptr))
	{
		fscanf(fptr, "%s", temp);
		if (temp[0] == ';') //判斷是否讀到列的尾巴
		{
			ri++;
			c = ci;
			ci = 0;
		}
		else if (temp[0] != 'E') //判斷是否讀到End_...
		{
			m[ri][ci] = atoi(temp);
			ci++;
		}
	}
	r = ri;

	//矩陣轉置
	for (int j = 0; j < c; j++)
	{
		for (int i = 0; i < r; i++)
		{
			printf("%4d ", m[i][j]);
		}
		printf("\n");
	}
}