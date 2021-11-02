#include <cstdlib>
#include <cstdio>
#include <iostream>     
#include <string>
#include <fstream>
#include <dlfcn.h>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>   
#include <memory>
#include <vector>
#include <stack>
#include "Expression_JIT.h"



bool binaryOp(char c)
{
    switch (c)
    {
        case '+': return true; break;
        case '-': return true; break;
        case '*': return true; break;
        case '/': return true; break;
        case '^': return true; break;
        default: return false; break;
    }
}

void Expression::set (std::string userInput)
{
	this->userInput = userInput;
}

std::string Expression::emitCode ()
{
	std::string code =  "class B : public jit_compile {\n"
                        "    Complex FUNCTION(Complex z, int N) const \n"
                        "    {\n"
                        "        return " + userInput + ";\n"
                        "    }\n"
                        "};";
    return code;
}

jit_compile* Expression::getFuncPtr ()
{
    return this->FuncPtr;
}


// Convert a mathematical expression into C++ code (Using my libraries)
void Expression::prepare ()
{
    std::vector<int> precedence;
    std::string expr1, expr2, temp, output;
    std::stack<int> bracket;
    std::stack<int> clearBracket;
    bool negative = false;
    int i = 0;
    int counter = 0;
    int backwards;

    // Stripping blank spaces and Preparing a vector which holds the precedence for each binary operator
    for (int i = 0; i < this->userInput.length(); i++)
    {
        if (this->userInput[i] == ' ') 
        {
            this->userInput.erase(i, 1);  
        }

        if (binaryOp(this->userInput[i]))
        {
            precedence.push_back(0);
        }
    }

    // Filling the precedence vector (used to determine wether I can read real and imaginary parts into one number)
    int helper = 0;
    for (int i = 0; i < this->userInput.length(); i++)
    {
        // Those operations all have higher precedence than + or -
        if (this->userInput[i] == '*' || this->userInput[i] == '/' || this->userInput[i] == '^')
        {
            if (helper == 0)
            {
                precedence[0] = precedence[1] = 1;
            }
            if (helper == precedence.size())
            {
                precedence[this->userInput.length() - 1] = precedence[this->userInput.length()] = 1;
                break;
            }
            else
            {
                precedence[helper] = precedence[helper - 1] = precedence[helper + 1] = 1;
            }   
            helper++;
        }

        // brackets change the precedence (this is not working perfectly)
        if (this->userInput[i] == '(')
        {
            if (precedence.size() > 1)
            {
                precedence[helper] = 0;
            }
        }

        if (this->userInput[i] == ')')
        {
            if (helper > 0)
            {
                precedence[helper - 1] = 0;
            }
        }
    }

    if (precedence.empty())
    {
        precedence.push_back(0);
    }

    while (i < this->userInput.length())
    {
        // Real part first
        if (isdigit(this->userInput[i]) || this->userInput[i] == '_')
        {
            if (this->userInput[i] == '_')
            {
                expr1 += "-";
                i++;
            }

            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
            {
                expr1 += this->userInput[i];
                i++;
            }
            
            if (precedence[counter] == 0)
            {
                switch (this->userInput[i])
                {
                    case '+':
                        if (this->userInput[i + 1] == 'i')
                        {
                            i += 2;
                            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
                            {
                                expr2 += this->userInput[i];
                                i++;
                            }
                        }
                        break;

                    case '-':
                        if(this->userInput[i + 1] == 'i')
                        {
                            i += 2;
                            if (negative)
                            {
                                expr2 += "+";
                            }
                            else 
                            {
                                expr2 += "-";
                            }
                            
                            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
                            {
                                expr2 += this->userInput[i];
                                i++;
                            }
                        }
                        break;

                    default: break;
                }
                counter++;
            }

            if (expr2 == "")
            {
                expr2 += "0";
            }

            temp = "Complex(" + expr1 + "," + expr2 + ")";
            output += temp;
            expr1 = expr2 = temp = "";
        }


        // Imaginary part first
        if (this->userInput[i] == 'i' || this->userInput[i] == '_')
        {   
            if (this->userInput[i] == '_')
            {
                expr2 += "-";
                i++;
            }
            i++;

            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
            {
                expr2 += this->userInput[i];
                i++;
            }

            if(precedence[counter])
            {
                switch (this->userInput[i])
                {
                    case '+':
                        if(isdigit(this->userInput[i + 1]))
                        {
                            i++;
                            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
                            {
                                expr1 += this->userInput[i];
                                i++;
                            }
                        } 
                        break;

                    case '-':
                        if(isdigit(this->userInput[i + 1]))
                        {
                            i += 2;
                            if (negative)
                            {
                                expr1 += "+";
                            }
                            else 
                            {
                                expr1 += "-";
                            }
                            
                            while (isdigit(this->userInput[i]) || this->userInput[i] == '.')
                            {
                                expr1 += this->userInput[i];
                                i++;
                            }
                        }
                        break;

                    default: break;
                }
                counter++;
            }

            if (expr1 == "")
            {
                expr1 += "0";
            }

            temp = "Complex(" + expr1 + "," + expr2 + ")";
            output += temp;
            expr1 = expr2 = temp = "";
        }

        // Reading mathematical operators (+, -, *, /, ^)
        if (binaryOp(this->userInput[i]) && this->userInput[i] != '^')
        {   
            output += this->userInput[i];
            if (this->userInput[i] == '-')
            {
                negative = true;
            }
            counter++;
        }

        // Reading the variable
        if (this->userInput[i] == 'z')
        {
            output += this->userInput[i];
        }

        // Reading closing/opening brackets
        if (this->userInput[i] == '(' || this->userInput[i] == ')')
        {
            output += this->userInput[i];
        }

        // Reading mathematical functions (not checking, the compiler will fail and done)
        if (this->userInput[i] >= 'a' && 'y' >= this->userInput[i])
        {
            output += 'c';
            while (this->userInput[i] >= 'a' && 'y' >= this->userInput[i])
            {
                output += this->userInput[i];
                i++;
            }
            output += '(';
        }

        // Reading Eisenstein series
        if (this->userInput[i] == 'E')
        {
            while (userInput[i] != ')')
            {
                output += this->userInput[i];
                i++;
            }
            output += this->userInput[i];
        }

        // Reading the iterator ("N" is an int)
        if (this->userInput[i] == 'N')
        {
            output += "Complex(N, 0)";
        }

        if (this->userInput[i] == 'P')
        {
            if (this->userInput[i + 1] == 'I')
            {
                output += "Complex(M_PI, 0)";
            }
        }

        // Reading exponentiation (first figure out where to put "POW(####" and then where to put ",INT)" )
        if (this->userInput[i] == '^')
        {
            if (this->userInput[i - 1] == ')')
            {
                int bracketS = 1;
                backwards = output.length() - 2;

                // Count the closing/opening brackets and as soon as all brackets are closed break out
                while (bracketS != 0 && backwards >= 0)
                {
                    if (output[backwards] == ')')
                    {
                        bracketS++;
                    }

                    if (output[backwards] == '(')
                    {
                        bracketS--;
                    }

                    if (bracketS == 0)
                    {
                        break;
                    }

                    backwards--;
            
                    
                }
                // Insert "POW(" in front of the last opening bracket
                output.insert(backwards, "POW(");
                bracket = clearBracket;
            }

            i++;

            // Read the exponent
            if (isdigit(this->userInput[i]) || this->userInput[i] == 'N')
            {
                output += ',';
                while(isdigit(this->userInput[i]) || this->userInput[i] == 'N')
                {
                    output += this->userInput[i];
                    i++;
                }
                output += ')';
            }          
        }
        

        // Skipping everything else
        else 
        {
            i++;
        }   
    }

    this->userInput = output;
    this->last = userInput;
    std::cout << "Interpreted as: " << output << std::endl;
}

