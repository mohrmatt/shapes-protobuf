

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from ShapeType.idl
using RTI Code Generator (rtiddsgen) version 4.6.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#include <iosfwd>
#include <iomanip>
#include <atomic>

#ifndef NDDS_STANDALONE_TYPE
#include "rti/topic/cdr/Serialization.hpp"
#include "ShapeTypePlugin.hpp"
#else
#include "rti/topic/cdr/SerializationHelpers.hpp"
#endif

#include "rti/topic/cdr/ProtobufInterpreterHelpers.hpp"

#include "ShapeType.hpp"

#include <rti/util/ostream_operators.hpp>

// ---- ShapeType:

constexpr RTIXCdrUnsignedLong ShapeType_g_tc_color_member_id = 1;
constexpr RTIXCdrUnsignedLong ShapeType_g_tc_x_member_id = 2;
constexpr RTIXCdrUnsignedLong ShapeType_g_tc_y_member_id = 3;
constexpr RTIXCdrUnsignedLong ShapeType_g_tc_shapesize_member_id = 4;

namespace rti {
    namespace topic {
    }
}

// --- Type traits: -------------------------------------------------

namespace rti { 
    namespace topic {

        namespace interpreter { namespace detail {

                template <>
                struct protobuf_message_access< ::ShapeType > {
                    using MessageType = ::ShapeType;
                    using MessageId = RTIXCdrUnsignedLong;
                    using FieldOffset = RTIXCdrUnsignedLong;
                    using FieldSize = RTIXCdrUnsignedLong;

                    static void allocate(MessageType & sample) {
                        field_init(::ShapeType_g_tc_color_member_id, sample);
                        field_init(::ShapeType_g_tc_x_member_id, sample);
                        field_init(::ShapeType_g_tc_y_member_id, sample);
                        field_init(::ShapeType_g_tc_shapesize_member_id, sample);
                    }

                    static void reset(MessageType & sample) {
                        sample.Clear();
                        allocate(sample);
                    }

                    static void copy(MessageType & dst, const MessageType & src) {
                        dst.CopyFrom(src);
                    }

