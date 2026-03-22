#include <bits/stdc++.h>    

class lnfa {
private:
    std::vector<std::string> alphabet,states,states_new;
    std::string init,init_new;
    std::unordered_map<std::string,bool> fi,fi_new;
    std::unordered_map<std::string,std::unordered_map<std::string,std::string> > g,g_new;
    std::unordered_map<std::string,std::string> maps;

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

    bool in_set(const std::string &str, const std::set<std::string> &s){
        if(s.find(str)!=s.end())
            return 1;
        return 0;
    }

    std::set<std::string> intersection(const std::set<std::string> &s1, const std::set<std::string> &s2) {
        std::set<std::string> ans;
        if(s1.size()<s2.size()){
            for(const auto &it:s1){
                if(!in_set(it,s2)){
                    continue;
                }
                ans.insert(it);
            }
        }
        else {
            for(const auto &it:s2){
                if(!in_set(it,s1)){
                    continue;
                }
                ans.insert(it);
            }
        }
        return ans;
    }   

    std::set<std::string> diff(const std::set<std::string> &s1, const std::set<std::string> &s2){
        std::set<std::string> ans;
        for(const auto &it:s1){
            if(in_set(it,s2)){
                continue;
            }
            ans.insert(it);
        }
        return ans;
    }

    void build_output(const std::vector<std::set<std::string> > &v){
        for(int i=0; i<(int)v.size(); i++){
            std::string s="q"+std::to_string(i);
            states_new.push_back(s);
            for(const auto &it:v[i]){
                maps[it]=s;
                if(it==init)
                    init_new=s;
                if(fi[it])
                    fi_new[s]=1;
            }
        }

        for(const auto &it:states){
            for(const auto &let:alphabet){
                if(g[it].find(let)==g[it].end())
                    continue;
                g_new[maps[it]][let]=maps[g[it][let]];
            }
        }
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

    void hopcroft() { // evil magic 
        std::vector<std::set<std::string> > p; // tranzitiile raman the same
        std::vector<std::set<std::string> > w;
        std::set<std::string> aux,pp,sec,dif;

        for(const auto &it:states) {
            if(fi[it])
                aux.insert(it);
            else
                pp.insert(it);
        }

        p.push_back(pp);
        p.push_back(aux);
        w.push_back(pp);
        w.push_back(aux);

        while(!w.empty()) {
            aux=w.back();
            w.pop_back();
            for(const auto &let:alphabet) {
                pp.clear();
                for(const auto state:states){
                    if(g[state].find(let)!=g[state].end() && in_set(g[state][let],aux)) { 
                        pp.insert(state);
                    }
                }

                bool gasite=1;
                while(gasite){
                    gasite=0;
                    for(const auto &it:p){
                        sec=intersection(pp,it);
                        dif=diff(it,pp);
                    
                        if(sec.empty() || dif.empty())
                            continue;
                        gasite=1;

                        const auto unde=find(w.begin(),w.end(),it);
                        if(unde!=w.end()){
                            w.erase(unde);
                            w.push_back(sec);
                            w.push_back(dif);
                        }
                        else {
                            if(sec.size()<=dif.size())
                                w.push_back(sec);
                            else
                                w.push_back(sec);
                        }

                        const auto unde1=find(p.begin(),p.end(),it);
                        p.erase(unde1);
                        p.push_back(dif);
                        p.push_back(sec);
                        break;
                    }
                }
            }
        }   
        build_output(p);
        // for(const auto &it:p){
        //     for(const auto &i:it){
        //         std::cout << i << " ";
        //     }
        //     std::cout << "\n";
        // }
    }

    void show(const std::string &file){
        std::ofstream fout(file);

        fout << (int)states_new.size() << "\n";
        for(const auto &it:states_new){
            fout << it;
            if(it==init_new)
                fout << " i ";
            if(fi_new[it]==1)
                fout << " f ";
            fout << "\n";
        }

        fout << (int)alphabet.size() << "\n";
        for(const auto &it:alphabet)
            fout << it << "\n";

        for(const auto &nod:states_new){
            for(const auto &vec:g_new[nod]){
                fout << nod << " " << vec.first << " " << vec.second << "\n";
            }
        }

        fout.close();
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
    v.hopcroft();
    fin.close();
}

void outputDfa() 
{
    v.show("dfa.out");
}

int main(int argc, char *argv[])
{
    readDfa();
    outputDfa();
    return 0;
}