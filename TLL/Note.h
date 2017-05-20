#ifndef G_NOTE_H
#define G_NOTE_H

/*
Generated on: 2013-01-17-15.34.41

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Audio.hpp>
#include "ErrorMessage.h"
#include "FileProc.h"
#include "CharArray.h"
#include "TemplateArray.h"
#include "Maths.h"
#include "FlagOps.h"

#define TWELTFHROOTOFTWO 1.05946
#define INCREASENOTE(X) (X*TWELTFHROOTOFTWO)
#define DECREASENOTE(X) (X/TWELTFHROOTOFTWO)
#define REALNOTETOSFMLNOTE(A) (A/440.0)
#define SFMLNOTETOREALNOTE(A) (440*A)

#define NOTETABLESIZE 88
#define STARTINGNOTE 49
#define STARTINGPITCH 440

namespace TL
{

const bool IsNote(const char Array[])
{
    ERRORFUNCTION(IsNote,!Array,RETURN_BOOL)
    SIZE_TYPE Length = strlen(Array);
    if(Length < 2 || Length > 3){return false;}
    if(CompareList(Array[0],"ABCDEFGabcdefg",14))
    {
        if(Length == 2)
        {
            if(CompareList(Array[1],"1234567",7))
            {
                return true;
            }

            if(Array[1] == '8')
            {
                if(Array[0] == 'C'){return true;}
            }

            if(Array[1] == '0')
            {
                if(CompareList(Array[0],"ABab",4))
                {
                    return true;
                }
            }
        }
        else
        {
            if(Array[1] == '#')
            {
                if(CompareList(Array[2],"1234567",7))
                {
                    if(!CompareList(Array[0],"BEbe",4)){return true;}
                }

                if(Array[2] == '0')
                {
                    if(CompareList(Array[0],"Aa",2))
                    {
                        return true;
                    }
                }
            }
        }
    }
}

const bool IsNoteHigher(const char Array[], const char To[])
{
    ERRORFUNCTION(IsNoteHigher,!Array,RETURN_BOOL)
    ERRORFUNCTION(IsNoteHigher,!To,RETURN_BOOL)
    ERRORFUNCTION(IsNoteHigher,!IsNote(Array),RETURN_BOOL)
    ERRORFUNCTION(IsNoteHigher,!IsNote(To),RETURN_BOOL)

    uint8_t Iter = strlen(Array)-1, Iter2 = strlen(To)-1;

    if(Array[Iter] > To[Iter2]){ return true; }
    if(Array[Iter] < To[Iter2]){return false;}
    if(Array[0] > To[0]){return true;}
    if(Array[0] < To[0]){return false;}
    if(Array[1] == '#' && To[1] != '#'){return true;}
    if(Array[1] != '#' && To[1] == '#'){return false;}
    return false;
}

class NoteTable
{
    protected:
        TemplateArray<float> Table;
        TemplateArray<CharArray> Char;

        void Clear(){Table.Open();}
        const bool PopulateTable(const short NoteTableSize, const short StartingNote, const short StartingPitch)
        {
            ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Table.SetSize(NoteTableSize),RETURN_BOOL)
            ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Char.SetSize(NoteTableSize),RETURN_BOOL)

            TemplateArray<CharArray> Temp;
            ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Temp.SetSize(12),RETURN_BOOL)

            if(!Temp[(StartingNote)%12].SetArray("A"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+1)%12].SetArray("A#"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+2)%12].SetArray("B"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+3)%12].SetArray("C"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+4)%12].SetArray("C#"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+5)%12].SetArray("D"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+6)%12].SetArray("D#"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+7)%12].SetArray("E"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+8)%12].SetArray("F"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+9)%12].SetArray("F#"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }

            if(!Temp[(StartingNote+10)%12].SetArray("G"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }
            if(!Temp[(StartingNote+11)%12].SetArray("G#"))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }

            Table[StartingNote-1] = StartingPitch;

            if(!Char[StartingNote-1].SetArray(Temp[StartingNote%12]))
            {
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
            }

            ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Char[StartingNote-1].Append( '0'+ ((StartingNote+8)/12) ),RETURN_BOOL)

            short Iter = StartingNote-1;

            while(Iter > 0)
            {
                Table[Iter-1] = DECREASENOTE(Table[Iter]);
                if(!Char[Iter-1].SetArray(Temp[Iter%12]))
                {
                    ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
                }

                ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Char[Iter-1].Append( '0'+ ((Iter+8)/12) ),RETURN_BOOL)
                Iter--;
            }

            Iter = StartingNote-1;
            while(Iter < (NoteTableSize-1) )
            {
                Table[Iter+1] = INCREASENOTE(Table[Iter]);
                if(!Char[Iter+1].SetArray(Temp[(Iter+2)%12]))
                {
                    ERRORCLASSFUNCTION(NoteTable,PopulateTable,true,RETURN_BOOL)
                }
                ERRORCLASSFUNCTION(NoteTable,PopulateTable,!Char[Iter+1].Append( '0'+ ((Iter+2+8)/12) ),RETURN_BOOL)
                Iter++;
            }

            return true;
        }
    public:

        NoteTable(){Open();}
        ~NoteTable(){Close();}

        void Open(){Clear(); Table.Open(); Char.Open(); PopulateTable(NOTETABLESIZE,STARTINGNOTE,STARTINGPITCH);}
        void Close(){Table.Close(); Char.Close();}
        void Reset(){Close(); Open();}

        const bool IsValid()
        {
            return Table.IsValid() && Char.IsValid() && (Char.GetSize() == 88) && (Table.GetSize() == 88);
        }

        const bool IsEmpty()
        {
            return Table.IsEmpty() && Char.IsEmpty();
        }

        const TemplateArray<float> GetFrequencyPitchTable() const {return Table;}
        const TemplateArray<CharArray> GetFrequencyTypeTable() const {return Char;}

        const bool IsValidType(const char Array[]) const
        {
            ERRORCLASSFUNCTION(NoteTable,IsValidType,!Table.IsValid(),RETURN_BOOL)
            SIZE_TYPE Iter = 0;
            while(Iter < Table.GetSize())
            {
                if(Compare(Char[Iter].GetArray(),Array,true)){return true;}
                Iter++;
            }
            return false;
        }

        const bool IsValidPitch(const float Pitch) const
        {
            ERRORCLASSFUNCTION(NoteTable,IsValidPitch,!Char.IsValid(),RETURN_BOOL)
            if((int)Pitch == (int)Table[0]){return true;}
            if((int)Pitch == (int)Table[Table.GetSize()-1]){return true;}

            SIZE_TYPE Iter = 0;
            while(Iter < Table.GetSize())
            {
                if((int)Pitch == (int)Table[Iter])
                {
                    return true;
                }
                Iter++;
            }
            ERRORCLASSFUNCTION(NoteTable,GetType,true,RETURN_BOOL)
        }

        const float GetPitch(const CharArray Type) const
        {
            SIZE_TYPE Iter = 0;
            while(Iter < Char.GetSize())
            {
                if(Char[Iter] == Type){return Table[Iter];}
                Iter++;
            }
            printf("%s\n",Type.GetArray());
            ERRORCLASSFUNCTION(NoteTable,GetPitch,true,return 0.0;)
        }

        //This only provides the 'closest match' due to float inaccuraties
        const CharArray GetType(const float Pitch) const
        {
            ERRORCLASSFUNCTION(NoteTable,GetType,!Char.IsValid(),return CharArray();)

            int Temp = RoundToInt(Pitch); //To reduce the number of calls to round

            if(Temp == RoundToInt(Table[0])){return Char[0];}
            if(Temp == RoundToInt(Table[Table.GetSize()-1])){return Char[Char.GetSize()-1];}

            SIZE_TYPE Iter = 0;
            while(Iter < Table.GetSize())
            {
                if(Temp == RoundToInt(Table[Iter]))
                {
                    return Char[Iter];
                }
                Iter++;
            }
            ERRORCLASSFUNCTION(NoteTable,GetType,true,printf("%f Not found!\n",Pitch);return CharArray();)
        }

        /*
        const uint8_t NoteTypeToByte(const CharArray &Type)
        {
            ERRORCLASSFUNCTION(NoteTable,NoteTypeToByte,!Type.IsValid(),return 0;)
            ERRORCLASSFUNCTION(NoteTable,NoteTypeToByte,!IsValidType(Type.GetArray()),return 0;)

            uint8_t Temp;
            if(Type.GetSize() == 3)
            {
                //Number (4), Letter (3), Hashtype (1)
                Temp = ((uint8_t) Type[1] - '0');
                Temp = Temp<<4;
                if(isupper(Type[0]))
                {
                    Temp = Temp | ((uint8_t));
                }
                else
                {

                }

            }
            else if(Type.GetSize() == 4)
            {

            }
            return 1;
        }
        */

        void TransferFrom(NoteTable &Temp)
        {
            Table.TransferFrom(Temp.Table);
            Char.TransferFrom(Temp.Char);
        }
};

