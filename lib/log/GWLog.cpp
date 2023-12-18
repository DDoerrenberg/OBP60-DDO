#include "GwLog.h"

class DefaultLogWriter: public GwLogWriter{
    public:
        virtual ~DefaultLogWriter(){};
        virtual void write(const char *data){
            Serial.print(data);
        }
};

GwLog::GwLog(int level, GwLogWriter *writer){
    logLevel=level;
    if (writer == NULL) writer=new DefaultLogWriter();
    this->writer=writer;
    locker = xSemaphoreCreateMutex();
}
GwLog::~GwLog(){
    vSemaphoreDelete(locker);
}
void GwLog::logString(const char *fmt,...){
    va_list args;
    va_start(args,fmt);
    xSemaphoreTake(locker, portMAX_DELAY);
    recordCounter++;
    vsnprintf(buffer,bufferSize-1,fmt,args);
    buffer[bufferSize-1]=0;
    if (! writer) {
        xSemaphoreGive(locker);
        return;
    }
    writer->write(prefix.c_str());
    char buf[20];
    snprintf(buf,20,"%lu:",millis());
    writer->write(buf);
    writer->write(buffer);
    writer->write("\n");
    xSemaphoreGive(locker);
}
void GwLog::logDebug(int level,const char *fmt,...){
    if (level > logLevel) return;
    va_list args;
    va_start(args,fmt);
    xSemaphoreTake(locker, portMAX_DELAY);
    recordCounter++;
    vsnprintf(buffer,bufferSize-1,fmt,args);
    buffer[bufferSize-1]=0;
    if (! writer) {
        xSemaphoreGive(locker);
        return;
    }
    writer->write(prefix.c_str());
    char buf[20];
    snprintf(buf,20,"%lu:",millis());
    writer->write(buf);
    writer->write(buffer);
    writer->write("\n");
    xSemaphoreGive(locker);
}
void GwLog::setWriter(GwLogWriter *writer){
    xSemaphoreTake(locker, portMAX_DELAY);
    if (this->writer) delete this->writer;
    this->writer=writer;
    xSemaphoreGive(locker);
}

void GwLog::flush(){
    xSemaphoreTake(locker, portMAX_DELAY);
    if (! this->writer) {
        xSemaphoreGive(locker);
        return;
    }
    this->writer->flush();
    xSemaphoreGive(locker);
}
