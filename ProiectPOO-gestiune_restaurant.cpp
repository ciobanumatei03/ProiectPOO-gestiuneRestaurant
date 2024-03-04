#include <iostream>
#include <ctime>
#include <string>
#include <exception>
#include <math.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#pragma warning(disable : 4996)
using namespace std;

class ExceptieInput : public exception
{
	string mesaj;
public:

	ExceptieInput(const string& mesaj) : mesaj(mesaj) {}

	virtual const char* what() const noexcept override
	{
		return mesaj.c_str();
	}

};

class ISerializabil
{
public:
	virtual void saveToFile(fstream& file) = 0;
	virtual void restoreFromFile(fstream& file) = 0;
};



class Ingredient : public ISerializabil
{
	char* numeIngredient = nullptr;
	int cantitate = 0;
	float pretUnitar = 0;
	struct tm data_expirarii;

public:

	Ingredient()
	{
		this->numeIngredient = new char[strlen("N/A") + 1];
		strcpy(this->numeIngredient, "N/A");
		time_t currentTime = time(nullptr);
		data_expirarii = *localtime(&currentTime);
	}
	Ingredient(const char* numeIngredient)
	{
		if (numeIngredient != nullptr)
		{
			this->numeIngredient = new char[strlen(numeIngredient) + 1];
			strcpy(this->numeIngredient, numeIngredient);
		}

		time_t currentTime = time(nullptr);
		data_expirarii = *localtime(&currentTime);
	}

	Ingredient(const char* numeIngredient, int cantitate)
	{
		if (numeIngredient != nullptr)
		{
			this->numeIngredient = new char[strlen(numeIngredient) + 1];
			strcpy(this->numeIngredient, numeIngredient);
		}
		if (cantitate > 0)this->cantitate = cantitate;
	}

	Ingredient(const char* numeIngredient, int cantitate, float pretUnitar)
	{
		if (numeIngredient != nullptr)
		{
			this->numeIngredient = new char[strlen(numeIngredient) + 1];
			strcpy(this->numeIngredient, numeIngredient);
		}
		if (cantitate > 0)this->cantitate = cantitate;
		if (pretUnitar > 0)this->pretUnitar = pretUnitar;
		time_t currentTime = time(nullptr);
		data_expirarii = *localtime(&currentTime);
	}

	Ingredient(const char* numeIngredient, int cantitate, float pretUnitar, int zi, int luna, int an)
	{
		if (numeIngredient != nullptr)
			this->numeIngredient = new char[strlen(numeIngredient) + 1];
		strcpy(this->numeIngredient, numeIngredient);
		if (cantitate > 0)this->cantitate = cantitate;
		if (pretUnitar > 0)this->pretUnitar = pretUnitar;
		if (zi > 0 && zi <= 31)this->data_expirarii.tm_mday = zi;
		if (luna > 0 && luna <= 12)this->data_expirarii.tm_mon = luna - 1;
		if (an >= 2023)this->data_expirarii.tm_year = an - 1900;

		time_t currentTime = time(nullptr);
		struct tm timpCurent = *localtime(&currentTime);

		if ((this->data_expirarii.tm_mday < timpCurent.tm_mday && this->data_expirarii.tm_mon == timpCurent.tm_mon && this->data_expirarii.tm_year == timpCurent.tm_year) || (this->data_expirarii.tm_mon < timpCurent.tm_mon && this->data_expirarii.tm_year == timpCurent.tm_year) || this->data_expirarii.tm_year < timpCurent.tm_year)this->data_expirarii = *localtime(&currentTime);
	}


	Ingredient(const Ingredient& i)
	{
		if (i.numeIngredient != nullptr)
			this->numeIngredient = new char[strlen(i.numeIngredient) + 1];
		strcpy(this->numeIngredient, i.numeIngredient);
		this->cantitate = i.cantitate;
		this->pretUnitar = i.pretUnitar;
		this->data_expirarii.tm_mday = i.data_expirarii.tm_mday;
		this->data_expirarii.tm_mon = i.data_expirarii.tm_mon;
		this->data_expirarii.tm_year = i.data_expirarii.tm_year;
		//this->data_expirarii = *localtime(&i.data_expirarii);

	}

	Ingredient& operator=(const Ingredient& i)
	{
		if (this != &i)
		{
			if (this->numeIngredient != nullptr)
				delete[] this->numeIngredient;
			this->numeIngredient = nullptr;

			if (i.numeIngredient != nullptr)
				this->numeIngredient = new char[strlen(i.numeIngredient) + 1];
			strcpy(this->numeIngredient, i.numeIngredient);
			this->cantitate = i.cantitate;
			this->pretUnitar = i.pretUnitar;
			this->data_expirarii.tm_mday = i.data_expirarii.tm_mday;
			this->data_expirarii.tm_mon = i.data_expirarii.tm_mon;
			this->data_expirarii.tm_year = i.data_expirarii.tm_year;
			//this->data_expirarii = *localtime(&i.data_expirarii);

		}

		return *this;
	}

	friend ostream& operator<<(ostream& out, Ingredient& i)
	{
		out << "\nNume ingredient: " << i.numeIngredient;
		out << "\nCantitate disponibila: " << i.cantitate;
		out << "\nPret unitar: " << i.pretUnitar << " u.m";
		out << "\nData expirarii: " << i.data_expirarii.tm_mday << '/' << i.data_expirarii.tm_mon + 1 << "/" << i.data_expirarii.tm_year + 1900;
		out << "\n----------------------------------------";
		return out;
	}

	friend istream& operator>>(istream& in, Ingredient& i)
	{
		delete[] i.numeIngredient;
		if (i.numeIngredient != nullptr)i.numeIngredient = nullptr;
		cout << "\nIntroduceti numele ingredientului: ";
		string buffer;
		getline(in, buffer);
		i.numeIngredient = new char[buffer.size() + 1];
		strcpy(i.numeIngredient, buffer.data());

		cout << "\nIntroduceti cantitatea: ";
		in >> i.cantitate;
		if (i.cantitate <= 0)
		{
			i.cantitate = 0;
			throw ExceptieInput("Cantitatea trebuie sa fie un numar pozitiv!");
		}

		cout << "\nIntroduceti pretul unitar: ";
		in >> i.pretUnitar;
		if (i.pretUnitar <= 0)
		{
			i.pretUnitar = 0;
			throw ExceptieInput("Pretul trebuie sa fie un numar pozitiv!");
		}

		cout << "\nIntroduceti data expirarii: ";
		cout << "\n Ziua: "; in >> i.data_expirarii.tm_mday;
		cout << "\n Luna: "; in >> i.data_expirarii.tm_mon;
		cout << "\n An: "; in >> i.data_expirarii.tm_year;
		i.data_expirarii.tm_mon -= 1;
		i.data_expirarii.tm_year -= 1900;
		time_t currentTime = time(nullptr);
		struct tm timpCurent = *localtime(&currentTime);

		if (i.data_expirarii.tm_mday < 1 && i.data_expirarii.tm_mday>31)
		{
			i.data_expirarii.tm_mday = timpCurent.tm_mday;
			throw ExceptieInput("Data invalida");
		}
		if (i.data_expirarii.tm_mon < 1 && i.data_expirarii.tm_mon>12)
		{
			i.data_expirarii.tm_mon = timpCurent.tm_mon;
			throw ExceptieInput("Data invalida");
		}
		if (i.data_expirarii.tm_year < timpCurent.tm_year)
		{
			i.data_expirarii.tm_year = timpCurent.tm_year;
			throw ExceptieInput("Data invalida");
		}

		if ((i.data_expirarii.tm_mday < timpCurent.tm_mday && i.data_expirarii.tm_mon == timpCurent.tm_mon && i.data_expirarii.tm_year == timpCurent.tm_year) || (i.data_expirarii.tm_mon < timpCurent.tm_mon && i.data_expirarii.tm_year == timpCurent.tm_year) || i.data_expirarii.tm_year < timpCurent.tm_year)i.data_expirarii = *localtime(&currentTime);

	}

