#include <string>
#include <stack>
#include <iostream>

using namespace std;

class Solution {
public:
    string simplyfyPath(string path) {
        stack<string> st;
        string res;

        for(int i = 0; i < path.size(); i++) {
            if(path[i] == '/')
                continue;
            
            string temp;
            // iterate till we do not encounter '/' or end of string
            while(i < path.size() && path[i] != '/') {
                temp += path[i];
                i++;
            }

            if(temp == ".") {
                continue;
            } else if(temp == "..") {
                if(!st.empty())
                    st.pop();
            } else {
                st.push(temp);
            }
        }

        while(!st.empty()) {
            res = "/" + st.top() + res;
            st.pop();
        }

        if(res.size() == 0) {
            return "/";
        }

        return res;
    }
};

int main() {
    Solution s;
    std::cout << "/home/" << " : " << s.simplyfyPath("/home/") << std::endl;
    std::cout << "/home//foo/" << " : " << s.simplyfyPath("/home//foo/") << std::endl;
    std::cout << "/home/user/Documents/../Pictures" << " : " << s.simplyfyPath("/home/user/Documents/../Pictures") << std::endl;
    std::cout << "/../" << " : " << s.simplyfyPath("/../") << std::endl;
    std::cout << "/.../a/../b/c/../d/./" << " : " << s.simplyfyPath("/.../a/../b/c/../d/./") << std::endl;
    std::cout << "." << " : " << s.simplyfyPath(".") ;

    return 0;
}