#include "CacheTester.h"
CacheTester::CacheTester(int nodeNum , string type, bool showtrace, bool showset):
	nodeNum_(nodeNum), type_(type), cache(nodeNum,type, showtrace, showset)
{
	hasE_ = false;
	if(!type.compare("MESI") || !type.compare("MOESI") )
		hasE_ = true;
	hasO_ = false;
	if (!type.compare("MOSI") || !type.compare("MOESI"))
		hasO_ = true;
}

void CacheTester::BFS()
{
	cout << "BFS: " << endl;
	cache.BFS();
}

void CacheTester::random(int length_)
{
	cout << "Random: " << endl;
	cache.random(length_);
}

void CacheTester::mcjammer(int length_)
{
	cout << "MCjammer: " << endl;
	cache.mcjammer(length_);
}

void CacheTester::test()
{
	int id = 1;
	// Vist hypercube
	for(int i = 0 ; i < nodeNum_; i++)
	{
		cache.nextAction(i*4);
		// If protocol has E, we load next core, then evict
		if(hasE_) {
		    int j = (i+1) % nodeNum_;
		    cache.nextAction(j*4);
		    cache.nextAction(2+j*4);
		}

		visit_hypercube(id,nodeNum_-1,i);
		cache.nextAction(2 + i*4);
	}

	if(hasE_)
		visit_e_vertex();

	visit_clique(0);

	int nstates = (1<<nodeNum_);
	if (!hasO_){
		// Visit write transition
		for(int i = 1 ; i < nstates; i++){
			do_write_transition_MSI(i);
			if(hasE_)
				do_write_transition_has_e(i);
		}
		cache.summary();
		return;
	}

	for(int opos = 0 ; opos < nodeNum_; opos++)
	{
		// node opos goes to M then O
		cache.nextAction(opos * 4 + 1);
		int k = (opos + 1) % nodeNum_;
		cache.nextAction(k * 4);
		cache.nextAction(2 + k * 4);

		for(int i = 0 ; i < nodeNum_ - 1; i++)
		{
			int p = i;
			if(p>=opos) p++;
			cache.nextAction(p*4);
			visit_hypercube_with_o(id,nodeNum_-2,i,opos);
			cache.nextAction(2 + p*4);
		}
		cache.nextAction(opos * 4 + 2);
	}

	// Visit IEII to IIIM
	if (hasE_)
		for (int i = 0 ; i < nodeNum_; i++) {
			for (int j = 0; j < nodeNum_; j++) {
				if (i != j){
					cache.nextAction(i*4);
					cache.nextAction(j*4 + 1);
					cache.nextAction(j*4 + 2);
				}
			}
		}
	// Visit write transition
	for(int i = 1 ; i < nstates; i++)
	{
		do_write_transition(i);
		if(hasE_) {
			do_write_transition_has_e(i);
		//	do_write_transition_moesi(i);
		}
	}
	nstates = (1<<(nodeNum_-1));
	for(int opos = 0 ; opos < nodeNum_; opos++)
	{
		cache.nextAction(opos * 4 + 1);
		int k = (opos + 1) % nodeNum_;
		cache.nextAction(k * 4);
		cache.nextAction(2 + k * 4);
		for(int i = 0 ; i < nstates; i++)
		{
			id = i*2+1;
			id = ((id<<opos) & ((1<<nodeNum_) -1 )) + (id >>(nodeNum_ - opos));
			do_write_transition_with_o(id,opos);
		}
	}
	cache.summary();
}


void CacheTester::visit_e_vertex()
{
	for(int i = 0 ; i < nodeNum_ ; i++)
	{
		cache.nextAction(i*4);
		for(int j=0;j < nodeNum_; j++)
		{
			cache.nextAction(j*4);
			if(i!=j)
			{
				cache.nextAction(2+j*4);
				cache.nextAction(2+i*4);
				cache.nextAction(i*4);
			}
		}
		cache.nextAction(2+i*4);
	}
}

void CacheTester::visit_clique(int m)
{
	cache.nextAction(m*4 + 1);
	cache.nextAction(m*4 + 1);
	cache.nextAction(m*4);
	cache.nextAction(m*4 + 2);
	cache.nextAction(m*4 + 1);
	//Access OSS or ISS states
	for(int j = 0; j < nodeNum_ ; j++)
	{
		if(j == m)
			continue;
		cache.nextAction(j*4);
		cache.nextAction(m*4 + 1);
	}
	//Recursively visits other parts of the clique
	for(int j = m+1 ; j < nodeNum_ ; j++)
	{
		cache.nextAction(j*4 + 1);
		if(j==m+1)
			visit_clique(j);
		cache.nextAction(m*4 + 1);
	}
	// Change back to global invalid state
	if(m==0)
		cache.nextAction(m*4 + 2);

}

