/**
 * Autogenerated by Thrift Compiler (0.13.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef movies_TYPES_H
#define movies_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>


namespace movies {

struct ErrorCode {
  enum type {
    SE_CONNPOOL_TIMEOUT = 0,
    SE_THRIFT_CONN_ERROR = 1,
    SE_UNAUTHORIZED = 2,
    SE_MEMCACHED_ERROR = 3,
    SE_MONGODB_ERROR = 4,
    SE_REDIS_ERROR = 5,
    SE_THRIFT_HANDLER_ERROR = 6,
    SE_RABBITMQ_CONN_ERROR = 7
  };
};

extern const std::map<int, const char*> _ErrorCode_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const ErrorCode::type& val);

std::string to_string(const ErrorCode::type& val);

class ServiceException;

typedef struct _ServiceException__isset {
  _ServiceException__isset() : errorCode(false), message(false) {}
  bool errorCode :1;
  bool message :1;
} _ServiceException__isset;

class ServiceException : public ::apache::thrift::TException {
 public:

  ServiceException(const ServiceException&);
  ServiceException& operator=(const ServiceException&);
  ServiceException() : errorCode((ErrorCode::type)0), message() {
  }

  virtual ~ServiceException() noexcept;
  ErrorCode::type errorCode;
  std::string message;

  _ServiceException__isset __isset;

  void __set_errorCode(const ErrorCode::type val);

  void __set_message(const std::string& val);

  bool operator == (const ServiceException & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const ServiceException &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ServiceException & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const noexcept;
};

void swap(ServiceException &a, ServiceException &b);

std::ostream& operator<<(std::ostream& out, const ServiceException& obj);

} // namespace

#endif
