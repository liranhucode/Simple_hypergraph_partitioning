#include <algorithm>

#include "checkhgraph.h"
#include "hgraph.h"

/*检查超图是否符合规则*/
void check_hgraph(s_hgraph* hgraph) {

	check_vertex(hgraph);
	check_hyperedge(hgraph);
	check_dependent_hyperedge(hgraph);
}



/*检查超边数组中是否包含了 {1 - nvtxs}中所有的顶点*/
void check_vertex(s_hgraph *hgraph)
{

	int i, n, k;
	vector<int> vtx(hgraph->nvtxs, 0);

	for (i = 0; i < hgraph->eind.size(); ++i) {
		k = hgraph->eind[i];

		vtx[k - 1] ++;
	}


	for (i = 0; i < hgraph->nvtxs; ++i) {

		if (vtx[i] == 0) {
			cout << hgraph->nvtxs;
			cout << " vertex " << i + 1 << " not apperance in hyperedge " << endl;
		}
	}
}


/*检查超边数组中是否包含了 {1 - nvtxs}中所有的顶点*/
void check_hyperedge(s_hgraph *hgraph)
{

	int i, n, k;
	vector<int> vtx(hgraph->nvtxs, 0);

	for ( n = 0; n < hgraph->nhedges; ++n ) {
		
		if (hgraph->eptr[n + 1] - hgraph->eptr[n] < 2) {
			cout << " hyperedge " << n << " is single hyperedge " << endl;
		}
	}


}

void check_dependent_hyperedge(s_hgraph * hgraph)
{
	int n;

	for (n = 0; n < hgraph->nhedges; ++n) {
		if (hgraph->is_dependent_hyperedge(n))
		{

			//cout << " hyperedge " << n << " is dependent hyperedge " << endl;
		}
	}

}

/*检查顶点的fs te与实际值是否一致*/
int check_fs_and_te(s_hgraph * hgraph, int higain)
{
	int n, i, k, j;
	int fs = 0;
	int te = 0;

	vector<int> adjcent = hgraph->incidence_hyperedge[higain-1];

	for (j = 0; j < adjcent.size(); ++j)
	{
		n = adjcent[j];

		vector<int> count(2,0);

		for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; i++)
		{

			k = hgraph->eind[i];

			count[hgraph->where[k - 1]]++;

		}

		if (count[ (hgraph->where[higain - 1] + 1) % 2] == 0 )
		{
			te += hgraph->hewgts[n];
		}
		else if (count[hgraph->where[higain - 1]] == 1) {

			/*k顶点所在分区的顶点个数为1*/
			fs += hgraph->hewgts[n];

		}

	}


	return fs - te;
}

/********************************************************
/*输入参数： 超图数据结构 
/*返回值：	超边的cut数
/*功能：		根据hgraph的where数组,计算出当前超图的cut数
*********************************************************/
int compute_mincut(s_hgraph * hgraph)
{
	int  i, n, k;
	int cut = 0;

	for (n = 0; n <  hgraph->nhedges; ++n)
	{
		vector<int> temp;

		for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) {
			k = hgraph->eind[i];
			temp.push_back(hgraph->where[k - 1]);
		}


		std::sort(temp.begin(), temp.end());

		if (temp.front() != temp.back()) 
		{
			cut += hgraph->hewgts[n];
		}

	}

	return cut;

}


void check_cutsign(s_hgraph * hgraph)
{
	int  i, n, k;
	int nhedges = hgraph->nhedges;


	for (n = 0; n < nhedges; ++n)
	{
		vector<int> temp;

		for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) {
			k = hgraph->eind[i];
			temp.push_back(hgraph->where[k - 1]);
		}


		std::sort(temp.begin(), temp.end());

		if (temp.front() != temp.back()) {

			if (hgraph->cutsign[n] == 0)
			{
				cout << "Error: H " << n << "is been cutted!" << endl;

			}

		}
		else {

			if (hgraph->cutsign[n] == 1)
			{
				cout << "Error: H " << n << "is not been cutted!" << endl;

			}
		}

	}

}

void check_fs_and_te(s_hgraph * hgraph)
{
	int k = 0;
	int n = 0;
	int i = 0;

	vector<float> fs(hgraph->nvtxs, 0);
	vector<float> te(hgraph->nvtxs, 0);
	vector<int> bndptr(hgraph->nvtxs, 0);
	vector<int> visit(hgraph->nvtxs, 0);

	for (n = 0; n < hgraph->nhedges; ++n)
	{


		if (hgraph->cutsign[n] == 1) 
		{

			vector<int> count(2, 0);

			for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) 
			{
				k = hgraph->eind[i];
				count[hgraph->where[k - 1]]++;
				bndptr[k - 1] = 1;
				visit[k - 1] = 1;

			}


			if (count[0] == 1) 
			{
				for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) 
				{
					k = hgraph->eind[i];
					if (hgraph->where[k - 1] == 0)
						fs[k - 1] += hgraph->hewgts[n];
				}
			}

			if (count[1] == 1) 
			{
				for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) 
				{
					k = hgraph->eind[i];
					if (hgraph->where[k - 1] == 1)
						fs[k - 1] += hgraph->hewgts[n];
				}

			}

		}
		else 
		{
			//没被切割的超边，所有顶点的te += 超边权值
			for (i = hgraph->eptr[n]; i < hgraph->eptr[n + 1]; ++i) {

				k = hgraph->eind[i];
				te[k - 1] += hgraph->hewgts[n];
			}
		}

	}


	for (i = 0; i < hgraph->nvtxs; ++i)
	{
		if (fs[i] != hgraph->fs[i] )
		{
			cout << " fs is error : " << i  << " "<< fs[i] << " " << hgraph->fs[i]<< endl;
		}

		if (te[i] != hgraph->te[i])
		{
			cout << " te is error : " << i << " " << te[i] << " " << hgraph->te[i] << endl;
		}

		/*if (bndptr[i] != hgraph->bndptr[i])
		{
			cout << " bndptr is error : " << i << " " << bndptr[i] << " " << hgraph->bndptr[i] << endl;
		}*/
	}

}
