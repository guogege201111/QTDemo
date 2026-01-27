QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    UseCudaApi.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    UseCudaApi.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# CUDA配置
CUDA_SDK = "/usr/local/cuda"    # CUDA SDK安装路径
CUDA_DIR = "/usr/local/cuda"    # CUDA工具包安装路径
SYSTEM_NAME = linux             # 系统名称
SYSTEM_TYPE = 64                # 32位或64位系统

# AGX-ORIN的计算能力
CUDA_ARCH = sm_87

NVCC_OPTIONS = --use_fast_math

# 如果创建共享库，需要添加-fPIC
contains(TEMPLATE, lib) {
    NVCC_OPTIONS += -Xcompiler -fPIC
}

# 包含路径
INCLUDEPATH += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/lib64/

# CUDA源文件
CUDA_SOURCES += CUDAKernel.cu
CUDA_OBJECTS_DIR = ./

# CUDA库
CUDA_LIBS = cudart cufft nppig
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')

# 调试模式配置
CONFIG(debug, debug|release) {
    # Debug模式
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -g -G -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release模式
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -O3 -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

# 链接CUDA库
LIBS += -L$$CUDA_DIR/lib64/ -lcuda -lcudart -lcufft -lnppig

# 部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 额外的分发文件
DISTFILES += \
    CUDAKernel.cu
