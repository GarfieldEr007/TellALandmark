/**
 * ���֮ǰ�Ѿ��Ը���landmarkͼ���������ȡ��ϣ��Ѿ��õ�
 * IMAGE_FEATURE�ļ���
 * �����ø��������ж�һ����Ƭ��������һ��landmark
 * 
 * �ó�����Ҫ��ȡ֮ǰ������IMAGE_FEATURE�ļ�(����landmark��ͼ��feature
 * ������)��CENTER�ļ�������ѵ��ͼ���keypoints�ľ������ģ���������һ��ͼ��
 * ��SIFT������keypoints�ļ�.key������Ĭ����keypoint��132ά����ǰ���
 * �ĸ�������keypoints�ķ����������Ϣ����.key�ļ�ֻ��128ά��keypoint
 * ���ݣ���Ҫ�Գ�����΢�޸�һ�£�
 *
 * Author: huangmeilong
 * Email: meilonghuang@zju.edu.cn
 * Date: 2012/1/11
 **/

#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>


using namespace std;

int main()
{
	char image_name[5][20] = {"caozhu", "maoxiang", "shengyilou", "tushuguan", "zhengmen"};
	double discard[4];
	char *out_cluster_file = "CENTER";  //����֮�����������
	ifstream infile;
	ofstream outfile;
	
	char name[50];
	double center[100][128];
	int image_feature[5][100];
	int feature[100];
	int one_key[128];

	cout<<"������ͼ��keypoint��Ϣ��Ӧ��.key�ļ�!"<<endl;
	for(int j = 0; j < 100; j++)
		feature[j] = 0;

	//scanf("%s", name);
	cin>>name;

	//�ȶ���CENTER
	cout<<"��ʼ����CENTER�ļ�..."<<endl;
	infile.open(out_cluster_file);
	if(!infile.is_open())
	{
		cout<<"���ļ�����!������'CENTER'�ļ�!"<<endl;getchar();getchar();
		return 1;
	}
	for(int j = 0; j < 100; j ++)
		for(int k = 0; k < 128; k++)
		{
			infile>>center[j][k];
		}
		infile.close();

	//Ȼ�����IMAGE_FEATURE
	cout<<"��ʼ����IMAGE_FEATURE�ļ�..."<<endl;
	infile.open("IMAGE_FEATURES");
	if(!infile.is_open())
	{
		cout<<"���ļ�����!������'IMAGE_FEATURES'�ļ�"<<endl;getchar();getchar();
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
		cout<<"ָ����.key�ļ��򿪴���!��ȷ���ļ�����!"<<endl;getchar();getchar();
		return 1;
	}

	cout<<"��ʼ����ÿ��keypoints��������һ��..."<<endl;
	double cur_mindist = 10.0e20;
	int cur_minnumber = 0;
	int counter = 0;
	float temp;
	while(!infile.eof())  //�ж�ÿһ��keypoint��������һ��center�����
	{
		//��Ҫ��ǰ����ĸ����������ߣ���.key�ļ������ݽ���
		//��128ά��keypoint��Ϣ����ÿһ�н�����128����������
		//��Ҫ��һ�����д���ע�͵�
		infile>>discard[0];
		infile>>discard[1];
		infile>>discard[2];
		infile>>discard[3];
		for(int j = 0; j < 128; j++)
			infile>>one_key[j];
		counter++;
		cout<<"�����"<<counter<<"��keypoints..."<<endl;
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

	cout<<"��ʼ��������ͼ��������������Ϣ���жϳ��������..."<<endl;
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
	cout<<"����ͼ��������ǣ�"<<image_name[cur_minnumber]<<"!"<<endl;

	getchar();getchar();
	return 0;
}