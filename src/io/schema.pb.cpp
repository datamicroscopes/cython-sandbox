// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: microscopes/io/schema.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "microscopes/io/schema.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace microscopes {
namespace io {

namespace {

const ::google::protobuf::Descriptor* CRP_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  CRP_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_microscopes_2fio_2fschema_2eproto() {
  protobuf_AddDesc_microscopes_2fio_2fschema_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "microscopes/io/schema.proto");
  GOOGLE_CHECK(file != NULL);
  CRP_descriptor_ = file->message_type(0);
  static const int CRP_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CRP, alpha_),
  };
  CRP_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      CRP_descriptor_,
      CRP::default_instance_,
      CRP_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CRP, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(CRP, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(CRP));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_microscopes_2fio_2fschema_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    CRP_descriptor_, &CRP::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_microscopes_2fio_2fschema_2eproto() {
  delete CRP::default_instance_;
  delete CRP_reflection_;
}

void protobuf_AddDesc_microscopes_2fio_2fschema_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\033microscopes/io/schema.proto\022\016microscop"
    "es.io\"\024\n\003CRP\022\r\n\005alpha\030\001 \002(\002", 67);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "microscopes/io/schema.proto", &protobuf_RegisterTypes);
  CRP::default_instance_ = new CRP();
  CRP::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_microscopes_2fio_2fschema_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_microscopes_2fio_2fschema_2eproto {
  StaticDescriptorInitializer_microscopes_2fio_2fschema_2eproto() {
    protobuf_AddDesc_microscopes_2fio_2fschema_2eproto();
  }
} static_descriptor_initializer_microscopes_2fio_2fschema_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int CRP::kAlphaFieldNumber;
#endif  // !_MSC_VER

CRP::CRP()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void CRP::InitAsDefaultInstance() {
}

CRP::CRP(const CRP& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void CRP::SharedCtor() {
  _cached_size_ = 0;
  alpha_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

CRP::~CRP() {
  SharedDtor();
}

void CRP::SharedDtor() {
  if (this != default_instance_) {
  }
}

void CRP::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* CRP::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return CRP_descriptor_;
}

const CRP& CRP::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_microscopes_2fio_2fschema_2eproto();
  return *default_instance_;
}

CRP* CRP::default_instance_ = NULL;

CRP* CRP::New() const {
  return new CRP;
}

void CRP::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    alpha_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool CRP::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required float alpha = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   float, ::google::protobuf::internal::WireFormatLite::TYPE_FLOAT>(
                 input, &alpha_)));
          set_has_alpha();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void CRP::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required float alpha = 1;
  if (has_alpha()) {
    ::google::protobuf::internal::WireFormatLite::WriteFloat(1, this->alpha(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* CRP::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required float alpha = 1;
  if (has_alpha()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFloatToArray(1, this->alpha(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int CRP::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required float alpha = 1;
    if (has_alpha()) {
      total_size += 1 + 4;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void CRP::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const CRP* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const CRP*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void CRP::MergeFrom(const CRP& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_alpha()) {
      set_alpha(from.alpha());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void CRP::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CRP::CopyFrom(const CRP& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CRP::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void CRP::Swap(CRP* other) {
  if (other != this) {
    std::swap(alpha_, other->alpha_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata CRP::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = CRP_descriptor_;
  metadata.reflection = CRP_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace io
}  // namespace microscopes

// @@protoc_insertion_point(global_scope)