// This is totally a security risk as it allows to run custom C++ code. However it is also very simple and fast. 
void Expression::parse ()
{
	// temporary cc/library output files
    std::string HEADER_1  = "jit_compile.h";
    std::string HEADER_2  = "math.h";
    std::string HEADER_3  = "Function.h";
    std::string HEADER_4  = "Eisenstein.h";
    std::string HEADER_5  = "Colors.h";
    std::string HEADER_6  = "Colormaps.h";
    std::string HEADER_7  = "Interpolator.h";
    std::string HEADER_8  = "Colors.cc";
    std::string HEADER_9  = "Complex.cc";
    std::string HEADER_10 = "Function.cc";
    std::string HEADER_11 = "Eisenstein.cc";
    std::string HEADER_12 = "Interpolator.cc";

    std::filesystem::path HOME_DIR = getenv("HOME");
    std::filesystem::path INCLUDE_PATH = HOME_DIR / "HarryPlotter/JITsource/";
    std::string INCLUDE = INCLUDE_PATH.string();

    std::string SRC_FILE = OUTPATH + "/jit_function.cc";
    std::string LIB_FILE = OUTPATH + "/jit_function.so";
    std::string LOG_FILE = OUTPATH + "/JIT.log";

    std::ofstream output(SRC_FILE.c_str(), std::ofstream::out);

    // add necessary header to the code
    std::string code = "#include \"" + INCLUDE + HEADER_1 + "\"" + "\n"
    				   "#include \"" + HEADER_2 + "\"" + "\n"
    				   "#include \"" + INCLUDE + HEADER_3 + "\"" + "\n"
    				   "#include \"" + INCLUDE + HEADER_4 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_5 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_6 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_7 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_8 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_9 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_10 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_11 + "\"" + "\n"
                       "#include \"" + INCLUDE + HEADER_12 + "\"" + "\n"
                       + this->emitCode() + "\n\n"
                       "extern \"C\" {\n"
                       "jit_compile* maker()\n"
                       "{\n"
                       "    return (jit_compile*) new B(); \n"
                       "}\n"
                       "} // extern C\n";

    // output code to file
    if(output.bad()) 
    {
        std::cout << "cannot open " << SRC_FILE << std::endl;
        this->FuncPtr = nullptr;
        return;
    }

    // write to the source file and close it
    output << code;
    output.flush();
    output.close();

     // compile the code now using g++ and -Ofast
    std::string COMPILE_CMD = "g++  " + SRC_FILE + " -o " + LIB_FILE + " -I" + INCLUDE +
                      + " -Ofast -shared -msse3 -std=c++2a -fPIC &>" + LOG_FILE;
    int STATUS = system(COMPILE_CMD.c_str());
    if(WEXITSTATUS(STATUS) != EXIT_SUCCESS) {
        std::cout << "Parsing failed, see " << LOG_FILE << std::endl;
        this->FuncPtr = nullptr;
        return;
    }

     // load dynamic library
    LOAD_DYNAMIC_LIB = dlopen (LIB_FILE.c_str(), RTLD_LAZY);
    if(!LOAD_DYNAMIC_LIB) {
        std::cerr << "error loading library:\n" << dlerror() << std::endl;
        this->FuncPtr = nullptr;
        return;
    }

    // Symbolic linker
    void* SYMLINK = dlsym(LOAD_DYNAMIC_LIB, "maker");
    const char* SYMLINK_ERROR = dlerror();
    if(SYMLINK_ERROR != nullptr)  {
        std::cerr << "error loading symbol:\n" << SYMLINK_ERROR << std::endl;
        this->FuncPtr = nullptr;
        return;
    }

    // Create and return function pointer 
    jit_compile* FUNC = reinterpret_cast<jit_compile*(*)()> (SYMLINK)();
    FUNC->INIT(userInput);
    this->FuncPtr = FUNC;
}

void Expression::reset ()
{
    this->userInput = "";

    // Reset the pointer and not leaking memory in the process
    delete FuncPtr;
    this->FuncPtr = nullptr;
    
    
    // Unlink the library (hopefully this gets rid of all symbols (turns out only using macOS)) 
    dlclose(LOAD_DYNAMIC_LIB);

    // Remove the jit_function.* files 
    std::string rm_CMD = "rm " + OUTPATH + "/jit_function*";
    system(rm_CMD.c_str());
}

Complex Expression::compute (Complex z, int N)
{
	return FuncPtr->FUNCTION(z, N);
}