	Ingredient operator+(int cantitateSuplimentara)
	{
		if (cantitateSuplimentara > 0)
		{
			Ingredient rez;
			rez = *this;
			rez.cantitate += cantitateSuplimentara;
			return rez;

		}

	}

	Ingredient& operator+=(int cantitateSuplimentara)
	{
		if (cantitateSuplimentara > 0)
		{
			this->cantitate += cantitateSuplimentara;
			return *this;
		}
	}

	Ingredient operator-(int cantitateFolosita)
	{
		if (cantitateFolosita > 0)
		{
			Ingredient rez;
			rez = *this;
			rez.cantitate -= cantitateFolosita;
			return rez;
		}
	}

	Ingredient& operator-=(int cantitateFolosita)
	{
		if (cantitateFolosita > 0)
		{
			this->cantitate -= cantitateFolosita;
			return *this;
		}
	}

	bool operator>=(int valoare)
	{
		if (this->cantitate >= valoare)return true;
		return false;
	}

	explicit operator int()
	{
		return this->cantitate;
	}

	bool operator!()
	{
		return !this->cantitate;
	}

	bool operator==(Ingredient i)
	{
		if (strcmp(this->numeIngredient, i.numeIngredient) != 0)return false;
		if (this->cantitate != i.cantitate)return false;
		if (this->pretUnitar != i.pretUnitar)return false;
		if (this->data_expirarii.tm_mday != i.data_expirarii.tm_mday)return false;
		if (this->data_expirarii.tm_mon != i.data_expirarii.tm_mon)return false;
		if (this->data_expirarii.tm_year != i.data_expirarii.tm_year)return false;
		return true;

	}

	const char* getNumeIngredient()
	{
		return this->numeIngredient;
	}

	void setNumeIngredient(const char* numeNouIngredient)
	{
		if (numeNouIngredient != nullptr && strlen(numeIngredient) >= 2)
		{
			delete[]this->numeIngredient;
			this->numeIngredient = nullptr;
			this->numeIngredient = new char[strlen(numeNouIngredient) + 1];
			strcpy(this->numeIngredient, numeNouIngredient);

		}
	}

	void saveToFile(fstream& file)
	{
		int lg = strlen(this->numeIngredient) + 1;
		file.write((char*)&lg, sizeof(lg));
		file.write(this->numeIngredient, lg);


		file.write((char*)&this->cantitate, sizeof(int));
		file.write((char*)&this->pretUnitar, sizeof(float));

		int zi, luna, an;
		zi = this->data_expirarii.tm_mday;
		luna = this->data_expirarii.tm_mon + 1;
		an = this->data_expirarii.tm_year + 1900;
		file.write((char*)&zi, sizeof(int));
		file.write((char*)&luna, sizeof(int));
		file.write((char*)&an, sizeof(int));

	}

	void restoreFromFile(fstream& file)
	{
		delete[]this->numeIngredient;
		int lg;
		file.read((char*)&lg, sizeof(int));
		this->numeIngredient = new char[lg];
		file.read(this->numeIngredient, lg);

		file.read((char*)&this->cantitate, sizeof(int));
		file.read((char*)&this->pretUnitar, sizeof(float));
		int zi, luna, an;
		file.read((char*)&zi, sizeof(int));
		file.read((char*)&luna, sizeof(int));
		file.read((char*)&an, sizeof(int));
		this->data_expirarii.tm_mday = zi;
		this->data_expirarii.tm_mon = luna - 1;
		this->data_expirarii.tm_year = an - 1900;

	}

	int getCantitate() const {
		return this->cantitate;
	}

	void setCantitate(int cantitateNoua)
	{
		if (cantitateNoua > 0)this->cantitate = cantitateNoua;
	}

	float getPretUnitar()
	{
		return this->pretUnitar;
	}

	void setPretUnitar(float pretNou)
	{
		if (pretNou > 0) this->pretUnitar = pretNou;
	}

	tm getDataExpirarii()
	{
		return this->data_expirarii;
	}

	void setDataExpirarii(int zi, int luna, int an)
	{
		if (zi > 0 && zi <= 31)this->data_expirarii.tm_mday = zi;
		if (luna > 0 && luna <= 12)this->data_expirarii.tm_mon = luna - 1;
		if (an >= 2023)this->data_expirarii.tm_year = an - 1900;

		time_t currentTime = time(nullptr);
		struct tm timpCurent = *localtime(&currentTime);

		if ((this->data_expirarii.tm_mday < timpCurent.tm_mday && this->data_expirarii.tm_mon == timpCurent.tm_mon && this->data_expirarii.tm_year == timpCurent.tm_year) || (this->data_expirarii.tm_mon < timpCurent.tm_mon && this->data_expirarii.tm_year == timpCurent.tm_year) || this->data_expirarii.tm_year < timpCurent.tm_year)this->data_expirarii = *localtime(&currentTime);


	}

	~Ingredient()
	{
		delete[] this->numeIngredient;
		if (this->numeIngredient != nullptr)this->numeIngredient = nullptr;
	}


};





class GestiuneStoc : public ISerializabil
{

	static Ingredient* listaIngrediente;
	static int nrIngrediente;

public:

	GestiuneStoc()
	{
	}

	GestiuneStoc(Ingredient* listaIngrediente, int nrIngrediente)
	{
		if (listaIngrediente != nullptr && nrIngrediente > 0)
		{
			this->listaIngrediente = new Ingredient[nrIngrediente];
			this->nrIngrediente = nrIngrediente;
			for (int i = 0; i < this->nrIngrediente; i++)
			{
				this->listaIngrediente[i] = listaIngrediente[i];
			}
		}
	}

	GestiuneStoc(const GestiuneStoc& g)
	{
		if (g.listaIngrediente != nullptr && g.nrIngrediente > 0)
		{
			this->listaIngrediente = new Ingredient[g.nrIngrediente];
			this->nrIngrediente = g.nrIngrediente;
			for (int i = 0; i < this->nrIngrediente; i++)
			{
				this->listaIngrediente[i] = listaIngrediente[i];
			}
		}

	}

