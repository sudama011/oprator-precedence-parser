#include <bits/stdc++.h>
using namespace std;
#define ll long long
unordered_map<string, int> precedence;

int find_precedence(string a, string b)
{

    if (precedence.count(a) == 0 && precedence.count(b) == 0)
        return 0;
    if (a == "$")
        return -1;
    if (b == "$")
        return 1;

    if (a == "(")
        return -1;
    if (b == ")")
        return 1;

    if ((precedence.count(a) == 0 && b == "(") || (a == ")" && precedence.count(b) == 0) || (a == ")" && b == "("))
        return 0;
    if (a == ")")
        return 1;
    if (b == "(")
        return -1;

    if (precedence.count(a) == 0)
        return 1;
    if (precedence.count(b) == 0)
        return -1;
    if (precedence[a] > precedence[b])
        return 1;
    if (precedence[a] < precedence[b])
        return -1;
    if (a == "++" || a == "--" || a == "!" || a == "~" || a == "=" || a == "+=" || a == "-=" || a == "*=" || a == "/=" || a == "%=" || a == "<<=" || a == ">>=" || a == "&=" || a == "|=" || a == "^=" || a == "(")
        return -1;
    return 1;
}
vector<vector<string>>
precedence_table(set<string> &token)
{

    int n = token.size();
    vector<vector<string>> result(n + 1, vector<string>(n + 1, "-"));

    result[0][0] = " ";
    int ind = 0;
    int j = 1;
    for (auto t : token)
    {
        result[0][j] = t;
        if ("$" == t)
            ind = j;
        j++;
    }

    swap(result[0][n], result[0][ind]);

    for (int i = 1; i <= n; i++)
    {
        result[i][0] = result[0][i];
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (i == n && j == n)
            {
                result[i][j] = "A";
                continue;
            }

            int a = find_precedence(result[i][0], result[0][j]);
            if (a == -1)
            {
                result[i][j] = "<.";
            }
            if (a == 1)
            {
                result[i][j] = ".>";
            }
        }
    }
    return result;
}

string stackToString(stack<string> st)
{
    string x = "";
    while (st.size())
    {
        x = st.top() + x;
        st.pop();
    }
    for (int i = x.size(); i < 15; i++)
    {
        x += " ";
    }
    return x;
}

string vectorToString(vector<string> v, int i, int n)
{
    string x = "";
    for (int j = i; j < v.size(); j++)
    {
        x += v[j];
    }
    while (x.size() < n)
    {
        x = " " + x;
    }
    return x;
}

