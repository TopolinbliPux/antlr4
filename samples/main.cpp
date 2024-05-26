#include "Polynomial.h"

int main() {
    Polynomial pol1("x^3y^4+z^2+x^2y^2z^2-2x^4y^7");
    Polynomial pol2("0");
    Polynomial pol3("-x^3y^4+12-48x^2y^2");
    Polynomial pol4("1");
    Polynomial pol5("x^3");
    Polynomial pol6 = pol1 + pol3;
    pol6.print();
    return 0;
}