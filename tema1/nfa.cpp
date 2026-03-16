#include <bits/stdc++.h>    

// same ca la dfa, $ o sa fie luat drept lambda
// cum difera aici?
// daca vrem doar sa decidem daca un cuvant este acceptat sau nu 
// putem sa rulam un dp care ruleaza in O(nr_states * len)
// daca vrem sa afisam tot parcursul ne ducem spre o exponentiala 

class nfa {
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
        g[x][let].push_back(y);
    }

    void compute_word_dp(const std::string &s){
        
    }

    void compute_word(const std::string &s){

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

void readNfa()
{
    std::ifstream fin("nfa.in");
    int n;

    fin >> n;
    fin.get();

    for(int i=1; i<=n; i++){
        getline(fin,aux);
        std::pair<std::string,int> u=split(aux);
        v.add_state(u.first,u.second);
    }

    fin >> n;
    fin.get();
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

}

int main(int argc, char *argv[])
{
    return 0;
}