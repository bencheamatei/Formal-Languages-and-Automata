#include <bits/stdc++.h>    

// same ca la dfa, $ o sa fie luat drept lambda
// cum difera aici?
// daca vrem doar sa decidem daca un cuvant este acceptat sau nu 
// putem sa rulam un dp care ruleaza in O(nr_states * len)
// daca vrem sa afisam tot parcursul ne ducem spre o exponentiala 

class lnfa {
private:
    std::vector<std::string> alphabet,states;
    std::string init;
    std::unordered_map<std::string,bool> fi;
    std::unordered_map<std::string,std::unordered_map<std::string,std::vector<std::string> > > g;

    bool validate_edge(const std::string &x, const std::string &let, const std::string &y){
        if(find(states.begin(),states.end(),x)==states.end()){
            return 0;
        }

        if(find(states.begin(),states.end(),y)==states.end()){
            return 0;
        }

        if(find(alphabet.begin(),alphabet.end(),let)==alphabet.end() && let!="$"){
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
        g[x][let].push_back(y);
        // std::cout << x << "-" << let << "-" << y << "\n";
    }

    const std::vector<std::string> compute_closure(const std::string &state) {
        std::vector<std::string> ans;
        std::queue<std::string> q;
        std::unordered_map<std::string,bool> viz;

        q.push(state);
        viz[state]=1;

        while(!q.empty()){
            std::string nod=q.front();
            q.pop();

            ans.push_back(nod);
            for(const auto it:g[nod]){
                if(it.first!="$"){
                    continue;
                }

                for(const auto vec:it.second){
                    if(!viz[vec]){
                        q.push(vec);
                        viz[vec]=1;
                    }
                }
            }
        }
        return ans;
    }

    void compute_word(const std::string &s){
        std::set<std::string> curr,nxt;
        std::vector<std::string> aux;
        int sz=(int)s.size();

        if(s=="$"){
            sz=0;
        }
        int idx=0;

        aux=compute_closure(init);
        for(const auto it:aux)
            curr.insert(it);

        std::cout << "$ : {";
        for(const auto it:curr){
            if(it!=*--curr.end())
                std::cout << it << ", ";
            else    
                std::cout << it << "}\n";
        }

        while(idx<sz){
            int mx=-1;
            nxt.clear();
            for(const auto st:curr){
                for(const auto [let,state]:g[st]){
                    if(idx+(int)let.size()-1>=sz){
                        continue;
                    }

                    if(s.substr(idx,(int)let.size())==let && (int)let.size()>mx){
                        mx=(int)let.size();
                        nxt.clear();
                        for(const auto i:state){
                            aux=compute_closure(i);
                            for(const auto j:aux){
                                nxt.insert(j);
                            }
                        }
                    }
                    else if(s.substr(idx,(int)let.size())==let && (int)let.size()==mx){
                        for(const auto i:state){
                            aux=compute_closure(i);
                            for(const auto j:aux){
                                nxt.insert(j);
                            }
                        }
                    }
                }
            }

            if(mx==-1){
                std::cout << "RESPINS (abort)\n";
                return;
            }

            std::cout << s.substr(0,idx+mx) << " : {";
            for(const auto i:nxt){
                if(i!=*--nxt.end())
                    std::cout << i << ", ";
                else 
                    std::cout << i << "}\n";
            }
            idx+=mx;    
            swap(curr,nxt);
        }

        for(const auto it:curr){
            if(fi[it]){
                std::cout << "ACCEPTAT\n";
                return;
            }
        }
        std::cout << "RESPINS\n";
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
    std::ifstream fin("lnfa.in");
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
        // std::cout << x << "-" << aux << "-" << y << "\n";
        v.add_edge(x,aux,y);
    }

    fin.close();
}

void solve()
{
    std::ifstream fin("words.in");
    while(fin >> aux){
        std::cout << aux << "\n";
        v.compute_word(aux);
    }
    fin.close();
}

int main(int argc, char *argv[])
{
    readLnfa();
    solve();

    // std::vector<std::string> aux=v.compute_closure("q0");
    // for(const auto it:aux){
    //     std::cout << it << "\n";
    // }

    return 0;
}