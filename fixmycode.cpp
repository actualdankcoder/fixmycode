#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
using namespace std;
int main(int argc, char** argv) {
	string line;
	ofstream logs;
	string lineno;
	string traceback="	˪-----> ";
	int i;
	if(argc==1){
		cout << "Please pass the file name, for eg: fixmycode bot.py" << endl;
		return 0;
	}
	ifstream codefile(argv[1]);
	logs.open("log.txt");
	if(codefile.is_open()) {
		logs << "------------------------------------------------------" << "\n\n";
		while(getline(codefile,line)) {
			i++;
			if(i>32000){
				cout<<"Memory Error: please retry the command" << endl;
				break;
			}
			lineno="line [" + to_string(i) + "] -> ";

			if(line.find("import discord") != string::npos){
				string checkexistence=exec("python3 -m discord");
				if(checkexistence.find("No Module") != string::npos){
					logs <<  lineno << "Discord Module was imported but has not been downloaded" << "\n";
				}
				if(checkexistence.find("ImportError: cannot import name 'Deque'") != string::npos) {
					logs << lineno << "Python version is below 3.6.5" << "\n";
				}
			}
			if(line.find("client.send_message") != string::npos) {
				logs <<  lineno << "Possible Attribute error: Code uses 'client.send_message', replace it with 'message.channel.send'" << "\n";
				logs << traceback << line << "\n";
			}
			if(line.find("(") != string::npos){
				if (!(line.find(")") != string::npos)) {
					logs << lineno << "Possible syntax error: parathesis were not closed" << "\n";
					logs << traceback << line << "\n";
				}
			}
		}
		logs << "\n" << "------------------------------------------------------" << "\n";
		codefile.close();
		logs.close();
	}
	return 0;
}
