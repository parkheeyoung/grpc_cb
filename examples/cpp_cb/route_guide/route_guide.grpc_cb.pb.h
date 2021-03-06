// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: route_guide.proto
#ifndef GRPC_CB_route_5fguide_2eproto__INCLUDED
#define GRPC_CB_route_5fguide_2eproto__INCLUDED

#include <grpc_cb/grpc_cb.h>  // Include all for user.

#include "route_guide.pb.h"

// package routeguide
namespace routeguide {

namespace RouteGuide {  // service RouteGuide

const ::google::protobuf::ServiceDescriptor& GetServiceDescriptor();

class Stub : public ::grpc_cb::ServiceStub {
 public:
  explicit Stub(const ::grpc_cb::ChannelSptr& channel);

  inline ::grpc_cb::Status BlockingGetFeature(
       const ::routeguide::Point& request) {
    ::routeguide::Feature response;
    return BlockingGetFeature(request, &response);  // Ignore response.
  }
  ::grpc_cb::Status BlockingGetFeature(
      const ::routeguide::Point& request,
      ::routeguide::Feature* response);

  using GetFeatureCallback =
      std::function<void (const ::routeguide::Feature& response)>;
  inline void AsyncGetFeature(
      const ::routeguide::Point& request) {
    return AsyncGetFeature(request, &IgnoreResponse<::routeguide::Feature>);
  }
  inline void AsyncGetFeature(
      const ::routeguide::Point& request,
      const GetFeatureCallback& cb) {
    return AsyncGetFeature(request, cb, GetErrorCallback());  // Use default error callback.
  }
  void AsyncGetFeature(
      const ::routeguide::Point& request,
      const GetFeatureCallback& cb,
      const ::grpc_cb::ErrorCallback& ecb);

  ::grpc_cb::ClientSyncReader<::routeguide::Feature>
  SyncListFeatures(const ::routeguide::Rectangle& request);

  using ListFeaturesMsgCb = std::function<
      void(const ::routeguide::Feature&)>;
  void AsyncListFeatures(const ::routeguide::Rectangle& request,
      const ListFeaturesMsgCb& on_msg = ListFeaturesMsgCb(),
      const ::grpc_cb::StatusCallback& on_status = ::grpc_cb::StatusCallback());

  ::grpc_cb::ClientSyncWriter<::routeguide::Point>
  SyncRecordRoute();

  ::grpc_cb::ClientAsyncWriter<
      ::routeguide::Point,
      ::routeguide::RouteSummary>
  AsyncRecordRoute();

  ::grpc_cb::ClientSyncReaderWriter<
      ::routeguide::RouteNote,
      ::routeguide::RouteNote>
  SyncRouteChat();

  ::grpc_cb::ClientAsyncReaderWriter<
      ::routeguide::RouteNote,
      ::routeguide::RouteNote>
  AsyncRouteChat(const ::grpc_cb::StatusCallback& on_status);

};  // class Stub

std::unique_ptr<Stub> NewStub(const ::grpc_cb::ChannelSptr& channel);

class Service : public ::grpc_cb::Service {
 public:
  Service();
  virtual ~Service();

  virtual const std::string& GetMethodName(size_t i) const GRPC_OVERRIDE;
  virtual void CallMethod(size_t method_index, grpc_byte_buffer& request_buffer,
                          const ::grpc_cb::CallSptr& call_sptr,
                          const ::grpc_cb::CompletionQueueSptr& cq_sptr)
      GRPC_OVERRIDE;

 protected:
  using GetFeature_Replier = ::grpc_cb::ServerReplier<
      ::routeguide::Feature>;
  void GetFeature(
      grpc_byte_buffer& request_buffer,
      const GetFeature_Replier& replier);
  // Todo: virtual void GetFeature(const std::string& request_buffer, replier);
  virtual void GetFeature(
      const ::routeguide::Point& request,
      GetFeature_Replier replier);

  using ListFeatures_Writer = ::grpc_cb::ServerWriter<
      ::routeguide::Feature>;
  void ListFeatures(grpc_byte_buffer& request_buffer,
      const ListFeatures_Writer& writer);
  virtual void ListFeatures(
      const ::routeguide::Rectangle& request,
      const ListFeatures_Writer& writer);

  void RecordRoute(const ::grpc_cb::CallSptr& call_sptr);
  using RecordRoute_Replier = ::grpc_cb::ServerReplier<
      ::routeguide::RouteSummary>;
  virtual void RecordRoute_OnStart(
      const RecordRoute_Replier& replier);
  virtual void RecordRoute_OnMsg(
      const ::routeguide::Point& point,
      const RecordRoute_Replier& replier);
  virtual void RecordRoute_OnEnd(
      const RecordRoute_Replier& replier);

  void RouteChat(const ::grpc_cb::CallSptr& call_sptr,
                 const ::grpc_cb::CompletionQueueSptr& cq_sptr);
  using RouteChat_Writer = ::grpc_cb::ServerWriter<::routeguide::RouteNote>;
  virtual void RouteChat_OnStart(
      const RouteChat_Writer& writer);
  virtual void RouteChat_OnMsg(
      const ::routeguide::RouteNote& msg,
      const RouteChat_Writer& writer);
  virtual void RouteChat_OnEnd(
      const RouteChat_Writer& writer);

 private:
  virtual const ::google::protobuf::ServiceDescriptor& GetDescriptor()
      const GRPC_OVERRIDE {
    return GetServiceDescriptor();
  }
};

}  // namespace RouteGuide

}  // namespace routeguide

#endif  // GRPC_CB_route_5fguide_2eproto__INCLUDED
