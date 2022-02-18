/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics

 Modified by Frederic Pillon <frederic.pillon@st.com> for STMicroelectronics
 Modified by Technik Gegg for Arduino libraries compatibility 2022/02

 */

#ifndef __SD_H__
#define __SD_H__

#include <Arduino.h>

#include "Sd2Card.h"
#include "SdFatFs.h"

// flags for ls()
/** ls() flag to print modify date */
uint8_t const LS_DATE = 1;
/** ls() flag to print file size */
uint8_t const LS_SIZE = 2;
/** ls() flag for recursive list of subdirectories */
uint8_t const LS_R = 4;

// added inheritance of Print, as done in Arduino libs 2022/02 Technik.Gegg
class File : public Print {
  public:
    File(FRESULT res = FR_OK);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual size_t write(const char *buf, size_t size);

    virtual int read();
    virtual int peek();
    virtual int available();
    virtual void flush();
    int read(void *buf, size_t len);
    bool seek(uint32_t pos);
    uint32_t position();
    uint32_t size();
    void close();
    operator bool();
    // added 2022/02 Technik.Gegg
    int fgets(TCHAR* buf, size_t len);
    bool rewind() {
      return seek(0); 
    };

    char *name(void);
    char *fullname(void)
    {
      return _name;
    };
    bool isDirectory();
    File openNextFile(uint8_t mode = FILE_READ);
    void rewindDirectory(void);

    virtual size_t print(const char *data);
    virtual size_t println();
    virtual size_t println(const char *data);
    virtual size_t println(String &data);

    // added optional parameter 'print' 2022/02 Technik.Gegg
    // Print to Serial line or to instance referenced in parameter print
    void ls(uint8_t flags, uint8_t indent = 0, Print* print = &Serial);
    static void printFatDate(uint16_t fatDate, Print* print);
    static void printFatTime(uint16_t fatTime, Print* print);
    static void printTwoDigits(uint8_t v, Print* print);


    char *_name = NULL; //file or dir name
    FIL *_fil = NULL; // underlying file object structure pointer
    DIR _dir = {}; // init all fields to 0
    FRESULT _res = FR_OK;

    FRESULT getErrorstate(void) {return _res;}

};

class SDClass {

  public:

    /* Initialize the SD peripheral */
    bool begin(uint32_t detectpin = SD_DETECT_NONE);
    static File open(const char *filepath, uint8_t mode = FA_READ);
    static bool exists(const char *filepath);
    static bool mkdir(const char *filepath);
    static bool remove(const char *filepath);
    static bool rmdir(const char *filepath);

    File openRoot(void);

    friend class File;

  private:
    Sd2Card _card;
    SdFatFs _fatFs;

};

extern SDClass SD;

#endif
