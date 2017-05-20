#ifndef G_DIRECTORY_H
#define G_DIRECTORY_H

/*
Generated on: 2012-04-16-19.38.31

Version: 2.2
*/

/*
Notes:
Cross-compile compatible
Fixed some title errors
Added path_max for linux side operations

Implemented Open, Close and Reset, which are part of the TL standard
Implemented transferfrom
*/

/*
Suggestions for improvements:


*/

#include "CharArray.h"
#include "CommonFunctions.h"
#include <limits.h>

#ifdef IS_WINDOWS
#include <direct.h>
#endif
#ifdef IS_LINUX
#include <sys/stat.h>
#endif

#define DIR_ERR_CHARS_WINDOWS "\001\002\003\004\005\006\a\010\t\n\v\f\r\016\017\020\021\022\023\024\025\026\027\030\031\032\033\034\035\036\037\"<>\?|\177"
#define DIR_SUB_ERR_CHARS_WINDOWS ":"

#define DIR_ALPHABET "qwertyuiopasdfghjklzxcvbnm"

namespace TL
{

class Directory
{
	private:
	protected:
        CharArray FullDir;
        CharArray Dir;

        const bool ClearDuplicateSeperators()
        {
            return ClearDuplicateSeperators(FullDir);
        }

        const bool ClearDuplicateSeperators(CharArray &Temp)
        {
            if(Temp.IsValid())
            {
                CharArray TempDir;
                ERRORCLASSFUNCTION(Directory,ClearDuplicateSeperators,!TempDir.SetArray(Temp),RETURN_BOOL)
                bool ClickBool = false;
                while(1)
                {
                    ClickBool = false;
                    if(Find(TempDir.GetArray(),TempDir.GetSize(),"\\\\",2))
                    {
                        ERRORCLASSFUNCTION(Directory,ClearDuplicateSeperators,!TempDir.Replace("\\\\",2,"\\",1),RETURN_BOOL)
                        ClickBool = true;
                    }
                    if(Find(TempDir.GetArray(),TempDir.GetSize(),"\\/",2))
                    {
                        ERRORCLASSFUNCTION(Directory,ClearDuplicateSeperators,!TempDir.Replace("\\/",2,"\\",1),RETURN_BOOL)
                        ClickBool = true;
                    }
                    if(Find(TempDir.GetArray(),TempDir.GetSize(),"//",2))
                    {
                        ERRORCLASSFUNCTION(Directory,ClearDuplicateSeperators,!TempDir.Replace("//",2,"/",1),RETURN_BOOL)
                        ClickBool = true;
                    }
                    if(Find(TempDir.GetArray(),TempDir.GetSize(),"/\\",2))
                    {
                        ERRORCLASSFUNCTION(Directory,ClearDuplicateSeperators,!TempDir.Replace("/\\",2,"\\",1),RETURN_BOOL)
                        ClickBool = true;
                    }
                    if(ClickBool)
                    {
                        continue;
                    }
                    break;
                }
                Temp.Swap(TempDir);
            }
            return true;
        }

        //Note: this assumes that a seperator is always appended to the end
        //It also assumes that the drive is the lowest level, and a count of lower than 1 is not possible
        const unsigned short CountSeperators(const CharArray &ItemCopy) const
        {
            if(!ItemCopy.IsValid()){return 0;}
            const char * Temp = ItemCopy.GetArray(), * Temp2 = ItemCopy.GetArray()+ItemCopy.GetSize();
            unsigned short Count = 0;
            while(Temp < Temp2)
            {
                //Iterate to the seperator
                while((Temp < Temp2) && !IsCompareChar(*Temp,"\\/")){++Temp;}
                //Count it if it's valid (it might exit before a seperator so we have to check)
                if(IsCompareChar(*Temp,"\\/")){++Count;}
                //Iterate to the next non-seperator character (this is in case of the rare instance of multiple seperators in a row)
                while((Temp < Temp2) && IsCompareChar(*Temp,"\\/")){++Temp;}
            }
            return Count;
        }

        const bool IsValidWindowsDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,!TempDir.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,!IsCompareChar(TempDir.GetArray()[0],DIR_ALPHABET,true),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,TempDir.GetArray()[1] != ':',RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,!IsCompareChar(TempDir.GetArray()[2],"\\/"),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,!IsItem('\0',TempDir.GetArray(),TempDir.GetSize()),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory, (TempDir.GetArray()[TempDir.GetSize()-1] != '\0'), RETURN_BOOL)

