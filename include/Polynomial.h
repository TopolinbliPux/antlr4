#include <iostream>
#include <string>s
using namespace std;

class Polinom {
private:
    struct monom
    {
        monom* next;
        int step;
        int koef;
        monom(int _koef, int _step) : step(_step), koef(_koef), next(nullptr) {};
    };
    monom* head;
public:
    Polinom() : head(nullptr) {};
    ~Polinom()
    {
        head = nullptr;
    };
    void addMonom(int koeff, int step)
    {
        monom* new_monom = new monom(koeff, step);
        if ((head == nullptr) || (step > head->step))
        {
            new_monom->next = head;
            head = new_monom;
        }
        else
        {
            monom* place = head;
            monom* pr = nullptr;
            while (place != nullptr && place->step > step)
            {
                pr = place;
                place = place->next;
            }
            if (place != nullptr && place->step == step)
            {
                place->koef += koeff;
                if (place->koef == 0)
                {
                    if (pr == nullptr)
                    {
                        head = place->next;
                    }
                    else
                    {
                        pr->next = place->next;
                    }
                }
            }
            else
            {
                new_monom->next = place;
                if (pr == nullptr)
                {
                    head = new_monom;
                }
                else
                {
                    pr->next = new_monom;
                }
            }
        }
    };
    Polinom(string str)
    {
        head = nullptr;
        if (str == "0")
        {
            return;
        }
        int koef = 0;                //коэффициент перед мономом (как части полинома)
        int monomSign = 1;              //знак перед мономом (как части полинома)
        string koefStr = "";            //коэффициент монома (как части полинома) в виде строки
        string stepStr = "000";         //степень у x,y,z в мономе (как части полинома) в виде строки
        //size_t lengthStr = str.length();

        for (int i = 0; i <= str.length(); i++)
        {
            if (i == str.length() || str[i] == '+' || str[i] == '-')
            {
                if (!koefStr.empty())
                {
                    koef = stoi(koefStr) * monomSign;
                    addMonom(koef, stoi(stepStr));
                    koefStr = "";
                    stepStr = "000";
                }
                if (i < str.length() && str[i] == '-')
                {
                    monomSign = -1;
                }
                else
                {
                    monomSign = 1;
                }
                //на этом этапе знаем знак перед мономом 
            }
            else if (isdigit(str[i]))
            {
                koefStr += str[i];
            }
            else if (str[i] == 'x' || str[i] == 'y' || str[i] == 'z')
            {
                if (koefStr.empty() && (i == 0 || str[i - 1] != '^'))
                {
                    koefStr = "1";
                }
                size_t pos;
                if (str[i] == 'x')
                {
                    pos = 0;
                }
                else if (str[i] == 'y')
                {
                    pos = 1;
                }
                else {
                    pos = 2;
                }
                i++;
                if ((i < str.length()) && (str[i] == '^'))
                {
                    i++;
                    string exp = "";
                    while (i < str.length() && isdigit(str[i]))
                    {
                        exp += str[i++];
                    }
                    i--;
                    if (exp.empty())
                    {
                        stepStr[pos] = '1';
                    }
                    else
                    {
                        stepStr[pos] = exp[0];
                    }
                }
                else
                {
                    stepStr[pos] = '1';
                    i--;
                }
            }
        }
    };
    class iterator {
    private:
        monom* now;
    public:
        iterator(monom* now = nullptr) :now(now) {};
        monom& operator *()
        {
            return *now;
        };
        iterator& operator++()
        {
            if (now) { now = now->next; }
            return *this;
        };
        bool operator!=(const iterator& other)
        {
            return now != other.now;
        };
    };
    iterator begin()
    {
        return iterator(head);
    };
    iterator end()
    {
        return iterator(nullptr);
    };
    void print()
    {
        if (head == nullptr)
        {
            cout << "0";
            return;
        }
        for (auto i = begin(); i != end(); ++i)
        {
            monom monom = *i;
            if ((i != begin()) && (monom.koef > 0))
            {
                cout << "+";
            }
            if (monom.koef != 1)
            {
                cout << monom.koef;
            }
            int step = monom.step;
            int stepX = step / 100;
            int stepY = (step - stepX * 100) / 10;
            int stepZ = step % 10;
            if (stepX > 0)
            {
                if (stepX == 1) cout << "x";
                else cout << "x^" << stepX;
            }
            if (stepY > 0)
            {
                if (stepY == 1) cout << "y";
                else cout << "y^" << stepY;
            }
            if (stepZ > 0)
            {
                if (stepZ == 1) cout << "z";
                else cout << "z^" << stepZ;
            }
            if (stepX == 0 && stepY == 0 && stepZ == 0) cout << 1;
        }
    };
    Polinom operator+(const Polinom& other) const
    {
        Polinom result;
        monom* monom1 = this->head;
        monom* monom2 = other.head;
        while (monom1 != nullptr || monom2 != nullptr)
        {
            if (monom1 == nullptr)
            {
                result.addMonom(monom2->koef, monom2->step);
                monom2 = monom2->next;
            }
            else if (monom2 == nullptr)
            {
                result.addMonom(monom1->step, monom1->koef);
                monom1 = monom1->next;
            }
            else if ((monom1->step) == (monom2->step))
            {
                int newKoef = monom1->koef + monom2->koef;
                if (newKoef != 0)
                {
                    result.addMonom(newKoef, monom1->step);
                }
                monom1 = monom1->next;
                monom2 = monom2->next;
            }
            else if (monom1->step > monom2->step)
            {
                result.addMonom(monom1->koef, monom1->step);
                monom1 = monom1->next;
            }
            else
            {
                result.addMonom(monom2->koef, monom2->step);
                monom2 = monom2->next;
            }
        }
        return result;
    };
    Polinom operator*(int k) const
    {
        Polinom result;
        if (k == 0) return result;
        for (monom* now = head; now != nullptr; now = now->next)
        {
            result.addMonom((now->koef) * k, now->step);
        }
        return result;
    };
    Polinom operator*(const Polinom& other) const
    {
        Polinom result;
        for (monom* now1 = head; now1 != nullptr; now1 = now1->next)
        {
            for (monom* now2 = other.head; now2 != nullptr; now2 = now2->next)
            {
                result.addMonom((now1->koef) * (now2->koef), (now1->step) + (now2->step));
            }
        }
        return result;
    };
    Polinom operator-(const Polinom& other) const {
        Polinom result;
        Polinom s = other * (-1);
        result = this->operator+(s);
        return result;
    };
    bool operator==(const Polinom& other) const {
        monom* now1 = head;
        monom* now2 = other.head;
        while (now1 != nullptr && now2 != nullptr) {
            if (((now1->step) != (now2->step)) || ((now1->koef) != (now2->koef))) {
                return false;
            }

            now1 = now1->next;
            now2 = now2->next;
        }

        if (now1 == nullptr && now2 == nullptr) {
            return true;
        }
        return false;
    };
    double Polinom::calculate(double x, double y, double z) const {
        double result = 0;
        monom* current = head;
        while (current != nullptr) {
            int termValue = current->koef;
            termValue *= pow(x, (current->step) / 100);
            termValue *= pow(y, (current->step % 100) / 10);
            termValue *= pow(z, (current->step % 10));
            result += termValue;
            current = current->next;
        }
        return result;
    }
};
