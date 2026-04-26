#include <bits/stdc++.h>

// $ e lambda, iar @ e multimea vida 

class regex {
private:
    std::string s;
    int prec;
public:

    regex() : s("@"), prec(1) {}
    regex(const std::string &s,int prec=1) : s(s), prec(prec) {}

    regex& operator=(const regex &other) {
        if(this==&other)
            return *this;
        this->s=other.s;
        this->prec=other.prec;
        return *this;
    }

    regex operator+(const regex &other) const {
        if(this->s=="@") 
            return other;
        if(other.s=="@")
            return *this;
        if(this->s==other.s)    
            return *this;
        return regex(this->s+"+"+other.s,3);
    }

    regex operator*(const regex &other) const {
        if(this->s=="@" || other.s=="@") 
            return regex();
        if(this->s=="$")
            return other;
        if(other.s=="$")
            return *this;

        std::string p1=this->s,p2=other.s;
        if(this->prec==3) {
            p1="("+p1+")";
        } 

        if(other.prec==3) {
            p2="("+p2+")";
        }

        return regex(p1+"."+p2,2);
    }

    regex star() const {
        if(this->s=="$" || this->s=="@")
            return regex("$");
        if(this->prec>=2) {
            return regex("("+this->s+")"+"*",1);
        }
        return regex(this->s+"*",1);
    }

    friend std::ostream& operator<<(std::ostream &os, const regex &p) {
        os << p.s;
        return os;
    }
};

class lnfa {
private:
    std::vector<std::string> alphabet,states;
    std::string init,last;
    std::unordered_map<std::string,bool> fi;
    std::unordered_map<std::string,std::unordered_map<std::string,regex> > g;

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
        if(std::find(alphabet.begin(),alphabet.end(),let)!=alphabet.end())
            return;
        alphabet.push_back(let);
    }

    void add_state(const std::string &state,int mask){
        if(std::find(states.begin(),states.end(),state)!=states.end())
            return;
        states.push_back(state);
        if(mask&1){
            init=state;
        }
        if(mask&2){
            fi[state]=1;
            last=state;
        }
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &y){
        regex aux(let);
        if(g[x].find(y)==g[x].end()) {
            g[x][y]=aux;
        }
        else {
            g[x][y]=g[x][y]+aux;
        }
    }

    void initial_prep() {
        bool change_fin=0;
        bool change_init=0;

        if(fi.find(init)!=fi.end()) {
            change_fin=change_init=1;
        }

        if(fi.size()>1) {
            change_fin=1;
        }
        else {
            for(const auto &state:states) {
                if(fi.find(state)==fi.end())
                    continue;
                if(g[state].size()>0) {
                    change_fin=1;
                    break;
                }
            }
        }

        for(const auto &state:states) {
            if(g[state].find(init)!=g[state].end()) {
                change_init=1;
                break;
            }
        }

        if(change_init) {
            states.push_back("q_true_init");
            add_edge("q_true_init", "$", init);
            init="q_true_init";
        }

        if(change_fin) {
            states.push_back("q_true_final");
            for(const auto &[state,_]:fi) {
                add_edge(state,"$","q_true_final");
            }
            last="q_true_final";
            fi.clear();
            fi["q_true_final"]=1;
        }
    }

    regex transform2regex() {
        initial_prep();
        for(const auto &state:states) {
            if(state==init || (fi.find(state)!=fi.end() && fi[state])) {
                continue;
            }

            std::vector<std::string> ant;
            std::vector<std::string> nxt;

            for(const auto &it:g) {
                if(it.first!=state && it.second.find(state)!=it.second.end()) {
                    ant.push_back(it.first);
                }
            }

            for(const auto &it:g[state]) {
                if(it.first==state)
                    continue;

                nxt.push_back(it.first);
            }

            regex r_kk("$");
            if(g[state].find(state)!=g[state].end()) {
                r_kk=g[state][state].star();
            }

            for(const auto &i:ant) {
                for(const auto &j:nxt) {
                    regex r_ik=g[i][state];
                    regex r_kj=g[state][j];
                    regex ans=r_ik*r_kk*r_kj;
                    if(g[i].find(j)==g[i].end()) {
                        g[i][j]=ans;
                    }
                    else {
                        g[i][j]=g[i][j]+ans;
                    }
                }
            }

            g.erase(state);
            for(auto &it:g) {
                it.second.erase(state);
            }
        }

        if(g[init].find(last)==g[init].end()) {
            return regex();
        }
        return g[init][last];
    }

    friend std::istream& operator>>(std::istream& is, lnfa& l) {
        int n;
        std::string aux;
        is >> n;
        getline(is,aux);

        for(int i=1; i<=n; i++){
            getline(is,aux);
            std::pair<std::string,int> u=l.split(aux);
            l.add_state(u.first,u.second);
        }

        is >> n;
        getline(is,aux);
        for(int i=1; i<=n; i++){
            is >> aux;
            l.add_letter(aux);
        }

        std::string x,y;
        while(is >> x >> aux >> y){
            l.add_edge(x,aux,y);
        }
        return is;
    }
};

int main() {
    lnfa x;
    std::ifstream fin("b.in");
    std::cin >> x;
    fin.close();
    std::cout << x.transform2regex() << "\n";
    return 0;
}