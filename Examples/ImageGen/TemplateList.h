#ifndef G_TEMPLATELIST_H
#define G_TEMPLATELIST_H

/*
Generated on: 2011-10-27-13.45.31

Version: 1.2
*/

/*
Notes:
Added RemoveDuplicates as a list might already start off with duplicates within
Added AppendUnique to solve the issue of adding unique variables to a list

*/

/*
Suggestions for improvements:
Find function does not appear to be working correctly, please fix it.

*/

#include "CommonFunctions.h"
//#include "TemplateIterator.h"
#include "TemplateIteratorNonConst.h"

namespace TL
{

template<typename TemplateItem>
class TemplateList : public TemplateIterator<TemplateItem>
{
	protected:

        using TemplateIterator<TemplateItem>::Start;
        using TemplateIterator<TemplateItem>::End;
        using TemplateIterator<TemplateItem>::Curr;

        using TemplateIterator<TemplateItem>::Size;
        using TemplateIterator<TemplateItem>::Iter;

		void Clear(){TemplateIterator<TemplateItem>::Clear();}

        const bool AddNode()
        {
            ERRORCLASSFUNCTION(TemplateList,AddNode,Curr,RETURN_BOOL)
            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *Temp = NULL;
            ERRORCLASSFUNCTION(TemplateList,AddNode,!CREATEB(Temp),RETURN_BOOL)
            Curr = Temp;
            Start = Curr;
            End = Curr;
            Size = 1;
            Iter = 1;
            return true;
        }

        const bool RemoveNode()
        {
            ERRORCLASSFUNCTION(TemplateList,RemoveNode,!Curr,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,RemoveNode,!Curr->IsEmpty(),RETURN_BOOL)
            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *Temp = Curr;
            Clear();
            ERRORCLASSFUNCTION(TemplateList,RemoveNode,!DELETEB(Temp),RETURN_BOOL)
            return true;
        }

