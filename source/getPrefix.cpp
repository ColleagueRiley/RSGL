#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

int main(){
    FILE *fp;
    char path[1035];
    std::vector<std::string> paths;

    fp = fopen("gcc-cmd", "r");

    if (fp == NULL)
        return 0;

    while (fgets(path, sizeof(path), fp) != NULL)
        paths.push_back(path);

    for (int i = 0; i < paths.size(); i++)
        if (paths[i].find("/lib/") < paths[i].size()){
            std::string str = paths[i]; 
            str.replace(str.begin() + str.find("lib/") - 1, str.end(), "");

            str.replace(str.begin(), str.begin() + str.find_first_of("/"), "");

            printf(str.c_str());
            break;
        }


    fclose(fp);
}