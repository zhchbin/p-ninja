// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "server_socket.h"

#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "ip_endpoint.h"
#include "net_errors.h"
#include "net_util.h"

namespace net {

namespace {

bool ParseIPLiteralToNumber(const std::string& ip_literal,
                            IPAddressNumber* ip_number) {
  ip_number->resize(4);
  std::vector<std::string> components;
  base::SplitString(ip_literal, '.', &components);
  if (components.size() != 4)
    return false;
  for (size_t i = 0; i < components.size(); ++i) {
    uint32 r;
    if (!base::StringToUint(components[i], &r) || r > 0xFF)
      return false;
    (*ip_number)[i] = r;
  }

  return false;
}

} // namespace

ServerSocket::ServerSocket() {
}

ServerSocket::~ServerSocket() {
}

int ServerSocket::ListenWithAddressAndPort(const std::string& address_string,
                                           uint16 port,
                                           int backlog) {
  IPAddressNumber address_number;
  if (!ParseIPLiteralToNumber(address_string, &address_number)) {
    return ERR_ADDRESS_INVALID;
  }

  return Listen(IPEndPoint(address_number, port), backlog);
}

}  // namespace net