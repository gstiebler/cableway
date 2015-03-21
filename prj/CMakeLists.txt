cmake_minimum_required (VERSION 2.8.11)
project (cableway)



# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)

#INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(SRC_BASE ../src)
set(QT_UI_BASE_PATH ../data/qt)

set(ALL_SOURCES ${SRC_BASE}/UContainerDesenhos.cpp
				${SRC_BASE}/UDadosGenerico.cpp
				${SRC_BASE}/UDebug.cpp
				${SRC_BASE}/UDefines.cpp
				${SRC_BASE}/UErros.cpp
				${SRC_BASE}/UGeometria.cpp
				${SRC_BASE}/UGrafoDesenho.cpp
				${SRC_BASE}/UInfoCelula.cpp
				${SRC_BASE}/UInfoCircuitos.cpp
				${SRC_BASE}/UListaItensCelula.cpp
				${SRC_BASE}/UListaPontoEIndiceCabo.cpp
				${SRC_BASE}/UListaV.cpp
				${SRC_BASE}/UMostraDesenho.cpp
				${SRC_BASE}/UOpenGL.cpp
				${SRC_BASE}/GLCoords.cpp
				${SRC_BASE}/UParamsStructs.cpp
				${SRC_BASE}/UStructs.cpp
				${SRC_BASE}/UVerticesArestas.cpp
				${SRC_BASE}/CweLoader.cpp
				${SRC_BASE}/MainExecution.cpp
				${SRC_BASE}/interface/MainWindow.cpp
				${SRC_BASE}/interface/DrawingWindow.cpp
				${SRC_BASE}/presenters/MainPresenter.cpp
				${SRC_BASE}/presenters/DrawingPresenter.cpp
				${SRC_BASE}/InputCircuitsReader.cpp
				${SRC_BASE}/Reports.cpp
				${SRC_BASE}/UserParams/LoadUserParams.cpp
				${SRC_BASE}/UserParams/UserParams.cpp  )
                
SET( QT_UIS
     ${QT_UI_BASE_PATH}/MainWindow.ui
     ${QT_UI_BASE_PATH}/DrawingWindow.ui
)
                
QT5_WRAP_CPP(cw_HEADERS_MOC ${SRC_BASE}/interface/MainWindow.h
                            ${SRC_BASE}/presenters/MainPresenter.h 
                            ${SRC_BASE}/interface/DrawingWindow.h
                            ${SRC_BASE}/presenters/DrawingPresenter.h)
                            
QT5_WRAP_UI(QTTEST_GEN_UIS ${QT_UIS} )
				
set(LIB_DIRS ../lib/libxls/include)                
                
include_directories(${SRC_BASE} ${LIB_DIRS} ${CMAKE_CURRENT_BINARY_DIR} )

link_directories(../lib/libxls/build_vs2012/Debug)
                     
add_executable(cableway ${SRC_BASE}/main.cpp 
					${ALL_SOURCES}
                    ${cw_HEADERS_MOC}
                    ${QTTEST_GEN_UIS})
                    
                    
qt5_use_modules(cableway Widgets OpenGL)

set(CW_LIBRARIES xlsReader ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ) 
                   
target_link_libraries(cableway ${CW_LIBRARIES} ${QT_LIBRARIES})

# Tests
#set(CMAKE_CXX_FLAGS_DEBUG  "-fprofile-arcs -ftest-coverage")

#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-lgcov")

if (WIN32)
    set(CMAKE_CXX_FLAGS  "/MP")
    add_definitions( -D_VARIADIC_MAX=10 )
endif (WIN32)

set(GTEST_PATH ../lib/gtest-1.7.0)

link_directories(${GTEST_PATH}/Debug)
                 
include_directories(${GTEST_PATH}/include)

set(TESTS_SOURCES ${SRC_BASE}/tests/main_tests.cpp
                  ${SRC_BASE}/tests/basic_tests.cpp
                  ${SRC_BASE}/tests/TestsUtil.cpp
                  ${SRC_BASE}/tests/UserParamsLoadingTests.cpp
                  ${SRC_BASE}/tests/DrawingLoaderTests.cpp
                  ${SRC_BASE}/tests/InputCircuitsReaderTests.cpp)

add_executable(cableway_tests ${ALL_SOURCES} 
                    ${TESTS_SOURCES}
                    ${cw_HEADERS_MOC}
                    ${QTTEST_GEN_UIS}) 
                    
qt5_use_modules(cableway_tests Widgets OpenGL)
                    
target_link_libraries(cableway_tests  ${CW_LIBRARIES} gtest)
target_link_libraries(cableway_tests ${QT_LIBRARIES})