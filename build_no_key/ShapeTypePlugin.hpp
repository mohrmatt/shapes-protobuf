

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from ShapeType.idl
using RTI Code Generator (rtiddsgen) version 4.6.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef ShapeTypePlugin_434673798_h
#define ShapeTypePlugin_434673798_h

#include "ShapeType.hpp"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if defined(NDDS_USER_DLL_EXPORT) && defined(RTI_WIN32)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#if !defined(RTI_WIN32) && defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __attribute__((visibility("default")))
#endif

#define ShapeTypePlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define ShapeTypePlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define ShapeTypePlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define ShapeTypePlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define ShapeTypePlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern ShapeType*
ShapeTypePluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern ShapeType*
ShapeTypePluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern ShapeType*
ShapeTypePluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
ShapeTypePluginSupport_copy_data(
    ShapeType *out,
    const ShapeType *in);

NDDSUSERDllExport extern void 
ShapeTypePluginSupport_destroy_data_w_params(
    ShapeType *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
ShapeTypePluginSupport_destroy_data_ex(
    ShapeType *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
ShapeTypePluginSupport_destroy_data(
    ShapeType *sample);

NDDSUSERDllExport extern void 
ShapeTypePluginSupport_print_data(
    const ShapeType *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
ShapeTypePlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
ShapeTypePlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
ShapeTypePlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
ShapeTypePlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
ShapeTypePlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    ShapeType *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
ShapeTypePlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    ShapeType *out,
    const ShapeType *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
ShapeTypePlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const ShapeType *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
ShapeTypePlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    ShapeType **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
ShapeTypePlugin_deserialize_from_cdr_buffer(
    ShapeType *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
ShapeTypePlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
ShapeTypePlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
ShapeTypePlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
ShapeTypePlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
ShapeTypePlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    ShapeType ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *cdrStream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
ShapeTypePlugin_new(void);

NDDSUSERDllExport extern void
ShapeTypePlugin_delete(struct PRESTypePlugin *);

#if defined(NDDS_USER_DLL_EXPORT) || defined(NDDS_USER_SYMBOL_EXPORT)
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* ShapeTypePlugin_434673798_h */
