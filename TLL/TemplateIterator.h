#ifndef G_TEMPLATEITERATOR_H
#define G_TEMPLATEITERATOR_H

/*
Generated on: 2011-10-27-13.46.51

Version: 1.1
*/

/*
Notes:
Find replaced and fixed with an upgraded version, which uses function pointers to improve performance

*/

/*
Suggestions for improvements:


*/

#include "MemAlloc.h"
#include "ErrorMessage.h"
#include <vector>

namespace TL
{

template<typename TemplateItem>
class TemplateIteratorNonConst;

template<typename TemplateItem>
class TemplateIterator
{
	protected:

        template<typename TemplateItem2>
        class TemplateNode
        {
            protected:
                void Clear(){Next = NULL; Prev = NULL;}

            public:
                TemplateNode *Next;
                TemplateNode *Prev;

                TemplateItem2 Item;

                inline const bool IsEmpty() const { return ((!Next)&&(!Prev)); }
                inline const bool IsValid() const { return ((Next)&&(Prev)); }

                const bool operator!() const {return IsEmpty();}
                operator const bool () const {return IsValid();}

                void Reset(){ Close(); Open(); }
                inline void Open(){ Clear(); }
                inline void Close(){ Clear(); }

                TemplateNode(){Open();}
                ~TemplateNode(){Close();}

                const SIZE_TYPE InsertNodeNext(TemplateNode * &ItemCopy)
                {
                    ERRORCLASSFUNCTION(TemplateNode,InsertNodeNext,!ItemCopy,return 0;)
                    SIZE_TYPE TempIter = 1;
                    TemplateNode *ItemCopyEnd = ItemCopy;
                    while(ItemCopyEnd->Next != NULL)
                    {
                        ItemCopyEnd = ItemCopyEnd->Next;
                        ++TempIter;
                    }
                    ItemCopyEnd->Next = Next;
                    if(Next){Next->Prev = ItemCopyEnd;}
                    if(ItemCopy->Prev){ItemCopy->Prev->Next = NULL;}
                    Next = ItemCopy;
                    ItemCopy.Prev = this;

                    ItemCopy = NULL;
                    ItemCopyEnd = NULL;
                    return TempIter;
                }

                const SIZE_TYPE InsertNodePrev(TemplateNode * &ItemCopy)
                {
                    ERRORCLASSFUNCTION(TemplateNode,InsertNodeNext,!ItemCopy,return 0;)
                    SIZE_TYPE TempSize = 1;
                    TemplateNode *ItemCopyEnd = ItemCopy;
                    while(ItemCopyEnd->Next != NULL)
                    {
                        ItemCopyEnd = ItemCopyEnd->Next;
                        ++TempSize;
                    }

                    if(Prev){Prev->Next = ItemCopy;}
                    if(ItemCopy->Prev){ItemCopy->Prev->Next = NULL;}
                    ItemCopy->Prev = Prev;
                    ItemCopyEnd->Next = this;
                    Prev = ItemCopyEnd;

                    ItemCopy = NULL;
                    ItemCopyEnd = NULL;
                    return TempSize;
                }

                const bool Swap(TemplateNode * &Temp)
                {
                    ERRORCLASSFUNCTION(TemplateNode,SwapNode,!Temp,RETURN_BOOL)

                    TemplateNode *TempNext = Temp->Next;
                    TemplateNode *TempPrev = Temp->Prev;

                    Temp->Next = Next;
                    Temp->Prev = Prev;

                    if(Temp->Next)
                    {
                        Temp->Next->Prev = Temp;
                    }
                    if(Temp->Prev)
                    {
                        Temp->Prev->Next = Temp;
                    }

                    Next = TempNext;
                    Prev = TempPrev;

                    if(Next)
                    {
                        Next->Prev = this;
                    }
                    if(Prev)
                    {
                        Prev->Next = this;
                    }

                    return true;
                }

                const bool AddNodeNext()
                {
                    TemplateNode *Temp = NULL;
                    ERRORCLASSFUNCTION(TemplateNode,AddNodeNext,!CREATEB(Temp,1),RETURN_BOOL)
                    if(Next)
                    {
                        Next->Prev = Temp;
                        Temp->Next = Next;
                    }
                    Next = Temp;
                    Temp->Prev = this;
                    Temp = NULL;
                    return true;
                }

