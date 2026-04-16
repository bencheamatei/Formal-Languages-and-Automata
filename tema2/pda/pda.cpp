#include <bits/stdc++.h>

// conditiile de acceptare le modelez astfel:
// 0 pentru stare finala 
// 1 pentru stiva goala 
// 2 pentru stare finala & stiva goala

// o sa iau simbolul de inceput de stack drept !

class pda {
private:

    struct element {
        std::string stare;
        std::string prefix;
        std::vector<std::string> stk;
        element(const std::string &stare, const std::string &prefix, const std::vector<std::string> &stk) :
            stare(stare), prefix(prefix), stk(stk) {}
        bool operator<(const element &other) const {
            if (stare!=other.stare) return stare<other.stare;
            if (prefix!=other.prefix) return prefix<other.prefix;
            return stk<other.stk;
        }
    };  

    int acc_type;

    std::vector<std::string> states;
    std::vector<std::string> alphabet, stack_alphabet;
    std::unordered_map<std::string, 
        std::unordered_map<std::string,
        std::unordered_map<std::string,std::vector<std::pair<std::string, std::vector<std::string> > > 
        > > > g;
    std::unordered_map<std::string,bool> fi;
    std::string init;
    std::string init_stack;

    bool is_accepted(bool final_state, bool empty_stack) const {
        if(acc_type==0) {
            return final_state;
        }

        if(acc_type==1) {
            return empty_stack;
        }

        if(acc_type==2) {
            return final_state && empty_stack;
        }
        return 0;
    }

    std::string find_next_letter(const std::string &word) const {
        std::string ans="";
        int mx=0;
        for(const auto &let:alphabet) {
            if(let.size()>word.size())
                continue;

            if(word.substr(0,(int)let.size())==let && (int)let.size()>mx) {
                mx=(int)let.size();
                ans=let;
            }
        }
        return ans;
    }

public:

    void set_acceptance_type(int x) {
        acc_type=x;
    }

    void add_init_stack(const std::string &s) {
        if(std::find(stack_alphabet.begin(),stack_alphabet.end(),s)==stack_alphabet.end()) {
            return;
        }
        init_stack=s;
    }

    void add_state(const std::string &state,int mask) {
        if(std::find(states.begin(),states.end(),state)==states.end()) {
            states.push_back(state);
            if(mask&1) {
                init=state;
            }
            if(mask&2) {
                fi[state]=1;
            }
        }
    }

    void add_letter(const std::string &let) {
        if(std::find(alphabet.begin(),alphabet.end(),let)==alphabet.end()) {
            alphabet.push_back(let);
        }
    }   

    void add_letter_stack(const std::string &let) {
        if(std::find(stack_alphabet.begin(),stack_alphabet.end(),let)==stack_alphabet.end()) {
            stack_alphabet.push_back(let);
        }
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &top, const std::string &y,
        const std::vector<std::string> &to_push) {
        
        if(std::find(states.begin(),states.end(),x)==states.end() || 
            std::find(states.begin(),states.end(),y)==states.end()) {
            return ;
        }

        if(let!="$" && std::find(alphabet.begin(),alphabet.end(),let)==alphabet.end()) {
            return ;
        }

        if(std::find(stack_alphabet.begin(),stack_alphabet.end(),top)==stack_alphabet.end()) {
            return ;
        }

        for(const auto &it:to_push) {
            if(it!="$" && std::find(stack_alphabet.begin(),stack_alphabet.end(),it)==stack_alphabet.end()) {
                return ;
            }
        }

        g[x][let][top].push_back({y,to_push});
    }

    bool compute_word(const std::string &word) {
        std::set<element> viz;
        std::queue<element> q;
        q.push({init,word,{init_stack}});
        while(!q.empty()) {
            element nod=q.front();
            q.pop();

            if(viz.count(nod)>0) {
                continue;
            }
            viz.insert(nod);

            if(nod.prefix=="") {
                bool final_state=(fi.find(nod.stare)!=fi.end());
                bool empty_stack=(nod.stk.empty());
                if(is_accepted(final_state,empty_stack))
                    return 1;
            }

            std::string let=find_next_letter(nod.prefix);
            std::string the_top="";
            if(!nod.stk.empty()) {
                the_top=nod.stk.back();
            }
            
            if(let!="") {
                if(g[nod.stare].find(let)!=g[nod.stare].end() && 
                g[nod.stare][let].find(the_top)!=g[nod.stare][let].end()) {
                    for(const auto [vec,to_push]:g[nod.stare][let][the_top]) {
                        std::string pref=nod.prefix.substr((int)let.size());
                        std::vector<std::string> stk=nod.stk;
                        if(!stk.empty())
                            stk.pop_back();
                        
                        for(auto it=to_push.rbegin(); it!=to_push.rend(); it++) {
                            if(*it!="$") {
                                stk.push_back(*it);
                            }
                        }
                        q.push({vec,pref,stk});
                    }
                }
            }
            
            if(g[nod.stare].find("$")!=g[nod.stare].end() && 
            g[nod.stare]["$"].find(the_top)!=g[nod.stare]["$"].end()) {
                for(const auto [vec,to_push]:g[nod.stare]["$"][the_top]) {
                    std::string pref=nod.prefix;
                    std::vector<std::string> stk=nod.stk;
                    if(!stk.empty())
                        stk.pop_back();
                    
                    for(auto it=to_push.rbegin(); it!=to_push.rend(); it++) {
                        if(*it!="$") {
                            stk.push_back(*it);
                        }
                    }
                    q.push({vec,pref,stk});
                }
            }
        }   
        return 0;
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

void read_pda()
{
    std::ifstream fin("pda.in");
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

    fin >> n;
    getline(fin,aux);
    for(int i=1; i<=n; i++){
        fin >> aux;
        v.add_letter_stack(aux);
    }

    fin >> aux;
    v.add_init_stack(aux);

    std::string x,y,let,top;
    int nr;
    while(fin >> x >> let >> top >> y >> nr){
        std::vector<std::string> pp;
        for(int i=0; i<nr; i++) {
            fin >> aux;
            pp.push_back(aux);
        }
        v.add_edge(x,let,top,y,pp);
    }
    fin.close();
}

int main() {
    read_pda();
    std::string word;
    std::cin >> word;

    if(v.compute_word(word)) {
        std::cout << "Acceptat\n";
    }
    else {
        std::cout << "Respins\n";
    }
    return 0;
}