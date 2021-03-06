// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "client_async_reader_helper.h"

#include <cassert>  // for assert()

#include <grpc_cb/impl/client/client_async_read_handler.h>  // for HandleMsg()

#include "client_reader_async_read_cqtag.h"  // for ClientReaderAsyncReadCqTag

namespace grpc_cb {

ClientAsyncReaderHelper::ClientAsyncReaderHelper(
    CompletionQueueSptr cq_sptr, CallSptr call_sptr,
    const AtomicBoolSptr& status_ok_sptr,
    const ClientAsyncReadHandlerSptr& read_handler_sptr,
    const OnEnd& on_end)
    : cq_sptr_(cq_sptr),
      call_sptr_(call_sptr),
      status_ok_sptr_(status_ok_sptr),
      read_handler_sptr_(read_handler_sptr),
      on_end_(on_end) {
  assert(cq_sptr);
  assert(call_sptr);
  assert(status_ok_sptr);
  assert(on_end);
}

ClientAsyncReaderHelper::~ClientAsyncReaderHelper() {}

// Setup to read each.
void ClientAsyncReaderHelper::Start() {
  if (started_) return;
  started_ = true;
  Next();
}

// Setup next async read.
void ClientAsyncReaderHelper::Next() {
  assert(started_);
  if (!(*status_ok_sptr_)) return;

  auto sptr = shared_from_this();
  auto* tag = new ClientReaderAsyncReadCqTag(sptr);
  if (tag->Start()) return;

  delete tag;
  status_.SetInternalError("Failed to async read server stream.");
  *status_ok_sptr_ = false;  // XXX return status to parent
  // DEL if (on_status_) on_status_(status_);  // XXX no on_status in ReaderHelper
}

void ClientAsyncReaderHelper::OnRead(ClientReaderAsyncReadCqTag& tag) {
  if (!(*status_ok_sptr_))
    return;
  assert(status_.ok());
  if (!tag.HasGotMsg()) {
    // End of read.
    // XXX AsyncRecvStatus();
    // XXX Do not recv status in Reader. Do it after all reading and writing.
    return;
  }

  status_ = tag.GetResultMsg(read_handler_sptr_->GetMsg());
  if (status_.ok()) {
    read_handler_sptr_->HandleMsg();
    Next();
    return;
  }

  *status_ok_sptr_ = false;
  // XXX CallOnEnd(status);
}

//void ClientAsyncReaderHelper::AsyncRecvStatus() {
//  assert(status_sptr_->ok());
//
//  // XXX input status_sptr_ to CqTag? To abort writing?
//  auto* tag = new ClientReaderAsyncRecvStatusCqTag(call_sptr_, on_status_);
//  if (tag->Start()) return;
//
//  delete tag;
//  status_sptr_->SetInternalError("Failed to receive status.");
//  if (on_status_) on_status_(*status_sptr_);
//}

#if 0
template <class Response>
inline void OnReadEach(const Response& msg,
    const ClientReaderDataSptr<Response>& data_sptr) {
  Status& status = data_sptr->status;
  assert(status.ok());

  std::function<void(const Response&)>& on_msg = data_sptr->on_msg;
  if (on_msg) on_msg(msg);

  AsyncReadNext(data_sptr);
  // Old tag will be deleted after return in BlockingRun().
}

template <class Response>
inline void OnEnd(const Status& status,
    const ClientReaderDataSptr<Response>& data_sptr) {
  StatusCallback& on_status = data_sptr->on_status;
  if (status.ok()) {
    AsyncRecvStatus(data_sptr->call_sptr,
        data_sptr->status, on_status);
    return;
  }

  if (on_status) on_status(status);
}
#endif

}  // namespace grpc_cb