                const bool AddNodePrev()
                {
                    TemplateNode *Temp = NULL;
                    ERRORCLASSFUNCTION(TemplateNode,AddNodePrev,!CREATEB(Temp,1),RETURN_BOOL)
                    if(Prev)
                    {
                        Prev->Next = Temp;
                        Temp->Prev = Prev;
                    }
                    Prev = Temp;
                    Temp->Next = this;
                    Temp = NULL;
                    return true;
                }

                const bool RemoveNodeNext()
                {
                    if(!Next){return false;}
                    TemplateNode *Temp = Next;
                    Next = Temp->Next;
                    if(Temp->Next){Temp->Next->Prev = this;}
                    ERRORCLASSFUNCTION(TemplateNode,RemoveNodeNext,!DELETEB(Temp),RETURN_BOOL)
                    return true;
                }

                const bool RemoveNodePrev()
                {
                    if(!Prev){return false;}
                    TemplateNode *Temp = Prev;
                    Prev = Temp->Prev;
                    if(Temp->Prev){Temp->Prev->Next = this;}
                    ERRORCLASSFUNCTION(TemplateNode,RemoveNodePrev,!DELETEB(Temp),RETURN_BOOL)
                    return true;
                }

                TemplateNode &operator--(int)
                {
                    ERRORCLASSFUNCTION(TemplateNode,operator--,!RemoveNodeNext(),RETURN_THIS)
                    return *this;
                }

                TemplateNode &operator--()
                {
                    ERRORCLASSFUNCTION(TemplateNode,operator--,!RemoveNodePrev(),RETURN_THIS)
                    return *this;
                }

                TemplateNode &operator++(int)
                {
                    ERRORCLASSFUNCTION(TemplateNode,operator++,!AddNodeNext(),RETURN_THIS)
                    return *this;
                }

                TemplateNode &operator++()
                {
                    ERRORCLASSFUNCTION(TemplateNode,operator++,!AddNodePrev(),RETURN_THIS)
                    return *this;
                }

                inline TemplateNode &operator=(const TemplateItem2 &ItemCopy)
                {
                    Item = ItemCopy;
                    return *this;
                }
        };

        TemplateNode<TemplateItem> *Start;
        TemplateNode<TemplateItem> *End;
        TemplateNode<TemplateItem> *Curr;

        SIZE_TYPE Size;
        SIZE_TYPE Iter;

		void Clear(){Start = NULL; End = NULL; Curr = NULL; Size = 0; Iter = 0;}

	public:

        const bool FindItem(const TemplateItem &ItemCopy)
        {
            if(!IsValid()){return false;}
            TemplateIterator Temp = *this;
            do
            {
                if(Temp.GetCurrItem() == ItemCopy)
                {
                    Curr = Temp.Curr;
                    Iter = Temp.Iter;
                    return true;
                }
            }while(Temp.CurrNext());
            return false;
        }

        const bool IsSubList(const TemplateIterator &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateIterator,Find,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,Find,!ItemCopy.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,Find,ItemCopy.GetSize() > (Size-Iter+1),RETURN_BOOL)
            TemplateIterator Temp1 = *this;
            TemplateIterator Temp2 = ItemCopy;
            Temp2.CurrToStart();

            do
            {
                if(Temp1.GetCurrItem() != Temp2.GetCurrItem()){return false;}
                Temp1.CurrNext();
            }while(Temp2.CurrNext());
            return true;
        }

        const bool IsValidIter(const SIZE_TYPE L)
        {
            if(!Size){return false;}
            if(L < 1){return false;}
            if(L > Size){return false;}
            return true;
        }

        const bool IsSubArray(const TemplateItem Data[], const SIZE_TYPE S)
        {
            ERRORCLASSFUNCTION(TemplateIterator,IsSubArray,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,IsSubArray,!IsValidArray(Data,S),RETURN_BOOL)
            //ERRORCLASSFUNCTION(TemplateIterator,IsSubArray,S > (Size-Iter+1),RETURN_BOOL)
            if(S > (Size-(Iter+1))){return false;}
            TemplateIterator Temp1 = *this;
            const TemplateItem *Temp2 = Data+S;

            while(Data != Temp2)
            {
                if(Temp1.GetCurrItem() != *Data){return false;}
                Temp1.CurrNext();
                ++Data;
            }
            return true;
        }

