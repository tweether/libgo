#ifndef _DLL_EXPORTER_H_
#define _DLL_EXPORTER_H_

#ifdef libgo_dynamic_EXPORTS
#define DLL_CLASS_DECL __declspec(dllexport)
#else
#define DLL_CLASS_DECL __declspec(dllimport)
#endif // libgo_dynamic_EXPORTS

#ifdef _WIN32
#ifdef __cplusplus
#define DLL_EXPORT_C_DECL extern "C" __declspec(dllexport)
#define DLL_IMPORT_C_DECL extern "C" __declspec(dllimport)
#define DLL_DECL extern DLL_CLASS_DECL
#define DLL_C_DECL extern "C" DLL_DECL
#else
#define DLL_DECL extern DLL_CLASS_DECL
#define DLL_C_DECL DLL_DECL
#endif //__cplusplus
#else
#define DLL_DECL extern
#ifdef __cplusplus
#define DLL_C_DECL extern "C"
#define DLL_CLASS_DECL
#endif //__cplusplus
#endif


#endif  //_DLL_EXPORTER_H_
