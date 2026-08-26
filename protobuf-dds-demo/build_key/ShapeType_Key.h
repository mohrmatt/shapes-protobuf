

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from ShapeType_Key.idl
using RTI Code Generator (rtiddsgen) version 4.6.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef ShapeType_Key_976076619_h
#define ShapeType_Key_976076619_h

#ifndef NDDS_STANDALONE_TYPE
#ifndef ndds_cpp_h
#include "ndds/ndds_cpp.h"
#endif
#include "rti/xcdr/Interpreter.hpp"
#include "cdr/cdr_typeCode.h"
#else
#include "ndds_standalone_type.h"
#endif

class ShapeType;

#if defined(NDDS_USER_DLL_EXPORT) && defined(RTI_WIN32)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#if !defined(RTI_WIN32) && defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __attribute__((visibility("default")))
#endif

DDS_SEQUENCE(ShapeTypeSeq, ShapeType);

#if defined(NDDS_USER_DLL_EXPORT) || defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

extern "C" {

    extern const char *ShapeTypeTYPENAME;

}

struct ShapeTypeSeq;
#ifndef NDDS_STANDALONE_TYPE
class ShapeTypeTypeSupport;
class ShapeTypeDataWriter;
class ShapeTypeDataReader;
#endif
class ShapeType 
{
  public:
    typedef struct ShapeTypeSeq Seq;
    #ifndef NDDS_STANDALONE_TYPE
    typedef ShapeTypeTypeSupport TypeSupport;
    typedef ShapeTypeDataWriter DataWriter;
    typedef ShapeTypeDataReader DataReader;
    #endif

    DDS_Char * color;
    DDS_Long x;
    DDS_Long y;
    DDS_Long shapesize;

};
#if defined(NDDS_USER_DLL_EXPORT) && defined(RTI_WIN32)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#if !defined(RTI_WIN32) && defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __attribute__((visibility("default")))
#endif

#ifndef NDDS_STANDALONE_TYPE
NDDSUSERDllExport DDS_TypeCode * ShapeType_get_typecode(void); /* Type code */
NDDSUSERDllExport RTIXCdrTypePlugin *ShapeType_get_type_plugin_info(void);
NDDSUSERDllExport RTIXCdrSampleAccessInfo *ShapeType_get_sample_access_info(void);
NDDSUSERDllExport RTIXCdrSampleAccessInfo *ShapeType_get_sample_seq_access_info(void);
#endif

NDDSUSERDllExport
RTIBool ShapeType_initialize(
    ShapeType* self);

NDDSUSERDllExport
RTIBool ShapeType_initialize_ex(
    ShapeType* self,RTIBool allocatePointers,RTIBool allocateMemory);

NDDSUSERDllExport
RTIBool ShapeType_initialize_w_params(
    ShapeType* self,
    const struct DDS_TypeAllocationParams_t * allocParams);  

NDDSUSERDllExport
RTIBool ShapeType_finalize_w_return(
    ShapeType* self);

NDDSUSERDllExport
void ShapeType_finalize(
    ShapeType* self);

NDDSUSERDllExport
void ShapeType_finalize_ex(
    ShapeType* self,RTIBool deletePointers);

NDDSUSERDllExport
void ShapeType_finalize_w_params(
    ShapeType* self,
    const struct DDS_TypeDeallocationParams_t * deallocParams);

NDDSUSERDllExport
void ShapeType_finalize_optional_members(
    ShapeType* self, RTIBool deletePointers);  

NDDSUSERDllExport
RTIBool ShapeType_copy(
    ShapeType* dst,
    const ShapeType* src);

#if defined(NDDS_USER_DLL_EXPORT) || defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#ifndef NDDS_STANDALONE_TYPE
namespace rti { 
    namespace xcdr {
        template <>
        struct type_code< ShapeType> {
            static const RTIXCdrTypeCode * get();
        };

    } 
}

#endif

#endif /* ShapeType_Key */
