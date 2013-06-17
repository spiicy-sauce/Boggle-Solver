
#include <iostream>
using namespace std;

char letts[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

int main(){
    int k = 0;

    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            cout << letts[k++ % 26];
        }
        cout << endl;
    }
}
