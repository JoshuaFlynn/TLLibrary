#ifndef G_TEMPLATEARRAY_H
#define G_TEMPLATEARRAY_H

/*
Generated on: 2011-10-21-17.48.58

Version: 2.0
*/

/*
Notes:
This is the compressor edition
with modifications to replace, insert
Added parentheses around truth value
Added error message to ReplaceAll
Implemented RemoveAll
Added ReplaceAll support for std::vector
Added AppendAll, upgraded to version 2.0 as it now supports all the planned features
*/

/*
Suggestions for improvements:


*/


#include "ErrorMessage.h"
#include "MemAlloc.h"
#include "CommonFunctions.h"
#include <stdarg.h>

namespace TL
{

template<typename TemplateItem>
class TemplateArray
{
	protected:

        SIZE_TYPE Size; //SIZE_TYPE is used as a macro so what variable type (short, int, long, etc) can be changed for all classes to suit
        TemplateItem *Array;

		void Clear(){Size = 0; Array = NULL;}

		const bool AppendAll(const SIZE_TYPE Total, const TemplateArray<const TemplateItem *> &ItemCopy, const TemplateArray<SIZE_TYPE> &ItemCopy2)
		{
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,Total < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy2.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,ItemCopy.GetSize() != ItemCopy2.GetSize(),RETURN_BOOL)

            TemplateArray<TemplateItem> Temp;
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp.SetSize(Total),RETURN_BOOL)
            SIZE_TYPE Iter = 0;
            TemplateItem * ItemCopy3 = Temp.GetArray();

            while(Iter < ItemCopy.GetSize())
            {
                Copy(ItemCopy3,ItemCopy[Iter],ItemCopy2[Iter]);
                ItemCopy3 = ItemCopy3 + ItemCopy2[Iter];
                Iter++;
            }

