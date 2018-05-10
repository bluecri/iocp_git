#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <SDKDDKVer.h>

#include <WinSock2.h>
#include <Mswsock.h>

#include "windows.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <tchar.h>

#include <algorithm>
#include <string.h>
#include <vector>
#include <deque>
#include <queue>
#include <map>

#include <process.h>
#include <assert.h>
#include <limits.h>

#include <cstdint>
#include <memory>

#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "google/protobuf/io/zero_copy_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl_lite.h"
#include "google/protobuf/text_format.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/message_lite.h"

#include "sql.h"


#include "origin\FastSpinlock.h"
#include "origin\Exception.h"

#include "protoGen/GetSource/PacketRecvHadler.h"
#include "protoGen/GetSource/protoTypeEnum.h"
#include "protoGen/GetSource/protoPack.pb.h"
using namespace glm;
using namespace google;