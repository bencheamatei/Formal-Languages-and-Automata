#include <bits/stdc++.h>    

// pentru noi, $ o sa fie luat drept lambda

class dfa {
private:
    std::vector<std::string> alphabet,states;
    std::string init;
    std::unordered_map<std::string,bool> fi;
    std::unordered_map<std::string,std::unordered_map<std::string,std::string> > g;

    bool validate_edge(const std::string &x, const std::string &let, const std::string &y){
        if(find(states.begin(),states.end(),x)==states.end()){
            return 0;
        }

        if(find(states.begin(),states.end(),y)==states.end()){
            return 0;
        }

        if(find(alphabet.begin(),alphabet.end(),let)==alphabet.end()){
            return 0;
        }

        return 1;
    }

public:

    void add_letter(const std::string &let){
        if(find(alphabet.begin(),alphabet.end(),let)!=alphabet.end()){
            return;
        }
        alphabet.push_back(let);
    }

    void add_state(const std::string &state,int mask){
        if(find(states.begin(),states.end(),state)!=states.end()){
            return;
        }
        states.push_back(state);

        if(mask&1){
            init=state;
        }

        if(mask&2){
            fi[state]=1;
        }
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &y){
        if(!validate_edge(x,let,y)){
            std::cerr << "bad edge\n";
            return ;
        }
        g[x][let]=y;
    }

    void compute_word(const std::string &s){
        int sz=(int)s.size();
        std::string curr=this->init,nxt="";
        std::cout << "$ : " << init << "\n";
        int idx=0;

        if(s=="$"){
            sz=0;
        }
        
        while(idx<sz){
            int mx=-1;
            for(const auto [let,state]:g[curr]){
                if(idx+(int)let.size()-1>=sz){
                    continue;
                }

                if(s.substr(idx,(int)let.size())==let && (int)let.size()>mx){
                    mx=(int)let.size();
                    nxt=state;
                }
            }

            if(mx==-1){
                std::cout << "RESPINS (abort)\n";
                return ;
            }

            std::cout << s.substr(0,idx+mx) << " : " << nxt << "\n";
            idx+=mx; 
            curr=nxt;
        }

        if(!fi[curr]){
            std::cout << "RESPINS\n";
        }
        else {
            std::cout << "ACCEPTAT\n";
        }
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

void readDfa()
{
    std::ifstream fin("dfa.in");
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

void solve()
{
    std::ifstream fin("words.in");
    while(fin >> aux){
        v.compute_word(aux);
    }
    fin.close();
}

int main(int argc, char *argv[])
{
    readDfa();
    solve();
    return 0;
}