#ifndef _LIBGO_EXPORTER_H_
#define _LIBGO_EXPORTER_H_

#ifdef _WIN32
    #ifdef LIBGO_EXPORT
        #define LIBGO_CLASS_DECL __declspec(dllexport)
    #else
        #define LIBGO_CLASS_DECL __declspec(dllimport)
    #endif // LIBGO_EXPORT
    #ifdef __cplusplus
        #define LIBGO_EXPORT_C_DECL extern "C" __declspec(dllexport)
        #define LIBGO_IMPORT_C_DECL extern "C" __declspec(dllimport)
        #define LIBGO_DECL extern LIBGO_CLASS_DECL
        #define LIBGO_C_DECL extern "C" LIBGO_DECL
    #else
        #define LIBGO_DECL extern DLL_CLASS_DECL
        #define LIBGO_C_DECL LIBGO_DECL
    #endif //__cplusplus
#else
    #define LIBGO_DECL extern
    #ifdef __cplusplus
        #define LIBGO_EXPORT_C_DECL extern "C"
        #define LIBGO_C_DECL extern "C"
        // #define LIBGO_CLASS_DECL __attribute__((visibility("default")))
        #define LIBGO_CLASS_DECL
    #else
        #define LIBGO_EXPORT_C_DECL extern
        #define LIBGO_C_DECL extern
        // #define LIBGO_CLASS_DECL __attribute__((visibility("default")))
        #define LIBGO_CLASS_DECL
    #endif //__cplusplus
#endif


#endif  //_LIBGO_EXPORTER_H_
