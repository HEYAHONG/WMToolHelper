#ifndef WXRC_H
#define WXRC_H
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif

//通过名称获取资源大小
size_t wxRCGetSize(const char * name);

//通过名称获取资源指针
const unsigned char * wxRCGetHandle(const char * name);

#ifdef __cplusplus
};
#endif // __cplusplus

#ifdef __cplusplus
#include "wx/string.h"
//通过名称获取资源大小
size_t wxRCGetSize(wxString name);

//通过名称获取资源指针
const unsigned char * wxRCGetHandle(wxString  name);

#endif // __cplusplus

#endif // WXRC_H
