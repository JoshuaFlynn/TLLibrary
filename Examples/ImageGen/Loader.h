#ifndef G_LOADER_H
#define G_LOADER_H

/*
Generated on: 2012-08-17-17.55.24

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include "TemplateList.h"

namespace TL
{

template<typename Item>
class Loader : public TemplateList<Item>
{
	private:
	protected:
	public:
        using TemplateList<Item>::Open;
        using TemplateList<Item>::Close;

		Loader(){Open();}
		~Loader(){Close();}
};

}
#endif
