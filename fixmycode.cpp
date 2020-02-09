#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <stdlib.h>

std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    auto pipe = popen(cmd, "r");
    
    if (!pipe) throw std::runtime_error("popen() failed!");
    
    while (!feof(pipe))
    {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }
    
    auto rc = pclose(pipe);
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
				string checkexistence=exec("python3 -m discord >stderr.log 2>&1");
				ifstream error_file("stderr.log");
				getline(error_file, checkexistence);
				if(checkexistence.find("No module") != string::npos){
					logs <<  lineno << "Discord Module was imported but has not been downloaded" << "\n";
					string checkpip=exec("pip3 >stderr.log 2>&1");
					ifstream error_file("stderr.log");
					getline(error_file, checkpip);					
					if(checkpip.find("not found") != string::npos){
						logs <<  lineno << "Pip hasn't been installed" << "\n";
					}
					else {
						system("pip3 install discord");
						logs << lineno << "Discord module was installed" << "\n";
					}
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
		cout << "Check log.txt" << endl;
		logs << "\n" << "------------------------------------------------------" << "\n";
		codefile.close();
		logs.close();
	}
	return 0;
}