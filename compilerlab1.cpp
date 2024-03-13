#include<bits/stdc++.h>
using namespace std;
vector<string> str_cmd;
vector<string> words;
unordered_map<string,int>registerMap;//寄存器映射
unordered_map<string,int>para_pos;//变量对应的内存中的位置
int reg_cnt = 0;

bool is_number(string s)
{
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
bool is_sign(string s)
{
    if(s == "+") return true;
    if(s == "-") return true;
    if(s == "*") return true;
    if(s == "/") return true;
    return false;
}
bool is_operator(string s)
{
    if(is_sign(s))return true;
    if(s == "("||s == ")")return true;
    return false;
}
bool is_para(string s)
{
    if(is_number(s)) return false;
    if(is_operator(s)) return false;
    return true;
}
void deal(string s)
{
    cout<<"pop ebx"<<endl;
    cout<<"pop eax"<<endl;
    switch (s[0]) {
        case '+':
            cout<<"add eax, ebx"<<endl;
            break;
        case '-':
            cout<<"sub eax, ebx"<<endl;
            break;
        case '*':
            cout<<"mov ecx, 0"<<endl;
            cout<<"imul eax,ebx"<<endl;
            break;
        case '/':
            cout<<"cdq"<<endl;
            cout<<"idiv ebx"<<endl;
            break;
    }
    cout<<"push eax"<<endl;
}
void Process_phrase(const string& phrase)
{
    words.clear();
    istringstream iss(phrase);
    string token;
    while (getline(iss, token, ' ')) {
        if(!token.empty()){
            //cout << "Token: " << token << endl;
            words.push_back(token);
        }
    }
    if(words[0] == "int")
    {
        para_pos[words[1]] = (++reg_cnt) * 4;

        cout<<"mov DWORD PTR [ebp-"<<para_pos[words[1]]<<"], 0     # int "<<words[1]<<endl;
    }
    else if(words[0] == "return")
    {
        cout<<"mov eax, DWORD PTR [ebp-"<<para_pos[words[1]]<<"]      # return "<<words[1]<<endl;
    }
    /*else if(1)
    {
        if(words.size() == 3 && words[1] == "=")//直接赋值
        {
            if(is_number(words[2])) // 赋值数字
            {
                cout<<"mov DWORD PTR [ebp-"<<para_pos[words[0]]<<"], "<<words[2]<<"     # "<<words[0]<<" = "<<words[2]<<endl;
            }
            else //赋值变量 把变量提到寄存器中，再从寄存器中赋值到内存，期间要保护寄存器以及恢复寄存器
            {
                cout<<"push eax"<<endl;
                cout<<"mov eax, DWORD PTR [ebp-"<<para_pos[words[2]]<<"]      # "<<words[0]<<" = "<<words[2]<<endl;
                cout<<"mov DWORD PTR [ebp-"<<para_pos[words[0]]<<"], eax    "<<endl;
                cout<<"pop eax"<<endl;
            }
        }
        else if (words.size()>3 && words[1] == "=")
        {//考虑其中一种情况：如果=的后面是一个负号，这是否会影响呢，还是说题中并没有考虑这个事情

            cout<<"push ebx"<<endl;
            cout<<"push ecx"<<endl;
            cout<<"push edx"<<endl;
            cout<<"push eax"<<endl;
            cout<<"mov ecx, 0"<<endl;
            int sign = 1;// + - * / 分别是1 2 3 4
            for(int i = 2;i<words.size();i++)
            {
                if (is_para(words[i]))
                {
                    if(sign == 1)
                    {
                        cout<<"add ecx, DWORD PTR [ebp-"<<para_pos[words[i]]<<"]   # add" <<endl;
                    }
                    else if(sign == 2)
                    {
                        cout<<"sub ecx, DWORD PTR [ebp-"<<para_pos[words[i]]<<"]   # sub" <<endl;
                    }
                    else if(sign == 3) // 乘法和加法不一样，
                    {
                        //cout<<"push eax"<<endl;
                        //cout<<"mov eax, ecx"<<endl;
                        cout<<"imul ecx, DWORD PTR [ebp-"<<para_pos[words[i]]<<"]   # imul" <<endl;
                        //cout<<"mov ecx, eax"<<endl;
                        //cout<<"pop eax"<<endl;
                    }
                    else if(sign == 4)
                    {
                        cout<<"push eax"<<endl;
                        cout<<"push edx"<<endl;
                        cout<<"mov eax, ecx"<<endl;
                        cout<<"mov edx, 0"<<endl;
                        cout<<"cdq"<<endl;
                        cout<<"idiv DWORD PTR [ebp-"<<para_pos[words[i]]<<"]     # idiv"<<endl;
                        cout<<"mov ecx, eax"<<endl;
                        cout<<"pop edx"<<endl;
                        cout<<"pop eax"<<endl;
                    }
                }
                if (is_sign(words[i]))
                {
                    if(words[i]=="+")sign = 1;
                    else if(words[i]=="-")sign = 2;
                    else if(words[i]=="*")sign = 3;
                    else if(words[i]=="/")sign = 4;
                }
                if (is_number(words[i]))
                {
                    if(sign == 1)
                    {
                        cout<<"add ecx, "<<words[i]<<"   # add" <<endl;
                    }
                    else if(sign == 2)
                    {
                        cout<<"sub ecx, "<<words[i]<<"   # sub" <<endl;
                    }
                    else if(sign == 3) // 乘法和加法不一样，
                    {
                        cout<<"push ebx"<<endl;
                        cout<<"mov ebx, "<<words[i]<<" "<<endl;
                        cout<<"imul ecx, ebx   # imul" <<endl;

                        cout<<"pop ebx"<<endl;
                    }
                    else if(sign == 4)
                    {
                        cout<<"push eax"<<endl;
                        cout<<"push edx"<<endl;
                        cout<<"push ebx"<<endl;
                        cout<<"mov eax, ecx"<<endl;
                        cout<<"mov edx, 0"<<endl;
                        cout<<"mov ebx, "<<words[i]<<" "<<endl;
                        cout<<"cdq"<<endl;
                        cout<<"idiv ebx     # idiv"<<endl;
                        cout<<"mov ecx, eax"<<endl;
                        cout<<"pop ebx"<<endl;
                        cout<<"pop edx"<<endl;
                        cout<<"pop eax"<<endl;
                    }
                }
            }
            cout<<"mov DWORD PTR [ebp-"<<para_pos[words[0]]<<"], ecx     # = is done"<<words[1]<<endl;
            cout<<"pop eax"<<endl;
            cout<<"pop edx"<<endl;
            cout<<"pop ecx"<<endl;
            cout<<"pop ebx"<<endl;

        }

    }*/
    else if(words.size() == 3 && words[1] == "=")//直接赋值
    {
        if(is_number(words[2])) // 赋值数字
        {
            cout<<"mov DWORD PTR [ebp-"<<para_pos[words[0]]<<"], "<<words[2]<<"     # "<<words[0]<<" = "<<words[2]<<endl;
        }
        else //赋值变量 把变量提到寄存器中，再从寄存器中赋值到内存，期间要保护寄存器以及恢复寄存器
        {
            cout<<"push eax"<<endl;
            cout<<"mov eax, DWORD PTR [ebp-"<<para_pos[words[2]]<<"]      # "<<words[0]<<" = "<<words[2]<<endl;
            cout<<"mov DWORD PTR [ebp-"<<para_pos[words[0]]<<"], eax    "<<endl;
            cout<<"pop eax"<<endl;
        }
    }
    else
    {
        bool end_flag = false;
        stack<string> op;
        while(!op.empty())op.pop();
        op.push("(");
        int cnt = 1;
        while(!end_flag)
        {
            cnt++;
            string word;
            if(cnt != words.size())
                word = words[cnt];
            if(cnt == words.size())
            {
                op.push(")");
                end_flag = true;
            }
            else if(is_number(word))
            {
                printf("mov eax,%s\npush eax\n",word.c_str());
                continue;
            }
            else if (is_para(word))
            {
                printf("push DWORD PTR [ebp-%d]  # word = %s \n",para_pos[word],word.c_str());
                continue;
            }
            else if (is_operator(word))
            {
                op.push(word);
            }
            string op1,op2;
            while(!op.empty())
            {
                op2 = op.top();
                op.pop();
                op1 = op.top();
                if(op2 == "(")
                {
                    op.push(op2);
                    break;
                }
                if(op2 == ")")
                {
                    if(op1 == "(")
                    {
                        op.pop();
                        break;
                    }
                    deal(op1);
                    op.pop();

                }
                else if (op2 == "+" || op2 == "-")
                {
                    if (op1 == "(")
                    {
                        op.push(op2);
                        break;
                    }
                    deal(op1);//op1优先级比2高
                    op.pop();
                }
                else if (op2 == "*"||op2 == "/")
                {
                    if(op1 == "("||op1 =="+"||op1 =="-")
                    {
                        op.push(op2);
                        break;
                    }
                    deal(op1);//op1优先级比2高
                    op.pop();
                }
                op.push(op2);
            }

        }
        cout<<"pop eax"<<endl;
        printf("mov DWORD PTR [ebp-%d], eax\n", para_pos[words[0]]);
    }
}
int main(int argc,char** argv)
{
    if(argc != 2)
    {
        cerr<<"wrong argument count"<<endl;
        return -1;
    }

    ifstream read_file(argv[1]); //读取文本
    //ifstream read_file("../2.txt");
    if (!read_file) {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string content((std::istreambuf_iterator<char>(read_file)),
                        std::istreambuf_iterator<char>());
    content.erase(remove(content.begin(), content.end(), '\n'), content.end());
    content.erase(remove(content.begin(), content.end(), '\r'), content.end());

        istringstream iss(content);
        string phrase;
        while (getline(iss, phrase, ';')) {
            //cout << "phrase: " << phrase << endl;
            str_cmd.push_back(phrase);
            Process_phrase(phrase);
        }

}