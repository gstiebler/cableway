cmake_minimum_required (VERSION 2.8.4)
project (cableway)
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(SRC_BASE ../src)
set(QT_UI_BASE_PATH ../data/qt)

set(ALL_SOURCES ${SRC_BASE}/UAuxString.cpp
				${SRC_BASE}/UCallbacks.cpp
				${SRC_BASE}/UConfig.cpp
				${SRC_BASE}/UContainerDesenhos.cpp
				${SRC_BASE}/UDadosGenerico.cpp
				${SRC_BASE}/UDebug.cpp
				${SRC_BASE}/UDefines.cpp
				${SRC_BASE}/UErros.cpp
				${SRC_BASE}/UFila.cpp
				${SRC_BASE}/UGeometria.cpp
				${SRC_BASE}/UGrafoDesenho.cpp
				${SRC_BASE}/UInfoCelula.cpp
				${SRC_BASE}/UInfoCircuitos.cpp
				${SRC_BASE}/UItemCelula.cpp
				${SRC_BASE}/UListaCelulas.cpp
				${SRC_BASE}/UListaItensCelula.cpp
				${SRC_BASE}/UListaPontoEIndiceCabo.cpp
				${SRC_BASE}/UListaV.cpp
				${SRC_BASE}/UNiveisProjeto.cpp
				${SRC_BASE}/UParamsStructs.cpp
				${SRC_BASE}/UStructs.cpp
				${SRC_BASE}/UTCallbackStatusCarregamento.cpp
				${SRC_BASE}/UTempoExec.cpp
				${SRC_BASE}/UTPilhaDesenhos.cpp
				${SRC_BASE}/UVerticesArestas.cpp
				${SRC_BASE}/DrawingLoader.cpp
				${SRC_BASE}/DwgLoader.cpp
				${SRC_BASE}/MainExecution.cpp
				${SRC_BASE}/interface/MainWindow.cpp
				${SRC_BASE}/InputCircuitsReader.cpp
				${SRC_BASE}/UserParams/LoadUserParams.cpp
				${SRC_BASE}/UserParams/UserParams.cpp  )
                
SET( QT_UIS
     ${QT_UI_BASE_PATH}/MainWindow.ui
)
                
QT4_WRAP_CPP(cw_HEADERS_MOC ${SRC_BASE}/interface/MainWindow.h)
QT4_WRAP_UI(QTTEST_GEN_UIS ${QT_UIS} )
				
set(LIB_DIRS ../lib/dxflib/src
             ../lib/libredwg/src
             ../lib/libxls/include)                
                
include_directories(${SRC_BASE} ${LIB_DIRS} ${CMAKE_CURRENT_BINARY_DIR} )

link_directories(../lib/dxflib/lib/
                 ../lib/libredwg/src/.libs
                 ../lib/libxls/src/.libs)
                     
add_executable(cableway ${SRC_BASE}/main.cpp 
					${ALL_SOURCES}
                    ${cw_HEADERS_MOC}
                    ${QTTEST_GEN_UIS})

set(CW_LIBRARIES redwg dxflib xlsreader iconv) 
                   
target_link_libraries(cableway ${CW_LIBRARIES})
target_link_libraries(cableway ${QT_LIBRARIES})

# Tests
#set(CMAKE_CXX_FLAGS_DEBUG  "-fprofile-arcs -ftest-coverage")

#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-lgcov")

set(GTEST_PATH ../lib/gtest-1.7.0)

link_directories(${GTEST_PATH})
                 
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
                    
target_link_libraries(cableway_tests  ${CW_LIBRARIES} gtest)
target_link_libraries(cableway_tests ${QT_LIBRARIES})