            TransferFrom(Temp);
            return true;
		}

	public:

        const bool OverWrite(const SIZE_TYPE Position, const TemplateItem Data[], const SIZE_TYPE S)
		{
		    if(S == 0){return false;}
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,!IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,!Data,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,Position > Size,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,Position < 0,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,S > Size,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,OverWrite,(Position+S) > Size,RETURN_BOOL)
            Copy(Array+Position,Data,S);
            return true;
		}

		const bool OverWrite(const SIZE_TYPE Position, const TemplateItem &ItemCopy)
		{
		    return OverWrite(Position,&ItemCopy,1);
		}

        //TrasnferFrom is an explicit move function as users will not anticipate implicit behaviour
        void TransferFrom(TemplateArray &ItemCopy)
        {
            //The moved variables are moved into temporaries, in order to prevent self-assigment issues
            SIZE_TYPE TempSize = ItemCopy.Size;
            ItemCopy.Size = 0;

            TemplateItem *TempArray = ItemCopy.Array;
            ItemCopy.Array = NULL;

            Close();

            Array = TempArray;
            Size = TempSize;
            return;
        }

        void Swap(TemplateArray &ItemCopy)
        {
            TemplateArray Temp;
            Temp.TransferFrom(ItemCopy);
            ItemCopy.TransferFrom(*this);
            TransferFrom(Temp);
            return;
        }

        //KeepState is defined in case the user doesn't want an expensive coping of the array
        //A default argument is used to keep SetSize as simple as possible
        //It defaults to true as the user will more often want their data intact than not
        const bool SetSize(const SIZE_TYPE S, const bool KeepState = true)
        {
            //Macros like this are used in order to permit a consistent error message that can be easily reused.
            //If the error message layout needs changing, one modifies the macro.
            //Functions do not support this level of flexibility in displaying data and are ergo not used.
            ERRORCLASSFUNCTION(TemplateArray,SetSize,S < 0,RETURN_BOOL)
            //The usage of no size is permitted given the user will likely expect it to close the array
            if(!S)
            {
                Reset();
                return true;
            }
            //Memory is allocated to a temporary so the state of the class is maintained even in the event of failure
            //This also prevents self-assignment issues
            TemplateItem *TempArray = NULL;
            ERRORCLASSFUNCTION(TemplateArray,SetSize,!CREATEB(TempArray,S),RETURN_BOOL)
            if(KeepState)
            {
                if(Array)
                {
                    SIZE_TYPE TempSize = (Size < S) ? Size : S;
                    Copy(TempArray,Array,TempSize);
                }
            }
            Reset();
            //TransferFrom isn't used here for more efficienct operations
            //given that we know the temp variables are a different class
            Array = TempArray;
            TempArray = NULL;
            Size = S;
            return true;
        }

        //Conversion to vector is supplied here for two reasons
        //That a shared data-type can be used across all classes without inheritance problems (less efficient though)
        //and so that programmers with an absolute obession with std can't complain it's incompatible
        const std::vector<TemplateItem> ToVector() const
        {
            return ToVector(Array,Size);
        }

        inline TemplateItem* GetArray()
        {
            return Array;
        }

        inline const TemplateItem* GetArray() const
        {
            return Array;
        }

        inline const SIZE_TYPE GetSize() const
        {
            return Size;
        }

        const bool SetArray(const TemplateItem Data[], const SIZE_TYPE S)
        {
            ERRORCLASSFUNCTION(TemplateArray,SetSize,Data == NULL,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,SetArray,S < 0,RETURN_BOOL)
            //Tempories mean in the event of an error the classes state is consistent
            //and prevents self-assignment issues
            TemplateArray Temp;
            ERRORCLASSFUNCTION(TemplateArray,SetArray,!Temp.SetSize(S,false),RETURN_BOOL)
            Copy(Temp.Array,Data,S);
            TransferFrom(Temp); //Used to keep the function concise
            return true;
        }

        inline const bool SetArray(const TemplateArray &ItemCopy)
        {
            if(!ItemCopy.IsValid())
            {
                Reset();
                return true;
            }
            return SetArray(ItemCopy.Array,ItemCopy.Size);
        }

        inline const bool SetArray(const std::vector<TemplateItem> &ItemCopy)
        {
            //The reason this doesn't simply 'quit out' is because SetArray is supposed to
            //duplicate the state of the class passed to it as closely as possible
            //If the user supplies a dud array, SetArray can only assume they wanted a duplicate dud array
            //Even if this might not make much sense, it makes even less sense a user should even pass a dud array in the first place
            //WYSIWYG
            if(ItemCopy.empty())
            {
                Reset();
                return true;
            }
            return SetArray(&ItemCopy[0],ItemCopy.size());
        }

        inline const bool SetArrayAll(const SIZE_TYPE NumberOfArrays, const TemplateItem ItemCopy[], const SIZE_TYPE ItemCopy2, ...)
        {
            ERRORCLASSFUNCTION(TemplateArray,SetArrayAll,NumberOfArrays < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,SetArrayAll,!ItemCopy,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,SetArrayAll,ItemCopy2 < 1,RETURN_BOOL)

            TemplateArray<const TemplateItem *> Temp;
            TemplateArray<SIZE_TYPE> Temp1;
            va_list valist;

            SIZE_TYPE Iter = 0;
            SIZE_TYPE Total = ItemCopy2;

            Temp[Iter] = ItemCopy;
            Temp1[Iter] = ItemCopy2;
            Iter = Iter + 1;

            while(Iter < NumberOfArrays)
            {
                Temp[Iter] = va_arg(valist,const TemplateItem *);
                ERRORCLASSFUNCTION(TemplateArray,SetArrayAll,!Temp[Iter],RETURN_BOOL)
                Temp1[Iter] = va_arg(valist,SIZE_TYPE);
                ERRORCLASSFUNCTION(TemplateArray,SetArrayAll,!Temp1[Iter],RETURN_BOOL)
                Total = Total + Temp1[Iter];
                Iter++;
            }

            return AppendAll(Total,Temp,Temp1);
        }

        //Consider an event where Size is not zero but Array is NULL
        //Although this should never happen, one cannot assume anything
		inline const bool IsEmpty() const { return ((Size == 0)&&(!Array)); }
		//IsValid is REQUIRED, notably in the cases of malloc where the internal state is not defined, and in-case of user tampering
		//Also, this is to ensure consistency with more complex classes where a non-empty class may not necessarily be valid
		inline const bool IsValid() const { return ((Size > 0)&&(Array)); }

		const bool operator!() const {return IsEmpty();}
		operator const bool () const {return IsValid();} //This has to be defined in order to permit if(Class) operations

        //Malloc friendly functions
        //It has been noted that these functions offer potential bugs (Open causing leak, Close causing double-free esque crash etc)
        //Whilst this is true, the functions are exempted, given that:
        //A garbage filled class is potentially indistinguishable from a valid class to Close
        //A valid class is indistinguishable from a garbage filled class to open
        //Therefore these functions are at the (advanced) user's discretion, and generally should not needed to be used
        //These should normally be public but to likely prevent accidental abuse, made non-public
		void Open(){ Clear(); }
		void Close()
		{
            if(Array)
            {
                ERRORCLASSFUNCTION(TemplateArray,Close,!DELETEB(Array),)
            }
			Clear();
		}
		void Reset(){ Close(); }

        TemplateArray(const TemplateItem Data[], const SIZE_TYPE S){Open(); ERRORCLASSFUNCTION(TemplateArray,TemplateArray,!SetArray(Data,S),) }
		TemplateArray(const TemplateArray &ItemCopy){Open(); ERRORCLASSFUNCTION(TemplateArray,TemplateArray,!SetArray(ItemCopy),) }
		TemplateArray(const std::vector<TemplateItem> &ItemCopy){Open(); ERRORCLASSFUNCTION(TemplateArray,TemplateArray,!SetArray(ItemCopy),)}
		TemplateArray(){Open();}
		~TemplateArray(){Close();}

        //The reason for these verbose versions of operators is twofold:
        //One, several programmers complained a class should not rely on implicit operations (EG operator=) as what they might do is ambiguous (apparently)
        //and suggested there be verbose functions that can be called that describe what they do
        //Two, because these functions return const bool allowing them to easily denote error/failure
        //which cannot be done with a function that returns *this except by unnecessarily complicated methodologies
        //The const bool support is also to permit 'function stacking' for the more complicated classes later on
        //It also permits that later classes can enforce the 'quit on first error' rule
        const bool Remove(const TemplateItem Data[], const SIZE_TYPE S, const SIZE_TYPE Position = 0)
        {
            ERRORCLASSFUNCTION(TemplateArray,Remove,!IsValidArray(Data,S),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,Remove,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,Remove,S > Size,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,Remove,Position < 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,Remove,Position >= Size,RETURN_BOOL)
            const TemplateItem *RemoveStart = Find(Array+Position,Size-Position,Data,S,false);
            if(RemoveStart == NULL){return false;}
            const TemplateItem *RemoveEnd = RemoveStart+(S-1);
            if(S == Size)
            {
                Reset();
                return true;
            }
            TemplateArray Temp;
            ERRORCLASSFUNCTION(TemplateArray,Remove,!Temp.SetSize(Size-S,false),RETURN_BOOL)
            const TemplateItem *Start = Array, *End = Array+Size;
            TemplateItem *Temp2 = Temp.Array;
            while(Start != End)
            {
                if((Start < RemoveStart) || (Start > RemoveEnd))
                {
                    *Temp2 = *Start;
                    Temp2++;
                }
                ++Start;
            }
            TransferFrom(Temp);
            return true;
        }

        inline const bool Remove(const TemplateItem &ItemCopy)
        {
            return Remove(&ItemCopy,1);
        }

		inline const bool Remove(const TemplateArray &ItemCopy)
		{
			return Remove(ItemCopy.Array,ItemCopy.Size);
		}

		inline const bool Remove(const std::vector<TemplateItem> &ItemCopy)
		{
		    if(ItemCopy.empty()){return false;}
		    else{return Remove(&ItemCopy[0],ItemCopy.size());}
		}

		const bool RemoveAll(const TemplateItem Target[], const SIZE_TYPE T1)
        {
            ERRORCLASSFUNCTION(TemplateArray,RemoveAll,!IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,RemoveAll,!IsValidArray(Target,T1),RETURN_BOOL)
		    TemplateItem *TempStart = Find(Array,Size,Target,T1,false);
		    if(TempStart == NULL){return false;}
		    TempStart = TempStart + T1;

            SIZE_TYPE NumberOfOccurrences = 1;
            while( (TempStart = Find(TempStart,Array+Size,Target,T1,false)) )
            {
                TempStart = TempStart + T1;
                NumberOfOccurrences++;
            }
            TemplateArray<TemplateItem> Updated;

            ERRORCLASSFUNCTION(TemplateArray,RemoveAll,!Updated.SetSize( (Size-(T1*NumberOfOccurrences))),RETURN_BOOL)

            TempStart = Array;
            TemplateItem *UpdatedStart = Updated.Array;
            TemplateItem *ArrayStart = Array;
            while(NumberOfOccurrences > 0)
            {
                //Find target
                TempStart = Find(TempStart,Array+Size,Target,T1,false);
                ERRORCLASSFUNCTION(TemplateArray,RemoveAll,!TempStart,RETURN_BOOL)
                //Copy all characters up to target from Array
                Copy(UpdatedStart,ArrayStart,TempStart); //Maybe -1, who knows?
                //Update position based on copied number of characters
                UpdatedStart = UpdatedStart + (TempStart-ArrayStart);
                //Update TempStart based on target length
                TempStart = TempStart + T1;
                //Update ArrayStart to end of previous target
                ArrayStart = TempStart;
                //Repeat!
                NumberOfOccurrences--;
            }

            if(ArrayStart != (Array+Size))
            {
                Copy(UpdatedStart,ArrayStart,Array+Size);
            }
            TransferFrom(Updated);
		    return true;
        }

        const bool RemoveAll(const TemplateArray &Target)
        {
            return RemoveAll(Target.GetArray(),Target.GetSize());
        }

        const bool Insert(const SIZE_TYPE &Position, const TemplateItem Data[], const SIZE_TYPE S)
		{
            if(!IsValid())
		    {
		        ERRORCLASSFUNCTION(TemplateArray,Insert,Position != 0,RETURN_BOOL)
                return SetArray(Data,S);
		    }

		    ERRORCLASSFUNCTION(TemplateArray,Insert,Position < 0,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Insert,Position > Size,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Insert,!IsValidArray(Data,S),RETURN_BOOL)
            TemplateArray Temp;
            ERRORCLASSFUNCTION(TemplateArray,Insert,!Temp.SetSize(Size+S),RETURN_BOOL)
            Copy(Temp.GetArray(),Array,Position);
            Copy(Temp.GetArray()+Position,Data,S);
            Copy(Temp.GetArray()+Position+S,Array+Position,Size-Position);
            TransferFrom(Temp);
            return true;
		}

		inline const bool Insert(const SIZE_TYPE &Position, const TemplateItem &Data)
		{
		    return Insert(Position,&Data,1);
		}

		inline const bool Insert(const SIZE_TYPE &Position, const std::vector<TemplateItem> &ItemCopy)
		{
		    ERRORCLASSFUNCTION(CharArray,Insert,ItemCopy.empty(),RETURN_BOOL)
		    return Insert(Position,&ItemCopy[0],ItemCopy.size());
		}
		inline const bool Insert(const SIZE_TYPE &Position, const TemplateArray &ItemCopy)
		{
		    return Insert(Position,ItemCopy.Array,ItemCopy.Size);
		}

		const bool Replace(const TemplateItem Target[], const SIZE_TYPE S1, const TemplateItem With[], const SIZE_TYPE S2, const SIZE_TYPE Position = 0)
		{
		    ERRORCLASSFUNCTION(TemplateArray,Replace,!IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Replace,!IsValidArray(Target,S1),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Replace,!With,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Replace,S2 < 0,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Replace,Position < 0,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,Replace,Position >= Size,RETURN_BOOL)
		    const TemplateItem *TempStart = Find(Array+Position,Size-Position,Target,S1,false);
		    if(TempStart == NULL){return false;}
		    if(S1 == S2)
		    {
		        ERRORCLASSFUNCTION(TemplateArray,Replace,!OverWrite(TempStart-Array,With,S2),RETURN_BOOL)
		    }
		    else
		    {
		        const SIZE_TYPE TempSize = TempStart-Array;
		        TemplateArray Temp;

		        ERRORCLASSFUNCTION(TemplateArray,Replace,!Temp.SetSize(Size-S1+S2), RETURN_BOOL)
                Temp.OverWrite(0,Array,TempSize);
                Temp.OverWrite(TempSize,With,S2);
                Temp.OverWrite(TempSize+S2,TempStart+S1,Size-(S1+TempSize));
                TransferFrom(Temp);
		    }
		    return true;
		}

		inline const bool Replace(const TemplateItem Target[], const SIZE_TYPE S1, const TemplateItem &With, const SIZE_TYPE Position = 0)
		{
		    return Replace(Target,S1,&With,1,Position);
		}

		inline const bool Replace(const TemplateItem &Target, const TemplateItem &With, const SIZE_TYPE Position = 0)
		{
		    return Replace(&Target,1,&With,1,Position);
		}

		inline const bool Replace(const TemplateArray &Target, const TemplateArray &With, const SIZE_TYPE Position = 0)
		{
		    return Replace(Target.GetArray(),Target.GetSize(),With.GetArray(),With.GetSize(),Position);
		}

		inline const bool Replace(const TemplateArray &Target, const TemplateItem &With, const SIZE_TYPE Position = 0)
		{
		    return Replace(Target.GetArray(),Target.GetSize(),&With,1,Position);
		}

		inline const bool Replace(const TemplateItem &Target, const TemplateItem With[], const SIZE_TYPE End, const SIZE_TYPE Position = 0)
		{
		    return Replace(&Target,1,With,End,Position);
        }

        //This class is designed for repeated calls to Replace, as Repeat is resource intensive when done individually
		const bool ReplaceAll(const TemplateItem Target[], const SIZE_TYPE T1, const TemplateItem With[], const SIZE_TYPE W1)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!IsValid(),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!IsValidArray(Target,T1),RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!With,RETURN_BOOL)
		    TemplateItem *TempStart = Find(Array,Size,Target,T1,false);
		    if(TempStart == NULL){return false;}
		    TempStart = TempStart + T1;

            SIZE_TYPE NumberOfOccurrences = 1;
            while( (TempStart = Find(TempStart,Array+Size,Target,T1,false)) )
            {
                TempStart = TempStart + T1;
                NumberOfOccurrences++;
            }
            TemplateArray<TemplateItem> Updated;

            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!Updated.SetSize( (Size-(T1*NumberOfOccurrences))+(W1*NumberOfOccurrences) ),RETURN_BOOL)

            TempStart = Array;
            TemplateItem *UpdatedStart = Updated.Array;
            TemplateItem *ArrayStart = Array;
            while(NumberOfOccurrences > 0)
            {
                //Find target
                TempStart = Find(TempStart,Array+Size,Target,T1,false);
                ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!TempStart,RETURN_BOOL)
                //Copy all characters up to target from Array
                Copy(UpdatedStart,ArrayStart,TempStart); //Maybe -1, who knows?
                //Update position based on copied number of characters
                UpdatedStart = UpdatedStart + (TempStart-ArrayStart);
                //Update TempStart based on target length
                TempStart = TempStart + T1;
                //Copy in replacement string into Updated array
                Copy(UpdatedStart,With,W1);
                //Move UpdatedStart past W1
                UpdatedStart = UpdatedStart + W1;
                //Update ArrayStart to end of previous target
                ArrayStart = TempStart;
                //Repeat!
                NumberOfOccurrences--;
            }

            if(ArrayStart != (Array+Size))
            {
                Copy(UpdatedStart,ArrayStart,Array+Size);
            }
            TransferFrom(Updated);
		    return true;
        }

        inline const bool ReplaceAll(const TemplateArray &Target, const TemplateArray &With)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!Target.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!With.IsValid(),RETURN_BOOL)
            return ReplaceAll(Target.Array,Target.Size,With.Array,With.Size);
        }

        inline const bool ReplaceAll(const char Target, const char With[], const SIZE_TYPE W1)
        {
            return ReplaceAll(&Target,1,With,W1);
        }

        inline const bool ReplaceAll(const TemplateArray &Target, const char With)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!Target.IsValid(),RETURN_BOOL)
            return ReplaceAll(Target.Array,Target.Size,&With,1);
        }

        inline const bool ReplaceAll(const std::vector<TemplateItem> &Target, const std::vector<TemplateItem> &With)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,Target.empty(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,With.empty(),RETURN_BOOL)
            return ReplaceAll(&Target[0],Target.size(),&With[0],With.size());
        }

        inline const bool ReplaceAll(const TemplateArray &Target, const std::vector<TemplateItem> &With)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!Target.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,With.empty(),RETURN_BOOL)
            return ReplaceAll(Target.GetArray(),Target.GetSize(),&With[0],With.size());
        }

        inline const bool ReplaceAll(const std::vector<TemplateItem> &Target, const TemplateArray &With)
        {
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,Target.empty(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,ReplaceAll,!With.IsValid(),RETURN_BOOL)
            return ReplaceAll(&Target[0],Target.size(),With.GetArray(),With.GetSize());
        }

        //It was complained about that Append/TemplateArray does not make use of reserve memory allocation for 'more efficient' operations
        //The justified reasoning of it not making use of reserved memory allocation is thusly:
        //If the user is making use of Append that frequently it needs to be made like that, they likely have the wrong class
        //Reserve memory allocation adds additional complexity to the class for a very small user base (who have the wrong class)
        //Reserving additional unused memory (especially for images, it's intended role) adds a huge overhead (imagine 'just' reserving 2MB * 10 for a few textures!)
        //This is not a std::vector duplicate, so please don't try to make it into one
        //The whole point of the TemplateArray class was to omit the reserve memory allocation in the first place for memory efficiency, and
        //the user should punished and not accomadated for if they making such bad efficiency choices
        inline const bool Append(const TemplateItem Data[], const SIZE_TYPE S)
        {
            return Insert(Size,Data,S);
        }

        inline const bool Append(const TemplateItem &ItemCopy)
        {
            return Append(&ItemCopy,1);
        }

		inline const bool Append(const TemplateArray &ItemCopy)
		{
			return Append(ItemCopy.Array,ItemCopy.Size);
		}

		inline const bool Append(const std::vector<TemplateItem> &ItemCopy)
		{
		    if(ItemCopy.empty()){return false;}
		    else{return Append(&ItemCopy[0],ItemCopy.size());}
		}

		//NumberOfArrays counts how many arrays you are passing, and does not count the size-type requirements
		//Format is TemplateItem Array[], SIZE_TYPE size
		//Number of arrays should include the first array
		inline const bool AppendAll(const SIZE_TYPE NumberOfArrays, const TemplateItem ItemCopy[], const SIZE_TYPE ItemCopy2, ...)
		{
		    ERRORCLASSFUNCTION(TemplateArray,AppendAll,NumberOfArrays < 1,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy,RETURN_BOOL)
		    ERRORCLASSFUNCTION(TemplateArray,AppendAll,ItemCopy2 < 1,RETURN_BOOL)

		    SIZE_TYPE Total = Size;

		    TemplateArray<const TemplateItem *> Temp;
		    TemplateArray<SIZE_TYPE> Temp1;

            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp.SetSize(NumberOfArrays+IsValid()),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp1.SetSize(NumberOfArrays+IsValid()),RETURN_BOOL)

            va_list valist;
            va_start(valist,ItemCopy2);

		    SIZE_TYPE Iter = 0;

		    if(IsValid())
		    {
		        Temp[Iter] = Array;
		        Temp1[Iter] = Size;
		        Total = Total + Size;
		    }

		    Iter = IsValid();

		    Temp[Iter] = ItemCopy;
		    Temp1[Iter] = ItemCopy2;
		    Total = Total + ItemCopy2;

		    Iter++;

		    while(Iter < (NumberOfArrays+IsValid()) )
		    {
		        Temp[Iter] = va_arg(valist,const TemplateItem *);
		        ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp[Iter],RETURN_BOOL)
		        Temp1[Iter] = va_arg(valist,SIZE_TYPE);
		        ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp1[Iter],RETURN_BOOL)
		        Total = Total + Temp1[Iter];
		        Iter++;
		    }

            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!AppendAll(Total,Temp,Temp1),RETURN_BOOL)
		    return true;
		}

        const bool AppendAll(const TemplateArray<const TemplateItem *> &ItemCopy, const TemplateArray<SIZE_TYPE> &ItemCopy2)
		{
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy2.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,ItemCopy.GetSize() != ItemCopy2.GetSize(),RETURN_BOOL)

            SIZE_TYPE Total = 0, Iter = 0;

            while(Iter < ItemCopy.GetSize())
            {
                ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy[Iter],RETURN_BOOL)
                ERRORCLASSFUNCTION(TemplateArray,AppendAll,ItemCopy2[Iter] < 1,RETURN_BOOL)
                Total = Total + ItemCopy2[Iter];
                Iter++;
            }

            TemplateArray<TemplateItem> Temp;
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp.SetSize(Total),RETURN_BOOL)

            TemplateItem * ItemCopy3 = Temp.GetArray();

            while(Iter < ItemCopy.GetSize())
            {
                Copy(ItemCopy3,ItemCopy[Iter],ItemCopy2[Iter]);
                ItemCopy3 = ItemCopy3 + ItemCopy2[Iter];
                Iter++;
            }

            TransferFrom(Temp);
            return true;
		}
		
		const bool AppendAll(const TemplateArray<TemplateArray<TemplateItem> > &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy.IsValid(),RETURN_BOOL)

            SIZE_TYPE Total = 0, Iter = 0;

            while(Iter < ItemCopy.GetSize())
            {
                ERRORCLASSFUNCTION(TemplateArray,AppendAll,!ItemCopy[Iter].IsValid(),RETURN_BOOL)
                Total = Total + ItemCopy[Iter].GetSize();
                Iter++;
            }

            TemplateArray<TemplateItem> Temp;
            ERRORCLASSFUNCTION(TemplateArray,AppendAll,!Temp.SetSize(Total),RETURN_BOOL)

            TemplateItem * ItemCopy3 = Temp.GetArray();

            while(Iter < ItemCopy.GetSize())
            {
                Copy(ItemCopy3,ItemCopy[Iter].GetArray(),ItemCopy[Iter].GetSize());
                ItemCopy3 = ItemCopy3 + ItemCopy.GetSize();
                Iter++;
            }

            TransferFrom(Temp);
            return true;
		}

		TemplateArray &operator-=(const TemplateArray &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator-=,!Remove(ItemCopy),RETURN_THIS)
			return *this;
		}

		inline TemplateArray &operator-=(const TemplateItem &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator-=,!Remove(ItemCopy),RETURN_THIS)
			return *this;
		}

		inline TemplateArray &operator-=(const std::vector<TemplateItem> &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator-=,!Remove(ItemCopy),RETURN_THIS)
			return *this;
		}

		TemplateArray &operator+=(const TemplateArray &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator+=,!Append(ItemCopy),RETURN_THIS)
			return *this;
		}

		inline TemplateArray &operator+=(const TemplateItem &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator+=,!Append(ItemCopy),RETURN_THIS)
			return *this;
		}

		inline TemplateArray &operator+=(const std::vector<TemplateItem> &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator+=,!Append(ItemCopy),RETURN_THIS)
			return *this;
		}

		const bool operator==(const TemplateArray &ItemCopy) const
		{
		    if(IsEmpty() && ItemCopy.IsEmpty()){ return true; }
            return Compare(Array,Size,ItemCopy.Array,ItemCopy.Size);
		}

		inline const bool operator==(const std::vector<TemplateItem> &ItemCopy) const
		{
		    if(IsEmpty() && ItemCopy.empty()){return true;}
		    return Compare(Array,Size,&ItemCopy[0],ItemCopy.size());
		}

		inline const bool operator!=(const TemplateArray &ItemCopy) const
		{
            return !(*this == ItemCopy);
		}

		inline const bool operator!=(const std::vector<TemplateItem> &ItemCopy) const
		{
            return !(*this == ItemCopy);
		}

        inline TemplateArray &operator=(const TemplateItem &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator=,!SetArray(&ItemCopy,1),RETURN_THIS)
			return *this;
		}

		TemplateArray &operator=(const TemplateArray &ItemCopy)
		{
            ERRORCLASSFUNCTION(TemplateArray,operator=,!SetArray(ItemCopy),RETURN_THIS)
			return *this;
		}

		inline TemplateItem &operator[](const SIZE_TYPE N)
		{
		    return Array[N];
		}

		inline const TemplateItem &operator[](const SIZE_TYPE N) const
		{
		    return Array[N];
		}
};

