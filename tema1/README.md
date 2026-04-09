# Laborator 1 & 2 - LFA
**Mocanu Ștefan**

---

## 1 DFA

Un automat finit determinist (**DFA**) este definit de tuplul $(Q, \Sigma, q_0, \delta, F)$, unde:

1. $Q$ este mulțimea de stări
2. $\Sigma$ este alfabetul
3. $q_0$ este starea inițială
4. $\delta : Q \times \Sigma \rightarrow Q$ este funcția de tranziție
5. $F$ este mulțimea de stări finale

### 1.1 Idee de implementare

Tranziția de la definiția matematică la un program presupune alegerea unei structuri de date eficiente pentru funcția $\delta$. Cele mai utilizate abordări sunt:

- **Matricea de tranziție (Tabelul de stare):** Este cea mai eficientă metodă pentru un DFA. Se definește o matrice $M$ de dimensiune $|Q| \times |\Sigma|$, unde elementul $M[q][a]$ reprezintă starea următoare.
  - *Avantaj:* Acces în timp constant $O(1)$.
  - *Dezavantaj:* Consumă memorie $O(|Q| \times |\Sigma|)$, ceea ce poate fi ineficient pentru alfabete mari.

- **Liste de adiacență / Dicționare:** Fiecare stare $q \in Q$ are asociată o listă de perechi de forma $(a, p)$, unde $a \in \Sigma$ și $p = \delta(q, a)$.
  - *Avantaj:* Eficiență în spațiu dacă automatul are puține tranziții definite (rar).
  - *Dezavantaj:* Timpul de căutare crește la $O(\log |\Sigma|)$ sau $O(|\Sigma|)$ în funcție de structură.

### 1.2 Algoritmul de Simulare

Indiferent de reprezentarea aleasă, algoritmul prin care un DFA decide dacă acceptă sau respinge un cuvânt $w = s_1 s_2 \ldots s_n$ este următorul:

1. Se inițializează starea curentă $\text{curr} \leftarrow q_0$.
2. Pentru fiecare simbol $s_i$ din cuvântul de intrare $w$:
   - Se actualizează starea: $\text{curr} \leftarrow \delta(\text{curr}, s_i)$.
3. Dacă $\text{curr} \in F$, atunci cuvântul este **acceptat**.
4. Altfel, cuvântul este **respins**.

---

## 2 NFA

Un Automat Finit Nedeterminist (**NFA**) este definit de tuplul $(Q, \Sigma, q_0, \delta, F)$, unde componentele $Q$, $\Sigma$, $q_0$ și $F$ au aceeași semnificație ca la DFA, iar:

4. $\delta : Q \times \Sigma \rightarrow \mathcal{P}(Q)$ este funcția de tranziție, care asociază unei stări și unui simbol o mulțime de stări posibile.

### 2.1 Idee de implementare

Spre deosebire de DFA, unde rezultatul funcției $\delta$ este o singură stare, la NFA rezultatul este o listă sau un set de stări.

- **Matrice de liste/seturi:** Se folosește o matrice de dimensiune $|Q| \times |\Sigma|$, unde fiecare element $M[q][a]$ este o colecție (vector sau set) de stări.
- **Liste de adiacență extinse:** Cea mai flexibilă metodă, unde pentru fiecare stare se stochează o listă de perechi (simbol, [stariUrmatoare]).

### 2.2 Algoritmul de Simulare

Simularea unui NFA necesită urmărirea simultană a tuturor stărilor posibile în care se poate afla automatul după citirea unui simbol.

1. Se inițializează mulțimea stărilor curente: $S \leftarrow \{q_0\}$.
2. Pentru fiecare simbol $s_i$ din cuvântul de intrare $w$:
   - Se creează o mulțime nouă de stări $S_{\text{next}} \leftarrow \emptyset$.
   - Pentru fiecare $q \in S$:
     - $S_{\text{next}} \leftarrow S_{\text{next}} \cup \delta(q, s_i)$.
   - $S \leftarrow S_{\text{next}}$.
