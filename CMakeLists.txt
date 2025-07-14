# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components"
                         "${CMAKE_CURRENT_LIST_DIR}/libraries/coreMQTT"
						 "${CMAKE_CURRENT_LIST_DIR}/libraries/Device-Shadow-for-AWS-IoT-embedded-sdk"
						 "${CMAKE_CURRENT_LIST_DIR}/libraries/coreJSON"
						 "${CMAKE_CURRENT_LIST_DIR}/libraries/backoffAlgorithm"
						 "${CMAKE_CURRENT_LIST_DIR}/libraries/common/posix_compat"
                        )

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(muscle-master)

#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/app/network/cloud/certs/client.crt" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/app/network/cloud/certs/client.key" TEXT)
#target_add_binary_data(${CMAKE_PROJECT_NAME}.elf "main/app/network/cloud/certs/root_cert_auth.crt" TEXT)