template<typename TemplateItem>
TemplateArray<TemplateItem> operator+(const TemplateArray<TemplateItem> &ItemCopy, const TemplateArray<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    SIZE_TYPE TempSize1 = (ItemCopy.GetSize()*ItemCopy.IsValid()), TempSize2 = (ItemCopy2.GetSize()*ItemCopy2.IsValid());
    ERRORCLASSFUNCTION(TemplateArray,operator+,!Temp.SetSize(TempSize1+TempSize2),return Temp;)
    Copy(Temp.GetArray(),ItemCopy.GetArray(),TempSize1);
    Copy(Temp.GetArray()+TempSize1,ItemCopy2.GetArray(),TempSize2);
    return Temp;
}

template<typename TemplateItem>
TemplateArray<TemplateItem> operator+(const TemplateArray<TemplateItem> &ItemCopy, const TemplateItem &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    SIZE_TYPE TempSize = (ItemCopy.GetSize()*ItemCopy.IsValid());
    ERRORFUNCTION(operator+,!Temp.SetSize(TempSize+1),return Temp;)
    Copy(Temp.GetArray(),ItemCopy.GetArray(),TempSize);
    Copy(Temp.GetArray()+TempSize,&ItemCopy2,1);
    return Temp;
}

template<typename TemplateItem>
TemplateArray<TemplateItem> operator+(const TemplateArray<TemplateItem> &ItemCopy, const std::vector<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    SIZE_TYPE TempSize1 = (ItemCopy.GetSize()*ItemCopy.IsValid()), TempSize2 = (ItemCopy2.size()*!ItemCopy2.empty());
    ERRORCLASSFUNCTION(TemplateArray,operator+,!Temp.SetSize(TempSize1+TempSize2),return Temp;)
    Copy(Temp.GetArray(),ItemCopy.GetArray(),TempSize1);
    Copy(Temp.GetArray()+TempSize1,&ItemCopy2[0],TempSize2);
    return Temp;
}