            char Array[] = DIR_ERR_CHARS_WINDOWS;
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,ScanUntil(TempDir.GetArray(),TempDir.GetSize(),Array,strlen(Array)),RETURN_BOOL)
            return true;
        }

        //As I am not familiar with the proper mac directory, this function will only do basic checks
        const bool IsValidMacDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory,!IsItem('\0',TempDir.GetArray(),TempDir.GetSize()),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsDirectory, (TempDir.GetArray()[TempDir.GetSize()-1] != '\0'), RETURN_BOOL)
            return true;
        }

        //Whilst Linux, Unix, Posix etc etc might have perhaps a different set-up
        //As I am not familiar with them, for ease of use, I have classified them all as linux for now
        //Different function definitions where necessary pending
        const bool IsValidLinuxDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidLinuxDirectory,TempDir.GetSize() > PATH_MAX,RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidLinuxDirectory,!IsItem('\0',TempDir.GetArray(),TempDir.GetSize()),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidLinuxDirectory, (TempDir.GetArray()[TempDir.GetSize()-1] != '\0'), RETURN_BOOL)
            return true;
        }

        const bool IsValidDefaultDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidDefaultDirectory,!IsItem('\0',TempDir.GetArray(),TempDir.GetSize()),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidDefaultDirectory, (TempDir.GetArray()[TempDir.GetSize()-1] != '\0'), RETURN_BOOL)
            return true;
        }

        const bool IsValidDirectory(const CharArray &TempDir) const
        {
            #ifdef _WIN64
            return IsValidWindowsDirectory(TempDir);
            #elif _WIN32
            return IsValidWindowsDirectory(TempDir);
            #elif __APPLE__
            return IsValidMacDirectory(TempDir);
            #elif __linux
            return IsValidLinuxDirectory(TempDir);
            #elif _unix
            return IsValidLinuxDirectory(TempDir);
            #elif _posix
            return IsValidLinuxDirectory(TempDir);
            #endif
            return IsValidDefaultDirectory(TempDir);
        }


        const bool IsValidWindowsSubDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidWindowsSubDirectory,!TempDir.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsSubDirectory,IsCompareArrayChar(TempDir.GetArray(),TempDir.GetSize(),DIR_ERR_CHARS_WINDOWS),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,IsValidWindowsSubDirectory,IsCompareArrayChar(TempDir.GetArray(),TempDir.GetSize(),DIR_SUB_ERR_CHARS_WINDOWS),RETURN_BOOL)
            return true;
        }
        
        const bool IsValidLinuxSubDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidLinuxSubDirectory,!TempDir.IsValid(),RETURN_BOOL)
            return true;
        }

        const bool IsValidDefaultSubDirectory(const CharArray &TempDir) const
        {
            ERRORCLASSFUNCTION(Directory,IsValidLinuxSubDirectory,!TempDir.IsValid(),RETURN_BOOL)
            return true;
        }

        const bool IsValidSubDirectory(const CharArray &TempDir) const
        {
            #ifdef _WIN64
            return IsValidWindowsSubDirectory(TempDir);
            #elif _WIN32
            return IsValidWindowsSubDirectory(TempDir);
            #elif __APPLE__
            return IsValidDefaultSubDirectory(TempDir);
            #elif __linux
            return IsValidDefaultSubDirectory(TempDir);
            #elif _unix
            return IsValidDefaultSubDirectory(TempDir);
            #elif _posix
            return IsValidDefaultSubDirectory(TempDir);
            #endif
            return IsValidDefaultSubDirectory(TempDir);
        }

	public:
        void Open(){Dir.Open(); FullDir.Open();}
        void Close(){Dir.Close(); FullDir.Close();}
        void Reset(){Close(); Open();}

        const bool IsValid() const
        {
            if(Dir.IsEmpty() || FullDir.IsEmpty()){return false;}
            if(!IsValidDirectory(FullDir)){return false;}
            return IsValidDirectory(Dir);
        }
        const bool IsEmpty() const {return (Dir.IsEmpty() && FullDir.IsEmpty()); }

        const bool MoveTo(const SIZE_TYPE N = 1)
        {
            ERRORCLASSFUNCTION(Directory,MoveTo,N < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveTo,IsEmpty(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveTo,N > CountLevelsFullDir(),RETURN_BOOL)
            unsigned short Count = CountLevelsDir();
            if(N == Count){return true;}
            if(N > Count)
            {
                return MoveForward(N-Count);
            }
            else
            {
                return MoveBack(Count-N);
            }
        }

        //This function assumes there is a seperator on the end of the directory
        //As in, there's a backlash at the end of it before the null
        const bool MoveBack(const SIZE_TYPE N = 1)
        {
            ERRORCLASSFUNCTION(Directory,MoveBack,N < 1,RETURN_BOOL);
            ERRORCLASSFUNCTION(Directory,MoveBack,IsEmpty(),RETURN_BOOL);
            unsigned short Count = CountSeperators(Dir);
            //ERRORCLASSFUNCTION(Directory,MoveBack,Count < 2, RETURN_BOOL);
            if(Count < 2)
            {
                return false;
            }
            ERRORCLASSFUNCTION(Directory,MoveBack,(Count-N) < 1,RETURN_BOOL);

            CharArray TempDir;
            ERRORCLASSFUNCTION(Directory,MoveBack,!TempDir.SetArray(Dir),RETURN_BOOL)
            const char * Temp = (TempDir.GetArray()+TempDir.GetSize());

            unsigned short Count2 = 0;
            while((Temp > TempDir.GetArray()) && (Count2 < N) )
            {
                while((Temp > TempDir.GetArray()) && !IsCompareChar(*Temp,"\\/")){--Temp;}
                if(IsCompareChar(*Temp,"\\/")){Count2++;}
                while((Temp > TempDir.GetArray()) && IsCompareChar(*Temp,"\\/")){--Temp;}
                while((Temp > TempDir.GetArray()) && !IsCompareChar(*Temp,"\\/")){--Temp;}
            }
            
            #ifdef IS_WINDOWS
            if(Temp == TempDir.GetArray()){return false;}
            #endif
            
            ++Temp;
            ERRORCLASSFUNCTION(Directory,MoveBack,!TempDir.SetSize((Temp-TempDir.GetArray())+1,true),RETURN_BOOL)
            TempDir.GetArray()[TempDir.GetSize()-1] = '\0';
            Dir.Swap(TempDir);
            return true;
        }

        inline const unsigned short CountLevelsDir() const { return CountSeperators(Dir); }
        inline const unsigned short CountLevelsFullDir() const { return CountSeperators(FullDir); }

        //This assumes both Dir and FullDir are in similar states to one another
        //If Dir is some other directory (somehow? This shouldn't happen) it will form an erroneous array
        const bool MoveForward(const SIZE_TYPE N = 1)
        {
            ERRORCLASSFUNCTION(Directory,MoveForward,N < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveForward,!FullDir.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveForward,!Dir.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveForward,!IsValidDirectory(Dir),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveForward,!IsValidDirectory(FullDir),RETURN_BOOL)

            if(CountSeperators(Dir) == CountSeperators(FullDir)){ return false; }

            SIZE_TYPE Iter = (Dir.GetSize()-1);
            while(!IsCompareChar(Dir[Iter],"\\/") && (Iter != 0) ){ --Iter; }
            ERRORCLASSFUNCTION(Directory,MoveForward,!IsCompareChar(Dir[Iter],"\\/"),RETURN_BOOL)
            Iter++;

            SIZE_TYPE Iter2 = 0;

            while(Iter != FullDir.GetSize())
            {
                if(IsCompareChar(FullDir[Iter],"\\/"))
                {
                    Iter2++;
                }
                if(Iter2 == N){break;}
                Iter++;
            }

            ERRORCLASSFUNCTION(Directory,MoveForward,Iter2 != N,RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,MoveForward,!IsCompareChar(FullDir[Iter],"\\/"),RETURN_BOOL)
            CharArray Temp;
            Iter++;
            ERRORCLASSFUNCTION(Directory,MoveForward,!Temp.SetArray(FullDir.GetArray(),Iter),RETURN_BOOL)
            Temp[Iter] = '\0';
            Dir.Swap(Temp);
            return true;
        }

        const bool Append(const CharArray &Array)
        {
            ERRORCLASSFUNCTION(Directory,Append,!Array.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,Append,!IsValidDirectory(Dir),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,Append,!IsValidSubDirectory(Array),RETURN_BOOL)
            CharArray Temp;

            ERRORCLASSFUNCTION(Directory,Append,!Temp.SetArray(Array),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,Append,!ClearDuplicateSeperators(Temp),RETURN_BOOL)
            if(IsCompareChar(Temp[0],"/\\"))
            {
                ERRORCLASSFUNCTION(Directory,Append,!Temp.Remove(Temp[0]),RETURN_BOOL)
            }

            const char * C_Ptr = Temp.GetArray()+(Temp.GetSize()-1);

            while(C_Ptr != Temp.GetArray() && IsCompareChar(*C_Ptr,"\0",false,true))
            {
                --C_Ptr;
            }
            ERRORCLASSFUNCTION(Directory,Append,C_Ptr == Temp.GetArray(),RETURN_BOOL)
            if(!IsCompareChar(*C_Ptr,"/\\"))
            {
                ERRORCLASSFUNCTION(Directory,Append,!Temp.Append('\\'),RETURN_BOOL)
            }
            CharArray Temp2;

            ERRORCLASSFUNCTION(Directory,Append,!Temp2.SetArray(Dir),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,Append,!Temp2.Append(Temp),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,Append,!Temp.SetArray(Temp2),RETURN_BOOL)

            FullDir.Swap(Temp);
            Dir.Swap(Temp2);
            return true;
        }

        const CharArray AppendFileName(const CharArray &FileName) const
        {
            ERRORCLASSFUNCTION(Directory,AppendFileName,!FileName.IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(Directory,AppendFileName,!IsValid(),return CharArray();)
            CharArray Temp;

            ERRORCLASSFUNCTION(Directory,AppendFileName,!Temp.SetArray(Dir),return CharArray();)
            ERRORCLASSFUNCTION(Directory,AppendFileName,!Temp.Append(FileName),return CharArray();)
            return Temp;
        }

        Directory(const Directory &ItemCopy)
        {
            ERRORCLASSFUNCTION(Directory,Directory,!CopyDirectory(ItemCopy),)
            return;
        }

        Directory(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(Directory,Directory,!SetDir(ItemCopy),)
            return;
        }

        Directory(const char Array[])
        {
            ERRORCLASSFUNCTION(Directory,Directory,!SetDir(Array),)
            return;
        }

		Directory(){Open();}
		~Directory(){Close();}

		const bool SetDir(const char Array[])
		{
		    return SetDir(CharArray(Array));
		}

        const bool SetDir(const CharArray &NewDir)
        {
            ERRORCLASSFUNCTION(Directory,SetDir,!IsValidDirectory(NewDir),RETURN_BOOL)

            CharArray TempDir;
            ERRORCLASSFUNCTION(Directory,SetDir,!TempDir.SetArray(NewDir.GetArray()),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,SetDir,!ClearDuplicateSeperators(TempDir),RETURN_BOOL)
            if((TempDir.GetArray()[TempDir.GetSize()-1] != '\\') || (TempDir.GetArray()[TempDir.GetSize()-1] != '/'))
            {
                ERRORCLASSFUNCTION(Directory,SetDir,!TempDir.Append('/'),RETURN_BOOL)
            }

            CharArray TempDir2;
            ERRORCLASSFUNCTION(Directory,SetDir,!TempDir2.SetArray(TempDir),RETURN_BOOL)
            FullDir.Swap(TempDir);
            Dir.Swap(TempDir2);
            return true;
        }

        const bool SetDir(const Directory &NewDir)
        {
            Directory New;
            ERRORCLASSFUNCTION(Directory,SetDir,!New.Dir.SetArray(NewDir.Dir),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,SetDir,!New.FullDir.SetArray(NewDir.FullDir),RETURN_BOOL)
            Dir.TransferFrom(New.Dir);
            FullDir.TransferFrom(New.FullDir);
            return true;
        }

        const bool SetDirArgV(const CharArray &ArgV)
        {
            ERRORCLASSFUNCTION(Directory,SetDirArgV,!IsValidDirectory(ArgV),RETURN_BOOL)
            const char * Temp = (ArgV.GetArray()+strlen(ArgV.GetArray()));
            while((Temp > ArgV.GetArray()) && !IsCompareChar(*Temp,"\\/")){--Temp;}
            ERRORCLASSFUNCTION(Directory,SetDirArgV,!IsCompareChar(*Temp,"\\/"),RETURN_BOOL)
            ++Temp;
            CharArray TempDir;
            ERRORCLASSFUNCTION(Directory,SetDirArgV,!TempDir.SetArray(ArgV.GetArray(),(Temp-ArgV.GetArray())),RETURN_BOOL)
            TempDir.GetArray()[TempDir.GetSize()-1] = '\0';

            ERRORCLASSFUNCTION(Directory,SetDirArgV,!ClearDuplicateSeperators(TempDir),RETURN_BOOL)

            CharArray TempDir2;
            ERRORCLASSFUNCTION(Directory,SetDirArgV,!TempDir2.SetArray(TempDir),RETURN_BOOL)
            FullDir.Swap(TempDir);
            Dir.Swap(TempDir2);
            return true;
        }

        const bool MakeAllCurrDir() const
        {
            ERRORCLASSFUNCTION(Directory,MakeCurrDir,!IsValid(),RETURN_BOOL)
            if(CountLevelsFullDir() == 1){return true;}
            Directory CD;
            ERRORCLASSFUNCTION(Directory,MakeAllCurrDir,!CD.SetDir(*this),RETURN_BOOL);
            ERRORCLASSFUNCTION(Directory,MakeAllCurrDir,!CD.MoveTo(2),RETURN_BOOL)

            unsigned short Count = 2, CountEnd = CountLevelsDir();
            do
            {
                ERRORCLASSFUNCTION(Directory,MakeAllCurrDir,!CD.MakeCurrDir(),RETURN_BOOL)
                Count++;
                if(Count > CountEnd){return true;}
                ERRORCLASSFUNCTION(Directory,MakeAllCurrDir,!CD.MoveForward(1),RETURN_BOOL)
            }while(1);
        }

		const bool MakeCurrDir() const
		{
		    ERRORCLASSFUNCTION(Directory,MakeCurrDir,!IsValid(),RETURN_BOOL)
		    
		    #ifdef IS_LINUX
		    if(mkdir(Dir.GetArray(),S_IRWXU) != 0)
		    #else
		    if(mkdir(Dir.GetArray()) != 0)
		    #endif
		    {
                ERRORCLASSFUNCTION(Directory,MakeCurrDir,errno != EEXIST,RETURN_BOOL)
		    }
		    return true;
		}

		const bool RemoveCurrDir() const
		{
		    ERRORCLASSFUNCTION(Directory,RemoveCurrDir,!IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(Directory,RemoveCurrDir,rmdir(Dir.GetArray()) != 0,RETURN_BOOL)
		    return true;
		}

        const CharArray &GetDir() const {return Dir;}
        const CharArray &GetFullDir() const {return FullDir;}

        void TransferFrom(Directory &Item)
        {
            FullDir.TransferFrom(Item.FullDir);
            Dir.TransferFrom(Item.Dir);
        }

        void Swap(Directory &Item)
        {
            FullDir.Swap(Item.FullDir);
            Dir.Swap(Item.Dir);
        }

        const bool CopyDirectory(const Directory &ItemCopy)
        {
            CharArray TempDir, TempDir2;
            ERRORCLASSFUNCTION(Directory,CopyDirectory,!TempDir.SetArray(ItemCopy.Dir),RETURN_BOOL)
            ERRORCLASSFUNCTION(Directory,CopyDirectory,!TempDir2.SetArray(ItemCopy.FullDir),RETURN_BOOL)
            Dir.Swap(TempDir);
            FullDir.Swap(TempDir2);
            return true;
        }

        const bool operator!=(const Directory &ItemCopy) const{ return ( (Dir != ItemCopy.Dir) || (FullDir != ItemCopy.FullDir) ); }
        const bool operator==(const Directory &ItemCopy) const { return ((Dir == ItemCopy.Dir) && (FullDir == ItemCopy.FullDir) ) ; }
        Directory &operator=(const Directory &ItemCopy)
        {
            ERRORCLASSFUNCTION(Directory,operator=,!CopyDirectory(ItemCopy),RETURN_THIS)
            return *this;
        }

        Directory &operator=(const char ItemCopy[])
        {
            ERRORCLASSFUNCTION(Directory,operator=,!SetDir(ItemCopy),RETURN_THIS)
            return *this;
        }
};

}

#undef DIR_ALPHABET
#endif
