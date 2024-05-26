#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "antlr4-runtime.h"
#include "TLexer.h"
#include "TParser.h"
#include "tree/IterativeParseTreeWalker.h"
#include "CalculatorInterpreter.h"

class ExceptionErrorListener : public antlr4::BaseErrorListener {
public:
    virtual void syntaxError(
        antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol,
        size_t line, size_t charPositionInLine, const std::string& msg,
        std::exception_ptr e) override {

        std::ostringstream oss;
        oss << "line: " << line << ":" << charPositionInLine << " " << msg;
        error_msg = oss.str();
        throw antlr4::ParseCancellationException(error_msg);
    }
private:
    std::string error_msg;
};

using namespace antlrcpptest;
using namespace antlr4;

void execute_expression(const std::string& expr) {
    antlr4::ANTLRInputStream input(expr);

    TLexer lexer(&input);
    lexer.removeErrorListeners();
    auto lexerErrorListener = std::make_unique<ExceptionErrorListener>();
    lexer.addErrorListener(lexerErrorListener.get());

    antlr4::CommonTokenStream tokens(&lexer);

    TParser parser(&tokens);
    parser.removeErrorListeners();
    auto parserErrorListener = std::make_unique<ExceptionErrorListener>();
    parser.addErrorListener(parserErrorListener.get());

    antlr4::tree::ParseTree* tree = nullptr;

    try {
        tree = parser.input();
    }
    catch (antlr4::ParseCancellationException& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    CalculatorInterpreter interpreter;
    antlr4::tree::IterativeParseTreeWalker walker;
    walker.walk(&interpreter, tree);
}

int main(int argc, char* argv[]) {
    std::ifstream stream;

    std::cout << "run " << argv[0] << std::endl;
    if (argc == 1) {
        std::cout << "C:/Users/user/Desktop/antlr4v2/ANTLR4_Polynomial/input.txt" << std::endl;
        stream.open("C:/Users/user/Desktop/antlr4v2/ANTLR4_Polynomial/input.txt");
    }
    else if (argc == 2) {
        std::cout << argv[1] << std::endl;
        stream.open(argv[1]);
    }
    else {
        throw std::invalid_argument("Invalid number of arguments");
    }

    if (!stream.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return EXIT_FAILURE;
    }

    std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    std::cout << str << std::endl;

    execute_expression(str);

    return 0;
}
