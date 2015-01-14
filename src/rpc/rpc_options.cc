// Copyright (c) 2015 Chaobin Zhang. All rights reserved.
// Use of this source code is governed by the BSD license that can be
// found in the LICENSE file.

#include "rpc/rpc_options.h"

namespace rpc {

const char kDefaultBindIP[] = "0.0.0.0";
const int kDefaultPort = 20015;

}  // namespace rpc

namespace switches {

const char kBindIP[] = "bind_ip";
const char kPort[] = "port";
const char kServerIP[] = "server_ip";

}  // namespace switches
