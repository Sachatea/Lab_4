#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int class_symb (char w)
{
    switch (w)
    {
        case '0': return 0;
        case 'x': return 2;
        case 'X': return 2;
        case '_': return 4;
        case '+': return 5;
        case '-': return 5;
        case '*': return 5;
        case '/': return 5;
        case '>': return 5;
        case '<': return 5;
        case '=': return 5;
        case '!': return 5;
        case '%': return 5;
        case '&': return 5;
        case '|': return 5;
        case '^': return 5;
        case '~': return 5;
        case '#': return 6;
        case '\'': return 7;
        case '"': return 8;
        case '.': return 9;
        case ' ': return 10;
        case '(': return 10;
        case ')': return 10;
        case '[': return 10;
        case ']': return 10;
        case '{': return 10;
        case '}': return 10;
        case ',': return 10;
        case ':': return 10;
        case '\n': return 11;
        default: 
            if (isdigit(w)) return 1;
            if (isalpha(w))
            {
                if ((toupper(w) >= 'A') && (toupper(w) <= 'F')) return 3;
                return 4;
            }
            return 12;
    }
}

bool is_operator(const string slovo)          // оператори
{
    vector<string> python_operators = {
        "+", "-", "*", "/", "%", "**", "//",
        "==", "!=", "<", ">", "<=", ">=",
        "=", "+=", "-=", "*=", "/=", "%=",
        "==", "!=", "<", ">", "<=", ">=",
        "&", "|", "^", "~", "<<", ">>"
    };
    for (string a : python_operators)
    {
        if (slovo == a)
        {
            return true;
        }
    }
    return false;
}

bool is_reth(const string slovo)               //зарезервовані слова
{
    vector<string> reserved_words = {
        "False", "None", "True", "as", "assert", "async", "await", "break",
        "class", "continue", "def", "del", "elif", "else", "except", "finally",
        "for", "from", "global", "if", "import", "lambda", "nonlocal",
        "pass", "raise", "return", "try", "while", "with", "yield", "and", "or", "not","in", "is"
    };
    for (string a : reserved_words)
    {
        if (slovo == a)
        {
            return true;
        }
    }
    return false;
}

void ind(string slovo)
{
    slovo += " ";
    //cout << "< " << slovo << "| OP>,";

    string word;
    for (char c : slovo)
    {
        if (c == '.' or c == ' ')
        {
            cout << "< " << word << "| IN>,";
            if (c == '.')
            {
                cout << "<" << c << "|PM>,";
            }
            word = "";
        }
        else
        {
            word += c;
        }
    }
}