	GestiuneStoc& operator=(const GestiuneStoc& g)
	{
		if (this != &g)
		{
			if (this->listaIngrediente != nullptr)
				delete[] this->listaIngrediente;
			this->listaIngrediente = nullptr;

			if (g.listaIngrediente != nullptr && g.nrIngrediente > 0)
			{
				this->listaIngrediente = new Ingredient[g.nrIngrediente];
				this->nrIngrediente = g.nrIngrediente;
				for (int i = 0; i < this->nrIngrediente; i++)
				{
					this->listaIngrediente[i] = g.listaIngrediente[i];
				}
			}
		}

		return *this;
	}

	friend ostream& operator<<(ostream& out, GestiuneStoc& g)
	{
		out << "\nLista Ingrediente: ";
		for (int i = 0; i < g.nrIngrediente; i++)out << g.listaIngrediente[i];
		return out;
	}

	friend ofstream& operator<<(ofstream& out, GestiuneStoc& g)
	{

		for (int i = 0; i < g.nrIngrediente; i++)out << g.listaIngrediente[i];
		return out;
	}

	int getNrIngrediente()
	{
		return this->nrIngrediente;
	}

	void setListaIngrediente(int nrIngrediente, Ingredient* listaIngrediente)
	{
		if (nrIngrediente > 0 && listaIngrediente != nullptr)
		{
			delete[] this->listaIngrediente;
			this->listaIngrediente = new Ingredient[nrIngrediente];
			this->nrIngrediente = nrIngrediente;
			for (int i = 0; i < this->nrIngrediente; i++)
				this->listaIngrediente[i] = listaIngrediente[i];
		}
	}

	const Ingredient* getListaIngrediente()
	{
		return this->listaIngrediente;
	}

	void saveToFile(fstream& file)
	{
		file.write((char*)&this->nrIngrediente, sizeof(int));
		for (int i = 0; i < this->nrIngrediente; i++)
		{
			int lg = strlen(this->listaIngrediente[i].getNumeIngredient()) + 1;
			file.write((char*)&lg, sizeof(int));
			file.write(listaIngrediente[i].getNumeIngredient(), lg);

			int cantitate = listaIngrediente[i].getCantitate();
			file.write((char*)&cantitate, sizeof(int));

			float pret = listaIngrediente[i].getPretUnitar();
			file.write((char*)&pret, sizeof(float));

			struct tm data_expirarii = listaIngrediente[i].getDataExpirarii();
			int zi, luna, an;
			zi = data_expirarii.tm_mday;
			luna = data_expirarii.tm_mon + 1;
			an = data_expirarii.tm_year + 1900;
			file.write((char*)&zi, sizeof(int));
			file.write((char*)&luna, sizeof(int));
			file.write((char*)&an, sizeof(int));
		}

	}

	void restoreFromFile(fstream& file)
	{
		delete[] listaIngrediente;
		listaIngrediente = nullptr;
		nrIngrediente = 0;

		file.read((char*)&nrIngrediente, sizeof(int));
		listaIngrediente = new Ingredient[nrIngrediente];

		for (int i = 0; i < nrIngrediente; i++)
		{
			int lg;
			file.read((char*)&lg, sizeof(int));
			char* numeIngredient = new char[lg];
			file.read(numeIngredient, lg);
			int cantitate;
			file.read((char*)&cantitate, sizeof(int));
			float pretUnitar;
			file.read((char*)&pretUnitar, sizeof(float));
			int zi, luna, an;
			file.read((char*)&zi, sizeof(int));
			file.read((char*)&luna, sizeof(int));
			file.read((char*)&an, sizeof(int));
			listaIngrediente[i] = Ingredient(numeIngredient, cantitate, pretUnitar, zi, luna, an);

			delete[] numeIngredient;
		}
	}

	void adaugaIngredient(const Ingredient& i)
	{
		if (i.getCantitate() > 0)
		{
			Ingredient* listaCopie = new Ingredient[this->nrIngrediente];
			for (int i = 0; i < this->nrIngrediente; i++)listaCopie[i] = this->listaIngrediente[i];
			delete[] this->listaIngrediente;
			this->nrIngrediente++;
			this->listaIngrediente = new Ingredient[this->nrIngrediente];
			for (int i = 0; i < this->nrIngrediente - 1; i++)
				this->listaIngrediente[i] = listaCopie[i];
			this->listaIngrediente[this->nrIngrediente - 1] = i;
			delete[]listaCopie;
		}

	}

	void stergeIngredient(const string& numeIngredient)
	{
		for (int p = this->nrIngrediente - 1; p >= 0; p--)
		{
			if (strcmp(this->listaIngrediente[p].getNumeIngredient(), numeIngredient.c_str()) == 0)
			{
				for (int i = p; i < this->nrIngrediente - 1; i++)
					this->listaIngrediente[i] = this->listaIngrediente[i + 1];

				this->nrIngrediente--;

			}
		}
	}

	void actualizeazaCantitate(const string& numeIngredient, int cantitate)
	{
		if (cantitate != 0)
			for (int i = 0; i < this->nrIngrediente; i++)
			{
				if (strcmp(this->listaIngrediente[i].getNumeIngredient(), numeIngredient.c_str()) == 0)
					if (cantitate > 0)
						this->listaIngrediente[i] += cantitate;
					else if (cantitate < 0 && (this->listaIngrediente[i] - abs(cantitate) >= 0))this->listaIngrediente[i] -= abs(cantitate);
			}
	}


	static Ingredient* cautaIngredient(const string& numeIngredient)
	{
		for (int i = 0; i < nrIngrediente; i++)
		{
			if (strcmp(listaIngrediente[i].getNumeIngredient(), numeIngredient.c_str()) == 0)
			{
				return &(listaIngrediente[i]);
			}
		}
		return nullptr;
	}

	~GestiuneStoc()
	{
		delete[] this->listaIngrediente;
		if (this->listaIngrediente != nullptr)this->listaIngrediente = nullptr;
	}

};

Ingredient* GestiuneStoc::listaIngrediente = nullptr;
int GestiuneStoc::nrIngrediente = 0;


class Preparat : public ISerializabil
{
	char* numePreparat = nullptr;
	vector<pair<Ingredient*, int>> Reteta;
	float pret;

public:

	Preparat()
	{
		this->numePreparat = new char[strlen("N/A") + 1];
		strcpy(this->numePreparat, "N/A");
	}

	Preparat(const char* numePreparat, const vector<pair<Ingredient*, int>>& Reteta, float pret)
	{
		if (numePreparat != nullptr)
		{
			this->numePreparat = new char[strlen(numePreparat) + 1];
			strcpy(this->numePreparat, numePreparat);
		}
		if (Reteta.empty() == 0)
			this->Reteta = Reteta;
		if (pret != 0)this->pret = pret;
	}

	Preparat(const Preparat& p)
	{
		if (p.numePreparat != nullptr)
		{
			this->numePreparat = new char[strlen(p.numePreparat) + 1];
			strcpy(this->numePreparat, p.numePreparat);
		}
		this->Reteta = p.Reteta;
		if (p.pret != 0)this->pret = p.pret;
	}

