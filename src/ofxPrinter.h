#pragma once
#include "ofMain.h"

#define IM_TEXT_SIZE 1000
#define ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


//----- reffered from
//      https://gist.github.com/robotconscience/2554110
//#include "ofThread.h"

class SysCommand : private ofThread{
public:
    ofEvent<string> commandComplete;

    void callCommand( string command ){
        cmd = command;
        stopThread();
        startThread();
    }

    std::string exec(char* cmd) {
#ifdef WIN32
        FILE* pipe = _popen(cmd, "r");
#else
        FILE* pipe = popen(cmd, "r");
#endif
        if (!pipe) return "ERROR";
        char buffer[128];
        std::string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
#ifdef WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
        return result;
    }

private:
    void threadedFunction(){
        ofLog( OF_LOG_VERBOSE, "call "+cmd );
        string result = exec( (char *) cmd.c_str() );
        ofLog( OF_LOG_VERBOSE, "RESULT "+result );
        stopThread();
        ofNotifyEvent( commandComplete, result, this );
    }

    string cmd;
};




struct OFX_PRINTER {
	string printerName = "default";
	string paperSize = "A4";
	ofVec4f margin = ofVec4f(100, 100, 100, 100);
	bool landscape = false;
	bool color = true;
	bool fitToPaper = true;
};


class ofxPrinter{
public:
	ofxPrinter();
	~ofxPrinter();

	void updatePrinterList();
	void simplePrintOut(string imageFilePath, string printerName);
	void printOut(string imageFilePath, OFX_PRINTER p);

	bool setPrintFilePath(string filePath);

	string getCurrentPrintFilePath();
	string getCurrentSelectPrinterName();
	string getCurrentSelectPaperSize();
	vector<string> getPrinterList();
	vector<string> getPrinterPaperSizes(string printerName);

private:

	void loadSettings();
	void saveSettings();

	string getSelectPrinterName();
	string getSelectPaperSizeName();

	vector<string> printerList;
	vector<vector<string>> paperSizes;
	int printerIndex;
	int paperIndex;

	string printFilePath;
	string logMessage;

	float inputMargin[4];
	bool inputLandscape, inputColor, inputFitToPaper;

};

