// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef GRPC_CB_SERVICE_H
#define GRPC_CB_SERVICE_H

#include <string>

#include <grpc/grpc.h>  // for grpc_server_register_method_payload_handling

#include <grpc_cb/impl/call_sptr.h>  // for CallSptr
#include <grpc_cb/impl/completion_queue_sptr.h>  // for CompletionQueueSptr
#include <grpc_cb/support/protobuf_fwd.h>  // for ServiceDescriptor

struct grpc_byte_buffer;

namespace grpc_cb {

// Service base class.
class Service {
 public:
  const std::string& GetFullName() const;
  size_t GetMethodCount() const;
  grpc_server_register_method_payload_handling GetMethodPayloadHandling(
    size_t method_index) const;

 public:
  virtual const std::string& GetMethodName(size_t method_index) const = 0;

  // TODO: need request_context. Need client address in Ctr?
  virtual void CallMethod(
      size_t method_index, grpc_byte_buffer& request,
      const CallSptr& call_sptr, const CompletionQueueSptr& cq_sptr) = 0;

 private:
  virtual const ::google::protobuf::ServiceDescriptor& GetDescriptor()
      const = 0;
};

}  // namespace grpc_cb

#endif  // GRPC_CB_SERVICE_H