	const char* getNumePreparat()
	{
		return this->numePreparat;
	}

	void setNumePreparat(const char* numeNou)
	{
		if (numeNou != nullptr)
		{
			delete[] this->numePreparat;
			this->numePreparat = nullptr;

			this->numePreparat = new char[strlen(numeNou) + 1];
			strcpy(this->numePreparat, numeNou);
		}
	}

	vector<pair<Ingredient*, int>> getReteta() const
	{
		return this->Reteta;
	}

	void setReteta(const vector<pair<Ingredient*, int>>& reteta)
	{
		if (reteta.empty() == 0)
		{
			this->Reteta.clear();
			for (const auto& ingredient : reteta)
			{
				this->Reteta.push_back(ingredient);
			}
		}
		else throw ExceptieInput("Reteta este goala!");
	}

	float getPretPreparat()
	{
		return this->pret;
	}

	void setPretPreparat(float pretNou)
	{
		if (pretNou > 0)
		{
			this->pret = pretNou;
		}
		else throw ExceptieInput("Pretul trebuie sa fie pozitiv!");
	}


	Preparat& operator=(const Preparat& p)
	{
		if (this != &p)
		{
			if (this->numePreparat != nullptr)
				delete[] this->numePreparat;
			this->numePreparat = nullptr;

			if (p.numePreparat != nullptr)
			{
				this->numePreparat = new char[strlen(p.numePreparat) + 1];
				strcpy(this->numePreparat, p.numePreparat);
			}
			this->Reteta = p.Reteta;
			if (p.pret != 0)this->pret = p.pret;

		}
	}

	friend ostream& operator<<(ostream& out, Preparat& p)
	{
		out << "\n**********PREPARAT*************";
		out << "\nNume preparat: " << p.numePreparat;
		out << "\nPret: " << p.pret;
		out << "\n~~~~~~~~~~~Reteta~~~~~~~~~~~~~" << endl;
		for (int i = 0; i < p.Reteta.size(); i++)
		{
			out << i + 1 << ". " << "Cantitate necesara " << p.Reteta[i].first->getNumeIngredient() << ":" << p.Reteta[i].second;
			out << *p.Reteta[i].first << endl;

		}
		out << "*******************************\n";
		return out;
	}

	friend istream& operator>>(istream& in, Preparat& p)
	{
		if (p.numePreparat != nullptr)
			delete[] p.numePreparat;
		p.numePreparat = nullptr;
		cout << "\nIntroduceti numele preparatului: ";
		string buffer;
		getline(in, buffer);
		p.numePreparat = new char[buffer.size() + 1];
		strcpy(p.numePreparat, buffer.data());
		cout << "\nIntroduceti pretul preparatului: ";
		in >> p.pret;
		in.ignore();
		int numarIngrediente;
		cout << "\nIntroduceti numarul de ingrediente: ";
		in >> numarIngrediente;
		in.ignore();

		p.Reteta.clear();
		for (int i = 0; i < numarIngrediente; i++)
		{


			string numeIngredient;
			int cantitate;
			cout << "\nIntroduceti numele ingredientului " << i + 1 << ": ";
			getline(in, numeIngredient);
			Ingredient* pi = GestiuneStoc::cautaIngredient(numeIngredient);

			if (pi != nullptr)
			{
				cout << "\nIntroduceti cantitatea necesara prepararii: ";
				in >> cantitate;
				in.ignore();
				p.Reteta.push_back(make_pair(pi, cantitate));
			}
			else
				cout << "Ingredientul " << numeIngredient << " nu exista in stoc";
		}

		return in;
	}

	void saveToFile(fstream& file)
	{
		int lg = strlen(this->numePreparat) + 1;
		file.write((char*)&lg, sizeof(int));
		file.write(this->numePreparat, lg);
		int nrIngrediente = this->Reteta.size();
		file.write((char*)&nrIngrediente, sizeof(int));
		for (auto& pereche : this->Reteta)
		{
			pereche.first->saveToFile(file);
			file.write((char*)&pereche.second, sizeof(int));
		}
		file.write((char*)&this->pret, sizeof(float));

	}

	void restoreFromFile(fstream& file)
	{
		int lg;
		file.read((char*)&lg, sizeof(int));
		this->numePreparat = new char[lg];
		file.read(this->numePreparat, lg);

		int nrIngrediente;
		file.read((char*)&nrIngrediente, sizeof(int));
		this->Reteta.clear();
		for (int i = 0; i < nrIngrediente; i++)
		{
			Ingredient* ing = new Ingredient();
			ing->restoreFromFile(file);
			int cantitate;
			file.read((char*)&cantitate, sizeof(int));
			this->Reteta.push_back(make_pair(ing, cantitate));
		}

		file.read((char*)&this->pret, sizeof(float));
	}

	~Preparat()
	{
		if (this->numePreparat != nullptr)
			delete[] this->numePreparat;
		this->numePreparat = nullptr;

	}

};



class Meniu : public ISerializabil
{
	Preparat** listaPreparate = nullptr;
	int nrPreparate = 0;
	string categorie = "N/A";

public:
	Meniu() {}
	Meniu(Preparat** listaPreparate, int nrPreparate, string categorie)
	{

		if (listaPreparate != nullptr && nrPreparate > 0)
		{
			this->nrPreparate = nrPreparate;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++)
				this->listaPreparate[i] = new Preparat(*listaPreparate[i]);
		}

