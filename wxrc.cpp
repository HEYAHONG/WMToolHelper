#include "wxrc.h"
#include "RC.h"

//通过名称获取资源大小
size_t wxRCGetSize(const char * name)
{
    if(name == NULL)
    {
        return 0;
    }

    return wxRCGetSize(wxString(name));
}

//通过名称获取资源指针
const unsigned char * wxRCGetHandle(const char * name)
{
    if(name==NULL)
    {
        return NULL;
    }
    return wxRCGetHandle(wxString(name));
}

//通过名称获取资源大小
size_t wxRCGetSize(wxString name)
{
    if(name.length()==0)
    {
        return 0;
    }

    name.Replace("\\","/");
    //删除多余的/
    while(0!=name.Replace("//","/"));
    //去除开头的/
    while(0 == name.Find("/"))
    {
        name=name.substr(1);
    }

    return RCGetSize(name.ToStdString().c_str());
}

//通过名称获取资源指针
const unsigned char * wxRCGetHandle(wxString  name)
{
    if(name.length()==0)
    {
        return NULL;
    }

    name.Replace("\\","/");
    //删除多余的/
    while(0!=name.Replace("//","/"));
    //去除开头的/
    while(0 == name.Find("/"))
    {
        name=name.substr(1);
    }

    return RCGetHandle(name.ToStdString().c_str());
}
