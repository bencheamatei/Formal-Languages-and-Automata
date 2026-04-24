#include <bits/stdc++.h>    

class lnfa2dfa {
private:
    std::vector<std::string> alphabet,states,states_new;
    std::string init,init_new;
    std::unordered_map<std::string,bool> fi,fi_new;
    std::unordered_map<std::string,std::unordered_map<std::string,std::vector<std::string> > > g;
    std::unordered_map<std::string,bool> viz;
    std::unordered_map<std::string,std::unordered_map<std::string,std::string> > g_new;
    std::unordered_map<std::string,std::string> maps;
    int global_idx=0;
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

    bool in_set(const std::string &str,const std::set<std::string> &s){
        if(s.find(str)==s.end())
            return 0;
        return 1;
    }

    std::set<std::string> unite(const std::set<std::string> &s1, const std::set<std::string> &s2){
        std::set<std::string> ans;
        for(const auto &it:s1){
            ans.insert(it);
        }
        for(const auto &it:s2){
            ans.insert(it);
        }
        return ans;
    }

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

        // std::cout << state << " " << mask << " " << fi[state] << "\n"; 
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &y){
        if(!validate_edge(x,let,y)){
            std::cerr << "bad edge\n";
            return ;
        }
        g[x][let].push_back(y);
    }

    const std::set<std::string> compute_closure(const std::string &state) {
        std::set<std::string> ans;
        std::queue<std::string> q;
        std::unordered_map<std::string,bool> viz;

        q.push(state);
        viz[state]=1;

        while(!q.empty()){
            std::string nod=q.front();
            q.pop();

            ans.insert(nod);
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

    const std::set<std::string> big_closure(const std::set<std::string> &s){
        std::set<std::string> ans;
        for(const auto &it:s){
            ans=unite(ans,compute_closure(it));
        }
        return ans;
    }

    std::string stringify_set(const std::set<std::string> &s){
        std::string ans="";
        for(auto it:s){
            ans=ans+it+"-";
        }
        return ans;
    }

    void compute_dfa(){ 
        std::set<std::string> aux=compute_closure(init),aux1;
        std::string aux_s=stringify_set(aux),aux_s1;
        maps[aux_s]="q"+std::to_string(global_idx++);
        init_new=maps[aux_s];

        for(const auto &it:aux) {
            if(fi[it]) {
                fi_new[init_new]=1;
                break;
            }
        }

        std::queue<std::set<std::string> > q;
        q.push(aux);
        viz[aux_s]=1;

        while(!q.empty()){
            aux=q.front();
            q.pop();

            aux_s=stringify_set(aux);
            states_new.push_back(maps[aux_s]);
            // std::cout << (int)aux.size() << "\n";
            // for(const auto &it:aux){
            //     std::cout << it << " ";
            // }
            // std::cout << "\n";

            for(const auto &let:alphabet){
                aux1.clear();
                for(const auto &state:aux){
                    if(g[state].find(let)!=g[state].end()){
                        for(const auto &it:g[state][let]){
                            aux1.insert(it);
                        }
                    }
                }

                if(aux1.empty())
                    continue;

                // std::cout << pos_final << "\n";
                aux1=big_closure(aux1);
                aux_s1=stringify_set(aux1);
                if(!viz[aux_s1]){
                    maps[aux_s1]="q"+std::to_string(global_idx++);
                    viz[aux_s1]=1;
                    q.push(aux1);
                }

                // if(g_new[maps[aux_s]].find(let)!=g_new[maps[aux_s]].end()){
                //     if(g_new[maps[aux_s]][let]!=maps[aux_s1]){
                //         std::cout << "buba\n";
                //     }
                // }

                g_new[maps[aux_s]][let]=maps[aux_s1];
                bool pos_final=0;
                for(const auto &it:aux1){
                    if(fi[it]){
                        pos_final=1;
                        break;
                    }
                }

                if(pos_final){
                    fi_new[maps[aux_s1]]=1;
                }
            }
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const lnfa2dfa& x){
        os << (int)x.states_new.size() << "\n";
        for(const auto &it:x.states_new){
            os << it;
            if(it==x.init_new){
                os << " i";
            }
            if(x.fi_new.find(it) != x.fi_new.end() && x.fi_new.at(it)){
                os << " f";
            }
            os << "\n";
        }
        os << (int)x.alphabet.size() << "\n";
        for(const auto &it:x.alphabet){
            os << it << "\n";
        }

        for(const auto &nod:x.states_new){
            if(x.g_new.find(nod)==x.g_new.end())
                continue;
            for(const auto it:x.g_new.at(nod)){
                os << nod << " " << it.first << " " << it.second << "\n";
            }
        }
        return os;
    }

    friend std::istream& operator>>(std::istream &is, lnfa2dfa& lnfa) {
        std::string aux;
        int n;
        is >> n;
        getline(is,aux);

        for(int i=1; i<=n; i++){
            getline(is,aux);
            std::pair<std::string,int> u=lnfa.split(aux);
            lnfa.add_state(u.first,u.second);
        }
        is >> n;
        getline(is,aux);
        for(int i=1; i<=n; i++){
            is >> aux;
            lnfa.add_letter(aux);
        }

        std::string x,y;
        while(is >> x >> aux >> y){
            lnfa.add_edge(x,aux,y);
        }
        return is;
    }
};

int main(int argc, char *argv[])
{
    lnfa2dfa v;
    std::cin >> v;
    v.compute_dfa();
    std::cout << v;
    return 0;
}