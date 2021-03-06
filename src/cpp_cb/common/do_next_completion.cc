// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "do_next_completion.h"

#include <grpc/impl/codegen/grpc_types.h>  // for grpc_event

#include <grpc_cb/impl/completion_queue.h>      // for CompletionQueue
#include <grpc_cb/impl/completion_queue_tag.h>  // for CompletionQueueTag

namespace grpc_cb {

// Blocking go next and complete.
// Return false if shutdown.
bool DoNextCompletion(CompletionQueue& cq) {
    grpc_event ev = cq.Next();
    switch (ev.type) {
      case GRPC_OP_COMPLETE: {
        auto* tag = static_cast<CompletionQueueTag*>(ev.tag);
        assert(tag);
        tag->DoComplete(0 != ev.success);
        delete tag;  // created in RequestMethodCall()
        break;
      }  // case
      case GRPC_QUEUE_SHUTDOWN:
        return true;
      case GRPC_QUEUE_TIMEOUT:
        assert(false);
        break;
      default:
        assert(false);
        break;
    }  // switch
    return false;
}

}  // namespace grpv_cb