int main()
{
    freopen("1.txt", "r", stdin);
    freopen("2.txt", "w", stdout);

    int n;
    cout << "Enter the number of production lines: " << endl;
    cin >> n;

    map<string, vector<string>> production, reverse_production;
    set<string> token = {"$"};

    precedence[")"] = 1;
    precedence["$"] = -1;
    precedence["("] = 0;
    precedence["="] = 2;
    precedence["+="] = 2;
    precedence["-="] = 2;
    precedence["*="] = 2;
    precedence["/="] = 2;
    precedence["%="] = 2;
    precedence[">>="] = 2;
    precedence["<<="] = 2;
    precedence["&="] = 2;
    precedence["^="] = 2;
    precedence["|="] = 2;
    precedence["||"] = 3;
    precedence["&&"] = 4;
    precedence["|"] = 5;
    precedence["^"] = 6;
    precedence["&"] = 7;
    precedence["=="] = 8;
    precedence["!="] = 8;
    precedence["<"] = 9;
    precedence["<="] = 9;
    precedence[">"] = 9;
    precedence[">="] = 9;
    precedence["<=>"] = 10;
    precedence["<<"] = 11;
    precedence[">>"] = 11;
    precedence["+"] = 12;
    precedence["-"] = 12;
    precedence["*"] = 13;
    precedence["/"] = 13;
    precedence["%"] = 13;
    precedence[".*"] = 14;
    precedence["->*"] = 15;
    precedence["++"] = 16;
    precedence["--"] = 16;
    precedence["!"] = 16;
    precedence["~"] = 16;

    for (int i = 0; i < n; i++)
    {
        string p;
        cin >> p;
        string start = "";
        start += p[0];
        string end = "";
        string t = "";

        for (int j = 3; j < p.size(); j++)
        {

            if (p[j] >= 'A' && p[j] <= 'Z')
            {
                if (t != "")
                {
                    token.insert(t);
                }
                t = "";
            }
            else
            {
                t += p[j];
            }
            end += p[j];
        }

        production[start].push_back(end);
        reverse_production[end].push_back(start);

        if (t != "")
            token.insert(t);
    }

    vector<vector<string>> table = precedence_table(token);

    cout << "\n"
         << "Operator-Precedence Table"
         << "\n\n";

    for (auto i : table)
    {
        for (auto j : i)
        {
            cout << j << "\t|";
        }
        cout << "\n";
    }

    cout << "\n\n";

    cout << "Enter input string: " << '\n';
    string input;
    vector<string> inp;
    string fullInput = "";
    while (cin >> input)
    {
        inp.push_back(input);
        fullInput += input;
    }

    inp.push_back("$");
    for (int i = 0; i < inp.size(); i++)
    {
        cout << inp[i];
    }
    cout << endl;
    unordered_map<string, int> strToInd;
    for (int i = 1; i < table.size(); i++)
    {
        strToInd[table[0][i]] = i;
    }

    vector<string> stk = {"Stack          ", "$              "};
    vector<string> curInput = {"CurInput", fullInput + "$"};
    vector<string> action = {"\t\t\t\tAction"};

    int i = 0;
    stack<string> st;
    st.push("$");

    bool valid = true;

    while (i < inp.size())
    {

        if ((st.top() >= "A" && st.top() <= "Z"))
        {
            string x = st.top();
            st.pop();
            if (st.top() == "$" && inp[i] == "$")
            {
                action.push_back("\t\tAccept");
                i++;
                st.push(x);
                break;
            }
            else if (table[strToInd[st.top()]][strToInd[inp[i]]] == "<.")
            {
                st.push(x);
                st.push(inp[i]);
                action.push_back("\t\tShift");
                i++;
            }

            else if ((table[strToInd[st.top()]][strToInd[inp[i]]] == ".>") || (inp[i] == ")" && st.top() != "("))
            {
                action.push_back("\t\tReduce");
                string match = x;
                if (reverse_production.count(match))
                {
                    st.push(reverse_production[match][0]);
                }
                else
                {
                    while (st.size() && st.top() != "$")
                    {
                        match = st.top() + match;
                        st.pop();
                        if (reverse_production.count(match))
                        {
                            st.push(reverse_production[match][0]);
                            break;
                        }
                    }
                    if (st.size() == 0 || st.top() == "$")
                    {
                        valid = false;
                        cout << "Invalid Input\n";
                        break;
                    }
                }
            }
            else
            {
                cout << "Invalid Input\n";
                valid = false;
                break;
            }
        }
        else
        {
            if (table[strToInd[st.top()]][strToInd[inp[i]]] == "<.")
            {
                st.push(inp[i]);
                action.push_back("\t\tShift");
                i++;
            }
            else if (table[strToInd[st.top()]][strToInd[inp[i]]] == ".>")
            {
                action.push_back("\t\tReduce");
                string match = "";
                while (st.size() && st.top() != "$")
                {
                    match = st.top() + match;
                    st.pop();
                    if (reverse_production.count(match))
                    {
                        st.push(reverse_production[match][0]);
                        break;
                    }
                }
                if (st.size() == 0 || st.top() == "$")
                {
                    valid = false;
                    cout << "Invalid Input\n";
                    break;
                }
            }
            else
            {
                cout << "Invalid Input\n";
                valid = false;
                break;
            }
        }
        stk.push_back(stackToString(st));
        curInput.push_back(vectorToString(inp, i, fullInput.size() + 1));
    }

    if (valid)
    {
        for (int i = 0; i < stk.size(); i++)
        {
            cout << stk[i] << "\t" << curInput[i] << "\t" << action[i] << "\n";
        }
    }
    return 0;
}
