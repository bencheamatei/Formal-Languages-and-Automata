#include <bits/stdc++.h>

// $ e lambda, iar @ e multimea vida 

class lnfa {
private:
    std::vector<std::string> alphabet,states;
    std::string init,last;
    std::unordered_map<std::string,bool> fi;
    std::unordered_map<std::string,std::unordered_map<std::string,std::vector<std::string> > > g;
    bool is_final(const std::string &state) const {
        if(fi.find(state)==fi.end())
            return 0;
        return 1;
    }
public:
    void add_letter(const std::string &let) {
        if(std::find(alphabet.begin(),alphabet.end(),let)!=alphabet.end())
            return;
        alphabet.push_back(let);
    }

    void add_state(const std::string &state,int mask=0) {
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

    void change_init(const std::string &state) {
        if(std::find(states.begin(),states.end(),state)==states.end())
            return ;
        init=state;
    }

    void make_final(const std::string &state) {
        if(std::find(states.begin(),states.end(),state)==states.end() || fi.find(state)!=fi.end())
            return ;
        fi[state]=1;
    }

    void unmake_final(const std::string &state) {
        if(std::find(states.begin(),states.end(),state)==states.end() || fi.find(state)==fi.end())
            return ;
        fi.erase(state);
    }

    void add_edge(const std::string &x, const std::string &let, const std::string &y) {
        if(std::find(alphabet.begin(),alphabet.end(),let)!=alphabet.end() || let=="$") {
            g[x][let].push_back(y);
        }
    }

    friend std::ostream& operator<<(std::ostream &os, const lnfa &other) {
        os << (int)other.states.size() << "\n";
        for(const auto &it:other.states) {
            os << it << " ";
            if(it==other.init)
                os << "i ";
            if(other.is_final(it))
                os << "f";
            os << "\n";
        }

        for(const auto &i:other.g) {
            for(const auto &muc:i.second) {
                for(const auto &j:muc.second) {
                    os << i.first << " " << muc.first << " " << j << "\n";
                }
            }
        }
        return os;
    }
    std::string create_state(int &idx) {
        std::string pp="q"+std::to_string(idx);
        idx++;
        add_state(pp);
        return pp;
    }
}v;

class regex {
private:
    std::string s;
    std::string p;
    bool is_char(char c) {
        return c!=')' && c!='(' && c!='.' && c!='+' && c!='*';
    }

    int priority(char c) {
        if(c=='(')
            return 4;
        if(c=='*')
            return 3;
        if(c=='.')
            return 2;
        if(c=='+')
            return 1;
        return 0;
    }
public:

    regex() : s("@") {}
    regex(const std::string &s) : s(s) {}

    regex& operator=(const regex &other) {
        if(this==&other)
            return *this;
        this->s=other.s;
        return *this;
    }

    regex operator+(const regex &other) const {
        if(this->s=="@") 
            return other;
        if(other.s=="@")
            return *this;
        if(this->s==other.s)    
            return *this;
        return regex("("+this->s+")+("+other.s+")");
    }

    regex operator*(const regex &other) const {
        if(this->s=="@" || other.s=="@") 
            return regex();
        if(this->s=="$")
            return other;
        if(other.s=="$")
            return *this;
        return regex("("+this->s+").("+other.s+")");
    }

    regex star() const {
        if(this->s=="$" || this->s=="@")
            return regex("$");
        if(this->s.size()==1) {
            return regex(this->s+"*");
        }
        return regex("("+this->s+")*");
    }

    friend std::ostream& operator<<(std::ostream &os, const regex &p) {
        os << p.s;
        return os;
    }

    friend std::istream& operator>>(std::istream &is, regex &p) {
        is >> p.s;
        return is;
    }