void CacheTester::do_write_transition_MSI(int id){
	for(int i = 0 ; i < nodeNum_; i++)
	{
		for(int j=0;j < nodeNum_; j++)
		{
			if((id & (1<<j)) > 0)
			{
				cache.nextAction(j*4);
			}
		}
		if(i==0)
			for(int j=0;j < nodeNum_; j++)
			{
				if((id & (1<<j)) > 0)
				{
					cache.nextAction(j*4);
				}
			}
		cache.nextAction(i*4 + 1);
		// if i is not in S (id & (1<<i)==0), evict i
		// if only one node in S (id & id - 1 == 0), evict i
		// if i is the last one, evict i, to restore global invalid 
		if(((id & (1<<i)) == 0 ) || (id&id-1)==0 || (i == nodeNum_-1))
			cache.nextAction(i*4 + 2);
	}
}

void CacheTester::do_write_transition(int id)
{
	for(int j=0;j < nodeNum_; j++)
	{
		if((id & (1<<j)) > 0)
		{
			cache.nextAction(j*4);
		}
	}
	for(int i = 0 ; i < nodeNum_; i++)
	{
		//Cover all the silent loads
		if(i==0)
			for(int j=0;j < nodeNum_; j++)
				if((id & (1<<j)) > 0)
					cache.nextAction(j*4);

		if(((id & (1<<i)) == 0 )&&(i != nodeNum_ - 1))
		{
		    continue;
		}
		cache.nextAction(i*4 + 1);
		cache.nextAction(i*4 + 2);
		if(i != nodeNum_ - 1)
			for(int j=0;j < nodeNum_; j++)
			{
				if((id & (1<<j)) > 0)
				{
					cache.nextAction(j*4);
				}
			}
	}
}

void CacheTester::do_write_transition_with_o(int id,int opos)
{
	for(int i = 0 ; i < nodeNum_; i++)
	{
		for(int j=0;j < nodeNum_; j++)
		{
			if((id & (1<<j)) > 0)
			{
				cache.nextAction(j*4);
			}
		}
		//Cover all the silent loads
		if(i==0)
		{
			for(int j=0;j < nodeNum_; j++)
			{
				if((id & (1<<j)) > 0)
				{
					cache.nextAction(j*4);
				}
			}
			//cover transition back to the main hypercube
			cache.nextAction(opos*4 + 2);
			cache.nextAction(opos*4 + 1);
			int k = (opos + 1) % nodeNum_;
			cache.nextAction(k * 4);
			cache.nextAction(2 + k * 4);
			for(int j=0;j < nodeNum_; j++)
			{
				if((id & (1<<j)) > 0)
				{
					cache.nextAction(j*4);
				}
			}
		}
		cache.nextAction(i*4 + 1);
		cache.nextAction(opos*4 + 1);
		int k = (opos + 1) % nodeNum_;
		cache.nextAction(k * 4);
		cache.nextAction(2 + k * 4);
	}
}

void CacheTester::visit_hypercube(int id, int m ,int shift)
{
	for(int i = 1 ; i <= m; i++)
	{
		int newid = id + (1<<i);
		int p = (i + shift)%nodeNum_;
		p *= 4;
		cache.nextAction(p);
		if( i > 1)
			visit_hypercube(newid, i-1,shift);
		cache.nextAction(2 + p);
	}
}
/*
void CacheTester::visit_hypercube(int m, int r)
{
	int p = (m + r) % nodeNum_;
	cache.nextAction(p << 2);
	for (int i = 2; i <= m; i++)
		visit_hypercube(i - 1, r);
	cache.nextAction(p << 2 + 2);
}
*/

void CacheTester::visit_hypercube_with_o(int id, int m ,int shift, int opos)
{
	for(int i = 1 ; i <= m; i++)
	{
		int newid = id + (1<<i);
		int p = (i + shift)%(nodeNum_-1);
		if(p>=opos) p++;
		p *= 4;

		cache.nextAction(p);
		if( i > 1)
			visit_hypercube_with_o(newid, i-1,shift,opos);
		cache.nextAction(2 + p);
	}
}
      
//Cover transition from isiii state when e state is allowed
void CacheTester::do_write_transition_has_e(int id)
{
	if((id & (id-1)) > 0) return;
	for(int i = 0 ; i < nodeNum_; i++)
	{
		int j=0;
		for(j=0;j < nodeNum_; j++)
		{
			if((id & (1<<j)) > 0)
			{
				cache.nextAction(j*4);
				int k = (j+1) % nodeNum_;
				cache.nextAction(k*4);
				cache.nextAction(2+k*4);
				break;
			}
		}
		cache.nextAction(j*4);
		cache.nextAction(i*4 + 1);
		cache.nextAction(i*4 + 2);
	}
}


void CacheTester::do_write_transition_moesi(int id)
{
	if((id & (id-1)) > 0) return;
	for(int i = 0 ; i < nodeNum_; i++)
	{
		int j=0;
		for(j=0;j < nodeNum_; j++)
		{
			if((id & (1<<j)) > 0)
			{
				cache.nextAction(j*4);
				for(int t = 0 ; t < nodeNum_; t++)
				{
					if (t != j) {
						cache.nextAction(t*4 + 1);
						cache.nextAction(t*4 + 2);
						cache.nextAction(j*4);
					}
				}
				int k = (j+1) % nodeNum_;
				cache.nextAction(k*4);
				cache.nextAction(2+k*4);
				break;
			}
		}
		cache.nextAction(j*4);
		cache.nextAction(i*4 + 1);
		cache.nextAction(i*4 + 2);
	}
}
