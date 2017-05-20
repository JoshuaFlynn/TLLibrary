#ifndef G_FILEPROCEQUALS_H
#define G_FILEPROCEQUALS_H

/*
Generated on: 2012-04-10-14.25.47

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "FileProc.h"
#include "TemplateList.h"

namespace TL
{

class FileProcEquals : public FileProc
{
	private:
	protected:

	public:

		FileProcEquals(){Open();}
		FileProcEquals(const CharArray &FileN, const CharArray &AccessFlags)
		{
		    ERRORCLASSFUNCTION(FileProcEquals,FileProcEquals,!OpenFile(FileN, AccessFlags),)
		}
		~FileProcEquals(){Close();}

        const TemplateList<char> ToTemplateList() const
        {
            FileProcEquals Temp;

            if(FindItem(GetAccessMode().GetArray(),GetAccessMode().GetSize(),'b') != NULL || FindItem(GetAccessMode().GetArray(),GetAccessMode().GetSize(),'B') != NULL)
            {
                ERRORCLASSFUNCTION(FileProcEquals,ToTemplateList,!Temp.OpenFile(GetFileName().GetArray(),"rb"),return TemplateList<char>();)
            }
            else
            {
                ERRORCLASSFUNCTION(FileProcEquals,ToTemplateList,!Temp.OpenFile(GetFileName().GetArray(),"r"),return TemplateList<char>();)
            }

            TemplateList<char> TempList;
            int C = 0;
            do
            {
                C = fgetc(Temp.GetFilePtr());

                if(C == EOF)
                {
                    if(feof(Temp.GetFilePtr()) != 0)
                    {
                        break;
                    }
                    else
                    {
                        ERRORCLASSFUNCTION(FileProcEquals,ToTemplateList,(ferror(Temp.GetFilePtr()) != 0),clearerr(Temp.GetFilePtr()); rewind(Temp.GetFilePtr()); return TemplateList<char>();)
                    }
                }

                ERRORCLASSFUNCTION(FileProcEquals,ToTemplateList,!TempList.AddNodeEndNext(C), rewind(Temp.GetFilePtr()); return TemplateList<char>();)

            }while(feof(Temp.GetFilePtr()) == 0);

            rewind(Temp.GetFilePtr());
            return TempList;
        }

        const CharArray ToCharArray() const
        {
            return CharArray(ToTemplateList().ToVector());
        }

        const TemplateArray<char> ToTemplateArray() const
        {
            return TemplateArray<char>(ToTemplateList().ToVector());
        }

        const std::string ToString() const
        {
            return ToCharArray().ToString();
        }

        const std::vector<char> ToVector() const
        {
            return ToTemplateList().ToVector();
        }

        const bool WriteChar(const int C)
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteChar,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteChar,IsReadOnly(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteChar,fputc(C,FilePtr) != C, clearerr(FilePtr); rewind(FilePtr); RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteChar,fflush(FilePtr) == EOF,RETURN_BOOL);
            return true;
        }

        const bool WriteList(const TemplateList<char> &ItemCopy)
        {
            TemplateIterator<char> Temp;

            Temp = ItemCopy;
            ERRORCLASSFUNCTION(FileProcEquals,WriteList,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteList,IsReadOnly(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteList,!Temp.IsValid(),RETURN_BOOL)
            int C = 0;
            do
            {
                C = Temp.GetCurrItem();
                ERRORCLASSFUNCTION(FileProcEquals,WriteList,fputc(C,FilePtr) != C,clearerr(FilePtr); rewind(FilePtr); RETURN_BOOL)
            }while(Temp.CurrNext());
            ERRORCLASSFUNCTION(FileProcEquals,WriteList,fflush(FilePtr) == EOF, RETURN_BOOL);
            return true;
        }

        const bool WriteArray(const char Array[], const SIZE_TYPE S)
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteArray,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteArray,IsReadOnly(),RETURN_BOOL)
            ERRORCLASSFUNCTION(FileProcEquals,WriteArray,!IsValidArray(Array,S),RETURN_BOOL)
            const char * Temp = Array, * Temp2 = Array+S;
            int C = 0;
            do
            {
                C = *Temp;
                if(fputc(C,FilePtr) != C)
                {
                    //ERRORCLASSFUNCTION(FileProcEquals,WriteList,C != EOF,perror(""); clearerr(FilePtr); rewind(FilePtr); RETURN_BOOL)
                }

                ++Temp;
            }while(Temp != Temp2);
            ERRORCLASSFUNCTION(FileProcEquals,WriteArray,fflush(FilePtr) == EOF, RETURN_BOOL);
            return true;
        }

        const bool WriteArray(const char Array[])
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteArray,!Array,RETURN_BOOL)
            return WriteArray(Array,strlen(Array));
        }

        const bool WriteCharArray(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteCharArray,!ItemCopy.IsValid(),RETURN_BOOL)
            return WriteArray(ItemCopy.GetArray(),ItemCopy.GetSize());
        }

        const bool WriteTemplateArray(const TemplateArray<char> &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteTemplateArray,!ItemCopy.IsValid(),RETURN_BOOL)
            return WriteArray(ItemCopy.GetArray(),ItemCopy.GetSize());
        }

        const bool WriteString(const std::string &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,WriteString,ItemCopy.empty(),RETURN_BOOL)
            return WriteArray(&ItemCopy[0],ItemCopy.size());
        }

        FileProcEquals &operator=(const FileProc &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->DuplicateSettings(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const FileProcEquals &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->DuplicateSettings(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->WriteCharArray(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const TemplateArray<char> &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->WriteTemplateArray(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const std::string &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->WriteString(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const TemplateList<char> &ItemCopy)
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->WriteList(ItemCopy)),RETURN_THIS)
            return *this;
        }

        FileProcEquals &operator=(const char ItemCopy[])
        {
            ERRORCLASSFUNCTION(FileProcEquals,operator=,!(this->WriteArray(ItemCopy)),RETURN_THIS)
            return *this;
        }
};

}

#endif
