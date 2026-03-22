#include <bits/stdc++.h>

// aici vreau sa am un generator de dfa-uri pentru un numar finit de cuvinte
// cum arata un astfel de dfa? well iese fix o trie cu acele cuvinte
// important de mentionat aici ca tria nu este neaparat dfa-ul minimal pentru 
// chestia asta

class trie {

private:

    int global_idx;
    std::vector<std::string> alphabet,states;
    std::vector<std::tuple<std::string,std::string,std::string> > edges;
    struct nodd {
        int fi;
        int label;
        std::unordered_map<std::string,nodd*> nxt;
        nodd() : fi(0), label(0) {}
    }*root;

public:

    trie() {
        global_idx=0;
        alphabet.clear();
        states.clear();
        edges.clear();
        root=new nodd;
    }

    void add_letter(const std::string &let){
        if(find(alphabet.begin(),alphabet.end(),let)!=alphabet.end()){
            return;
        }
        alphabet.push_back(let);
    }

    void insert_word(const std::string &s){
        if(s=="$"){
            this->root->fi++;
            return;
        }

        nodd *curr=root;
        std::string aux;
        int idx=0;
        while(idx<(int)s.size()){
            int mx=-1;
            for(const auto it:alphabet){
                if(s.substr(idx,(int)it.size())==it && (int)it.size()>mx){
                    mx=(int)it.size();
                    aux=it;
                }
            }

            if(mx==-1){
                std::cerr << "bad word\n";
                return;
            }

            if(curr->nxt.find(aux)==curr->nxt.end()){
                curr->nxt[aux]=new nodd;
            }

            // std::cout << s.substr(idx,mx) << "\n";
            curr=curr->nxt[aux];
            idx+=mx;
        }
        curr->fi++;
    }

    int cnt_nodes(nodd *nod){
        int ans=1;
        for(const auto it:nod->nxt){
            ans+=cnt_nodes(it.second);
        }
        return ans;
    }

    std::string labelize(int x){
        return "q"+std::to_string(x);
    }

    void label_nodes(nodd *nod){
        nod->label=global_idx++;
        std::string ss=labelize(nod->label);
        if(nod==root){
            ss=ss+" i";
        }

        if(nod->fi>0){
            ss=ss+" f";
        }

        states.push_back(ss);
        for(const auto it:nod->nxt){
            edges.push_back(std::make_tuple(labelize(nod->label),it.first,labelize(global_idx)));
            label_nodes(it.second);
        }
    }

    void build_trie(const std::vector<std::string> &v){
        for(const auto it:v){
            insert_word(it);
        }
    }

    void build_input(const std::string &s){
        std::ofstream fout(s);

        label_nodes(root);

        fout << (int)states.size() << "\n";
        for(const auto it:states){
            fout << it << "\n";
        }
        fout << (int)alphabet.size() << "\n";
        for(const auto it:alphabet){
            fout << it << "\n";
        }

        for(const auto [x,l,y]:edges){
            fout << x << " " << l << " " << y << "\n";
        }
        fout.close();
    }

    void dfs_del(nodd *nod){
        for(const auto it:nod->nxt) {
            dfs_del(it.second);
        }
        delete nod;
    }

    ~trie() {
        dfs_del(root);
    }
}v;

int main()
{
    std::string aux;
    for(int i=0; i<26; i++){
        aux=(char)('a'+i);
        v.add_letter(aux);
    }

    int n;
    std::cin >> n;
    for(int i=0; i<n; i++){
        std::cin >> aux;
        v.insert_word(aux);
    }

    v.build_input("./mini/dfa.in");
    
    return 0;
}