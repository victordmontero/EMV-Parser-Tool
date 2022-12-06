#ifndef _APP_H_
#define _APP_H_

#include<wx/wxprec.h>

#ifndef WX_PRECOMP
#include<wx/wx.h>
#endif

class App : public wxApp
{
public:
	virtual bool OnInit();
};

#endif