		this->categorie = categorie;
	}
	Meniu(string categorie)
	{
		if (categorie.length() >= 3)this->categorie = categorie;
	}

	Meniu(const Meniu& m)
	{
		if (m.listaPreparate != nullptr && m.nrPreparate > 0)
		{
			this->nrPreparate = m.nrPreparate;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++)
				this->listaPreparate[i] = new Preparat(*m.listaPreparate[i]);
		}

		this->categorie = m.categorie;
	}

	~Meniu()
	{
		for (int i = 0; i < this->nrPreparate; i++)
			delete this->listaPreparate[i];
		if (this->listaPreparate != nullptr)
			delete[]this->listaPreparate;
		this->listaPreparate = nullptr;
	}

	void adaugaPreparatInMeniu(const Preparat& preparatNou)
	{
		Meniu copie = *this;
		for (int i = 0; i < this->nrPreparate; i++)delete this->listaPreparate[i];
		delete[] this->listaPreparate;
		this->nrPreparate++;
		this->listaPreparate = new Preparat * [this->nrPreparate];
		for (int i = 0; i < this->nrPreparate - 1; i++)
			this->listaPreparate[i] = new Preparat(*copie.listaPreparate[i]);
		this->listaPreparate[this->nrPreparate - 1] = new Preparat(preparatNou);

	}

	void stergePreparatMeniu(const string& numePreparat)
	{

		for (int p = this->nrPreparate - 1; p >= 0; p--)
		{
			if (strcmp(this->listaPreparate[p]->getNumePreparat(), numePreparat.c_str()) == 0)
			{
				for (int i = p; i < this->nrPreparate; i++)
					this->listaPreparate[i] = this->listaPreparate[i + 1];

				this->nrPreparate--;

			}
		}

	}

	void saveToFile(fstream& file)
	{
		file.write((char*)&this->nrPreparate, sizeof(int));
		int lg = this->categorie.length() + 1;
		file.write((char*)&lg, sizeof(int));
		file.write(this->categorie.data(), lg);
		for (int i = 0; i < this->nrPreparate; i++)
		{
			this->listaPreparate[i]->saveToFile(file);
		}
	}

	void restoreFromFile(fstream& file)
	{
		if (listaPreparate != nullptr)
			for (int i = 0; i < this->nrPreparate; i++)
				delete listaPreparate[i];
		delete[]listaPreparate;

		file.read((char*)&this->nrPreparate, sizeof(int));
		int lg;
		file.read((char*)&lg, sizeof(int));
		char* buffer = new char[lg];
		file.read(buffer, lg);
		this->categorie = buffer;
		delete[]buffer;
		listaPreparate = new Preparat * [this->nrPreparate];
		for (int i = 0; i < this->nrPreparate; i++)
		{
			listaPreparate[i] = new Preparat();
			listaPreparate[i]->restoreFromFile(file);
		}
	}

	const Preparat* cautaPreparatDupaNume(const string& nume)
	{
		for (int i = 0; i < this->nrPreparate; i++)
		{
			if (listaPreparate[i] != nullptr && strcmp(this->listaPreparate[i]->getNumePreparat(), nume.c_str()) == 0)
				return listaPreparate[i];
		}
		return nullptr;
	}


	Meniu& operator=(const Meniu& m)
	{
		if (this != &m)
		{
			for (int i = 0; i < this->nrPreparate; i++)
				delete this->listaPreparate[i];
			if (this->listaPreparate != nullptr)
				delete[]this->listaPreparate;
			this->listaPreparate = nullptr;

			if (m.listaPreparate != nullptr && m.nrPreparate > 0)
			{
				this->nrPreparate = m.nrPreparate;
				this->listaPreparate = new Preparat * [this->nrPreparate];
				for (int i = 0; i < this->nrPreparate; i++)
					this->listaPreparate[i] = new Preparat(*m.listaPreparate[i]);
			}

			this->categorie = m.categorie;

		}
		return *this;

	}

	const Preparat* operator[](int index)
	{
		if (index >= 0 && index < this->nrPreparate)
		{
			return this->listaPreparate[index];
		}
		else
		{
			throw ExceptieInput("Indexul este in afara limitelor!");
		}
	}

	friend ostream& operator<<(ostream& out, const Meniu& m)
	{
		out << "\n*******************MENIU*******************";
		out << "\n~~~~~~~~~~~~~" << m.categorie << "~~~~~~~~~~~~~";
		out << "\n          LISTA PREPARATE        " << endl;
		for (int i = 0; i < m.nrPreparate; i++)
		{
			out << i + 1 << ": " << m.listaPreparate[i]->getNumePreparat() << "       Pret: " << m.listaPreparate[i]->getPretPreparat() << endl;
		}
		return out;
	}

	friend istream& operator>>(istream& in, Meniu& m)
	{
		for (int i = 0; i < m.nrPreparate; i++)
			delete m.listaPreparate[i];
		if (m.listaPreparate != nullptr)
			delete[]m.listaPreparate;
		m.listaPreparate = nullptr;
		cout << "\nIntroduceti categoria de preparate: ";
		getline(in, m.categorie);

		cout << "\nIntroduceti nr de preparate: ";
		in >> m.nrPreparate;
		in.ignore();
		cout << "\nIntroduceti preparatele dorite: ";
		if (m.nrPreparate > 0)
		{
			m.listaPreparate = new Preparat * [m.nrPreparate];
			for (int i = 0; i < m.nrPreparate; i++)
			{
				m.listaPreparate[i] = new Preparat();
				in >> *m.listaPreparate[i];
			}

		}
		else throw ExceptieInput("Numarul de preparate trebuie sa fie strict mai mare decat 0");
		return in;
	}

};

class Comanda
{
	const int id = 0;
	static int nrComanda;
	Preparat** listaPreparate = nullptr;
	int nrPreparate = 0;
	vector<pair<Preparat*, int>> detaliiComanda;

public:
	Comanda() :id(++nrComanda)
	{}