        const bool RemoveNodeStartPrev()
        {
            if(!Start){return false;}
            if(!Start->Prev){return false;}
            if(Start->Prev == Curr)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeStartPrev,!CurrNext(),RETURN_BOOL)
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeStartPrev,!Start->RemoveNodePrev(),RETURN_BOOL)
            --Iter;
            --Size;
            return true;
        }

        const bool RemoveNodeEndNext()
        {
            if(!End){return false;}
            if(!End->Next){return false;}
            if(End->Next == Curr)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeEndNext,!CurrPrev(),RETURN_BOOL)
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeEndNext,!End->RemoveNodeNext(),RETURN_BOOL)
            --Size;
            return true;
        }

        const bool Merge(TemplateList &List, const SIZE_TYPE Position)
        {
            //Is a valid list, if it isn't, we can't merge
            ERRORCLASSFUNCTION(TemplateList,Merge,!List.IsValid(),RETURN_BOOL)
            //If the position exceeds the size, it's out of bounds, and thus invalid
            ERRORCLASSFUNCTION(TemplateList,Merge,Position > Size,RETURN_BOOL)

            //If this isn't a valid list EG it's empty, we'll merely take ownership of the other list
            //as it's less intensive operation
            if(!IsValid())
            {
                TransferFrom(List);
                return true;
            }

            //TempIter is to remember our current position, relative to the added data
            //so we can refind the position afterwards
            SIZE_TYPE TempIter = 0;
            if(GetIter() > Position)
            {
                //If it's after the position, we add the list to compensate for the expansion
                TempIter = GetIter()+List.GetSize();
            }
            else
            {
                TempIter = GetIter();
            }

            ERRORCLASSFUNCTION(TemplateList,Merge,!this->CurrTo(Position),RETURN_BOOL)

            List.Start->Prev = Curr;
            if(Curr->Next)
            {
                Curr->Next->Prev = List.End;
            }

            if(Position == Size)
            {
                End = List.End;
            }

            List.End->Next = Curr->Next;
            Curr->Next = List.Start;

            if(Position == 0)
            {
                Start = List.Start;
            }

            Size = Size + List.Size;
            this->CurrTo(TempIter);
            List.Clear();
            return true;
        }

        const bool Merge(typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> * &ItemCopyStart, typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> * &ItemCopyEnd, const SIZE_TYPE S, const SIZE_TYPE Position)
        {
            ERRORCLASSFUNCTION(TemplateList,Merge,!ItemCopyStart,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Merge,!ItemCopyEnd,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Merge,Position > Size,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Merge,Position < 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Merge,S < 1,RETURN_BOOL)

            if(ItemCopyStart->Prev){ItemCopyStart->Prev->Next = NULL;}
            ItemCopyStart->Prev = NULL;
            if(ItemCopyEnd->Next){ItemCopyEnd->Next->Prev = NULL;}
            ItemCopyEnd->Next = NULL;

            if(!IsValid())
            {
                Start = ItemCopyStart;
                End = ItemCopyEnd;
                Curr = Start;
                Iter = 1;
                Size = S;
                return true;
            }

            if(!Position)
            {
                ItemCopyEnd->Next = Start;
                Start->Prev = ItemCopyEnd;
                Start = ItemCopyStart;
                Iter = Iter + S;
                Size = Size + S;

                return true;
            }

            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *TempStart = Start;
            SIZE_TYPE TempIter = 1;

            while(TempIter < Position)
            {
                ERRORCLASSFUNCTION(TemplateList,Insert,!TempStart->Next,RETURN_BOOL)
                TempStart = TempStart->Next;
                ++TempIter;
            }

            if(TempStart->Next)
            {
                TempStart->Next->Prev = ItemCopyEnd;
            }
            ItemCopyEnd->Next = TempStart->Next;
            TempStart->Next = ItemCopyStart;
            ItemCopyStart->Prev = TempStart;

            if(Iter > Position){Iter = Iter + S;}
            Size = Size + S;

            if(TempStart == End)
            {
                End = ItemCopyEnd;
            }

            TempStart = NULL;
            ItemCopyStart = NULL;
            ItemCopyEnd = NULL;
            return true;
        }

        //0 Is before the first node
        const bool Merge(typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> * &ItemCopy, const SIZE_TYPE Position = 0)
        {
            ERRORCLASSFUNCTION(TemplateList,Insert,ItemCopy == NULL,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Insert,Position > Size,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Insert,Position < 0,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Insert,!IsValid(),RETURN_BOOL)

            SIZE_TYPE TempSize = 1;
            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *ItemCopyEnd = ItemCopy;
            while(ItemCopyEnd->Next != NULL)
            {
                ItemCopyEnd = ItemCopyEnd->Next;
                ++TempSize;
            }

            return Merge(ItemCopy,ItemCopyEnd,TempSize,Position);
        }

	public:

        using TemplateIterator<TemplateItem>::CurrNext;
        using TemplateIterator<TemplateItem>::CurrPrev;
        using TemplateIterator<TemplateItem>::CurrToStart;
        using TemplateIterator<TemplateItem>::CurrToEnd;
        using TemplateIterator<TemplateItem>::CurrTo;
        using TemplateIterator<TemplateItem>::Compare;
        using TemplateIterator<TemplateItem>::operator==;
        using TemplateIterator<TemplateItem>::ToVector;
        using TemplateIterator<TemplateItem>::IsValid;
        using TemplateIterator<TemplateItem>::IsEmpty;

        void TransferFrom(TemplateList &ItemCopy)
        {
            //Prevent self assignment. Also, in-case of list overlap!
            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *TempCurr = ItemCopy.Curr, *TempStart = ItemCopy.Start, *TempEnd = ItemCopy.End;
            const SIZE_TYPE TempSize = ItemCopy.Size, TempIter = ItemCopy.Iter;
            ItemCopy.Clear();
            Reset();
            Curr = TempCurr, Start = TempStart, End = TempEnd;
            Size = TempSize, Iter = TempIter;
            return;
        }

        void Swap(TemplateList &ItemCopy)
        {
            //Prevent self assignment. Also, in-case of list overlap!
            typename TemplateIterator<TemplateItem>::template TemplateNode<TemplateItem> *TempCurr = ItemCopy.Curr, *TempStart = ItemCopy.Start, *TempEnd = ItemCopy.End;
            const SIZE_TYPE TempSize = ItemCopy.Size, TempIter = ItemCopy.Iter;
            ItemCopy.Clear();

            ItemCopy.Curr = Curr;
            ItemCopy.Start = Start;
            ItemCopy.End = End;
            ItemCopy.Size = Size;
            ItemCopy.Iter = Iter;

            Curr = TempCurr, Start = TempStart, End = TempEnd;
            Size = TempSize, Iter = TempIter;
            return;
        }

        inline const bool Merge(TemplateList &ItemCopy)
        {
            return Merge(ItemCopy, Size);
        }

        /*
        const bool Merge(TemplateList &ItemCopy, const SIZE_TYPE Position)
        {
            ERRORCLASSFUNCTION(TemplateList,Merge,!ItemCopy.IsValid(),RETURN_BOOL)
            if(!IsValid())
            {
                TransferFrom(ItemCopy);
                return true;
            }
            ERRORCLASSFUNCTION(TemplateList,Merge,!Merge(ItemCopy.Start,ItemCopy.End,ItemCopy.Size,Position),RETURN_BOOL)
            ItemCopy.Clear();
            return true;
        }
        */

        const bool AddNodeCurr()
        {
            if(!Curr)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeCurr,!AddNodeCurrNext(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,AddNodeCurr,!CurrNext(),RETURN_BOOL)
            return true;
        }

        inline const bool AddNodeCurr(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeCurr,!AddNodeCurr(),RETURN_BOOL)
            Curr->Item = ItemCopy;
            return true;
        }

        const bool AddNodeCurrNext()
        {
            if(!Curr)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeCurrNext,!Curr->AddNodeNext(),RETURN_BOOL)
            if(End == Curr){End = Curr->Next;}
            ++Size;
            return true;
        }

        inline const bool AddNodeCurrNext(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeCurrNext,!AddNodeCurrNext(),RETURN_BOOL)
            if(Curr->Next)
            {
                Curr->Next->Item = ItemCopy;
            }
            else
            {
                Curr->Item = ItemCopy;
            }
            return true;
        }

        const bool AddNodeCurrPrev()
        {
            if(!Curr)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeCurrPrev,!Curr->AddNodePrev(),RETURN_BOOL)
            if(Start == Curr){Start = Curr->Prev;}
            ++Iter;
            ++Size;
            return true;
        }

        inline const bool AddNodeCurrPrev(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeCurrNext,!AddNodeCurrPrev(),RETURN_BOOL)
            if(Curr->Prev)
            {
                Curr->Prev->Item = ItemCopy;
            }
            else
            {
                Curr->Item = ItemCopy;
            }
            return true;
        }

        const bool AddNodeEndNext()
        {
            if(!End)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeEndNext,!End->AddNodeNext(),RETURN_BOOL)
            End = End->Next;
            ++Size;
            return true;
        }

        inline const bool AddNodeEndNext(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeEndNext,!AddNodeEndNext(),RETURN_BOOL)
            End->Item = ItemCopy;
            return true;
        }

        const bool AddNodeEndPrev()
        {
            if(!End)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeEndPrev,!End->AddNodePrev(),RETURN_BOOL)
            if(End == Start){Start = End->Prev;}
            if(Curr == End){++Iter;}
            ++Size;
            return true;
        }

        inline const bool AddNodeEndPrev(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeEndPrev,!AddNodeEndPrev(),RETURN_BOOL)
            if(End->Prev)
            {
                End->Prev->Item = ItemCopy;
            }
            else
            {
                End->Item = ItemCopy;
            }
            return true;
        }

        const bool AddNodeStartNext()
        {
            if(!Start)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeStartNext,!Start->AddNodeNext(),RETURN_BOOL)
            if(Curr != Start){++Iter;}
            ++Size;
            return true;
        }

        inline const bool AddNodeStartNext(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeStartNext,!AddNodeStartNext(),RETURN_BOOL)
            if(Start->Next)
            {
                Start->Next->Item = ItemCopy;
            }
            else
            {
                Start->Item = ItemCopy;
            }
            return true;
        }

        const bool AddNodeStartPrev()
        {
            if(!Start)
            {
                return AddNode();
            }

            ERRORCLASSFUNCTION(TemplateList,AddNodeStartPrev,!Start->AddNodePrev(),RETURN_BOOL)
            Start = Start->Prev;
            ++Iter;
            ++Size;
            return true;
        }

        inline const bool AddNodeStartPrev(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,AddNodeStartPrev,!AddNodeStartPrev(),RETURN_BOOL)
            if(Start->Prev)
            {
                Start->Prev->Item = ItemCopy;
            }
            else
            {
                Start->Item = ItemCopy;
            }
            return true;
        }

        const bool RemoveNodeCurr()
        {
            if(!Curr){return false;}
            if(Curr->IsEmpty()){return RemoveNode();}
            if(Curr->Next)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurr,!CurrNext(),RETURN_BOOL)
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurr,!RemoveNodeCurrPrev(),RETURN_BOOL)
            }
            else
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurr,!CurrPrev(),RETURN_BOOL)
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurr,!RemoveNodeCurrNext(),RETURN_BOOL)
            }

            return true;
        }

        const bool RemoveNodeCurrNext()
        {
            if(!Curr){return false;}
            if(!Curr->Next){return false;}
            if(Curr->Next == End)
            {
                End = Curr;
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurrNext,!Curr->RemoveNodeNext(),RETURN_BOOL)
            --Size;
            return true;
        }

        const bool RemoveNodeCurrPrev()
        {
            if(!Curr){return false;}
            if(!Curr->Prev){return false;}
            if(Curr->Prev == Start)
            {
                Start = Curr;
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeCurrPrev,!Curr->RemoveNodePrev(),RETURN_BOOL)
            --Iter;
            --Size;
            return true;
        }

        const bool RemoveStart()
        {
            if(!Start){return false;}
            if(Start->IsEmpty()){return RemoveNode();}
            Start = Start->Next;
            return RemoveNodeStartPrev();
        }

        const bool RemoveNodeStartNext()
        {
            if(!Start){return false;}
            if(!Start->Next){return false;}
            if(Start->Next == Curr)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeStartNext,!CurrPrev(),RETURN_BOOL)
            }
            if(Start->Next == End)
            {
                End = Start;
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeStartNext,!Start->RemoveNodeNext(),RETURN_BOOL)
            --Size;
            return true;
        }

        const bool RemoveNodeEnd()
        {
            if(!End){return false;}
            if(!End->Prev){ return RemoveNode(); }
            End = End->Prev;
            return RemoveNodeEndNext();
        }

        const bool RemoveNodeEndPrev()
        {
            if(!End){return false;}
            if(!End->Prev){return false;}
            if(End->Prev == Curr)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveNodeEndPrev,!CurrNext(),RETURN_BOOL)
            }
            if(End->Prev == Start)
            {
                Start = End;
            }
            ERRORCLASSFUNCTION(TemplateList,RemoveNodeEndPrev,!End->RemoveNodePrev(),RETURN_BOOL)
            return true;
        }
        
        //This is pretty inefficient version, implemented quickly due to time constraints
        //consider using a pointer substitution method
        const bool RemoveDuplicates()
        {
            if(IsEmpty()){return true;}
            
            TemplateList<TemplateItem> Temp;
            TemplateIterator<TemplateItem> Temp2 = *this;
            SIZE_TYPE TempIter = 0;
            
            Temp2.CurrToStart();
            do{
                switch(Temp.AppendUnique(Temp2.GetCurrItem()))
                {
                    case 0:
                        if(TempIter == 0)
                        {
                            if(Temp2.GetCurrItem() == GetCurrItem())
                            {
                                TempIter = Temp.GetIter();
                            }
                        }
                    break;
                    case 1:
                        //We didn't append, so we don't care
                    break;
                    case 2:
                        //There was an error appending! Erk!
                    default:
                    ERRORCLASSFUNCTION(TemplateList,RemoveDuplicates,true,RETURN_BOOL)
                    break;
                }
                
            }while(Temp2.CurrNext());
            
            if(TempIter != 0)
            {
                ERRORCLASSFUNCTION(TemplateList,RemoveDuplicates,!Temp.CurrTo(TempIter),RETURN_BOOL)
            }
            
            Swap(Temp);
            return true;
        }

        void CurrToStart()
        {
            TemplateIterator<TemplateItem>::CurrToStart();
            Iter = (bool)Size;
        }

        void CurrToEnd()
        {
            TemplateIterator<TemplateItem>::CurrToEnd();
            Iter = Size;
        }

        using TemplateIterator<TemplateItem>::GetStartItem;
        using TemplateIterator<TemplateItem>::GetEndItem;
        using TemplateIterator<TemplateItem>::GetCurrItem;

        inline TemplateItem &GetStartItem(){return Start->Item;}
        inline TemplateItem &GetEndItem(){return End->Item;}
        inline TemplateItem &GetCurrItem(){return Curr->Item;}

        using TemplateIterator<TemplateItem>::GetSize;
        inline const SIZE_TYPE GetIter() const {return Iter;}
        inline const SIZE_TYPE GetSize() const {return Size;}

		const bool operator!() const {return IsEmpty();}
		operator const bool () const {return IsValid();}

		void Reset(){ Close(); }
		inline void Open(){ Clear(); }
		void Close()
		{
            if(Start)
            {
                while(RemoveStart());
            }
			Clear();
		}

		TemplateList(){Open();}
		TemplateList(const std::vector<TemplateItem> &ItemCopy){Open(); ERRORCLASSFUNCTION(TemplateList,TemplateList,!Assign(ItemCopy),) CurrToStart();}

		TemplateList(const TemplateList &ItemCopy)
		{
		    Open();
		    ERRORCLASSFUNCTION(TemplateList,TemplateList,!Assign(ItemCopy),)
		    CurrToStart();
		}
		TemplateList(const TemplateIterator<TemplateItem> &ItemCopy)
		{
		    Open();
		    ERRORCLASSFUNCTION(TemplateList,TemplateList,!Assign(ItemCopy),)
            CurrToStart();
        }
        TemplateList(const TemplateItem Data[], const SIZE_TYPE S){ Open(); ERRORCLASSFUNCTION(TemplateList,TemplateList,!Assign(Data,S),) CurrToStart();}
        TemplateList(const TemplateItem &ItemCopy){Open(); ERRORCLASSFUNCTION(TemplateList,TemplateList,!Assign(ItemCopy),) CurrToStart();}

		~TemplateList(){Close();}

		const bool Insert(const TemplateIterator<TemplateItem> &ItemCopy,const SIZE_TYPE Position)
		{
		    ERRORCLASSFUNCTION(TemplateList,Insert,!ItemCopy.IsValid(),RETURN_BOOL)
            TemplateList Temp;
            ERRORCLASSFUNCTION(TemplateList,Insert,!Temp.Assign(ItemCopy),RETURN_BOOL)
            return Merge(Temp,Position);
		}

		const bool Insert(const TemplateItem Data[], const SIZE_TYPE S, const SIZE_TYPE Position)
		{
		    ERRORCLASSFUNCTION(TemplateList,Insert,!IsValidArray(Data,S),RETURN_BOOL)
            TemplateList Temp;
            ERRORCLASSFUNCTION(TemplateList,Insert,!Temp.Assign(Data,S),RETURN_BOOL)
            return Merge(Temp,Position);
		}

		const bool Insert(const std::vector<TemplateItem> &ItemCopy, const SIZE_TYPE Position)
		{
		    if(ItemCopy.empty())
		    {
		        return false;
		    }
		    return Insert(&ItemCopy[0],ItemCopy.size(),Position);
		}

		inline const bool Insert(const TemplateItem &ItemCopy, const SIZE_TYPE Position)
		{
		    return Insert(&ItemCopy,1,Position);
		}

		TemplateList GetSubList(const SIZE_TYPE StartCount, const SIZE_TYPE EndCount) const
		{
		    if(!IsValid()){return TemplateList();}
		    bool IsReverse = (StartCount > EndCount);
            TemplateList TempList;

		    if(IsReverse)
		    {
		        ERRORCLASSFUNCTION(TemplateList,GetSubList,StartCount < 1,return TemplateList();)
                ERRORCLASSFUNCTION(TemplateList,GetSubList,EndCount < 1,return TemplateList();)
                ERRORCLASSFUNCTION(TemplateList,GetSubList,StartCount > Size,RETURN_BOOL)
                TemplateIterator<TemplateItem> Temp = *this;

                ERRORCLASSFUNCTION(TemplateList,GetSubList,!Temp.CurrTo(StartCount),RETURN_BOOL)
                while(1)
                {
                        ERRORCLASSFUNCTION(TemplateList,GetSubList,!TempList.Append(Temp.GetCurrItem()),return TemplateList();)
                        if(Temp.GetIter() <= EndCount){break;}
                        Temp.CurrPrev();
                }
		    }
		    else
		    {
                ERRORCLASSFUNCTION(TemplateList,GetSubList,StartCount < 1,return TemplateList();)
                ERRORCLASSFUNCTION(TemplateList,GetSubList,StartCount > Size,return TemplateList();)
                ERRORCLASSFUNCTION(TemplateList,GetSubList,EndCount > Size,return TemplateList();)
                TemplateIterator<TemplateItem> Temp = *this;

                ERRORCLASSFUNCTION(TemplateList,GetSubList,!Temp.CurrTo(StartCount),return TemplateList();)
                while(1)
                {
                    ERRORCLASSFUNCTION(TemplateList,GetSubList,!TempList.Append(Temp.GetCurrItem()),return TemplateList();)
                    if(Temp.GetIter() >= EndCount){break;}
                    Temp.CurrNext();
                }
		    }
            return TempList;
		}

		TemplateList ExtractSubList(const SIZE_TYPE StartCount, const SIZE_TYPE EndCount)
		{
		    if(!IsValid()){return TemplateList();}
		    bool IsReverse = (StartCount > EndCount);

            SIZE_TYPE TempStartCount = 0, TempEndCount = 0;

            if(IsReverse)
            {
                TempStartCount = EndCount;
                TempEndCount = StartCount;
            }
            else
            {
                TempStartCount = StartCount;
                TempEndCount = EndCount;
            }

		    ERRORCLASSFUNCTION(TemplateList,ExtractSubList,TempStartCount < 1,return TemplateList();)
		    ERRORCLASSFUNCTION(TemplateList,ExtractSubList,TempStartCount > Size,return TemplateList();)
		    ERRORCLASSFUNCTION(TemplateList,ExtractSubList,TempEndCount > Size,return TemplateList();)

            TemplateList TempStart;
            TemplateIterator<TemplateItem> &TempStart2 = TempStart;
            TemplateList TempEnd;
            TemplateIterator<TemplateItem> &TempEnd2 = TempEnd;

            TempStart2 = *this;
            TempEnd2 = *this;

            ERRORCLASSFUNCTION(TemplateList,ExtractSubList,!TempStart2.CurrTo(TempStartCount),return TemplateList();)
            ERRORCLASSFUNCTION(TemplateList,ExtractSubList,!TempEnd2.CurrTo(TempEndCount),return TemplateList();)

            TemplateList TempList;

            TempList.Iter = 1;
            TempList.Size = (TempEndCount-TempStartCount)+1;
            Size = Size - TempList.Size;

            if(TempStart.Curr == Start)
            {
                Start = TempEnd.Curr->Next;
                Curr = TempEnd.Curr->Next;
                Iter = (bool)Size;
            }
            else
            {
                Curr = TempStart.Curr->Prev;
                Iter = Iter-TempStartCount;
            }

            if(TempEnd.Curr == End)
            {
                End = TempStart.Curr->Next;
            }

            if(TempStart.Curr->Prev)
            {
                TempStart.Curr->Prev->Next = TempEnd.Curr->Next;
            }

            if(TempEnd.Curr->Next)
            {
                TempEnd.Curr->Next->Prev = TempStart.Curr->Prev;
            }

            TempStart.Curr->Prev = NULL;
            TempEnd.Curr->Next = NULL;

            TempList.Start = TempStart.Curr;
            TempList.Curr = TempStart.Curr;
            TempList.End = TempEnd.Curr;


            TempStart2 = TemplateIterator<TemplateItem>();
            TempEnd2 = TemplateIterator<TemplateItem>();

            if(IsReverse)
            {
                ERRORCLASSFUNCTION(TemplateList,ExtractSubList,!TempList.Reverse(),RETURN_BOOL)
            }

            return TempList;
		}

        const bool SwapNode(const SIZE_TYPE Target, const SIZE_TYPE With)
        {
            ERRORCLASSFUNCTION(TemplateList,SwapNode,!IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,SwapNode,Target < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,SwapNode,Target > Size,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,SwapNode,With < 1,RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,SwapNode,With > Size,RETURN_BOOL)
            TemplateList ItemCopy, ItemCopy2;
            TemplateIterator<TemplateItem> &Temp = ItemCopy, &Temp2 = ItemCopy2;
            Temp = *this;
            Temp2 = *this;

            ERRORCLASSFUNCTION(TemplateList,SwapNode,!Temp.CurrTo(Target),Temp = TemplateIterator<TemplateItem>(); Temp2 = TemplateIterator<TemplateItem>(); RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,SwapNode,!Temp2.CurrTo(With),Temp = TemplateIterator<TemplateItem>(); Temp2 = TemplateIterator<TemplateItem>(); RETURN_BOOL)

            if(ItemCopy.Curr == Start)
            {
                Start = ItemCopy2.Curr;
            }
            else
            {
                if(ItemCopy2.Curr == Start)
                {
                    Start = ItemCopy.Curr;
                }
            }

            if(ItemCopy.Curr == End)
            {
                End = ItemCopy2.Curr;
            }
            else
            {
                if(ItemCopy2.Curr == End)
                {
                    End = ItemCopy.Curr;
                }
            }

            if(ItemCopy.Curr == Curr)
            {
                Curr = ItemCopy2.Curr;
            }
            else
            {
                if(ItemCopy2.Curr == Curr)
                {
                    Curr = ItemCopy.Curr;
                }
            }

            ItemCopy.Curr->Swap(ItemCopy2.Curr);

            Temp = TemplateIterator<TemplateItem>();
            Temp2 = TemplateIterator<TemplateItem>();
            return true;
        }

		const bool Reverse()
		{
		    if(!IsValid()){return true;}
		    TemplateIterator<TemplateItem> Temp = *this;
		    TemplateList Temp2;
		    Temp.CurrToEnd();
		    do
		    {
		        ERRORCLASSFUNCTION(TemplateList,Reverse,!Temp2.Append(Temp.GetCurrItem()),RETURN_BOOL)
		    }while(Temp.CurrPrev());
		    Swap(Temp2);
		    return true;
		}

		const bool Remove(const TemplateIterator<TemplateItem> &ItemCopy)
		{
		    if(!IsValid()){return false;}
		    if(!ItemCopy.IsValid()){return false;}
            TemplateIterator<TemplateItem> Temp = *this;


            if(Temp.Find(ItemCopy))
            {
                return ExtractSubList(Temp.GetIter(),(Temp.GetIter()+ItemCopy.GetSize())-1).IsValid();
            }
            return false;
		}

		const bool Remove(const TemplateItem Data[], const SIZE_TYPE &S)
		{
		    ERRORCLASSFUNCTION(TemplateList,Remove,!IsValidArray(Data,S),RETURN_BOOL)
            if(!IsValid()){return false;}
		    TemplateIterator<TemplateItem> Temp = *this;

            if(Temp.Find(Data,S))
            {
                return ExtractSubList(Temp.GetIter(),(Temp.GetIter()+S)-1).IsValid();
            }
            return false;
		}

		const bool Remove(const TemplateItem &ItemCopy)
		{
		    return Remove(&ItemCopy,1);
		}

		const bool Remove(const std::vector<TemplateItem> &ItemCopy)
        {
            if(ItemCopy.empty())
            {
                return false;
            }
            return Remove(&ItemCopy[0],ItemCopy.size());
        }

        const bool Replace(const TemplateIterator<TemplateItem> &Target, const TemplateIterator<TemplateItem> &With)
        {
            if(!IsValid()){return false;}
		    if(!Target.IsValid()){return false;}
		    if(!With.IsValid()){return false;}
            TemplateIterator<TemplateItem> Temp = *this;

            if(Temp.Find(Target))
            {
                ERRORCLASSFUNCTION(TemplateList,Replace,!Insert(With,Temp.GetIter()-1),RETURN_BOOL)
                return ExtractSubList(Temp.GetIter()+With.GetSize(),(Temp.GetIter()+With.GetSize()+Target.GetSize())-1 ).IsValid();
            }
            return false;
        }

        const bool Replace(const TemplateItem Target[], const SIZE_TYPE S1, const TemplateItem With[], const SIZE_TYPE S2)
        {
            ERRORCLASSFUNCTION(TemplateList,Replace,!IsValidArray(Target,S1),RETURN_BOOL)
            ERRORCLASSFUNCTION(TemplateList,Replace,!IsValidArray(With,S2),RETURN_BOOL)
            if(!IsValid()){return false;}
            TemplateIterator<TemplateItem> Temp = *this;

            if(Temp.Find(Target,S1))
            {
                ERRORCLASSFUNCTION(TemplateList,Replace,!Insert(With,S2,Temp.GetIter()-1),RETURN_BOOL)
                return ExtractSubList(Temp.GetIter()+S2,(Temp.GetIter()+S2+S1)-1 ).IsValid();
            }
            return false;
        }

        const bool Replace(const std::vector<TemplateItem> &Target, const std::vector<TemplateItem> &With)
        {
            if(Target.empty()){return false;}
            if(With.empty()){return false;}
            return Replace(&Target[0],Target.size(),&With[0],With.size());
        }

        const bool Append(const TemplateItem Data[], const SIZE_TYPE S)
        {
            return Insert(Data,S,Size);
        }

        const bool Append(const std::vector<char> &ItemCopy)
        {
            return Insert(ItemCopy,Size);
        }

        const bool Append(const TemplateIterator<TemplateItem> &ItemCopy)
        {
            return Insert(ItemCopy,Size);
        }

        const bool Append(const TemplateItem &ItemCopy)
        {
            return Append(&ItemCopy,1);
        }
        
        //0 means it worked
        //1 means it failed to append but didn't error
        //2 means it errored
        const uint8_t AppendUnique(const TemplateItem &ItemCopy)
        {
            if(IsEmpty())
            {
                //Trippy code man!
                //2 is an 'error', but append only returns 'true' or 'false'
                //To avoid code bloat we simply invert the 'false' to 'true' so when
                //2 * true occurs, it's equal to '2' - an error!
                //0 is 'succeeded', so 2 * false (2 * 0) is... nothing! 0!
                return (2*(!Append(&ItemCopy,1)));
            }
            
            TemplateIterator<TemplateItem> Check = *this;
            
            Check.CurrToStart();
            
            do{
                if(Check.GetCurrItem() == ItemCopy)
                {
                    return 1;
                }
            }while(Check.CurrNext());
            
            return (2*(!Append(&ItemCopy,1)));
        }

        const bool Assign(const TemplateIterator<TemplateItem> &ItemCopy)
        {
            if(!ItemCopy.IsValid())
            {
                Reset();
                return true;
            }

            TemplateIterator<TemplateItem> Temp = ItemCopy;
            TemplateList TempList;
            Temp.CurrToStart();

            do
            {
                ERRORCLASSFUNCTION(TemplateList,Assign,!TempList.AddNodeEndNext(Temp.GetCurrItem()),RETURN_BOOL)
            }while(Temp.CurrNext());
            TransferFrom(TempList);
			return true;
        }

        const bool Assign(const TemplateList &ItemCopy)
        {
            if(!ItemCopy.IsValid())
            {
                Reset();
                return true;
            }

            TemplateIterator<TemplateItem> Temp = ItemCopy;
            TemplateList TempList;
            Temp.CurrToStart();

            do
            {
                ERRORCLASSFUNCTION(TemplateList,Assign,!TempList.AddNodeEndNext(Temp.GetCurrItem()),RETURN_BOOL)
            }while(Temp.CurrNext());
            TempList.CurrToStart();
            TransferFrom(TempList);
			return true;
        }

        const bool Assign(const TemplateItem Data[], const SIZE_TYPE S)
        {
            if(!Data || !S)
            {
                Reset();
                return true;
            }
            TemplateList Temp;
            const TemplateItem *Temp2 = (Data+S);
            while(Data != Temp2)
            {
                ERRORCLASSFUNCTION(TemplateList,Assign,!Temp.AddNodeCurr(*Data),RETURN_BOOL)
                ++Data;
            }
            Temp.CurrToStart();
            TransferFrom(Temp);
			return true;
        }

        const bool Assign(const std::vector<TemplateItem> &ItemCopy)
        {
            if(ItemCopy.empty())
            {
                Reset();
                return true;
            }
            return Assign(&ItemCopy[0],ItemCopy.size());
        }

        const bool Assign(const TemplateItem &ItemCopy)
        {
            return Assign(&ItemCopy,1);
        }

        TemplateList &operator++()
        {
            ERRORCLASSFUNCTION(TemplateList,operator++,!AddNodeStartPrev(),RETURN_THIS)
            CurrToStart();
            return *this;
        }

        TemplateList &operator--()
        {
            ERRORCLASSFUNCTION(TemplateList,operator--,!RemoveNodeStartPrev(),RETURN_THIS)
            CurrToStart();
            return *this;
        }

        TemplateList &operator++(int)
        {
            ERRORCLASSFUNCTION(TemplateList,operator++,!AddNodeEndNext(),RETURN_THIS)
            CurrToEnd();
            return *this;
        }

        TemplateList &operator--(int)
        {
            ERRORCLASSFUNCTION(TemplateList,operator--,!RemoveNodeEndNext(),RETURN_THIS)
            CurrToEnd();
            return *this;
        }

        TemplateList &operator+=(const TemplateIterator<TemplateItem> &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator+=,!Append(ItemCopy),RETURN_THIS)
            return *this;
        }

        TemplateList &operator+=(const std::vector<TemplateItem> &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator+=,!Append(ItemCopy),RETURN_THIS)
            return *this;
        }

        TemplateList &operator+=(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator+=,!Append(ItemCopy),RETURN_THIS)
            return *this;
        }

        TemplateList &operator-=(const TemplateIterator<TemplateItem> &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator-=,!Remove(ItemCopy),RETURN_THIS)
            return *this;
        }

        TemplateList &operator-=(const std::vector<TemplateItem> &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator-=,!Remove(ItemCopy),RETURN_THIS)
            return *this;
        }

        TemplateList &operator-=(const TemplateItem &ItemCopy)
        {
            ERRORCLASSFUNCTION(TemplateList,operator-=,!Remove(ItemCopy),RETURN_THIS)
            return *this;
        }

		TemplateList &operator=(const TemplateIterator<TemplateItem> &ItemCopy)
		{
		    ERRORCLASSFUNCTION(TemplateList,operator=,!Assign(ItemCopy),RETURN_THIS)
			return *this;
		}

        TemplateList &operator=(const TemplateList<TemplateItem> &ItemCopy)
		{
		    ERRORCLASSFUNCTION(TemplateList,operator=,!Assign(ItemCopy),RETURN_THIS)
			return *this;
		}

		TemplateList &operator=(const std::vector<TemplateItem> &ItemCopy)
		{
		    ERRORCLASSFUNCTION(TemplateList,operator=,!Assign(ItemCopy),RETURN_THIS)
			return *this;
		}

		TemplateList &operator=(const TemplateItem &ItemCopy)
		{
		    ERRORCLASSFUNCTION(TemplateList,operator=,!Assign(ItemCopy),RETURN_THIS)
			return *this;
		}

        inline TemplateItem &operator[](const SIZE_TYPE N)
		{
		    TemplateIteratorNonConst<TemplateItem> Temp = *this;
		    Temp.CurrTo(N);
            return Temp.GetCurrItem();
		}

		inline const TemplateItem &operator[](const SIZE_TYPE N) const
		{
		    TemplateIterator<TemplateItem> Temp = *this;
		    Temp.CurrTo(N);
		    return Temp.GetCurrItem();
		}
};