        const bool Find(const TemplateIterator &ItemCopy, const bool ReturnLast = false)
        {
            ERRORCLASSFUNCTION(TemplateIterator,Find,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,Find,!ItemCopy.IsValid(),RETURN_BOOL)
            if(GetSize() > (Size-Iter+1)){return false;}
            TemplateIterator Temp1 = *this;
            TemplateIterator Temp2 = ItemCopy;
            Temp2.CurrToStart();

            while(Temp1.FindItem(Temp2.GetStartItem()))
            {
                if(ReturnLast)
                {
                    do
                    {
                        if(Temp1.GetCurrItem() != Temp2.GetCurrItem()){break;}
                        if(!Temp2.CurrNext())
                        {
                            Curr = Temp1.Curr;
                            Iter = Temp1.Iter;
                            return true;
                        }
                    }while(Temp1.CurrNext());
                    Temp2.CurrToStart();
                }
                else
                {
                    if(Temp1.IsSubList(Temp2))
                    {
                        Curr = Temp1.Curr;
                        Iter = Temp1.Iter;
                        return true;
                    }
                    Temp1.CurrNext();
                }
            }
            return false;
        }

        const bool Find(const TemplateItem &Item, const bool ReturnLast = false)
        {
            return Find(&Item,1,ReturnLast);
        }

        const bool Find(const TemplateItem ItemCopy[], const SIZE_TYPE Length, const bool ReturnLast = false)
        {
            ERRORCLASSFUNCTION(TemplateIterator,Find,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,Find,!IsValidArray(ItemCopy,Length),RETURN_BOOL)
            if(Length > (Size-Iter+1)){return false;}
            
            TemplateIterator TempIterator = *this;
            TemplateIterator TempIterComparitor;
            
            const TemplateItem *TempStart = ItemCopy;
            const TemplateItem *TempEnd = (ItemCopy+Length);
            const TemplateItem *TempIter;
            
            const bool (TL::TemplateIterator<TemplateItem>::*FunctionPointer)();

            //We define which direction we want the system to go in (find last goes backwards)
            if(ReturnLast)
            {
                FunctionPointer = &TL::TemplateIterator<TemplateItem>::CurrPrev;
                TempIterator.CurrToEnd();
            }
            else
            {
                FunctionPointer = &TL::TemplateIterator<TemplateItem>::CurrNext;
                TempIterator.CurrToStart();
            }
            
            do{
                //If we find a first match
                if(TempIterator.GetCurrItem() == *TempStart)
                {
                    //Copy an instance of this list
                    TempIterComparitor = TemplateIterator(TempIterator);
                    TempIter = TempStart;
                    //Iterate forwards through it
                    while(TempIter != TempEnd)
                    {
                        //If it's not a positive match, break
                        if(*TempIter != TempIterComparitor.GetCurrItem()){break;}
                        TempIterComparitor.CurrNext();
                        TempIter++;
                    }
                    //We can tell if it's a positive match by checking if Iter reached the TempEnd
                    if(TempIter == TempEnd)
                    {
                        ERRORCLASSFUNCTION(TempIterator,Find,!CurrTo(TempIterator.GetIter()),RETURN_BOOL)
                        return true;
                    }
                }
            }while( (TempIterator.*FunctionPointer)() );
            
            return false;
        }

        inline void CurrToStart(){ Curr = Start; Iter = (bool)Size; return; }
        inline void CurrToEnd()
        {
            Curr = End;
            Iter = Size;
            return;
        }
        const bool CurrTo(const SIZE_TYPE N)
        {
            ERRORCLASSFUNCTION(TemplateIterator,CurrTo,N < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateIterator,CurrTo,N > Size,RETURN_BOOL)
            if(N == Iter){ return true;}
            if(N == Size){ CurrToEnd(); return true;}
            if(N == (int)((bool)Size) ){ CurrToStart(); return true;}
            if(N < Iter)
            {
                while( (N < Iter) && CurrPrev() );
            }else
            {
                while( (N > Iter) && CurrNext() );
            }
            return true;
        }

        const bool CurrNext()
        {
            if(!Curr){return false;}
            if(!Curr->Next){return false;}
            Curr = Curr->Next;
            ++Iter;
            return true;
        }