void avt (string file_name) {
    ifstream file;

    file.open(file_name);
    if (!file.is_open()) 
    {
        cout << "error, no file" << endl;
    }

    int  delta[30][13] =
    {
        //  номер класу     множина символів
        //        0            цифра 0
        //        1            цифри 1-9
        //        2            буква x або X
        //        3            букви A-F, a-f
        //        4            інші букви + _
        //        5            символи операторів  + - / * ...
        //        6            #
        //        7            '
        //        8            "
        //        9            крапка .
        //        10           розділові знаки ( ) [ ] ...
        //        11           \n
        //        12           невідомі символи

        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
        {21,24,20,20,20, 3, 2, 4,10, 1, 0, 0, 1},  // стан 0 початки
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-1,-1, 1},  // стан 1 помилки, -1 кінцевий

        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,-2, 2},  // стан 2 короткі коментарі, -2 кінцевий
        { 1, 1, 1, 1, 1, 3, 1, 1, 1, 1,-3,-3, 1},  // стан 3 оператори, -3 кінцевий

        {16,16,16,16,16,16,16, 5,16,16,16, 1,16},  // стан 4 перший символ ',   лексема, що починається з '
        { 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1},  // стан 5 другій символ '
        { 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6},  // стан 6 третій символ ' ++ коментар
        { 6, 6, 6, 6, 6, 6, 6, 8, 6, 6, 6, 6, 6},  // стан 7 перший символ '
        { 6, 6, 6, 6, 6, 6, 6, 9, 6, 6, 6, 6, 6},  // стан 8 другій символ '
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-9,-9, 1},  // стан 9 третій символ, якщо не зліплено -9 великий коментар '''*'''
    
        {18,18,18,18,18,18,18,18,11,18,18, 1,18},  // стан 10 перший символ ",   лексема, що починається з "
        { 1, 1, 1, 1, 1, 1, 1, 1,12, 1, 1, 1, 1},  // стан 11 другій символ "
        {12,12,12,12,12,12,12,12,13,12,12,12,12},  // стан 12 третій символ " ++ коментар
        {12,12,12,12,12,12,12,12,14,12,12,12,12},  // стан 13 перший символ "
        {12,12,12,12,12,12,12,12,15,12,12,12,12},  // стан 14 другій символ "
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-15,-15,1}, // стан 15 третій символ, якщо не зліплено -9 великий коментар """*"""
    
        { 1, 1, 1, 1, 1, 1, 1,17, 1, 1, 1, 1, 1},  // стан 16 перший символ '+ шось -> символьна 
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-17,-17,1}, // стан 17 символьна константа -17
        
        {18,18,18,18,18,18,18,18,19,18,18, 1,18},  // стан 18 перший символ "+ шось
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,-19,-19,1}, // стан 19 константа -19

        { 1, 1,20,20,20, 1, 1, 1, 1,20,-20,-20,1}, // стан 20 перший символ буква, -20 слово
        
        { 1, 1,22, 1, 1, 1, 1, 1, 1,23, 1, 1, 1},  // стан 21 цифра 0
        {22,22, 1,22, 1, 1, 1, 1, 1, 1,-22,-22,1}, // стан 22 цифра в 16 сис, -22 кінцемий

        {23,23, 1, 1, 1, 1, 1, 1, 1, 1,-23,-23,1}, // стан 23 цифра 0.211651651

        {24,24, 1, 1, 1, 1, 1, 1, 1,25,-24,-24,1}, // стан 24 цифра не 0 на початку +0123456789
        {24,24, 1, 1, 1, 1, 1, 1, 1, 1,-25,-25,1}  // стан 25 цифра число 565.65656

    };

    char c;
    int stan = 0;
    string word;
    while (file.get(c))
    {
        stan = delta[stan][class_symb(c)];
    
        /////////////////////////////Фінішні стани/////////////////////////////

        if (stan == -1)  // помилка
        {
            stan = 0;
            cout << "< " << word << "| ?>,";
            word = "";
        }
        if (stan == -2)  // маленький коментар
        {
            stan = 0;
            cout << "<" << word << "| C>,";
            word = "";
        }
        if (stan == -3)  // оператор
        {
            stan = 0;
            if (is_operator(word) == true) // перевіряє чи такий оператор існує
            {
                cout << "< " << word << "| OP>,";
                word = "";
            }
            else
            {
                cout << "< " << word << "| ?>,";
                word = "";
            }
        }
        if (stan == -9 or stan == -15)  // коментар '''*''' чи """*"""
        {
            stan = 0;
            cout << "<" << word << "| C>,";
            word = "";
        }
        if (stan == -17 or stan == -19)  // константа
        {
            stan = 0;
            cout << "<" << word << "| CONST>,";
            word = "";
        }
        if (stan == -20)  // слово
        {
            stan = 0;
            if (is_reth(word))
            {
                cout << "<" << word << "| RETH>,";
            }
            else
            {
                ind(word);
            }
            word = "";
        }
        if (stan == -22 or stan == -23 or stan == -24 or stan == -25)  // число
        {
            stan = 0;
            cout << "<" << word << "| N>,";
            word = "";
        }

        /////////// Розділові знаки
        if (class_symb(c) == 10 or class_symb(c) == 11)
        {
            if (stan == 4 or stan == 5 or stan == 6 or stan == 7 or stan == 8 or stan == 10 or stan == 11 or stan == 12 or stan == 13 or stan == 14 or stan == 16 or stan == 18)
            {
                word += c;
            }
            else if (stan == 2)
            {
                if (c != '\n')
                {
                    word += c;
                }
                else
                {
                    cout << c;
                }
            }
            else 
            {
                if (class_symb(c) == 10 and c != ' ')
                {
                    cout << "< " << c << " | PM>,";
                }
                else
                {
                    cout << c;
                }
            }
        }
        else
        {
            word += c;
        }

    }
}


int main()
{
    avt("Test.txt");
    return 0;
}
