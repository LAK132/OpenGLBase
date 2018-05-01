#define LAKMAIN_MULTITHREAD

#ifndef DEFINES_H
#define DEFINES_H

extern bool debugOn;
extern bool throwOn;
#define DEBUG if(debugOn) cout << __FILE__ << "(" << __LINE__ << ")" << endl
#define THROW(x) if(throwOn) throw x

#endif