#include <bits/stdc++.h>
using namespace std;

vector<string> split(string st)
{
    int pos=st.find("->");
    vector<string> vec;
    vec.push_back(st.substr(0, pos));
    vec.push_back(st.substr(pos+2, st.size()-(pos+2)));
    return vec;
}

/**
�����ķ�
������
    path��ʾ�ķ��ļ����ڵ�·��
����ֵ��

**/
map<string, set<string> > input(string path)
{
    ifstream fin(path); //���ļ�
    int n;
    map<string, set<string> > grammer;
    fin>>n;
    cout<<n<<endl;
    for(int i=1;i<=n;++i)
    {
        string st;
        fin>>st;
        vector<string> vec=split(st);
        grammer[vec[0]].insert(vec[1]);
    }
    fin.close();
    return grammer;
}
int main()
{
    input("grammer.txt");
    return 0;
}
