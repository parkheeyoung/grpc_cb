// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef GRPC_CB_CLIENT_ASYNC_WRITER_IMPL_H
#define GRPC_CB_CLIENT_ASYNC_WRITER_IMPL_H

#include <string>

#include <grpc_cb/impl/channel_sptr.h>      // for ChannelSptr
#include <grpc_cb/impl/completion_queue_sptr.h>  // for CompletionQueueSptr
#include <grpc_cb/impl/message_sptr.h>           // for MessageSptr
#include <grpc_cb/support/config.h>              // for GRPC_FINAL

namespace grpc_cb {

class ClientAsyncWriterCloseHandler;
class ClientAsyncWriterImpl2;

// Only shared in ClientAsyncWriter, because we need dtr() to close writing.
class ClientAsyncWriterImpl GRPC_FINAL {
 public:
  ClientAsyncWriterImpl(const ChannelSptr& channel, const std::string& method,
                        const CompletionQueueSptr& cq_sptr);
  ~ClientAsyncWriterImpl();

  bool Write(const MessageSptr& request_sptr);

  // XXX extract it
  using CloseHandlerSptr = std::shared_ptr<ClientAsyncWriterCloseHandler>;
  void Close(const CloseHandlerSptr& handler_sptr);

  // Todo: Force to close, cancel all writing. CloseNow()
  // Todo: get queue size

 private:
  // Will live longer than ClientAsyncWriter.
  std::shared_ptr<ClientAsyncWriterImpl2> impl2_sptr_;
};  // class ClientAsyncWriterImpl

}  // namespace grpc_cb
#endif  // GRPC_CB_CLIENT_ASYNC_WRITER_IMPL_H