#define OPERATOR_PLUS_FUNCTION(TYPE_FIRST,TYPE_SECOND) \
template<typename TemplateItem>\
TemplateList<TemplateItem> operator+(const TYPE_FIRST ItemCopy, const TYPE_SECOND ItemCopy2) \
{ \
    TemplateList<TemplateItem> TempList;\
    ERRORCLASSFUNCTION(TemplateList,operator+,!TempList.Append(ItemCopy), errno = EIO; return TemplateList<TemplateItem>();)\
    ERRORCLASSFUNCTION(TemplateList,operator+,!TempList.Append(ItemCopy2),errno = EIO; return TemplateList<TemplateItem>();)\
    return TempList;\
}

OPERATOR_PLUS_FUNCTION(TemplateIterator<TemplateItem> &,TemplateIterator<TemplateItem> &)
OPERATOR_PLUS_FUNCTION(TemplateIterator<TemplateItem> &,std::vector<TemplateItem> &)
OPERATOR_PLUS_FUNCTION(TemplateIterator<TemplateItem> &,TemplateItem &)
OPERATOR_PLUS_FUNCTION(std::vector<TemplateItem> &, TemplateIterator<TemplateItem> &)
OPERATOR_PLUS_FUNCTION(TemplateItem &, TemplateIterator<TemplateItem> &)