template<typename TemplateItem>
TemplateArray<TemplateItem> operator+(const std::vector<TemplateItem> &ItemCopy, const TemplateArray<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    SIZE_TYPE TempSize1 = (ItemCopy.size()*!ItemCopy.empty()), TempSize2 = (ItemCopy2.GetSize()*ItemCopy2.IsValid());
    ERRORCLASSFUNCTION(TemplateArray,operator+,!Temp.SetSize(TempSize1+TempSize2),return Temp;)
    Copy(Temp.GetArray(),&ItemCopy[0],TempSize1);
    Copy(Temp.GetArray()+TempSize1,ItemCopy2.GetArray(),TempSize2);
    return Temp;
}

template<typename TemplateItem>
const TemplateArray<TemplateItem> operator-(const TemplateArray<TemplateItem> &ItemCopy, const TemplateArray<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.SetArray(ItemCopy),return Temp;)
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.Remove(ItemCopy2),return Temp;)
    return Temp;
}

template<typename TemplateItem>
TemplateArray<TemplateItem> operator-(const TemplateArray<TemplateItem> &ItemCopy, const TemplateItem &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.SetArray(ItemCopy),return Temp;)
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.Remove(&ItemCopy2,1),return Temp;)
    return Temp;
}

template<typename TemplateItem>
const TemplateArray<TemplateItem> operator-(const TemplateArray<TemplateItem> &ItemCopy, const std::vector<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.SetArray(ItemCopy),return Temp;)
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.Remove(ItemCopy2),return Temp;)
    return Temp;
}

template<typename TemplateItem>
const TemplateArray<TemplateItem> operator-(const std::vector<TemplateItem> &ItemCopy, const TemplateArray<TemplateItem> &ItemCopy2)
{
    TemplateArray<TemplateItem> Temp;
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.SetArray(ItemCopy),return Temp;)
    ERRORCLASSFUNCTION(TemplateArray,operator-,!Temp.Remove(ItemCopy2),return Temp;)
    return Temp;
}

}

#endif