3. Dacă $S \cap F \neq \emptyset$, atunci cuvântul este **acceptat**.
4. Altfel, cuvântul este **respins**.

---

## 3 λ-NFA

Un automat cu λ-tranziții adaugă posibilitatea de a schimba starea fără a consuma un simbol din alfabet. Acesta este definit de tuplul $(Q, \Sigma, q_0, \delta, F)$, unde:

4. $\delta : Q \times (\Sigma \cup \{\lambda\}) \rightarrow \mathcal{P}(Q)$ este funcția de tranziție.

### 3.1 Idee de implementare: λ-closure

Elementul central în implementarea unui λ-NFA este funcția de închidere (closure). Pentru o stare $q$, $\lambda\text{-closure}(q)$ este mulțimea stărilor în care se poate ajunge din $q$ folosind doar λ-tranziții.

- **Calculul λ-closure:** Se poate implementa folosind un algoritm de parcurgere în adâncime (DFS) sau lățime (BFS) pe graful format doar de λ-tranziții.

### 3.2 Algoritmul de Simulare

Pentru a simula un λ-NFA, trebuie să aplicăm închiderea λ atât la început, cât și după fiecare simbol procesat.

1. Se inițializează $S \leftarrow \lambda\text{-closure}(q_0)$.
2. Pentru fiecare simbol $s_i$ din cuvântul de intrare $w$:
   - Se calculează stările accesibile prin $s_i$: $S_{\text{temp}} \leftarrow \bigcup_{q \in S} \delta(q, s_i)$.
   - Se aplică închiderea pentru noile stări: $S \leftarrow \bigcup_{p \in S_{\text{temp}}} \lambda\text{-closure}(p)$.
3. Dacă $S \cap F \neq \emptyset$, atunci cuvântul este **acceptat**.

---

## 4 Temă de laborator

Scopul acestei teme este implementarea practică a modelelor teoretice prezentate anterior și înțelegerea modului în care nedeterminismul afectează procesul de simulare.

### 4.1 Cerințe

Să se implementeze câte un program (sau un singur program modular) într-un limbaj de programare la alegere (C++, Python, Java, Rust etc.) care să îndeplinească următoarele condiții:

1. **Citirea Automatului:** Programul trebuie să citească dintr-un fișier de intrare structura automatului (DFA, NFA sau λ-NFA).
2. **Reprezentarea Datelor:** Veți defini propria voastră sintaxă pentru fișierul de intrare (ex: JSON, YAML sau un format text simplu). Acesta trebuie să specifice în mod clar:
   - Mulțimea stărilor $Q$ (sau numărul total de stări).
   - Alfabetul $\Sigma$.
   - Starea inițială $q_0$.
   - Mulțimea stărilor finale $F$.
   - Tabelul de tranziții $\delta$.
3. **Validarea Cuvintelor:** Programul va citi dintr-un fișier separat (sau de la tastatură) o serie de cuvinte și va afișa pentru fiecare dacă este `ACCEPTAT` sau `RESPINS` de automat.
4. **Suport pentru λ:** În cazul automatelor λ-NFA, alegeți un caracter special (ex: `!`, `@`, șirul `"lambda"`, sau chiar `λ` dacă limbajul acceptă Unicode) pentru a reprezenta tranzițiile vide în fișierul de configurare.

### 4.2 Exemplu de flux de lucru

La laborator, vi se va prezenta diagrama unui automat (graf). Sarcina voastră este:

1. Să transcrieți graful într-un fișier de input conform sintaxei alese de voi.
2. Să rulați simulatorul implementat pe acel fișier.
3. Să verificați corectitudinea implementării folosind setul de cuvinte de test.

> **Notă:** Se recomandă utilizarea seturilor (`std::unordered_set` în C++, `set()` în Python) pentru a gestiona mulțimile de stări în cazul NFA și pentru a implementa eficient funcția λ-closure.

### 4.3 Bonus

Pentru nota 12, va trebui să implementați fie algoritmul de minimizare, fie transformarea de la λ-NFA la DFA.