	Comanda(Preparat** listaPreparate, int nrPreparate, vector<pair<Preparat*, int>> detaliiComanda) :id(++nrComanda)
	{
		if (listaPreparate != nullptr && nrPreparate > 0)
		{
			this->nrPreparate = nrPreparate;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++)
				this->listaPreparate[i] = new Preparat(*listaPreparate[i]);
		}
		if (!detaliiComanda.empty())
			this->detaliiComanda = detaliiComanda;
	}

	Comanda(const Comanda& c) :id(++nrComanda)
	{
		if (c.listaPreparate != nullptr && c.nrPreparate > 0)
		{
			this->nrPreparate = c.nrPreparate;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++)
				this->listaPreparate[i] = new Preparat(*c.listaPreparate[i]);
		}
		if (!c.detaliiComanda.empty())
			this->detaliiComanda = c.detaliiComanda;

	}

	~Comanda()
	{
		for (int i = 0; i < this->nrPreparate; i++)
			delete this->listaPreparate[i];
		if (this->listaPreparate != nullptr)
			delete[]this->listaPreparate;
		this->listaPreparate = nullptr;

	}


	void setListaPreparate(Preparat** listaPreparate, int nrPreparate)
	{
		if (listaPreparate != nullptr && nrPreparate > 0)
		{
			if (this->listaPreparate != nullptr)
				for (int i = 0; i < this->nrPreparate; i++)delete this->listaPreparate[i];
			delete[]this->listaPreparate;
			this->listaPreparate = nullptr;
			this->nrPreparate = nrPreparate;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++)
				this->listaPreparate[i] = new Preparat(*listaPreparate[i]);
		}
	}


	void adaugaPreparatComanda(Preparat* preparatNou, int cantitate)
	{
		if (preparatNou != nullptr && cantitate > 0)
		{
			Comanda copie = *this;
			for (int i = 0; i < this->nrPreparate; i++)delete this->listaPreparate[i];
			delete[] this->listaPreparate;
			this->nrPreparate++;
			this->listaPreparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate - 1; i++)
				this->listaPreparate[i] = new Preparat(*copie.listaPreparate[i]);
			this->listaPreparate[this->nrPreparate - 1] = new Preparat(*preparatNou);

			this->detaliiComanda.push_back(make_pair(preparatNou, cantitate));
		}

	}

	void stergePreparatComanda(const string& numePreparat)
	{

		for (int p = this->nrPreparate - 1; p >= 0; p--)
		{
			if (strcmp(this->listaPreparate[p]->getNumePreparat(), numePreparat.c_str()) == 0)
			{
				for (int i = p; i < this->nrPreparate; i++)
					this->listaPreparate[i] = this->listaPreparate[i + 1];

				this->nrPreparate--;

			}
		}

	}

	void reseteazaComanda()
	{
		for (int i = 0; i < this->nrPreparate; i++)
		{
			delete listaPreparate[i];
		}
		delete[]listaPreparate;
		listaPreparate = nullptr;
		this->nrPreparate = 0;
		detaliiComanda.clear();
	}

	bool suntIngredienteDisponibile(const GestiuneStoc& gestiune, Preparat* preparat, int cantitate)
	{
		if (preparat == nullptr)return false;
		for (const auto& ingredientReteta : preparat->getReteta())
		{
			Ingredient* ingredientStoc = gestiune.cautaIngredient(ingredientReteta.first->getNumeIngredient());
			if (ingredientStoc == nullptr || ingredientStoc->getCantitate() < ingredientReteta.second * cantitate)
				return false;
		}
		return true;
	}

	vector<pair<Preparat*, int>> getDetaliiComanda()
	{
		return this->detaliiComanda;
	}

	void setDetaliiComanda(vector<pair<Preparat*, int>> detaliiComanda)
	{
		if (!detaliiComanda.empty())
			this->detaliiComanda = detaliiComanda;
		else throw ExceptieInput("Comanda este goala!");
	}

	Comanda& operator=(const Comanda& c)
	{
		if (this != &c)
		{
			for (int i = 0; i < this->nrPreparate; i++)
				delete this->listaPreparate[i];
			if (this->listaPreparate != nullptr)
				delete[]this->listaPreparate;
			this->listaPreparate = nullptr;

			if (c.listaPreparate != nullptr && c.nrPreparate > 0)
			{
				this->nrPreparate = c.nrPreparate;
				this->listaPreparate = new Preparat * [this->nrPreparate];
				for (int i = 0; i < this->nrPreparate; i++)
					this->listaPreparate[i] = new Preparat(*c.listaPreparate[i]);
			}
			if (!c.detaliiComanda.empty())
				this->detaliiComanda = c.detaliiComanda;
		}
		return *this;
	}

	friend ostream& operator<<(ostream& out, const Comanda& c)
	{
		out << "\n******************COMANDA #" << c.id << "***************************";
		out << "\nLista preparate comandate: ";
		for (int i = 0; i < c.nrPreparate; i++)
			out << endl << i + 1 << ". " << c.listaPreparate[i]->getNumePreparat() << "----->Nr portii: " << c.detaliiComanda[i].second;
		float total = 0;
		out << "\nPret comanda: ";
		for (int i = 0; i < c.nrPreparate; i++)
			total += c.detaliiComanda[i].first->getPretPreparat() * c.detaliiComanda[i].second;
		out << total;
		return out;
	}


	friend istream& operator>>(istream& in, Comanda& c)
	{
		for (int i = 0; i < c.nrPreparate; i++)
			delete c.listaPreparate[i];
		if (c.listaPreparate != nullptr)
			delete[]c.listaPreparate;
		c.listaPreparate = nullptr;


		cout << "\nIntroduceti nr de preparate: ";
		in >> c.nrPreparate;
		in.ignore();
		cout << "\nIntroduceti preparatele dorite: ";
		if (c.nrPreparate > 0)
		{
			c.listaPreparate = new Preparat * [c.nrPreparate];
			for (int i = 0; i < c.nrPreparate; i++)
			{
				c.listaPreparate[i] = new Preparat();
				in >> *c.listaPreparate[i];
			}


			c.detaliiComanda.clear();
			for (int i = 0; i < c.nrPreparate; i++)
			{
				int cantitate;
				cout << "Introduceti cantitatea dorita din " << c.listaPreparate[i]->getNumePreparat() << ": ";
				in >> cantitate;
				c.detaliiComanda.push_back(make_pair(c.listaPreparate[i], cantitate));
			}

		}
		else throw ExceptieInput("Comanda trebuie sa contina cel putin un preparat");
	}

	Comanda& operator++()
	{
		for (auto& preparat : this->detaliiComanda)
		{
			preparat.second++;
		}
		return *this;
	}

	Comanda operator++(int)
	{
		Comanda rez = *this;
		++(*this);
		return rez;
	}

};
int Comanda::nrComanda = 0;

class GestiuneSerializabil
{
	list<ISerializabil*> listaObiecte;

public:
	void adaugaObiect(ISerializabil* obj)
	{
		this->listaObiecte.push_back(obj);
	}
	void saveObiecte(fstream& file_stoc, fstream& file_meniu, const string& tip)
	{
		list<ISerializabil*>::iterator itList;
		for (itList = listaObiecte.begin(); itList != listaObiecte.end(); itList++)
		{
			if (tip == "stoc")
			{
				GestiuneStoc* stocPtr = dynamic_cast<GestiuneStoc*>(*itList);
				if (stocPtr != nullptr)(*itList)->saveToFile(file_stoc);
			}
			if (tip == "meniu")
			{
				Meniu* meniuPtr = dynamic_cast<Meniu*>(*itList);
				if (meniuPtr != nullptr)(*itList)->saveToFile(file_meniu);
			}
		}
	}

	void restoreObiecte(fstream& file_stoc, fstream& file_meniu, const string& tip)
	{
		list<ISerializabil*>::iterator itList;
		for (itList = listaObiecte.begin(); itList != listaObiecte.end(); itList++)
		{
			if (tip == "stoc")
			{
				GestiuneStoc* stocPtr = dynamic_cast<GestiuneStoc*>(*itList);
				if (stocPtr != nullptr) {
					stocPtr->restoreFromFile(file_stoc);

					cout << *stocPtr;
				}
			}
			if (tip == "meniu")
			{
				Meniu* meniuPtr = dynamic_cast<Meniu*>(*itList);
				if (meniuPtr != nullptr) {
					meniuPtr->restoreFromFile(file_meniu);
					cout << *meniuPtr;
				}
			}
		}
	}
};

