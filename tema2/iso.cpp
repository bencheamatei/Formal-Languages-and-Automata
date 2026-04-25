#include <bits/stdc++.h>

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

    friend std::istream& operator>>(std::istream& is, dfa &d) {
        int n;
        std::string aux;
        is >> n;
        getline(is,aux);
        for(int i=1; i<=n; i++) {
            getline(is,aux);
            std::pair<std::string,int> u=d.split(aux);
            d.add_state(u.first,u.second);
        }

        is >> n;
        getline(is,aux);
        for(int i=1; i<=n; i++) {
            is >> aux;
            d.add_letter(aux);
        }

        std::string x,y;
        while(is >> x >> aux >> y) {
            d.add_edge(x,aux,y);
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const dfa& x) {
        os << (int)x.states.size() << "\n";
        for(const auto &it:x.states) {
            os << it << " ";
            if(it==x.init) {
                os << "i ";
            }
            if(x.fi.find(it)!=x.fi.end() && x.fi.at(it)) {
                os << "f ";
            }
            os << "\n";
        }
        os << (int)x.alphabet.size() << "\n";
        for(const auto &it:x.alphabet) {
            os << it << "\n";
        }

        for(const auto &it:x.g) {
            for(const auto &muc:it.second) {
                os << it.first << " " << muc.first << " " << muc.second << "\n";
            }
        }   

        return os;
    }
    
    bool operator==(const dfa &other) const {
        if(states.size()!=other.states.size() || alphabet.size()!=other.alphabet.size()) {
            return 0;
        }

        std::unordered_map<std::string, std::string> f,inv;
        std::queue<std::pair<std::string, std::string> > q;

        q.push({init, other.init});
        f[init]=other.init;
        inv[other.init]=init;

        while(!q.empty()) {
            std::string nod1=q.front().first;
            std::string nod2=q.front().second;
            q.pop();

            if((fi.find(nod1)==fi.end())^(other.fi.find(nod2)==other.fi.end())) {
                return 0;
            }

            for(const auto &let:alphabet) {
                if((g.at(nod1).find(let)==g.at(nod1).end())^(other.g.at(nod2).find(let)==other.g.at(nod2).end())) {
                    return 0;
                }

                if(g.at(nod1).find(let)==g.at(nod1).end())
                    continue; 

                std::string vec1=g.at(nod1).at(let),vec2=other.g.at(nod2).at(let);
                if((f.find(vec1)!=f.end() && f.at(vec1)!=vec2) || (inv.find(vec2)!=inv.end() && inv.at(vec2)!=vec1)) {
                    return 0;
                }

                if(f.find(vec1)==f.end()) {
                    f[vec1]=vec2;
                    inv[vec2]=vec1;
                    q.push({vec1,vec2});
                }
            }
        }

        return f.size()==states.size();
    }
};

int main() {
    dfa x,y;
    std::cin >> x >> y;
    std::cout << (x==y) << "\n";
    return 0;
}