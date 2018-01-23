#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <Eigen/Eigen>

#define k 3//簇的数目
#define m 1
using namespace std;
//存放元组的属性信息
typedef vector<double> Tuple;//存储每条数据记录

int dataNum;//数据集中数据记录数目
int dimNum;//每条记录的维数

//计算两个元组间的欧几里距离
double getDistXY(const Tuple& t1, const Tuple& t2) {
	double sum = 0;
	for(int i=1; i<=dimNum; ++i)
	{
		sum += (t1[i]-t2[i]) * (t1[i]-t2[i]);
	}
	return sqrt(sum);
}

//根据质心，决定当前元组属于哪个簇
int clusterOfTuple(Tuple means[],const Tuple& tuple){
	double dist=getDistXY(means[0],tuple);
	double tmp;
	int label=0;//标示属于哪一个簇
	for(int i=1;i<k;i++){
		tmp=getDistXY(means[i],tuple);
		if(tmp<dist) {dist=tmp;label=i;}
	}
	return label;	
}
//获得给定簇集的平方误差
double getVar(vector<Tuple> clusters[],Tuple means[]){
	double var = 0;
	for (int i = 0; i < k; i++){
		vector<Tuple> t = clusters[i];
		for (int j = 0; j< t.size(); j++){
			var += getDistXY(t[j],means[i]);
		}
	}
	//cout<<"sum:"<<sum<<endl;
	return var;
}
//获得当前簇的均值（质心）
Tuple getMeans(const vector<Tuple>& cluster){
	int num = cluster.size();
	Tuple t(dimNum+1, 0);
	for (int i = 0; i < num; i++){
		for(int j=1; j<=dimNum; ++j)
		{
			t[j] += cluster[i][j];
		}
	}
	for(int j=1; j<=dimNum; ++j)
		t[j] /= num;
	return t;
	//cout<<"sum:"<<sum<<endl;
}

void print(const vector<Tuple> clusters[]){
	for(int lable=0; lable<k; lable++)
	{
		cout<<"第"<<lable+1<<"个簇："<<endl;
		vector<Tuple> t = clusters[lable];
		for(int i=0; i<t.size(); i++)
		{
			cout<<i+1<<".(";
			for(int j=0; j<=dimNum; ++j)
			{
				cout<<t[i][j]<<", ";
			}
			cout<<")\n";
		}
	}
}

