#include <bits/stdc++.h>
using namespace std;

map<string,set<string> > grammer;
set<string> non_terminal;
set<char> terminal;
map<string,set<char> > first, follow;
set<string> vis;
map<pair<string,char>,string> predict;

vector<string> split(string st)
{
    int pos=st.find("->");
    vector<string> vec;
    vec.push_back(st.substr(0, pos));
    vec.push_back(st.substr(pos+2, st.size()-(pos+2)));
    return vec;
}

void input(string path)
{
    ifstream fin(path);
    int n;
    fin>>n;
    for(int i=1;i<=n;++i)
    {
        string st;
        fin>>st;
        vector<string> vec=split(st);
        grammer[vec[0]].insert(vec[1]);
        non_terminal.insert(vec[0]);
        for(int j=0;j<vec[1].size();++j)
            if(!('A'<=vec[1][j]&&vec[1][j]<='Z')&&vec[1][j]!='\''&&vec[1][j]!='~')
                terminal.insert(vec[1][j]);
    }
    terminal.insert('#');
    fin.close();
}

void dfs(string left)
{
    if(vis.count(left)) return;
    for(string right:grammer[left])
    {
        for(int i=0;i<right.size();++i)
            if('A'<=right[i]&&right[i]<='Z')
            {
                string tmp;
                tmp+=right[i];
                if(i<right.size()-1&&right[i+1]=='\'')
                    tmp+=right[++i];
                dfs(tmp);
                first[left].insert(first[tmp].begin(),first[tmp].end());
                if(first[tmp].count('~')==0)
                    break;
            }
            else
            {
                first[left].insert(right[i]);
                break;
            }
    }
    vis.insert(left);
}
void calc_first()
{

    vis.clear();
    for(auto p:grammer) dfs(p.first);
    cout<<"**********FIRST集**********"<<endl;
    for(auto p:first)
    {
        cout<<"FIRST("<<p.first<<")={";
        bool flag=true;
        for(char c:p.second)
        {
            if(!flag) cout<<',';
            flag=false;
            cout<<c;
        }
        cout<<"}"<<endl;
    }
    cout<<endl;
}
bool check_empty(string st)
{
    for(int i=0;i<st.size();++i)
    {
        if('A'<=st[i]&&st[i]<='Z')
        {
            string tmp;
            tmp+=st[i];
            if(i<st.size()-1&&st[i+1]=='\'')
                tmp+=st[++i];
            if(!first[tmp].count('~'))
                return false;
        }
        else if(st[i]!='~')
            return false;
    }
    return true;
}
void calc_follow()
{
    map<string, set<char> > last_follow;
    follow["E"].insert('#');
    while(last_follow!=follow)
    {
        last_follow=follow;
        for(auto p:grammer)
        {
            string A=p.first;
            for(string right:p.second)
            {
                for(int i=0;i<right.size();++i)
                {
                    if('A'<=right[i]&&right[i]<='Z')
                    {
                        string B;
                        B+=right[i];
                        if(i<right.size()-1&&right[i+1]=='\'')
                            B+=right[++i];
                        if(i==right.size()-1)
                            follow[B].insert(follow[A].begin(),follow[A].end());
                        else
                        {
                            if('A'<=right[i+1]&&right[i+1]<='Z')
                            {
                                string beta;
                                beta+=right[i+1];
                                if(i+1<right.size()-1&&right[i+2]=='\'')
                                    beta+='\'';
                                follow[B].insert(first[beta].begin(),first[beta].end());
                                follow[B].erase('~');
                            }
                            else if(right[i+1]!='~')
                            {
                                follow[B].insert(right[i+1]);
                            }
                            if(check_empty(right.substr(i+1,right.size()-(i+1))))
                                follow[B].insert(follow[A].begin(),follow[A].end());
                        }
                    }
                }
            }
        }
    }
    cout<<"**********FOLLOW集**********"<<endl;
    for(auto p:follow)
    {
        cout<<"FOLLOW("<<p.first<<")={";
        bool flag=true;
        for(char c:p.second)
        {
            if(!flag) cout<<',';
            flag=false;
            cout<<c;
        }
        cout<<'}'<<endl;
    }
    cout<<endl;
}
void print(string st,int col)
{
    cout<<st;
    for(int i=st.size();i<col;++i)
        cout<<' ';
    cout<<'|';
}
void print_sep(char c,int num)
{
    for(int i=1;i<=num;++i) cout<<c;
    cout<<endl;
}
void calc_predict()
{
    for(auto p:grammer)
    {
        string left=p.first;
        for(string right:p.second)
        {
            for(int i=0;i<right.size();++i)
            {
                if('A'<=right[i]&&right[i]<='Z')
                {
                    string tmp;
                    tmp+=right[i];
                    if(i<right.size()-1&&right[i+1]=='\'')
                        tmp+=right[++i];
                    for(char c:first[tmp])
                        if(c!='~')
                            predict[make_pair(left,c)]=right;
                    if(!first[tmp].count('~'))
                        break;
                }
                else if(right[i]!='~')
                {
                    predict[make_pair(left,right[i])]=right;
                    break;
                }
            }
            if(check_empty(right))
            {
                for(char c:follow[left])
                    predict[make_pair(left,c)]=right;
            }
        }
    }
    cout<<"**********预测分析表**********"<<endl;
    int col=5;
    print("",col);
    for(char c:terminal)
        print(string(1,c),col);
    cout<<endl;
    print_sep('-',(col+1)*(terminal.size()+1));
    for(string st:non_terminal)
    {
        print(st,col);
        for(char c:terminal)
            print(predict[make_pair(st,c)],col);
        cout<<endl;
        print_sep('-',(col+1)*(terminal.size()+1));
    }
    cout<<endl;
}
void analyze(string text)
{
    cout<<"**********分析过程**********"<<endl;
    text+="#";
    string sta="#E";
    int top=1,col=8;
    while(!sta.empty())
    {
        print(sta,col);
        print(text,col);
        cout<<endl;
        string left;
        if(sta[top]=='\'')
        {
            left=sta.substr(top-1,2);
            sta.erase(top-1,2);top-=2;
        }
        else
        {
            left=sta.substr(top,1);
            sta.erase(top,1);--top;
        }
        if(left==text.substr(0,1))
        {
            text.erase(0,1);
        }
        else
        {
            string right=predict[make_pair(left,text[0])];
            if(right=="~") continue;
            for(int i=right.size()-1;i>=0;--i)
            {
                if(right[i]=='\'')
                    sta+=right.substr(--i,2);
                else
                    sta+=right[i];
            }
            top+=right.size();
        }
    }
}
int main()
{
    input("grammer2.txt");
    calc_first();
    calc_follow();
    calc_predict();
    string text;
    cout<<"输入要分析的语句："<<endl;
    cin>>text;
    analyze(text);
    return 0;
}
