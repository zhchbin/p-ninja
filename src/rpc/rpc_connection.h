// Copyright (c) 2015 Chaobin Zhang. All rights reserved.
// Use of this source code is governed by the BSD license that can be
// found in the LICENSE file.

#ifndef  RPC_RPC_CONNECTION_H_
#define  RPC_RPC_CONNECTION_H_

#include <queue>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/io_buffer.h"

namespace net {
class StreamSocket;
}  // namespace net

namespace rpc {

// A container which has all information of a rpc connection. It includes
// id, underlying socket, and pending read/write data.
class RpcConnection {
 public:
  // IOBuffer for data read.  It's a wrapper around GrowableIOBuffer, with more
  // functions for buffer management.  It moves unconsumed data to the start of
  // buffer.
  class ReadIOBuffer : public net::IOBuffer {
   public:
    static const int kInitialBufSize = 1024;
    static const int kMinimumBufSize = 128;
    static const int kCapacityIncreaseFactor = 2;
    static const int kDefaultMaxBufferSize = 1 * 1024 * 1024;  // 1 Mbytes.

    ReadIOBuffer();

    // Capacity.
    int GetCapacity() const;
    void SetCapacity(int capacity);
    // Increases capacity and returns true if capacity is not beyond the limit.
    bool IncreaseCapacity();

    // Start of read data.
    char* StartOfBuffer() const;
    // Returns the bytes of read data.
    int GetSize() const;
    // More read data was appended.
    void DidRead(int bytes);
    // Capacity for which more read data can be appended.
    int RemainingCapacity() const;

    // Removes consumed data and moves unconsumed data to the start of buffer.
    void DidConsume(int bytes);

    // Limit of how much internal capacity can increase.
    int max_buffer_size() const { return max_buffer_size_; }
    void set_max_buffer_size(int max_buffer_size) {
      max_buffer_size_ = max_buffer_size;
    }

   private:
    ~ReadIOBuffer() override;

    scoped_refptr<net::GrowableIOBuffer> base_;
    int max_buffer_size_;

    DISALLOW_COPY_AND_ASSIGN(ReadIOBuffer);
  };

  // IOBuffer of pending data to write which has a queue of pending data. Each
  // pending data is stored in std::string.  data() is the data of first
  // std::string stored.
  class QueuedWriteIOBuffer : public net::IOBuffer {
   public:
    static const int kDefaultMaxBufferSize = 1 * 1024 * 1024;  // 1 Mbytes.

    QueuedWriteIOBuffer();

    // Whether or not pending data exists.
    bool IsEmpty() const;

    // Appends new pending data and returns true if total size doesn't exceed
    // the limit, |total_size_limit_|.  It would change data() if new data is
    // the first pending data.
    bool Append(const std::string& data);

    // Consumes data and changes data() accordingly.  It cannot be more than
    // GetSizeToWrite().
    void DidConsume(int size);

    // Gets size of data to write this time. It is NOT total data size.
    int GetSizeToWrite() const;

    // Total size of all pending data.
    int total_size() const { return total_size_; }

    // Limit of how much data can be pending.
    int max_buffer_size() const { return max_buffer_size_; }
    void set_max_buffer_size(int max_buffer_size) {
      max_buffer_size_ = max_buffer_size;
    }

   private:
    ~QueuedWriteIOBuffer() override;

    std::queue<std::string> pending_data_;
    int total_size_;
    int max_buffer_size_;

    DISALLOW_COPY_AND_ASSIGN(QueuedWriteIOBuffer);
  };

  RpcConnection(int id, scoped_ptr<net::StreamSocket> socket);
  ~RpcConnection();

  int id() const { return id_; }
  net::StreamSocket* socket() { return socket_.get(); }
  ReadIOBuffer* read_buf() const { return read_buf_.get(); }
  QueuedWriteIOBuffer* write_buf() const { return write_buf_.get(); }

 private:
  int id_;
  const scoped_ptr<net::StreamSocket> socket_;
  const scoped_refptr<ReadIOBuffer> read_buf_;
  const scoped_refptr<QueuedWriteIOBuffer> write_buf_;

  DISALLOW_COPY_AND_ASSIGN(RpcConnection);
};

}  // namespace rpc

#endif  // RPC_RPC_CONNECTION_H_