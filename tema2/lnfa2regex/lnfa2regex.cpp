#include <bits/stdc++.h>

class regex {

};

class lnfa {
private:
    std::vector<std::string> alphabet,states;
    std::string init;
    std::unordered_map<std::string,bool> fi;
    std::unordered_map<std::string,std::unordered_map<std::string,std::vector<std::string> > > g;
public:
    void add_letter(const std::string &let){
        if(find(alphabet.begin(),alphabet.end(),let)!=alphabet.end())
            return;
        alphabet.push_back(let);
    }

    void add_state(const std::string &state,int mask){
        if(find(states.begin(),states.end(),state)!=states.end())
            return;
        states.push_back(state);
        if(mask&1){
            init=state;
        }
        if(mask&2){
            fi[state]=1;
        }
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &y){
        g[x][let].push_back(y);
    }

    void delete_edge(const std::string &x, const std::string &muc, const std::string &y) {
        if(g[x].find(muc)==g[x].end())
            return ;
        g[x].erase(muc);
    }
}v;

std::string aux;

std::pair<std::string,int> split(const std::string &s){
    int mask=0;
    std::string ans="",curr;
    int cnt=0;
    for(int i=0; i<(int)s.size(); i++){
        if(s[i]==' ')
            continue;

        int j=i;
        curr="";
        while(j<(int)s.size() && s[j]!=' '){
            curr.push_back(s[j]);
            j++;
        }

        if(cnt==0){
            ans=curr;
        }
        if(curr=="i"){
            mask|=1;
        }
        if(curr=="f"){
            mask|=2;
        }
        cnt++;
        i=j-1;
    }

    return std::make_pair(ans,mask);
}

void readLnfa()
{
    std::ifstream fin("b.in");
    int n;

    fin >> n;
    getline(fin,aux);

    for(int i=1; i<=n; i++){
        getline(fin,aux);
        std::pair<std::string,int> u=split(aux);
        v.add_state(u.first,u.second);
    }

    fin >> n;
    getline(fin,aux);
    for(int i=1; i<=n; i++){
        fin >> aux;
        v.add_letter(aux);
    }

    std::string x,y;
    while(fin >> x >> aux >> y){
        v.add_edge(x,aux,y);
    }

    fin.close();
}

int main() {

    return 0;
}