extern NoteTable NoteTable_G;
NoteTable NoteTable_G;

class Note
{
    protected:
        sf::SoundBuffer Buffer;
        sf::Sound Sound;

        void Clear(){Buffer = sf::SoundBuffer(); Sound = sf::Sound();}

    public:
        Note(){Open();}
        Note(const std::string &File)
        {
            ERRORCLASSFUNCTION(Note,Note,!OpenFromFile(File),)
        }
        ~Note(){Close();}



        void Close(){Clear();}
        void Open(){Clear();}
        void Reset(){Close();}

        const sf::Sound &GetSound() const {return Sound;}

        const bool OpenFromFile(const std::string &File, const bool RetainPitch = false)
        {
            ERRORCLASSFUNCTION(Note,OpenFromFile,!FileExists(File.c_str()),RETURN_BOOL)
            Buffer = sf::SoundBuffer();
            ERRORCLASSFUNCTION(Note,OpenFromFile,!Buffer.LoadFromFile(File),RETURN_BOOL)
            Sound.SetBuffer(Buffer);
            if(!RetainPitch){Sound.SetPitch(1.0);}
            return true;
        }

        void IncreaseNote()
        {
            Sound.SetPitch(REALNOTETOSFMLNOTE(INCREASENOTE(SFMLNOTETOREALNOTE(Sound.GetPitch()))));
        }

