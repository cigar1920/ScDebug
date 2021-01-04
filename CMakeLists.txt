# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5.1)
set(BIN_DIR ./bin)
# 项目信息
project (tac)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})
message(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "llvm include path: ${LLVM_INCLUDE_DIRS}")
message(STATUS "llvm lib path: ${LLVM_LIBRARY_DIRS}")


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/trans DIR_SRCS_TAC)
# 指定生成目标
add_executable(tac ${DIR_SRCS_TAC})



#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5.1)

# 项目信息
project (insp)
find_package(LLVM REQUIRED CONFIG)



set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/insert DIR_SRCS_INSP)
# 指定生成目标
add_executable(insp ${DIR_SRCS_INSP})


#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# 项目信息
project (tacop)
find_package(LLVM REQUIRED CONFIG)



set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/transop DIR_SRCS_TACOP)
# 指定生成目标
add_executable(tacop ${DIR_SRCS_TACOP})


#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5.1)

# 项目信息
project (tacstmt)
find_package(LLVM REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/transstmt DIR_SRCS_TACSTMT)
# 指定生成目标
add_executable(tacstmt ${DIR_SRCS_TACSTMT})



#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# 项目信息
project (partion)
find_package(LLVM REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/partion DIR_SRCS_PARTION)
# 指定生成目标
add_executable(partion ${DIR_SRCS_PARTION})

#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")

# 项目信息
project (transblock)
find_package(LLVM REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/transblock DIR_SRCS_TRANSBLOCK)
# 指定生成目标
add_executable(transblock ${DIR_SRCS_TRANSBLOCK})

#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
# 项目信息
project (ext)
find_package(LLVM REQUIRED CONFIG)


set(CMAKE_CXX_STANDARD 11)
add_compile_options(-fno-rtti)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/extr DIR_SRCS_EXT)
# 指定生成目标
add_executable(ext ${DIR_SRCS_EXT})



#llvm_map_components_to_libnames(llvm_libs all)
#target_link_libraries(./bin/${PROJECT_NAME} -I/home/shijia/.local/llvm3.6/include -L/home/shijia/.local/llvm3.6/lib  -fno-rtti -lclangFrontend  -lclangTooling -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers  -lclangStaticAnalyzerCore  -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangLex -lclangBasic    -lclangAnalysis -lclangAST    -lclangASTMatchers  -lLLVMSupport -lpthread )

set (LIBS -g -fno-rtti -lclangFrontend -lclangDriver -lclangTooling -lclangDriver -lclangTooling -lclangFrontendTool -lclangFrontend -lclangDriver -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangCodeGen -lclangSema -lclangAnalysis -lclangAST -lclangParse -lclangLex -lclangASTMatchers -lclangBasic -lLLVMSupport -I/home/shijia/.local/llvm3.6/include/ -lpthread 
-I/home/shijia/.local/llvm3.6/include -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -O3 -fomit-frame-pointer -std=c++11 -fvisibility-inlines-hidden -fno-exceptions -fno-rtti -fPIC -ffunction-sections -fdata-sections -Wcast-qual -L/home/shijia/.local/llvm3.6/lib -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMipo -lLLVMVectorize -lLLVMBitWriter -lLLVMTableGen -lLLVMDebugInfo -lLLVMOption -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMIRReader -lLLVMAsmParser -lLLVMLineEditor -lLLVMInstrumentation -lLLVMInterpreter -lLLVMExecutionEngine -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMScalarOpts -lLLVMProfileData -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMCore -lLLVMSupport -lz -lpthread -ltinfo -lrt -ldl -lm
)
target_link_libraries(${PROJECT_NAME} ${LIBS} )


SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES

                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
# 项目信息
project (main)



set(CMAKE_CXX_STANDARD 11)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/main DIR_SRCS_MAIN)
# 指定生成目标
add_executable(main ${DIR_SRCS_MAIN})

target_link_libraries(${PROJECT_NAME} ${LIBS} )
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")


                      
                      
                      