int main(int argc, char* argv[])
{
	Ingredient oua("Oua", 200, 4, 1, 12, 2024), faina("Faina", 1000, 20, 3, 3, 2024), lapte("Lapte", 5000, 20, 3, 5, 2024), drojdie("Drojdie", 1000, 5, 12, 9, 2024), masline("Masline", 2000, 15, 7, 4, 2025);
	Ingredient carnePui("Carne de Pui", 2000, 30, 6, 2, 2025), cartofi("Cartofi", 3000, 20, 7, 9, 2025), carneVita("Carne de vita", 4000, 35, 12, 3, 2025), carnePorc("Carne de porc", 5000, 40, 14, 2, 2026), uleiMasline("Ulei de masline", 2000, 15, 5, 3, 2024), branza("Branza", 500, 10, 7, 8, 2024), rosii("Rosii", 300, 5, 2, 5, 2026), paste("Paste", 1000, 10, 4, 2, 2025), sare("Sare", 1000, 5, 6, 2, 2025);
	Ingredient listaIngrediente[] = { oua,faina,lapte,drojdie,masline,carnePui,cartofi,carneVita,carnePorc,uleiMasline,branza,rosii,paste,sare };
	Ingredient* pListaIngrediente[14];
	for (int i = 0; i < 14; i++)
	{
		pListaIngrediente[i] = new Ingredient(listaIngrediente[i]);
	}
	/*GestiuneStoc g(listaIngrediente, sizeof(listaIngrediente) / sizeof(Ingredient));*/

	vector<pair<Ingredient*, int>> ingredientePizza =
	{
		{pListaIngrediente[1],100}, //faina
		{pListaIngrediente[11],50}, //rosii
		{pListaIngrediente[9],20},  //ulei de masline
		{pListaIngrediente[13],10}  //sare
	};
	vector<pair<Ingredient*, int>> ingredienteOmleta =
	{
		{pListaIngrediente[0],2}, //oua
		{pListaIngrediente[10],30}, //branza
		{pListaIngrediente[9],20}, //ulei de masline
		{pListaIngrediente[13],10} //sare
	};
	vector<pair<Ingredient*, int>> ingredientePasteCarbonara =
	{
		{pListaIngrediente[12],100}, //paste
		{pListaIngrediente[1],150}, //faina
		{pListaIngrediente[0],5}, //oua
		{pListaIngrediente[10],30}, //branza
		{pListaIngrediente[13],10} //sare
	};

	vector<pair<Ingredient*, int>> ingredienteSalataGreceasca =
	{
		{pListaIngrediente[11],50}, //rosii
		{pListaIngrediente[4],20}, //masline
		{pListaIngrediente[10],30}, //branza
		{pListaIngrediente[13],10} //sare
	};

	vector<pair<Ingredient*, int>> ingredientePieptDePuiLaGratar =
	{
		{pListaIngrediente[5],150}, //carne pui
		{pListaIngrediente[9],30}, //ulei de masline
		{pListaIngrediente[13],10} // sare
	};

	vector<pair<Ingredient*, int>> ingredienteCeafaDePorc =
	{
		{pListaIngrediente[8],150}, //carne de porc
		{pListaIngrediente[9],30}, //ulei de masline 
		{pListaIngrediente[13],10} //sare
	};

	vector<pair<Ingredient*, int>> ingredienteCartofiPrajiti =
	{
		{pListaIngrediente[6],150}, //cartofi 
		{pListaIngrediente[9],30}, //ulei de masline
		{pListaIngrediente[10],20}, //branza
		{pListaIngrediente[13],10} //sare
	};

	vector<pair<Ingredient*, int>> ingredienteAntricotDeVita =
	{
		{pListaIngrediente[7],150}, // care de vita
		{pListaIngrediente[9],30}, //ulei de masline
		{pListaIngrediente[13],10} //sare
	};

	vector<pair<Ingredient*, int>> ingredienteMicDejunSpecial =
	{
		{pListaIngrediente[0],2}, //oua
		{pListaIngrediente[11],50}, //rosii
		{pListaIngrediente[10],30} //branza
	};
	Preparat pizza("Pizza Margherita", ingredientePizza, 45);
	Preparat omleta("Omleta", ingredienteOmleta, 20);
	Preparat pasteCarbonara("Paste Carbonara", ingredientePasteCarbonara, 35);
	Preparat salataGreceasca("Salata Greceasca", ingredienteSalataGreceasca, 20);
	Preparat pieptDePuiLaGratar("Piept de pui la gratar", ingredientePieptDePuiLaGratar, 40);
	Preparat ceafaDePorc("Ceafa de porc", ingredienteCeafaDePorc, 40);
	Preparat cartofiPrajiti("Cartofi prajiti", ingredienteCartofiPrajiti, 15);
	Preparat antricotDeVita("Antricot de vita", ingredienteAntricotDeVita, 45);
	Preparat micDejunSpecial("Mic Dejun Special", ingredienteMicDejunSpecial, 30);
	Preparat* listaPreparate[] = { new Preparat(pizza),new Preparat(pasteCarbonara),new Preparat(salataGreceasca),new Preparat(pieptDePuiLaGratar),new Preparat(ceafaDePorc),new Preparat(cartofiPrajiti),new Preparat(antricotDeVita),new Preparat(omleta),new Preparat(micDejunSpecial) };
	Preparat* listaPreparateDimineata[] = { new Preparat(omleta),new Preparat(micDejunSpecial),new Preparat(cartofiPrajiti),new Preparat(salataGreceasca) };
	Preparat* mainCourse[] = { new Preparat(pizza),new Preparat(pasteCarbonara),new Preparat(salataGreceasca),new Preparat(pieptDePuiLaGratar),new Preparat(ceafaDePorc),new Preparat(cartofiPrajiti),new Preparat(antricotDeVita) };
	Meniu dimineata(listaPreparateDimineata, 4, "Mic Dejun");
	Meniu felPrincipal(mainCourse, 7, "Main Course");
	Comanda comandaClient;
	vector<pair<Preparat*, int>> detaliiComanda;
	Preparat* listaPreparateComandate[9] = { };
	int optiune = 0;
	int cantitate = 0;
	string numePreparat;
	int nrPreparateComandate = 0;




	if (argc < 3) {
		std::cerr << "Utilizare: " << argv[0] << " fisier_stoc.txt fisier_meniu.txt" << std::endl;
		return 1;
	}
	GestiuneStoc gestiuneStoc;


	// Deschide fișierul de stoc
	ifstream fileStoc(argv[1]);
	if (!fileStoc.is_open()) {
		cerr << "Eroare la deschiderea fisierului de stoc: " << argv[1] << std::endl;
		return 1;
	}
	// Procesează fișierul de stoc
	string linieStoc;
	while (getline(fileStoc, linieStoc)) {
		stringstream ss(linieStoc);
		string nume, cantitateStr, pretStr, ziStr, lunaStr, anStr;
		getline(ss, nume, ',');
		getline(ss, cantitateStr, ',');
		getline(ss, pretStr, ',');
		getline(ss, ziStr, ',');
		getline(ss, lunaStr, ',');
		getline(ss, anStr);

		int cantitate = stoi(cantitateStr);
		float pret = stof(pretStr);
		int zi = stoi(ziStr);
		int luna = stoi(lunaStr);
		int an = stoi(anStr);

		Ingredient ingredient(nume.c_str(), cantitate, pret, zi, luna, an);
		gestiuneStoc.adaugaIngredient(ingredient);
	}

	// Procesare fișierul de meniu

	vector<Preparat*> vectorPreparate;
	string linie;
	ifstream fisier_meniu("fisier_meniu.txt");

	if (fisier_meniu.is_open()) {
		while (getline(fisier_meniu, linie)) {
			stringstream ss(linie);
			string numePreparat, pret, ingrediente;
			getline(ss, numePreparat, ',');
			getline(ss, pret, ',');
			vector<pair<Ingredient*, int>> listaIngrediente;

			while (getline(ss, ingrediente, ',')) {
				stringstream ssIng(ingrediente);
				string numeIngred, cantitate;
				getline(ssIng, numeIngred, '|');
				getline(ssIng, cantitate, '|');
				Ingredient* ingredient = GestiuneStoc::cautaIngredient(numeIngred);
				if (ingredient != nullptr) {
					listaIngrediente.push_back(make_pair(ingredient, stoi(cantitate)));
				}
			}

			float pretPreparat = stof(pret);
			Preparat* preparatNou = new Preparat(numePreparat.c_str(), listaIngrediente, pretPreparat);
			vectorPreparate.push_back(preparatNou);
		}
		fisier_meniu.close();
	}
	else {
		cout << "Nu s-a putut deschide fisierul 'fisier_meniu.txt'" << endl;
		return 1;
	}

	Preparat** preparate = new Preparat * [vectorPreparate.size()];
	for (size_t i = 0; i < vectorPreparate.size(); ++i) {
		preparate[i] = new Preparat(*vectorPreparate[i]);
	}

	// Populeaza meniul cu preparate
	Meniu meniu(preparate, vectorPreparate.size(), "Meniu Principal");


	GestiuneStoc* pStoc = &gestiuneStoc;
	Meniu* pMeniu = &meniu;
	ISerializabil* iS = &meniu;
	GestiuneSerializabil gestiune;
	gestiune.adaugaObiect(pStoc);
	gestiune.adaugaObiect(pMeniu);
	fstream fisierStoc("Stoc.bin", ios::out | ios::binary);
	fstream fisierMeniu("Meniu.bin", ios::out | ios::binary);

	while (true)
	{
		cout << "\n----------GESTIUNE RESTAURANT--------------";
		cout << endl << endl << "1.AFISEAZA MENIU\n";
		cout << "2.Adauga preparat in comanda\n";
		cout << "3.Sterge preparat din comanda\n";
		cout << "4.Verifica comanda\n";
		cout << "5.Finalizeaza comanda\n";
		cout << "6.Comanda noua\n";
		cout << "7.Salvare stoc in fisier\n";
		cout << "8.Salvare meniu in fisier\n";
		cout << "9.Iesire\n";
		cout << "\nAlegeti o optiune:";
		cin >> optiune;
		cin.ignore();

		switch (optiune)
		{
		case 1:
		{

			cout << meniu;
			break;
		}
		case 2:
		{
			cout << "\nAti ales optiunea:Adaugare preparat in comanda\n";
			cout << "\nIntroduceti numele preparatului: ";
			getline(cin, numePreparat);
			cout << "\nIntroduceti cantitatea: ";
			cin >> cantitate;
			cin.ignore();
			const Preparat* copie = meniu.cautaPreparatDupaNume(numePreparat);
			if (copie != nullptr)
			{


				Preparat* p = new Preparat(*meniu.cautaPreparatDupaNume(numePreparat));
				if (p != nullptr && comandaClient.suntIngredienteDisponibile(gestiuneStoc, p, cantitate))
				{
					detaliiComanda.push_back(make_pair(p, cantitate));
					for (const auto& ingredient : p->getReteta())
					{
						gestiuneStoc.actualizeazaCantitate(ingredient.first->getNumeIngredient(), -ingredient.second * cantitate);

					}
					cout << "\nPreparatul " << p->getNumePreparat() << " a fost adaugat cu succes.";


				}
				else
				{
					cout << "\nPreparatul nu dispune de toate ingredientele necesare sau nu exista.\n";
				}

			}
			else
			{
				cout << "\nPreparatul nu dispune de toate ingredientele necesare sau nu exista.\n";
			}
			break;
		}
		case 3:
		{
			cout << "\nAti ales optiunea:Stergere preparat din comanda\n";
			cout << "Introduceti numele preparatului: ";
			getline(cin, numePreparat);
			bool gasit = false;
			for (auto i = detaliiComanda.begin(); i != detaliiComanda.end();)
			{
				if (i->first->getNumePreparat() == numePreparat)
				{
					for (const auto& ingredient : i->first->getReteta())
					{
						gestiuneStoc.actualizeazaCantitate(ingredient.first->getNumeIngredient(), i->second * ingredient.second);
					}


					i = detaliiComanda.erase(i);

					gasit = true;

				}
				else {
					++i;
				}
			}
			if (gasit)
				cout << "\nPreparatul a fost sters din comanda.";
			else cout << "\nPreparatul nu a fost gasit in comanda!";

			break;
		}
		case 4:
		{
			try
			{


				comandaClient.setDetaliiComanda(detaliiComanda);
				nrPreparateComandate = detaliiComanda.size();
				for (int i = 0; i < detaliiComanda.size(); i++)
				{
					listaPreparateComandate[i] = new Preparat(*detaliiComanda[i].first);

				}
				comandaClient.setListaPreparate(listaPreparateComandate, detaliiComanda.size());
				cout << comandaClient;
			}
			catch (ExceptieInput& e)
			{
				cout << e.what() << endl;
			}
			break;
		}

		case 5:
		{
			if (nrPreparateComandate == 0)
			{
				cout << "Va rugam mai intai verificati comanda!";
				break;
			}
			ofstream g("comenzi.txt", ios::app);
			g << comandaClient;
			g.close();
			cout << "Comanda a fost finalizata si lansata in sistem." << endl;
			break;
		}

		case 6:
		{
			comandaClient.reseteazaComanda();
			Comanda c;
			comandaClient = c;
			for (int i = 0; i < nrPreparateComandate; i++)delete listaPreparateComandate[i];
			nrPreparateComandate = 0;
			detaliiComanda.clear();
			cout << "Puteti incepe o noua comanda. " << endl;
			break;

		}
		case 7:
		{
			if (!fisierStoc.is_open() && !fisierMeniu.is_open())
			{
				fisierStoc.open("Stoc.bin", ios::app | ios::binary);
				fisierMeniu.open("Meniu.bin", ios::app | ios::binary);
			}

			gestiune.saveObiecte(fisierStoc, fisierMeniu, "stoc");
			fisierStoc.close();
			fisierMeniu.close();
			fisierStoc.open("Stoc.bin", ios::in | ios::binary);
			fisierMeniu.open("Meniu.bin", ios::in | ios::binary);
			gestiune.restoreObiecte(fisierStoc, fisierMeniu, "stoc");
			fisierStoc.close();
			fisierMeniu.close();
			cout << "\n!!!!!!STOCUL DE INGREDIENTE A FOST SALVAT CU SUCCES!!!!!!";

			break;

		}

		case 8:
		{
			if (!fisierStoc.is_open() && !fisierMeniu.is_open())
			{
				fisierStoc.open("Stoc.bin", ios::app | ios::binary);
				fisierMeniu.open("Meniu.bin", ios::app | ios::binary);
			}

			gestiune.saveObiecte(fisierStoc, fisierMeniu, "meniu");
			fisierStoc.close();
			fisierMeniu.close();
			fisierStoc.open("Stoc.bin", ios::in | ios::binary);
			fisierMeniu.open("Meniu.bin", ios::in | ios::binary);
			gestiune.restoreObiecte(fisierStoc, fisierMeniu, "meniu");
			fisierStoc.close();
			fisierMeniu.close();
			cout << "\n!!!!!!MENIUL A FOST SALVAT CU SUCCES!!!!!!";

			break;
		}
		case 9:
		{

			cout << "Iesire din program..." << endl;

			return 0;
		}

		default:
			cout << "Optiune invalida.Va rugam introduceti o optiune valida.\n";
			break;

		}



	}

	return 0;
}