                    static FieldOffset field_offset(const MessageId member_id) {
                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            return RTI_PROTOBUF_FIELD_OFFSET(MessageType, color_);
                            case ::ShapeType_g_tc_x_member_id:
                            return RTI_PROTOBUF_FIELD_OFFSET(MessageType, x_);
                            case ::ShapeType_g_tc_y_member_id:
                            return RTI_PROTOBUF_FIELD_OFFSET(MessageType, y_);
                            case ::ShapeType_g_tc_shapesize_member_id:
                            return RTI_PROTOBUF_FIELD_OFFSET(MessageType, shapesize_);
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static bool field_is_set(const MessageId member_id, const MessageType & sample) {

                        RTIOsapiUtility_unusedParameter(sample);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            return true;
                            case ::ShapeType_g_tc_x_member_id:
                            return true;
                            case ::ShapeType_g_tc_y_member_id:
                            return true;
                            case ::ShapeType_g_tc_shapesize_member_id:
                            return true;
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static void field_clear(const MessageId member_id, MessageType & sample) {

                        RTIOsapiUtility_unusedParameter(sample);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            return sample.clear_color();
                            case ::ShapeType_g_tc_x_member_id:
                            return sample.clear_x();
                            case ::ShapeType_g_tc_y_member_id:
                            return sample.clear_y();
                            case ::ShapeType_g_tc_shapesize_member_id:
                            return sample.clear_shapesize();
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static void field_init(const MessageId member_id, MessageType & sample) {

                        RTIOsapiUtility_unusedParameter(sample);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            // Initialize required field to its default value.
                            field_set_default(member_id, sample);
                            break;
                            case ::ShapeType_g_tc_x_member_id:
                            // Initialize required field to its default value.
                            field_set_default(member_id, sample);
                            break;
                            case ::ShapeType_g_tc_y_member_id:
                            // Initialize required field to its default value.
                            field_set_default(member_id, sample);
                            break;
                            case ::ShapeType_g_tc_shapesize_member_id:
                            // Initialize required field to its default value.
                            field_set_default(member_id, sample);
                            break;
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static void field_set_default(const MessageId member_id, MessageType & sample) {

                        RTIOsapiUtility_unusedParameter(sample);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            // Initialize string to its default value.
                            sample.set_color("");
                            break;
                            case ::ShapeType_g_tc_x_member_id:
                            // Initialize string to its default value.
                            sample.set_x(0);
                            break;
                            case ::ShapeType_g_tc_y_member_id:
                            // Initialize string to its default value.
                            sample.set_y(0);
                            break;
                            case ::ShapeType_g_tc_shapesize_member_id:
                            // Initialize string to its default value.
                            sample.set_shapesize(0);
                            break;
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static char* field_value_ptr(
                        const MessageId member_id,
                        MessageType & sample,
                        const FieldSize element_index,
                        const bool allocate_if_null,
                        bool & is_null,
                        bool & is_empty)
                    {

                        RTIOsapiUtility_unusedParameter(sample);
                        RTIOsapiUtility_unusedParameter(element_index);
                        RTIOsapiUtility_unusedParameter(allocate_if_null);
                        RTIOsapiUtility_unusedParameter(is_null);
                        RTIOsapiUtility_unusedParameter(is_empty);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            is_empty = sample.mutable_color()->empty();
                            return is_empty ? nullptr : reinterpret_cast<char*>(&(*sample.mutable_color())[element_index]);
                            case ::ShapeType_g_tc_x_member_id:
                            // Primitive and Enums fields can be accessed by pointer arithmetic.
                            is_empty = false;
                            return reinterpret_cast<char*>(&sample) + field_offset(member_id);
                            case ::ShapeType_g_tc_y_member_id:
                            // Primitive and Enums fields can be accessed by pointer arithmetic.
                            is_empty = false;
                            return reinterpret_cast<char*>(&sample) + field_offset(member_id);
                            case ::ShapeType_g_tc_shapesize_member_id:
                            // Primitive and Enums fields can be accessed by pointer arithmetic.
                            is_empty = false;
                            return reinterpret_cast<char*>(&sample) + field_offset(member_id);
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static FieldSize field_size(const MessageId member_id, const MessageType & sample) {

                        RTIOsapiUtility_unusedParameter(sample);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            return static_cast<RTIXCdrUnsignedLong>(sequence_length(sample.color()));
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static void field_resize(
                        const MessageId member_id,
                        MessageType & sample,
                        const FieldSize size,
                        const bool allocate_if_null)
                    {

                        RTIOsapiUtility_unusedParameter(sample);
                        RTIOsapiUtility_unusedParameter(size);
                        RTIOsapiUtility_unusedParameter(allocate_if_null);

                        FieldSize current_size = 0;
                        char * value_ptr = nullptr;
                        RTIOsapiUtility_unusedParameter(current_size);
                        RTIOsapiUtility_unusedParameter(value_ptr);

                        switch (member_id) {
                            case ::ShapeType_g_tc_color_member_id:
                            sequence_resize<std::string>::resize(*sample.mutable_color(), size);
                            break;
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                    static void field_set_element_value(
                        const MessageId member_id,
                        MessageType & sample,
                        const FieldSize index,
                        char * const element_ptr)
                    {

                        RTIOsapiUtility_unusedParameter(sample);
                        RTIOsapiUtility_unusedParameter(index);
                        RTIOsapiUtility_unusedParameter(element_ptr);

                        switch (member_id) {
                            default:
                            throw std::runtime_error("unsupported member id");
                        }
                    }

                }; // protobuf_message_access< ::ShapeType >

            } } // namespace interpreter::detail

        template<>
        struct native_type_code< ::ShapeType > {

            static DDS_TypeCode * get()
            {
                using namespace ::rti::topic::interpreter;

                static std::atomic_bool is_initialized {false};

                static DDS_TypeCode ShapeType_g_tc_color_string;

                static DDS_TypeCode_Member ShapeType_g_tc_members[4]=
                {

                    {
                        (char *)"color",/* Member name */
                        {
                            1,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"x",/* Member name */
                        {
                            2,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"y",/* Member name */
                        {
                            3,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }, 
                    {
                        (char *)"shapesize",/* Member name */
                        {
                            4,/* Representation ID */
                            DDS_BOOLEAN_FALSE,/* Is a pointer? */
                            -1, /* Bitfield bits */
                            NULL/* Member type code is assigned later */
                        },
                        0, /* Ignored */
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        RTI_CDR_REQUIRED_MEMBER, /* Is a key? */
                        DDS_PUBLIC_MEMBER,/* Member visibility */
                        RTICdrTypeCodeAnnotations_INITIALIZER
                    }
                };

                static DDS_TypeCode ShapeType_g_tc =
                {{
                        DDS_TK_STRUCT| DDS_TK_MUTABLE_EXTENSIBILITY, /* Kind */
                        DDS_BOOLEAN_FALSE, /* Ignored */
                        -1, /*Ignored*/
                        (char *)"ShapeType", /* Name */
                        NULL, /* Ignored */      
                        0, /* Ignored */
                        0, /* Ignored */
                        NULL, /* Ignored */
                        4, /* Number of members */
                        ShapeType_g_tc_members, /* Members */
                        DDS_VM_NONE, /* Ignored */
                        RTICdrTypeCodeAnnotations_INITIALIZER,
                        DDS_BOOLEAN_TRUE, /* _isCopyable */
                        NULL, /* _sampleAccessInfo: assigned later */
                        NULL /* _typePlugin: assigned later */
                    }}; /* Type code for ShapeType*/

                if (is_initialized.load(std::memory_order_acquire)) {
                    return &ShapeType_g_tc;
                }

                ShapeType_g_tc_color_string = initialize_protobuf_string_member_typecode< ::ShapeType >();

                ShapeType_g_tc._data._annotations._allowedDataRepresentationMask = 5;

                ShapeType_g_tc_members[0]._representation._typeCode = (RTICdrTypeCode *)&ShapeType_g_tc_color_string;
                ShapeType_g_tc_members[1]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;
                ShapeType_g_tc_members[2]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;
                ShapeType_g_tc_members[3]._representation._typeCode = (RTICdrTypeCode *)&DDS_g_tc_long;

                /* Initialize the values for member annotations. */
                ShapeType_g_tc_members[0]._annotations._defaultValue._d = RTI_XCDR_TK_STRING;
                ShapeType_g_tc_members[0]._annotations._defaultValue._u.string_value = (DDS_Char *) "";
                ShapeType_g_tc_members[1]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[1]._annotations._defaultValue._u.long_value = 0;
                ShapeType_g_tc_members[1]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[1]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                ShapeType_g_tc_members[1]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[1]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;
                ShapeType_g_tc_members[2]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[2]._annotations._defaultValue._u.long_value = 0;
                ShapeType_g_tc_members[2]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[2]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                ShapeType_g_tc_members[2]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[2]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;
                ShapeType_g_tc_members[3]._annotations._defaultValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[3]._annotations._defaultValue._u.long_value = 0;
                ShapeType_g_tc_members[3]._annotations._minValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[3]._annotations._minValue._u.long_value = RTIXCdrLong_MIN;
                ShapeType_g_tc_members[3]._annotations._maxValue._d = RTI_XCDR_TK_LONG;
                ShapeType_g_tc_members[3]._annotations._maxValue._u.long_value = RTIXCdrLong_MAX;

                // This TypeCode does not have a valid SampleAccessInfo beyond member offsets,
                // because protobuf TypeCode must always be instantiated with one of the wrapper
                // templates to provide a SampleAccessInfo "relative" to the parent type.
                ShapeType_g_tc._data._sampleAccessInfo = sample_access_info();
                ShapeType_g_tc._data._typePlugin = type_plugin_info();

                is_initialized.store(true, std::memory_order_release);

                return &ShapeType_g_tc;
            }

            static RTIXCdrSampleAccessInfo * sample_access_info()
            {
                static std::atomic_bool is_initialized {false};

                static RTIXCdrMemberAccessInfo ShapeType_g_memberAccessInfos[4] =
                {RTIXCdrMemberAccessInfo_INITIALIZER};

                static RTIXCdrSampleAccessInfo ShapeType_g_sampleAccessInfo = 
                RTIXCdrSampleAccessInfo_INITIALIZER;

                if (is_initialized.load(std::memory_order_acquire)) {
                    return (RTIXCdrSampleAccessInfo*) &ShapeType_g_sampleAccessInfo;
                }

                ShapeType_g_memberAccessInfos[0].bindingMemberValueOffset[0] =
                interpreter::detail::protobuf_message_access< ::ShapeType >::field_offset(::ShapeType_g_tc_color_member_id);

                ShapeType_g_memberAccessInfos[1].bindingMemberValueOffset[0] =
                interpreter::detail::protobuf_message_access< ::ShapeType >::field_offset(::ShapeType_g_tc_x_member_id);

                ShapeType_g_memberAccessInfos[2].bindingMemberValueOffset[0] =
                interpreter::detail::protobuf_message_access< ::ShapeType >::field_offset(::ShapeType_g_tc_y_member_id);

                ShapeType_g_memberAccessInfos[3].bindingMemberValueOffset[0] =
                interpreter::detail::protobuf_message_access< ::ShapeType >::field_offset(::ShapeType_g_tc_shapesize_member_id);

                ShapeType_g_sampleAccessInfo.memberAccessInfos = 
                ShapeType_g_memberAccessInfos;

                {
                    size_t candidateTypeSize = sizeof(::ShapeType);

                    if (candidateTypeSize > RTIXCdrLong_MAX) {
                        ShapeType_g_sampleAccessInfo.typeSize[0] =
                        RTIXCdrLong_MAX;
                    } else {
                        ShapeType_g_sampleAccessInfo.typeSize[0] =
                        (RTIXCdrUnsignedLong) candidateTypeSize;
                    }
                }

                ShapeType_g_sampleAccessInfo.useGetMemberValueOnlyWithRef =
                RTI_XCDR_FALSE;

                ShapeType_g_sampleAccessInfo.getMemberValuePointerFcn = 
                interpreter::get_aggregation_value_pointer< ::ShapeType >;

                ShapeType_g_sampleAccessInfo.languageBinding = 
                RTI_XCDR_TYPE_BINDING_PROTOBUF_CPP_11_STL;

                is_initialized.store(true, std::memory_order_release);
                return (RTIXCdrSampleAccessInfo*) &ShapeType_g_sampleAccessInfo;
            }
            static RTIXCdrTypePlugin * type_plugin_info()
            {
                static RTIXCdrTypePlugin ShapeType_g_typePlugin = 
                {
                    NULL, /* serialize */
                    NULL, /* serialize_key */
                    NULL, /* deserialize_sample */
                    NULL, /* deserialize_key_sample */
                    NULL, /* skip */
                    NULL, /* get_serialized_sample_size */
                    NULL, /* get_serialized_sample_max_size_ex */
                    NULL, /* get_serialized_key_max_size_ex */
                    NULL, /* get_serialized_sample_min_size */
                    NULL, /* serialized_sample_to_key */
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL
                };

                return &ShapeType_g_typePlugin;
            }
        }; // native_type_code

        const ::dds::core::xtypes::StructType& dynamic_type< ::ShapeType >::get()
        {
            return static_cast<const ::dds::core::xtypes::StructType&>(
                ::rti::core::native_conversions::cast_from_native< ::dds::core::xtypes::DynamicType >(
                    *(native_type_code< ::ShapeType >::get())));
        }
    }
}

namespace dds { 
    namespace topic {
        void topic_type_support< ::ShapeType >:: register_type(
            ::dds::domain::DomainParticipant& participant,
            const std::string& type_name) 
        {

            ::rti::domain::register_type_plugin(
                participant,
                type_name,
                ::ShapeTypePlugin_new,
                ::ShapeTypePlugin_delete);
        }

        std::vector<char>& topic_type_support< ::ShapeType >::to_cdr_buffer(
            std::vector<char>& buffer, 
            const ::ShapeType& sample,
            ::dds::core::policy::DataRepresentationId representation)
        {
            // First get the length of the buffer
            unsigned int length = 0;
            RTIBool ok = ShapeTypePlugin_serialize_to_cdr_buffer(
                NULL, 
                &length,
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to calculate cdr buffer size");

            // Create a vector with that size and copy the cdr buffer into it
            buffer.resize(length);
            ok = ShapeTypePlugin_serialize_to_cdr_buffer(
                &buffer[0], 
                &length, 
                &sample,
                representation);
            ::rti::core::check_return_code(
                ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
                "Failed to copy cdr buffer");

            return buffer;
        }

        void topic_type_support< ::ShapeType >::from_cdr_buffer(::ShapeType& sample, 
        const std::vector<char>& buffer)
        {

            RTIBool ok  = ShapeTypePlugin_deserialize_from_cdr_buffer(
                &sample, 
                &buffer[0], 
                static_cast<unsigned int>(buffer.size()));
            ::rti::core::check_return_code(ok ? DDS_RETCODE_OK : DDS_RETCODE_ERROR,
            "Failed to create ::ShapeType from cdr buffer");
        }

        void topic_type_support< ::ShapeType >::reset_sample(::ShapeType& sample) 
        {
            ::rti::topic::interpreter::detail::protobuf_message_access< ::ShapeType >::reset(sample);
        }

        void topic_type_support< ::ShapeType >::allocate_sample(::ShapeType& sample, int, int) 
        {
            ::rti::topic::interpreter::detail::protobuf_message_access< ::ShapeType >::allocate(sample);
        }
    }
}  

