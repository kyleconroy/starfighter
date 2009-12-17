#ifndef _log_h_included_
#define _log_h_included_

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

class Trace
{
    static string filename;
    static int    tracelevel;
public:
    static bool log(const ostringstream& oss)
    {
        ofstream ofs(filename.c_str(), ios::out | ios::app);
        if (ofs.fail())
            return false;
        if (!(ofs << oss.str() << endl))
        {
            ofs.close();
            return false;
        }
        ofs.close();
        return true;
    }
    static int     getTraceLevel() { return tracelevel; }
    static void    setTraceLevel(int lev) { tracelevel = lev; }
    static string  getTraceFile() { return filename; }
    static void    setTraceFile(const string& tracefile) { filename = tracefile; }
};

#define LOG(text, level) \
{ostringstream oss; if (level >= Trace::getTraceLevel()) Trace::log((ostringstream&)(oss<<text));}

#endif
