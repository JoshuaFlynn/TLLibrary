#ifndef G_TEMPLATEITERATORNONCONST_H
#define G_TEMPLATEITERATORNONCONST_H

/*
Generated on: 2012-08-24-11.40.58

Version: 1.2
*/

/*
Notes:
Added 'getprevitem' version
Added 'getnextitem' version

*/

/*
Suggestions for improvements:


*/

#include "TemplateIterator.h"

namespace TL
{

template<typename TemplateItem>
class TemplateIteratorNonConst : public TemplateIterator<TemplateItem>
{
	private:
	protected:
        using TemplateIterator<TemplateItem>::Start;
        using TemplateIterator<TemplateItem>::End;
        using TemplateIterator<TemplateItem>::Curr;

        using TemplateIterator<TemplateItem>::Size;
        using TemplateIterator<TemplateItem>::Iter;

	public:

        using TemplateIterator<TemplateItem>::CurrNext;
        using TemplateIterator<TemplateItem>::CurrPrev;
        using TemplateIterator<TemplateItem>::CurrToStart;
        using TemplateIterator<TemplateItem>::CurrToEnd;
        using TemplateIterator<TemplateItem>::Compare;
        using TemplateIterator<TemplateItem>::operator==;
        using TemplateIterator<TemplateItem>::ToVector;
        using TemplateIterator<TemplateItem>::IsValid;
        using TemplateIterator<TemplateItem>::IsEmpty;

        using TemplateIterator<TemplateItem>::Open;
        using TemplateIterator<TemplateItem>::Close;

        TemplateIteratorNonConst(TemplateIterator<TemplateItem> &ItemCopy)
		{
		    Open();
		    *this = ItemCopy;
        }

        inline TemplateItem &GetStartItem() const {return Start->Item;}
        inline TemplateItem &GetEndItem() const {return End->Item;}
        inline TemplateItem &GetCurrItem() const {return Curr->Item;}
        
        inline TemplateItem &GetCurrPrevItem() const {return Curr->Prev->Item;}
        inline TemplateItem &GetCurrNextItem() const {return Curr->Next->Item;}

        TemplateIteratorNonConst &operator=(TemplateIterator<TemplateItem> &ItemCopy)
		{
            Start = ItemCopy.Start;
            End = ItemCopy.End;
            Curr = ItemCopy.Curr;
            Size = ItemCopy.Size;
            Iter = ItemCopy.Iter;
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

}

#endif
