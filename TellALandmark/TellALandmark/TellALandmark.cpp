/**
 *
 * Email: imsrch@melory.me
 * Date: 2012/1/11
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

#include "fast_kmeans.h"

using namespace std;

int main()
{
	FILE *ALL_KPS, *KPS_INFO, *KEY;
	char *fname1 = "ALL_KPS";  //������е�keypoints
	char *fname2 = "KPS_INFO"; //���keypoints�������Ϣ
	int N = 50; //ÿ����Ƭ�������Ŀ
	char image_name[5][20] = {"caozhu", "maoxiang", "shengyilou", "tushuguan", "zhengmen"};
	int image_number[5];
	int image_feature[5][100]; //image_featurep[0][34]����˼��
								//����caozhu����Ƭ��ȡ������keypoints��
								//��234�������ڵ�34����������
	char fname_key[20];
	int number, i, j, k, start, end = 0, _start[5], _end[5];
	double discard[4];


	if(NULL == (ALL_KPS = fopen(fname1, "w")))
	{
		printf("openning file 'ALL_KPS' failed!");
		return 1;
	}
	if(NULL == (KPS_INFO = fopen(fname2, "w")))
	{
		printf("openning file 'KPS_INFO' failed!");
		return 1;
	}

 /* ����һ�γ����������ͼƬ��Ӧ��keypoints�ļ�(*.key)�� ��������Щ�ļ�������д��һ��
 * ALL_KPS�ļ��У�ÿ��keypointΪһ�С�
 * 
 * ֮ǰ��ȡ��keypoints����128ά��֤����������Ҳ������ÿ��keypoint�ķ������Ϣ��
 * ����*.key�����ÿһ��keypoint��ÿһ�У���128 + 4 = 132����ֵ��ǰ4��λ�����
 * ��Ϣ�� �����128���������Ǹ���keypoint����ȡ������ֻ��ÿ�������128��������
 *
 * �����е�keypoints�����е���ȡ����������Ҫ��¼ÿ��keypoint��������һ��ͼƬ��
 * ����Щ��Ϣд��һ��KP_INFO���ļ��У������ĸ�ʽΪ��
 *	  	��Ƭ����� ��ALL_KPS����ʼ���� ��ALL_KPS����ֹ����
 * ����˵��ÿһ�����Ƭ��keypoints���еط���һ��һ��ALL_KPS�ļ������ݿ�����
 * ���������ģ�
 *		caozhu 1 7812
 *		tushuguang 7813 12331
 *		zhengmen 12332 18905
 *		maoxiang 18906 24966
 **/

	printf("��ȡ���е�keypoints!\n");
	for(i = 0; i< 5; i++)  //���εض�ȡÿ��ͼ���key�ļ�
	{
		image_number[i] = 0;
		start = end + 1;
		for(j = 0; j <= N; j++) //ÿ��ͼ�����Ŀ
		{
			sprintf(fname_key, "%s (%d).key", image_name[i], j); //��ȡÿͼ���Ӧ��key�ļ�
			printf("%s\n", fname_key);
			if(NULL != (KEY = fopen(fname_key, "r")))
			{
				image_number[i]++;
				while(EOF != fscanf(KEY, "%lf%lf%lf%lf", &discard[0], &discard[1], &discard[2], &discard[3]))
				{
					for(k = 1; k <= 128; k++)
					{
						if(EOF != fscanf(KEY, "%d", &number))
							fprintf(ALL_KPS, "%d ", number);
					}
					fprintf(ALL_KPS, "\n");
					end++;
				}
				fclose(KEY);
			}
			else
				printf("openning file '%s' failed!\n", fname_key);
		}
		if(start > end){
			printf("satrt > end on image: %s", image_name[i]);
			return 1;
		}
		fprintf(KPS_INFO, "%s %d %d\n", image_name[i], start, end);
		_start[i] = start;
		_end[i] = end;
	}	
	fclose(ALL_KPS);
	fclose(KPS_INFO);

	printf("׼�����о���!\n");
	//���о������
	//int dataSize = 1000;
	int dataSize;  //�����ĸ���
	int feat_dim = 128;  //������ά��
	int ncluster = 100;  //k-means�е�k
	//char *in_data_file = "data";
	char *in_data_file = fname1; //�����ļ�Ϊ���е�keypoints
	char *out_cluster_file = "CENTER";  //����֮�����������
	char *out_assign_file = "ASSIGNMENT";  //������������һ�����Ϣ������
											//˵ASSIGNMENT�ļ���i������Ϊj
											//��ʾ��i�������֮��Ӧ�����ڵ�
											//j��
	dataSize = end;
	//data and clusters
	vector< vector<double> > data(dataSize);
	vector< vector<double> > clusters;

	//reading data
	ifstream infile;
	infile.open(in_data_file);
	for (int j = 0; j < dataSize; j++)
	{
		data[j].resize(feat_dim);
		for (int dim = 0; dim < feat_dim; dim++)
		{
			infile>>data[j][dim];
		}
	}
	infile.close();

	vector<int> Assignment(dataSize);

	printf("��ʼ���о���!\n");
	//kmeans
	kmeans(&data, &clusters, &Assignment, ncluster, 500, TRUE);
	
	
	//output cluster file and assignment file
	ofstream outfile;
	outfile.open(out_cluster_file);
	for (i = 0; i < ncluster; i++)
	{
		for (int dim = 0; dim < feat_dim; dim++)
		{
			outfile<<clusters[i][dim]<<" ";
		}
		outfile<<endl;
	}
	outfile.close();
	outfile.clear();


	outfile.open(out_assign_file);
	for (j = 0; j < dataSize; j++)
	{
		outfile<<Assignment[j]+1<<" ";
	}
	outfile.close();
	outfile.clear();



	printf("��ȡÿһ����Ƭ��feature����!\n");
	//��ȡÿһ����Ƭ��feature����
	//ͳ��ÿһ����Ƭ����������feature��������image��keypoint����k-means����֮�󣬵õ�
	//k��center��ͳ��ÿһ����Ƭ�ж���keypoints����ÿһ��center��
	infile.open(out_assign_file);  //����'ASSIGNMENT'�ļ�
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 100; j++)
			image_feature[i][j] = 0;

		for (int j = _start[i]; j <= _end[i]; j++)
		{
			infile>>number;
			image_feature[i][number]++;
		}
		for (int j = 0; j < 100; j++)  
			if(image_number[i] > 0)
				image_feature[i][j] = image_feature[i][j] / image_number[i];
			else
			{
				printf("%s��ͼƬ����Ϊ0��\n", image_name[i]);
				//return 1;
			}
	}
	infile.close();
	//�����������ļ���
	outfile.open("IMAGE_FEATURES");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			outfile<<image_feature[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile.close();
	outfile.clear();



	//�����ر꾰��ͼ���ѵ����������ȡ�����Ѿ����
	//������ͼƬ��������Ϣ�������������'IMAGE_FEATURE'�ļ���
	//����ѡ���˳����������Ԥ��һ��ͼ���λ�ã�
	printf("�����ر꾰��ͼ���ѵ����������ȡ�����Ѿ����\n");
	printf("������ͼƬ��������Ϣ�������������'IMAGE_FEATURE'�ļ���\n");
	printf("����ѡ���˳����������Ԥ��һ��ͼ���λ��\n");
	printf("�˳������밴0\nԤ�⾰��ͼ���밴1\n");
	scanf("%d", &number);
	if(number == 0) return 0;
	if(number != 0 && number != 1)
	{
		printf("������󣬳����˳�!\n");
		return 1;
	}
	if(number == 1)
	{
		printf("������ͼ��keypoint��Ϣ��Ӧ��.key�ļ�!\n");
		char name[50];
		double center[100][128];
		int image_feature[5][100];
		int feature[100];
		int one_key[128];

		for(int j = 0; j < 100; j++)
			feature[j] = 0;

		scanf("%s", name);

		//�ȶ���CENTER
		printf("��ʼ����CENTER�ļ�...\n");
		infile.open(out_cluster_file);
		if(!infile.is_open())
		{
			printf("ָ�����ļ��򿪴���!\n");
			return 1;
		}
		for(int j = 0; j < 100; j ++)
			for(int k = 0; k < 128; k++)
			{
				infile>>center[j][k];
			}
			infile.close();

		//Ȼ�����IMAGE_FEATURE
		printf("��ʼ����IMAGE_FEATURE�ļ�...\n");
		infile.open("IMAGE_FEATURES");
		if(!infile.is_open())
		{
			printf("ָ�����ļ��򿪴���!\n");
			return 1;
		}
		for(int j = 0; j < 5; j ++)
			for(int k = 0; k < 100; k++)
			{
				infile>>image_feature[j][k];
			}
			infile.close();

		infile.open(name);
		if(!infile.is_open())
		{
			printf("ָ�����ļ��򿪴���!\n");getchar();
			return 1;
		}

		printf("��ʼ����ÿ��keypoints��������һ��...\n");
		double cur_mindist = 10.0e20;
		int cur_minnumber = 0;
		int counter = 0;
		float temp;
		while(!infile.eof())  //�ж�ÿһ��keypoint��������һ��center�����
		{
			//��Ҫ��ǰ����ĸ�����������
			infile>>discard[0];
			infile>>discard[1];
			infile>>discard[2];
			infile>>discard[3];
			for(int j = 0; j < 128; j++)
				infile>>one_key[j];
			printf("�����%d��keypoints...\n", ++counter);
			double dist;
			for(int l = 0; l < 100; l++)
			{
				dist = 0.0;
				for (int k = 0; k < 128; k++)
					dist += (one_key[k] - center[l][k]) * (one_key[k] - center[l][k]);
				if(dist < cur_mindist)
				{
					cur_mindist = dist;
					cur_minnumber = l;  //l����1
				}
			}

			feature[cur_minnumber]++;
		}
		infile.close();

		//���������ͼ���feature֮������image_feature��ȣ��ж�����һ��ͼ��������
		//�Ӷ��ж���������һ��

		printf("��ʼ��������ͼ��������������Ϣ���жϳ��������...\n");
		double dist;
		cur_mindist = 10.0e20;
		cur_minnumber = 0;
		for(int j = 0; j < 5; j++)
		{
			dist = 0.0;
			for(int k = 0; k < 100; k++)
				dist += (feature[k] - image_feature[j][k]) * (feature[k] - image_feature[j][k]);
			if(dist < cur_mindist)
			{
				cur_mindist = dist;
				cur_minnumber = j; 
			}
		}
		printf("����ͼ��������ǣ� %s!\n", image_name[cur_minnumber]);
	}

	getchar();getchar();
	return 0;
}