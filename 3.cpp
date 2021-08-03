#include "BigInt.hpp"

#include <string>
#include <map>
#include <functional>

template<typename T>
struct NameTable {
    std::map<std::string, T> vars;
    
    NameTable() = default;
    void push(const std::string & var_name, const T & var) {
        vars[var_name] = var;
    }
    bool has(const std::string & var_name) {
        return vars.find(var_name) != vars.end();
    }
    T & get(const std::string & var_name) {
        auto i = vars.find(var_name);
        return i->second;
    }
};


#include <iostream>
#include <string>

#include <memory>

void trim(std::string & str) {
    auto beginning = str.find_first_not_of(" \t");
    auto ending = str.find_last_not_of(" \t");
    if (ending != std::string::npos) str = str.substr(0, ending + 1);
    if (beginning != std::string::npos) str = str.substr(beginning);
}

auto fact(int n) {
    MyInt a = 1;
    for (int i = 1; i <= n; i++) {
        a *= i;
    }
    return a;
}

int main() {
    NameTable<MyInt> nums;
    NameTable<std::function<MyInt(int)>> func_1_op;

    func_1_op.push("fact", fact);

    bool running = true;
    while (running) {
        std::cout << ">>> " << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) running = false;
        trim(line);
        
        if (line.empty()) continue;
        if (line == "exit") break;

        auto assignment_pos = line.find("=");
        if (assignment_pos != std::string::npos) {
            auto var_name = line.substr(0, assignment_pos);
            auto rest = line.substr(assignment_pos + 1);
            trim(var_name);
            trim(rest);
            auto lb = rest.find('(');
            auto rb = rest.find(')');
            if (lb != std::string::npos && rb != std::string::npos) {
                if (rb > lb) {
                    auto f_name = rest.substr(0, lb);
                    auto arg = rest.substr(lb + 1, rb - lb - 1);
                    if (func_1_op.has(f_name)) {
                        auto result = func_1_op.get(f_name)(std::stoi(arg));
                        nums.push(var_name, result);
                    }
                    else {
                        std::cout << "No function named '" << f_name << "'. " << std::endl;
                    }
                }
            } else {
                nums.push(var_name, MyInt(rest));
            }
        }
        else {
            auto var_name = line.substr(0, line.find(" "));
            if (nums.has(var_name)) {
                std::cout << var_name << " = " << nums.get(var_name) << std::endl;
            } else {
                std::cout << var_name << " is not defiend. " << std::endl;
            }
        }
    }
}

