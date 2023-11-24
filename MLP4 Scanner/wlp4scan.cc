#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Token
{
public:
    string kind;
    string lexeme;

    Token(string kind, string lexeme)
    {
        this->kind = kind;
        this->lexeme = lexeme;
    }
    Token()
    {
        this->kind = "_UNINITIALIZED";
        this->lexeme = "";
    }
};

class Scanner
{
public:
    string str;
    int index;

    Scanner(string str)
    {
        this->str = str;
        this->index = 0;
    }

    char peek() { return str[this->index]; }
    char consume() { return this->str[index++]; }
};

Token generateIdToken(Scanner &scanner)
{
    string token = "";
    while ((scanner.index < scanner.str.length()) && isalnum(scanner.peek()))
    {
        token += scanner.consume();
    }

    if (token == "int")
        return Token("INT", "int");
    else if (token == "return")
        return Token("RETURN", "return");
    else if (token == "wain")
        return Token("WAIN", "wain");
    else if (token == "if")
        return Token("IF", "if");
    else if (token == "else")
        return Token("ELSE", "else");
    else if (token == "while")
        return Token("WHILE", "while");
    else if (token == "println")
        return Token("PRINTLN", "println");
    else if (token == "new")
        return Token("NEW", "new");
    else if (token == "delete")
        return Token("DELETE", "delete");
    else if (token == "NULL")
        return Token("NULL", "NULL");
    return Token("ID", token);
}

Token generateNumToken(Scanner &scanner)
{
    string token = "";
    while ((scanner.index < scanner.str.length()) && isdigit(scanner.peek()))
    {
        token += scanner.consume();
    }
    // the number starts with 0
    if (token[0] == '0' && token.length() > 1)
        return Token("INVALID", "Invalid number: " + token);

    // the number is out of bound
    try
    {
        unsigned long long value = stoull(token);
        if (value > 2147483647)
            return Token("INVALID", "Invalid number: " + token);
    }
    catch (out_of_range)
    {
        return Token("INVALID", "Invalid number: " + token);
    }

    return Token("NUM", token);
}

Token generateEqToken(Scanner &scanner)
{
    string token = "=";
    scanner.consume();
    if ((scanner.index < scanner.str.length()) && scanner.peek() == '=')
    {
        token += scanner.consume();
        return Token("EQ", token);
    }
    return Token("BECOMES", token);
}

Token generateLtToken(Scanner &scanner)
{
    string token = "<";
    scanner.consume();
    if ((scanner.index < scanner.str.length()) && scanner.peek() == '=')
    {
        token += scanner.consume();
        return Token("LE", token);
    }
    return Token("LT", token);
}

Token generateGtToken(Scanner &scanner)
{
    string token = ">";
    scanner.consume();
    if ((scanner.index < scanner.str.length()) && scanner.peek() == '=')
    {
        token += scanner.consume();
        return Token("GE", token);
    }
    return Token("GT", token);
}

int main()
{
    string s;
    char c;
    while (true)
    {
        c = getchar();
        if (c == EOF)
        {
            break;
        }
        s += c;
    }

    Scanner scanner(s);
    vector<Token> tokens;

    while (scanner.index < scanner.str.length())
    {
        char c = scanner.peek();
        Token t;
        if (isalpha(c))
        {
            t = generateIdToken(scanner);
        }
        else if (isdigit(c))
        {
            t = generateNumToken(scanner);
        }
        else if (c == '(')
        {
            scanner.consume();
            t = Token("LPAREN", "(");
        }
        else if (c == ')')
        {
            scanner.consume();
            t = Token("RPAREN", ")");
        }
        else if (c == '{')
        {
            scanner.consume();
            t = Token("LBRACE", "{");
        }
        else if (c == '}')
        {
            scanner.consume();
            t = Token("RBRACE", "}");
        }
        else if (c == '[')
        {
            scanner.consume();
            t = Token("LBRACK", "[");
        }
        else if (c == ']')
        {
            scanner.consume();
            t = Token("RBRACK", "]");
        }
        else if (c == '+')
        {
            scanner.consume();
            t = Token("PLUS", "+");
        }
        else if (c == '-')
        {
            scanner.consume();
            t = Token("MINUS", "-");
        }
        else if (c == '*')
        {
            scanner.consume();
            t = Token("STAR", "*");
        }
        else if (c == '%')
        {
            scanner.consume();
            t = Token("PCT", "%");
        }
        else if (c == '&')
        {
            scanner.consume();
            t = Token("AMP", "&");
        }
        else if (c == ',')
        {
            scanner.consume();
            t = Token("COMMA", ",");
        }
        else if (c == ';')
        {
            scanner.consume();
            t = Token("SEMI", ";");
        }
        else if (c == '/')
        {
            scanner.consume();
            if ((scanner.index < scanner.str.length()) && scanner.peek() == '/')
            {
                scanner.consume();
                while (scanner.peek() != 0x0a && scanner.peek() != 0x0d)
                    scanner.consume();
            }
            else
            {
                t = Token("SLASH", "/");
            }
        }
        else if (c == '=')
        {
            t = generateEqToken(scanner);
        }
        else if (c == '<')
        {
            t = generateLtToken(scanner);
        }
        else if (c == '>')
        {
            t = generateGtToken(scanner);
        }
        else if (c == '!')
        {
            scanner.consume();
            if ((scanner.index < scanner.str.length()) && scanner.peek() == '=')
            {
                scanner.consume();
                t = Token("NE", "!=");
            }
            else
            {
                t = Token("INVALID", "unrecognized character");
            }
        }
        else if (c == 0x0a || c == 0x0d || c == 0x20 || c == 0x09)
        {
            scanner.consume();
        }
        else
        {
            t = Token("INVALID", "unrecognized character");
        }

        if (t.kind == "INVALID")
        {
            cerr << "ERROR "
                 << "\n";
            return EXIT_FAILURE;
        }
        else if (t.kind != "_UNINITIALIZED")
        {
            tokens.push_back(t);
        }
    }
    for (Token t : tokens) {
        if (t.kind != "WHITESPACE" && t.kind != "COMMENT")
            cout << t.kind << " " << t.lexeme << endl;
    }
}