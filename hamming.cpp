#include <bits/stdc++.h>
using namespace std;
#define pb push_back

int rbits(int m)
{
	int r = 1;
	while(1)
	{
		int d = pow(2,r);
		if(d>=(m+r+1))
			break;
		r++;
	}
	return r;
}

int main()
{
	int m,r,ep,res=0;
	cout<<"Enter the no of data bits:";
	cin>>m;
	r = rbits(m);
	cout<<"\nEnter the string of data bits:";
	string s;
	cin>>s;
	int tbits = m+r;
	int arr[tbits+1],k=0;
	for(int i=1;i<=tbits;i++)
	{
		int p = log2(i);
		int d = pow(2,p);
		if(d==i)
		{
			arr[i]=0;
			continue;
		}
		arr[i]=s[k++]-'0';
	}
	map<int,vector<int> > mp;
	map<int,vector<int> >::iterator it;
	for(int i=0;i<r;i++)
	{
		int d = pow(2,i);
		for(int j=1;j<=tbits;j++)
		{
			int u = d&j;
			if(u>0)
				mp[d].pb(j);
		}
		int cnt=0;
		for(int j=0;j<mp[d].size();j++)
			if(arr[mp[d][j]]==1)
				cnt++;
		if(cnt%2==0)
			arr[d]=0;
		else
			arr[d]=1;
	}
	cout<<"\nEnter the position to introduce error:";
	cin>>ep;
	arr[ep]=!arr[ep];
	for(it=mp.begin();it!=mp.end();it++)
	{
		int cnt=0;
		for(int i=0;i<it->second.size();i++)
			if(arr[it->second[i]]==1)
				cnt++;
		if(cnt%2==1)
			res+=it->first;
	}
	cout<<"\nError detected in position: "<<res<<endl;
}
