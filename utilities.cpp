#include "main.h"

using namespace std;

void debug(const char* format, ...) {
	if (DEBUG) {
		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);
		fprintf(stdout, "\n");
	}
}

void getCurrentTime(TIME *currentTime) {
#ifdef _WIN32
    *currentTime = GetTickCount();
#else
    gettimeofday(currentTime, NULL);
#endif
}

float getTimeDiff(TIME currentTime, TIME lastTime) {
#ifdef _WIN32
    return (float)(currentTime - lastTime)*0.001f;
#else
    return (float)((currentTime.tv_sec - lastTime.tv_sec) + 1e-6*(currentTime.tv_usec - lastTime.tv_usec));
#endif
}

bool fequals(float actual, float expected) {
    return fabs(actual - expected) < 0.0000001;
}

void playAudio(const char *file) {
#ifdef OSX
    /* Courtesy Apple Mac OS X Reference Library */
    SystemSoundID ssid;
    CFURLRef urlRef;
    
    char path[100];
    strcpy(path, "audio/");
    if (kyleMode) {
        strcat(path, "k");
    }
    strcat(path, file);
    strcat(path, ".wav");
    
    CFStringRef fileCFStr = CFStringCreateWithCString(NULL, path, kCFStringEncodingMacRoman);
    urlRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, 
        fileCFStr, kCFURLPOSIXPathStyle, FALSE);
    /* Create a system sound ID to represent the sound file. */
    AudioServicesCreateSystemSoundID(urlRef, &ssid);
    /* Register the sound completion callback. */
    AudioServicesAddSystemSoundCompletion(ssid, NULL, NULL, audioCompleted, (void *) urlRef);
    /* Play the sound file. */
    AudioServicesPlaySystemSound(ssid);
    
    if (strstr(path, "soundtrack")) {
        loopBGMusic = true;
        bgMusic = ssid;
    }
#endif
}

void audioCompleted(SystemSoundID ssid, void *urlRef) {
#ifdef OSX
    if (ssid == bgMusic && loopBGMusic) {
        AudioServicesPlaySystemSound(ssid);
        return;
    }
    AudioServicesDisposeSystemSoundID(ssid);
    CFRelease(urlRef);
#endif
}

void audioCompletedForce(SystemSoundID ssid) {
#ifdef OSX
    AudioServicesDisposeSystemSoundID(ssid);
#endif
}

// JKM because stringstreams aren't working for me...
char *itoa(int i, char * buf) {
	int negativeFlag = 1; // 1 if positive, -1 if negative
	int numDigits = 1;
	int divisor = 1;
	// check for negative
	if (i < 0) {
		negativeFlag = -1;
	}
	// count the number of digits
	int i_copy = i * negativeFlag;
	while (i_copy >= 10) {
		numDigits++;
		i_copy = i_copy/10;
		divisor *= 10;
	}
	// negative/non-negative
	int index = 0;
	if (negativeFlag == -1) {
		buf[0] = '-';
		index = 1;
		buf[numDigits+1] = '\0';
	}
	else {
		buf[numDigits] = '\0';
	}
	// insert digits into char array
	i_copy = i * negativeFlag;
	while (divisor != 0) {
		int digit = i_copy/divisor;
		buf[index] = digit + '0';
		i_copy = i_copy - (digit * divisor);
		divisor = divisor/10;
		index++;
	}
	return buf;
}
