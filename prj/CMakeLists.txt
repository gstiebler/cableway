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

set( PRESENTER_SOURCES
    ${SRC_BASE}/presenters/MainPresenter.cpp
    ${SRC_BASE}/presenters/DrawingPresenter.cpp
)

set( WINDOW_SOURCES
    ${SRC_BASE}/interface/MainWindow.cpp
    ${SRC_BASE}/interface/DrawingWindow.cpp   
)

set( FILE_UTILS_SOURCES
    ${SRC_BASE}/file/CweLoader.cpp
    ${SRC_BASE}/file/InputCircuitsReader.cpp
    ${SRC_BASE}/file/CsvReader.cpp  
)

set( USER_PARAMS_SOURCES
    ${SRC_BASE}/UserParams/LoadUserParams.cpp
    ${SRC_BASE}/UserParams/UserParams.cpp
)

set( SCREEN_DRAWING_SOURCES
    ${SRC_BASE}/ScreenDrawing/UMostraDesenho.cpp
    ${SRC_BASE}/ScreenDrawing/UOpenGL.cpp
    ${SRC_BASE}/ScreenDrawing/GLCoords.cpp
)

set( GRAPH_RELATED_SOURCES
    ${SRC_BASE}/Graph.cpp
    ${SRC_BASE}/GraphBuilder.cpp
    ${SRC_BASE}/UVerticesArestas.cpp
)

set( CAD_DRAWING_SOURCES
    ${SRC_BASE}/UListaItensCelula.cpp
    ${SRC_BASE}/UInfoCelula.cpp
    ${SRC_BASE}/UDadosGenerico.cpp
)

set( UTILS_SOURCES
    ${SRC_BASE}/Debug.cpp
    ${SRC_BASE}/UErros.cpp
    ${SRC_BASE}/UGeometria.cpp
)

set( ELECTRICAL_SOURCES
    ${SRC_BASE}/Electrical/ElectricalElementsBuilder.cpp
    ${SRC_BASE}/Electrical/ArcCable.cpp
    ${SRC_BASE}/Electrical/StraightCable.cpp
    ${SRC_BASE}/Electrical/Instrument.cpp
    ${SRC_BASE}/Electrical/Bandeirola.cpp
    ${SRC_BASE}/Electrical/ElectricalElement.cpp
)

set( ALL_SOURCES 
    ${SRC_BASE}/UContainerDesenhos.cpp
    ${SRC_BASE}/UInfoCircuitos.cpp
    ${SRC_BASE}/InterfaceFeedback.cpp
    ${SRC_BASE}/MainExecution.cpp
    ${SRC_BASE}/Reports.cpp
    ${PRESENTER_SOURCES}
    ${WINDOW_SOURCES}
    ${FILE_UTILS_SOURCES}
    ${USER_PARAMS_SOURCES}
    ${SCREEN_DRAWING_SOURCES}
    ${GRAPH_RELATED_SOURCES}
    ${CAD_DRAWING_SOURCES}
    ${UTILS_SOURCES}
    ${ELECTRICAL_SOURCES}
)

source_group( "Source Files\\Presenters" FILES ${PRESENTER_SOURCES} )
source_group( "Source Files\\Windows" FILES ${WINDOW_SOURCES} )
source_group( "Source Files\\FileUtils" FILES ${FILE_UTILS_SOURCES} )
source_group( "Source Files\\UserParams" FILES ${USER_PARAMS_SOURCES} )
source_group( "Source Files\\ScreenDrawing" FILES ${SCREEN_DRAWING_SOURCES} )
source_group( "Source Files\\GraphRelated" FILES ${GRAPH_RELATED_SOURCES} )
source_group( "Source Files\\CadDrawing" FILES ${CAD_DRAWING_SOURCES} )
source_group( "Source Files\\Utils" FILES ${UTILS_SOURCES} )
source_group( "Source Files\\Electrical" FILES ${ELECTRICAL_SOURCES} )
            
SET( QT_UIS
     ${QT_UI_BASE_PATH}/MainWindow.ui
     ${QT_UI_BASE_PATH}/DrawingWindow.ui
)
                
QT5_WRAP_CPP(cw_HEADERS_MOC ${SRC_BASE}/interface/MainWindow.h
                            ${SRC_BASE}/presenters/MainPresenter.h 
                            ${SRC_BASE}/interface/DrawingWindow.h
                            ${SRC_BASE}/presenters/DrawingPresenter.h)
                          
set( MOC_BASE_DIR __/src )
                          
source_group( "Source Files\\Moc" FILES 
    ${MOC_BASE_DIR}/interface/moc_MainWindow.cpp 
    ${MOC_BASE_DIR}/presenters/moc_MainPresenter.cpp 
    ${MOC_BASE_DIR}/interface/moc_DrawingWindow.cpp 
    ${MOC_BASE_DIR}/presenters/moc_DrawingPresenter.cpp 
)
                            
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
                  ${SRC_BASE}/tests/InputCircuitsReaderTests.cpp
                  ${SRC_BASE}/tests/File2FileTests.cpp)
                  
source_group( "Source Files\\Tests" FILES ${TESTS_SOURCES} )

add_executable(cableway_tests ${ALL_SOURCES} 
                    ${TESTS_SOURCES}
                    ${cw_HEADERS_MOC}
                    ${QTTEST_GEN_UIS}) 
                    
qt5_use_modules(cableway_tests Widgets OpenGL)
                    
target_link_libraries(cableway_tests  ${CW_LIBRARIES} gtest)
target_link_libraries(cableway_tests ${QT_LIBRARIES})