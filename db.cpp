#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  string input = "";
  while (true) {
    cout << "db > ";
    cin >> input;

    if (input == ".exit") {
      exit(EXIT_SUCCESS);
    } else {
      cout << "Unrecognized command " << input << endl;
    }
  }
} 