        void DecreaseNote()
        {
            Sound.SetPitch(REALNOTETOSFMLNOTE(DECREASENOTE(SFMLNOTETOREALNOTE(Sound.GetPitch()))));
        }

        const bool IncreaseNoteBy(const short Temp)
        {
            ERRORCLASSFUNCTION(Note,IncreaseNoteBy,Temp < 1,RETURN_BOOL)
            short Iter = Temp;
            while(Iter > 0)
            {
                IncreaseNote();
                Iter--;
            }
            return true;
        }

        const bool DecreaseNoteBy(const short Temp)
        {
            ERRORCLASSFUNCTION(Note,DecreaseNoteBy,Temp < 1,RETURN_BOOL)
            short Iter = Temp;
            while(Iter > 0)
            {
                DecreaseNote();
                Iter--;
            }
            return true;
        }

        void SetVolume(const float Temp)
        {
            Sound.SetVolume(Temp);
        }

        const float SetPitchReal(const float Temp)
        {
            Sound.SetPitch(REALNOTETOSFMLNOTE(Temp));
        }

        const float SetPitchSFML(const float Temp)
        {
            Sound.SetPitch(Temp);
        }

        const float GetPitchReal()
        {
            return SFMLNOTETOREALNOTE(Sound.GetPitch());
        }

        const float GetPitchSFML()
        {
            return Sound.GetPitch();
        }

        void SetLoop(const bool Temp)
        {
            Sound.SetLoop(Temp);
        }

        void Play()
        {
            Sound.Stop();
            Sound.Play();
        }

        void Stop()
        {
            Sound.Stop();
        }

        void Pause()
        {
            Sound.Pause();
        }

        void TransferFrom(Note &Temp)
        {
            Buffer = Temp.Buffer;
            Sound = Temp.Sound;
            Sound.SetBuffer(Buffer);
            Temp.Reset();
        }

        Note &operator=(const Note &ItemCopy)
		{
		    Buffer = ItemCopy.Buffer;
		    Sound = ItemCopy.Sound;
		    Sound.SetBuffer(Buffer);
			return *this;
		}
};

}

#endif