#define OPERATOR_MINUS_FUNCTION(TYPE_FIRST,TYPE_SECOND)\
template<typename TemplateItem>\
TemplateList<TemplateItem> operator-(const TYPE_FIRST ItemCopy, const TYPE_SECOND ItemCopy2)\
{\
    TemplateList<TemplateItem> TempList;\
    ERRORFUNCTION(operator-,!TempList.Append(ItemCopy),errno = EIO; return TemplateList<TemplateItem>();)\
    TempList.Remove(ItemCopy2);\
    return TempList;\
}

OPERATOR_MINUS_FUNCTION(TemplateIterator<TemplateItem> &,TemplateIterator<TemplateItem> &)
OPERATOR_MINUS_FUNCTION(TemplateIterator<TemplateItem> &,std::vector<TemplateItem> &)
OPERATOR_MINUS_FUNCTION(TemplateIterator<TemplateItem> &,TemplateItem &)
OPERATOR_MINUS_FUNCTION(std::vector<TemplateItem> &, TemplateIterator<TemplateItem> &)
OPERATOR_MINUS_FUNCTION(TemplateItem &, TemplateIterator<TemplateItem> &)

}

#undef OPERATOR_MINUS_FUNCTION
#undef OPERATOR_PLUS_FUNCTION
#endif
