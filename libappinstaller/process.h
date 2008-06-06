#ifndef PROCESS_H
#define PROCESS_H

#include <kprocess.h>
#include <iostream>
#include <fstream>



class Process;



class Process : public QObject {
	Q_OBJECT

public:
	Process();
	~Process();

	QString getBuffer();
	int exitStatus();
	bool normalExit();
	void setCommand(QString command);
	void start(bool block=true);

protected:
	KProcess* _process;
	QString _buffer;

protected slots:
	void slotProcessOutput(KProcess* process, char* buffer, int buflen);
};



#endif