void KMeans(vector<Tuple>& tuples){
	vector<Tuple> clusters[k];//k个簇
    Tuple means[10][k];//k个中心点
    int i=0,lable=0;
	//一开始随机选取k条记录的值作为k个簇的质心（均值）
	srand((unsigned int)time(NULL));
	for(i=0;i<k;){
		int iToSelect = rand()%tuples.size();
		if(means[0][iToSelect].size() == 0){
			for(int j=0; j<dimNum; ++j)
			{
				means[0][i].push_back(tuples[iToSelect][j]);
			}
			i++;
		}
	} 
	//根据默认的质心给簇赋值
	for(i=0;i<tuples.size();i++){
		lable=clusterOfTuple(means[0],tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
    
	double oldVar=getVar(clusters,means[0]);

    //更新每个簇的中心点
    for (i = 0; i < k; i++) {
			means[1][i] = getMeans(clusters[i]);
	}
	
	for (i = 0; i < k; i++) //清空每个簇
		{
			clusters[i].clear();
		}

    //根据默认的质心给簇赋值
	for(i=0;i<tuples.size();i++){
		lable=clusterOfTuple(means[1],tuples[i]);
		clusters[lable].push_back(tuples[i]);
	}
    
	double newVar=getVar(clusters,means[1]);

    //更新每个簇的中心点
    for (i = 0; i < k; i++) {
			means[2][i] = getMeans(clusters[i]);
	}
	
	for (i = 0; i < k; i++) //清空每个簇
		{
			clusters[i].clear();
		}

    double f[10][10][3],g[10][10][3],gamma,x[10][10][3],t_f[10][10][3],t_g[10][10][3];
    int m_k,mark=0;
    int p;
	for(p=0;p<2;p++){
    	for(int q=0 ; q<2; q++){
			f[p][0][q]=means[p+1][0][q+1]-means[p][0][q+1];
			f[p][1][q]=means[p+1][1][q+1]-means[p][1][q+1];
			f[p][2][q]=means[p+1][2][q+1]-means[p][2][q+1];
			g[p][0][q]=means[p+1][0][q+1];
			g[p][1][q]=means[p+1][1][q+1];
			g[p][2][q]=means[p+1][2][q+1];
			x[p][0][q]=means[p][0][q+1];
			x[p][1][q]=means[p][1][q+1];
			x[p][2][q]=means[p][2][q+1];
    	}
	}
	if(abs(newVar - oldVar) <= 1){	
		cout<<"The result is:\n";
		print(clusters);
		return;
	}
    for(p=0; ;p++){
        m_k=min(m,p);
        for(int q=0 ; q<2; q++){
            f[p][0][q]=means[p+1][0][q+1]-means[p][0][q+1];
            f[p][1][q]=means[p+1][1][q+1]-means[p][1][q+1];
            f[p][2][q]=means[p+1][2][q+1]-means[p][2][q+1];
        }
        for(int q=0 ; q<2; q++){
            g[p][0][q]=means[p+1][0][q+1];
            g[p][1][q]=means[p+1][1][q+1];
            g[p][2][q]=means[p+1][2][q+1];
        }
        for(int q=0 ; q<2; q++){
            t_f[p][0][q]=f[p+1][0][q]-f[p][0][q];
            t_f[p][1][q]=f[p+1][1][q]-f[p][1][q];
            t_f[p][2][q]=f[p+1][2][q]-f[p][2][q];
        }
        for(int q=0 ; q<2; q++){
            t_g[p][0][q]=g[p+1][0][q]-g[p][0][q];
            t_g[p][1][q]=g[p+1][1][q]-g[p][1][q];
            t_g[p][2][q]=g[p+1][2][q]-g[p][2][q];
        }
        if(p==0) continue;

        gamma=0.5;//get_min();
    	x[p+1][0][0]=g[p][0][0]-t_g[p-1][0][0]*gamma;
		x[p+1][0][1]=g[p][0][1]-t_g[p-1][0][1]*gamma;
		x[p+1][1][0]=g[p][1][0]-t_g[p-1][1][0]*gamma;
		x[p+1][1][1]=g[p][1][1]-t_g[p-1][1][1]*gamma;
		x[p+1][2][0]=g[p][2][0]-t_g[p-1][2][0]*gamma;
		x[p+1][2][1]=g[p][2][1]-t_g[p-1][2][1]*gamma;
		means[p+1][0][1]=x[p+1][0][0];
		means[p+1][0][2]=x[p+1][0][1];
		means[p+1][1][1]=x[p+1][1][0];
		means[p+1][1][2]=x[p+1][1][1];
		means[p+1][2][1]=x[p+1][2][0];
		means[p+1][2][2]=x[p+1][2][1];

		for (i = 0; i < k; i++) //清空每个簇
		{
			clusters[i].clear();
		}
		//更新每个簇的中心点

		//根据默认的质心给簇赋值
		for(i=0;i<tuples.size();i++){
			lable=clusterOfTuple(means[p],tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters,means[p+1]); //计算新的准则函数值

		if(abs(newVar - oldVar) <= 1){	
			cout<<"The result is:\n";
			print(clusters);
			return;
		}
    }
    
    // cout<<"聚点为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<means[0][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    // cout<<"聚点为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<means[1][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    // cout<<"聚点为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<means[2][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    
    // cout<<"f为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<f[0][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    // cout<<"f为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<f[1][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    
    // cout<<"g为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<g[0][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
    // cout<<"g为："<<endl;
    // for(int i=0; i<k; i++){
    //     cout<<i+1<<".(";
    //     for(int j=0; j<dimNum; ++j)
    //     {
    //         cout<<g[1][i][j]<<", ";
    //     }
    //     cout<<")\n";
	// }
}
int main(){

	char fname[256]="test";
    dimNum=3;
    dataNum=150;
	ifstream infile(fname);
	if(!infile){
		cout<<"不能打开输入的文件"<<fname<<endl;
		return 0;
	}
	vector<Tuple> tuples;
	//从文件流中读入数据
	for(int i=0; i<dataNum && !infile.eof(); ++i)
	{
		string str;
		getline(infile, str);
		istringstream istr(str);
		Tuple tuple(dimNum+1, 0);//第一个位置存放记录编号，第2到dimNum+1个位置存放实际元素
		tuple[0] = i+1;
		for(int j=1; j<=dimNum; ++j)
		{
			istr>>tuple[j];
		}
		tuples.push_back(tuple);
	}

	cout<<endl<<"开始聚类"<<endl;
	
	KMeans(tuples);
	return 0;
}
