#ifndef G_FILEPROC_H
#define G_FILEPROC_H

/*
Generated on: 2012-04-02-13.12.01

Version: 2.2
*/

/*
Notes:
Cross-compiler edition (Linux compatibility)
Added error check for fclose
*/

/*
Suggestions for improvements:


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "FlagOps.h"
#include "CharArray.h"

namespace TL
{

const bool FileExists(const char FileName[])
{
    FILE *Check = NULL;

    Check = fopen(FileName,"r");
    if(Check == NULL)
    {
        return false;
    }
    //Anyone implement error checking for fclose please.
    if(fclose(Check) != 0)
    {
        return false;
    }
    return true;
}

const int GetAccessFlag(FILE * Ptr)
{
    if(Ptr == NULL)
    {
        return 0;
    }
    #ifdef IS_WINDOWS
    return Ptr->_flag;
    #else
    //Mainly for Linux
    return Ptr->_flags;
    #endif
}

const int GetReadOnly()
{
    #ifdef IS_WINDOWS
    return _IOREAD;
    #endif
    #ifdef IS_LINUX
    return _IO_NO_WRITES;
    #endif
    #ifdef IS_APPLE
    return __SRD;
    #elifndef IS_LINUX
    return _IOREAD;//Other OSes should be added
    #endif
}

const bool IsReadOnlyFlag(FILE * Ptr)
{
    if(Ptr == NULL)
    {
        return false;
    }

    if(AreFlagsEnabled(GetAccessFlag(Ptr),GetReadOnly()))
    {
        return true;
    }
    return false;
}

class FileProc
{
	private:
	protected:
        FILE *FilePtr;
        CharArray FileName;
        CharArray AccessMode;

        void Clear(){FilePtr = NULL; FileName.Open();}

        FILE * GetFilePtr(){return FilePtr;}

	public:
        void Open(){Clear();}
        const bool Close()
        {
            bool ReturnBool = true;
            if(FilePtr != NULL)
            {
                if(!IsReadOnlyFlag(FilePtr))
                {
                    ERRORCLASSFUNCTION(FileProc,Close,fflush(FilePtr),ReturnBool = false;)
                }
                ERRORCLASSFUNCTION(FileProc,Close,fclose(FilePtr) != 0,ReturnBool = false;)
                FilePtr = NULL;
            }

            FileName.Close();
            return ReturnBool;
        }

        const bool Reset()
        {
            ERRORCLASSFUNCTION(FileProc,Reset,!Close(),Open(); RETURN_BOOL)
            Open();
            return true;
        }

        inline const bool IsValid() const { return (FileName.IsValid() && AccessMode.IsValid() && FilePtr); }
        inline const bool IsEmpty() const { return (FileName.IsEmpty() && AccessMode.IsEmpty() && !FilePtr); }
        inline const bool IsReadOnly() const {return IsReadOnlyFlag(FilePtr); }

        const bool OpenFile(const CharArray &FileN, const CharArray &AccessFlags)
        {
            ERRORCLASSFUNCTION(FileProc,OpenFile,FileN.GetSize() < 1, RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,AccessFlags.GetSize() > 4,RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,AccessFlags.GetSize() < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,!FileName.SetArray(FileN),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,!AccessMode.SetArray(AccessFlags),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,!OpenFile(),RETURN_BOOL)

            return true;
        }

        const bool OpenFile()
        {
            ERRORCLASSFUNCTION(FileProc,OpenFile,FileName.GetSize() < 1, RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,AccessMode.GetSize() > 4,RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,AccessMode.GetSize() < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProc,OpenFile,!(FilePtr = fopen(FileName.GetArray(),AccessMode.GetArray()) ),perror(""); RETURN_BOOL)
            return true;
        }

		FileProc(){Open();}
		FileProc(const CharArray &FileN, const CharArray &AccessFlags)
		{
            ERRORCLASSFUNCTION(FileProc,FileProc,!OpenFile(FileN, AccessFlags),)
		}
		~FileProc(){Close();}

        const CharArray &GetFileName() const { return FileName; }
        const CharArray &GetAccessMode() const { return AccessMode; }

        const bool DuplicateSettings(const FileProc &ItemCopy)
        {
            FileProc Temp;

            ERRORCLASSFUNCTION(FileProc,DuplicateSettings(),!Temp.FileName.SetArray(ItemCopy.FileName),RETURN_BOOL);
            ERRORCLASSFUNCTION(FileProc,DuplicateSettings(),!Temp.AccessMode.SetArray(ItemCopy.AccessMode),RETURN_BOOL);
            if(!ItemCopy.FilePtr)
            {
                ERRORCLASSFUNCTION(FileProc,DuplicateSettings(),!Temp.OpenFile(),RETURN_BOOL);
            }

            FILE * TempFile = NULL;
            TempFile = Temp.FilePtr;
            Temp.FilePtr = FilePtr;
            FileName.Swap(Temp.FileName);
            AccessMode.Swap(Temp.AccessMode);
            FilePtr = TempFile;
            ERRORCLASSFUNCTION(FileProc,DuplicateSettings(),!Temp.Close(),RETURN_BOOL)

            return true;
        }

		void Swap(FileProc &ItemCopy)
		{
		    FileProc Temp;
		    Temp.Open();
		    Temp.FilePtr = ItemCopy.FilePtr;
		    Temp.FileName.Swap(ItemCopy.FileName);
		    Temp.AccessMode.Swap(ItemCopy.AccessMode);

            ItemCopy.FilePtr = FilePtr;
            ItemCopy.FileName.Swap(FileName);
            ItemCopy.AccessMode.Swap(AccessMode);

            FilePtr = Temp.FilePtr;
            FileName.Swap(Temp.FileName);
            AccessMode.Swap(Temp.AccessMode);
            return;
		}

        FileProc &operator=(const FileProc &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProc,operator=,!DuplicateSettings(ItemCopy),RETURN_THIS)
            return *this;
        }
};

}

#endif
