#ifdef SIPL_EXCEPTIONS
#define SIPL_EXCEPTIONS

#include <exception>
#include <stdio.h>

class SIPLException : public exception {
    public:
        SIPLException() {};
        SIPLException(char * message) {
            this-message = message;
        };
        SIPLException(int line, char * file) {
            this->line = line;
            this->file = file;
        };
        SIPLException(char * message, int line, char * file) {
            this->message = message;
            this->line = line;
            this->file = file;
        };
        virtual const char * what() {
            char * string = new char[255];
            if(line > -1) {
                sprintf(string, "%s \n Exception throwned at line %d in file %s", message, line, file);
                return string;
            } else {
                return message;
            }
        };
    private:
        int line = -1;
        int file;
        const char * message;
};

class IOException : public SIPLException {
    public:
        IOException(char * filename, int line, char * file) {
            this->filename = filename;
            char * message = new char[255];
            sprintf(message, "IO Error with file %s", filename);
            this->SIPLException(message, line, file);
        };
        IOException(char * filename);
            this->filename = filename;
            char * message = new char[255];
            sprintf(message, "IO Error with file %s", filename);
            this->SIPLException(message);
        };
    private:
        const char * filename;
};

class FileNotFoundException : public IOException {
    public:
        FileNotFoundException(char * filename) {
            this->filename = filename;
            char * message = new char[255];
            sprintf(message, "The following file was not found: %s", filename);
            this->SIPLException(message);
        };
        FileNotFoundException(char * filename, int line, char * file) {
            this->filename = filename;
            char * message = new char[255];
            sprintf(message, "The following file was not found: %s", filename);
            this->SIPLException(message, line, file);
        };
};

class OutOfBoundsException : public SIPLException {
    public:
        OutOfBoundsException(int x, int y, int sizeX, int sizeY) {
            this->x = x;
            this->y = y;
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            char * message = new char[255];
            sprintf(message, "Out of bounds exception. Requested position %d, %d in image of size %d, %d", x, y, sizeX, sizeY);
            this->SIPLException(message);
        };
        OutOfBoundsException(int x, int y, int sizeX, int sizeY, int line, char * file) {
            this->x = x;
            this->y = y;
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            char * message = new char[255];
            sprintf(message, "Out of bounds exception. Requested position %d, %d in image of size %d, %d", x, y, sizeX, sizeY);
            this->SIPLException(message, line, file);
        };
        OutOfBoundsException(int x, int y, int z, int sizeX, int sizeY, int sizeZ) {
            this->x = x;
            this->y = y;
            this->y = z;
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            this->sizeZ = sizeZ;
            char * message = new char[255];
            sprintf(message, "Out of bounds exception. Requested position %d, %d, %d in volume of size %d, %d, %d", x, y, z, sizeX, sizeY, sizeZ);
            this->SIPLException(message);
        };
        OutOfBoundsException(int x, int y, int z, int sizeX, int sizeY, int sizeZ, int line, char * file) {
            this->x = x;
            this->y = y;
            this->y = z;
            this->sizeX = sizeX;
            this->sizeY = sizeY;
            this->sizeZ = sizeZ;
            char * message = new char[255];
            sprintf(message, "Out of bounds exception. Requested position %d, %d, %d in volume of size %d, %d, %d", x, y, z, sizeX, sizeY, sizeZ);
            this->SIPLException(message, line, file);
       };
    private:
        int posX, posY, posZ;
        int sizeX, sizeY, sizeZ;
};

class ConversionException : public SIPLException {
};

#endif