    void correct_regex() {
        std::string aux="";
        for(int i=0; i<(int)s.size(); i++) {
            aux.push_back(s[i]);
            if(i<(int)s.size()-1) {
                if(is_char(s[i]) && is_char(s[i+1])) {
                    aux.push_back('.');
                }

                if(s[i]=='*' && (is_char(s[i+1]) || s[i+1]=='(')) {
                    aux.push_back('.');
                }

                if(is_char(s[i]) && s[i+1]=='(') {
                    aux.push_back('.');
                }

                if(s[i]==')' && is_char(s[i+1])) {
                    aux.push_back('.');
                }

                if(s[i]==')' && s[i+1]=='(') {
                    aux.push_back('.');
                }
            }
        }
        this->s=aux;
    }

    void shunting_yard() {
        correct_regex();
        p.clear();
        std::queue<char> q;
        std::vector<char> stk;

        for(int i=0; i<(int)s.size(); i++) {
            if(is_char(s[i])) {
                q.push(s[i]);
                // std::cout << s[i] << "\n";
            }
            else {
                if(s[i]=='(') {
                    stk.push_back(s[i]);
                }
                else if(s[i]==')') {
                    while(stk.size() && stk.back()!='(') {
                        q.push(stk.back());
                        stk.pop_back();
                    }
                    if(stk.size())
                        stk.pop_back();
                }
                else {
                    while(stk.size() && priority(stk.back())>=priority(s[i]) && stk.back()!='(') {
                        q.push(stk.back());
                        stk.pop_back();
                    }
                    stk.push_back(s[i]);
                }
            }
        }

        while(stk.size()) {
            q.push(stk.back());
            stk.pop_back();
        }

        while(q.size()) {
            p.push_back(q.front());
            q.pop();
        }
    }

    lnfa construct() {
        shunting_yard();
        lnfa x;

        int idx=0;

        std::vector<std::pair<std::string,std::string> > stk;
        for(int i=0; i<(int)p.size(); i++) {
            if(is_char(p[i])) {
                std::string let="";
                let.push_back(p[i]);
                std::string st=x.create_state(idx);
                std::string fi=x.create_state(idx);

                if(let!="$")
                    x.add_letter(let);
                x.add_edge(st,let,fi);
                stk.push_back({st,fi});
            }
            else {
                if(p[i]=='.') {
                    std::pair<std::string,std::string> aux1=stk.back();
                    stk.pop_back();
                    std::pair<std::string,std::string> aux2=stk.back();
                    stk.pop_back();

                    x.add_edge(aux2.second,"$",aux1.first);

                    stk.push_back({aux2.first,aux1.second});
                }
                else if(p[i]=='+') {
                    std::pair<std::string,std::string> aux2=stk.back();
                    stk.pop_back();
                    std::pair<std::string,std::string> aux1=stk.back();
                    stk.pop_back();

                    std::string aux_init=x.create_state(idx);
                    x.add_edge(aux_init,"$",aux1.first);
                    x.add_edge(aux_init,"$",aux2.first);

                    std::string aux_fin=x.create_state(idx);
                    x.add_edge(aux1.second,"$",aux_fin);
                    x.add_edge(aux2.second,"$",aux_fin);

                    stk.push_back({aux_init,aux_fin});
                }
                else {
                    std::pair<std::string,std::string> aux1=stk.back();
                    stk.pop_back();

                    std::string aux_init=x.create_state(idx);
                    std::string aux_fin=x.create_state(idx);

                    x.add_edge(aux_init,"$",aux_fin);
                    x.add_edge(aux_init,"$",aux1.first);
                    x.add_edge(aux1.second,"$",aux1.first);
                    x.add_edge(aux1.second,"$",aux_fin);

                    stk.push_back({aux_init,aux_fin});
                }
            }
        }

        if(stk.size()) {
            x.change_init(stk.back().first);
            x.make_final(stk.back().second);
        }
        return x;
    }
};

int main() {
    regex x;
    std::cin >> x;
    std::cout << x.construct() << "\n";
    return 0;
}