        const bool CurrPrev()
        {
            if(!Curr){return false;}
            if(!Curr->Prev){return false;}
            Curr = Curr->Prev;
            --Iter;
            return true;
        }

        inline const TemplateItem &GetStartItem() const {return Start->Item;}
        inline const TemplateItem &GetEndItem() const {return End->Item;}
        inline const TemplateItem &GetCurrItem() const {return Curr->Item;}
        
        inline const TemplateItem &GetCurrItemPrev() const {return Curr->Prev->Item;}
        inline const TemplateItem &GetCurrItemNext() const {return Curr->Next->Item;}
        
        inline const bool IsValidItem() const {return Curr;}

        inline const SIZE_TYPE GetSize() const {return Size;}
        inline const SIZE_TYPE GetIter() const {return Iter;}

		inline const bool IsEmpty() const { return ((!Start)&&(!End)&&(!Curr)&&(!Size)&&(!Iter)); }
		inline const bool IsValid() const { return ((Start)&&(End)&&(Curr)&&(Size)&&(Iter)); }

		inline const bool IsStart() const
		{
            if(!Start){return false;}
            return (Start == Curr);
        }

        inline const bool IsEnd() const
        {
            if(!End){return false;}
            return (End == Curr);
        }

		const bool operator!() const {return IsEmpty();}
		operator const bool () const {return IsValid();}

		void Reset(){ Close(); Open(); }
		inline void Open(){ Clear(); }
		inline void Close(){ Clear(); }

        TemplateIterator(const TemplateIterator &ItemCopy)
        {
            Open();
            *this = ItemCopy;
        }
		inline TemplateIterator(){Open();}
		~TemplateIterator(){Close();}

        inline const std::vector<TemplateItem> ToVector() const
        {
            std::vector<TemplateItem> Temp;
            if(!IsValid())
            {
                return Temp;
            }

            try
            {
                Temp.resize(Size);
            }
            catch(...)
            {
                return std::vector<TemplateItem>();
            }

            TemplateIterator Temp2 = *this;
            Temp2.CurrToStart();
            //this->CurrToStart();
            SIZE_TYPE TempSize = 0;

            do
            {
                Temp[TempSize] = Temp2.GetCurrItem();
                //this->CurrNext();
                ++TempSize;
            }while(Temp2.CurrNext());
            return Temp;
        }

        inline const bool Compare(const TemplateItem Data[], const SIZE_TYPE S)
        {
            if(Size != S){return false;}
            if(IsEmpty() && !Data && !Size){return true;}
            ERRORCLASSFUNCTION(TemplateIterator,Compare,!Data,RETURN_BOOL)
            TemplateIterator Temp = *this;
            Temp.CurrToStart();
            SIZE_TYPE TempSize = 0;

            while(TempSize < S)
            {
                if(Data[TempSize] != Temp.GetCurrItem()){return false;}
                ++TempSize;
                Temp.CurrNext();
            }
            return true;
        }

        inline const bool operator==(const std::vector<TemplateItem> &ItemCopy)
        {
            if(Size != ItemCopy.size()){return false;}
            if(IsEmpty() && ItemCopy.empty()){return true;}
            return Compare(&ItemCopy[0],ItemCopy.size());
        }

        inline const bool operator==(const TemplateIterator &ItemCopy)
        {
            if(Size != ItemCopy.Size){return false;}
            if(IsEmpty() && ItemCopy.IsEmpty()){return true;}

            TemplateIterator Temp1 = *this, Temp2 = ItemCopy;
            Temp1.CurrToStart();
            Temp2.CurrToStart();

            do
            {
                if(Temp1.GetCurrItem() != Temp2.GetCurrItem()){return false;}
            }while(Temp1.CurrNext() && Temp2.CurrNext());
            return true;
        }

		TemplateIterator &operator=(const TemplateIterator &ItemCopy)
		{
            Start = ItemCopy.Start;
            End = ItemCopy.End;
            Curr = ItemCopy.Curr;
            Size = ItemCopy.Size;
            Iter = ItemCopy.Iter;
			return *this;
		}

		inline const TemplateItem &operator[](const SIZE_TYPE N) const
		{
		    TemplateIterator Temp = *this;
		    Temp.CurrTo(N);
		    return Temp.GetCurrItem();
		}

        template<typename TemplateItem2>
		friend class TemplateIteratorNonConst;
};

}

#endif
