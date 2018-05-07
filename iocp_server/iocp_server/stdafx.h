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

#include "sql.h"


#include "origin\FastSpinlock.h"
#include "origin\Exception.h"
#include "../../3rdParty/google_protobuf/protoGen/GetSource/protoPack.pb.h"

using namespace glm;
