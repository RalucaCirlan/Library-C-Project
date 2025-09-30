#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
using namespace std;

// =========================
// CLASA CARTE
// =========================
class Carte {
private:
    string titlu;
    string autor;
    int an;
    string gen;
    static int nrCarti; 

public:
    // Constructor implicit
    Carte() : titlu("Necunoscut"), autor("Anonim"), an(1900), gen("General") {
        nrCarti++;
    }

    // Constructor cu parametri
    Carte(string titlu, string autor, int an, string gen)
        : titlu(titlu), autor(autor), an(an), gen(gen) {
        nrCarti++;
    }

    // Constructor de copiere
    Carte(const Carte& c)
        : titlu(c.titlu), autor(c.autor), an(c.an), gen(c.gen) {
        nrCarti++;
    }

    // Destructor
    ~Carte() {}

    // Getteri și setteri
    string getTitlu() const { return titlu; }
    void setTitlu(string t) { titlu = t; }

    string getAutor() const { return autor; }
    void setAutor(string a) { autor = a; }

    // Metoda proprie 1
    void descriere() const {
        cout << "Carte: " << titlu << " de " << autor << " (" << an << ") - " << gen << endl;
    }

    // Metoda proprie 2
    bool esteClasica() const {
        return an < 1950;
    }

    // Operator de afisare
    friend ostream& operator<<(ostream& out, const Carte& c) {
        out << "[Carte] " << c.titlu << " | " << c.autor << " | " << c.an << " | " << c.gen;
        return out;
    }

    // Pentru salvare în fisier binar
    void scrieInFisier(ofstream& f) const {
        int lg = titlu.size();
        f.write((char*)&lg, sizeof(int));
        f.write(titlu.c_str(), lg);

        lg = autor.size();
        f.write((char*)&lg, sizeof(int));
        f.write(autor.c_str(), lg);

        f.write((char*)&an, sizeof(int));

        lg = gen.size();
        f.write((char*)&lg, sizeof(int));
        f.write(gen.c_str(), lg);
    }

    // Pentru citire din fisier binar
    void citesteDinFisier(ifstream& f) {
        int lg;
        char buf[100];

        f.read((char*)&lg, sizeof(int));
        f.read(buf, lg);
        buf[lg] = '\0';
        titlu = buf;

        f.read((char*)&lg, sizeof(int));
        f.read(buf, lg);
        buf[lg] = '\0';
        autor = buf;

        f.read((char*)&an, sizeof(int));

        f.read((char*)&lg, sizeof(int));
        f.read(buf, lg);
        buf[lg] = '\0';
        gen = buf;
    }
};

int Carte::nrCarti = 0;

// =========================
// CLASA EXEMPLAR CARTE
// =========================
class ExemplarCarte {
private:
    int codUnic;
    const Carte* carte; // referinta la o Carte

public:
    // Constructor
    ExemplarCarte(int cod, const Carte* c) : codUnic(cod), carte(c) {}

    // Operator ==
    bool operator==(const ExemplarCarte& ec) const {
        return this->carte->getTitlu() == ec.carte->getTitlu() &&
            this->carte->getAutor() == ec.carte->getAutor();
    }

    // Operator de afisare
    friend ostream& operator<<(ostream& out, const ExemplarCarte& e) {
        out << "[Exemplar] Cod: " << e.codUnic << ", Carte: " << *(e.carte);
        return out;
    }

    int getCod() const { return codUnic; }
    const Carte* getCarte() const { return carte; }
};

// =========================
// CLASA CITITOR
// =========================
class Cititor {
private:
    string cnp;
    string nume;
    vector<ExemplarCarte> imprumuturi;

public:
    // Constructor
    Cititor(string cnp, string nume) : cnp(cnp), nume(nume) {}

    // Operator += (imprumută o carte)
    Cititor& operator+=(const ExemplarCarte& ec) {
        imprumuturi.push_back(ec);
        return *this;
    }

    // Operator -= (returneaza o carte dupa cod)
    Cititor& operator-=(int codExemplar) {
        for (auto it = imprumuturi.begin(); it != imprumuturi.end(); ++it) {
            if (it->getCod() == codExemplar) {
                imprumuturi.erase(it);
                break;
            }
        }
        return *this;
    }

    // Afisare
    friend ostream& operator<<(ostream& out, const Cititor& c) {
        out << "Cititor: " << c.nume << " (CNP: " << c.cnp << ")\n";
        out << "Imprumuturi:\n";
        for (const auto& e : c.imprumuturi)
            out << "  - " << e << endl;
        return out;
    }
};

// =========================
// MAIN DE TEST
// =========================
int main() {
    
    Carte c1("Enigma Otiliei", "George Calinescu", 1940, "Roman");
    Carte c2("Morometii", "Marin Preda", 1955, "Roman");

    
    ExemplarCarte ex1(1001, &c1);
    ExemplarCarte ex2(1002, &c1);
    ExemplarCarte ex3(1003, &c2);

    Cititor cit("1234567890123", "Ana Popescu");
    cit += ex1;
    cit += ex3;

    cout << cit;

    cit -= 1001;
    cout << "\nDupa returnare:\n" << cit;


    ofstream fout("carte.bin", ios::binary);
    c1.scrieInFisier(fout);
    fout.close();

    ifstream fin("carte.bin", ios::binary);
    Carte cNoua;
    cNoua.citesteDinFisier(fin);
    fin.close();
    cout << "\nCitita din fisier:\n" << cNoua << endl;

    return 0;
}
