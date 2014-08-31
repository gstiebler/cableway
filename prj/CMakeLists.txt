cmake_minimum_required (VERSION 2.6)
project (cableway)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(SRC_BASE ../src)


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
				${SRC_BASE}/UVetor.cpp
				)
				

add_executable(cableway ${ALL_SOURCES} 
						${SRC_BASE}/main.cpp)

set(GTEST_PATH ../lib/gtest-1.6.0)
include_directories(${GTEST_PATH}/include)
add_definitions(-D_VARIADIC_MAX=10)
add_executable(cableway_tests ${ALL_SOURCES} 
		${SRC_BASE}/tests/main_tests.cpp
		${SRC_BASE}/tests/basic_tests.cpp)
target_link_libraries(cableway_tests ${GTEST_PATH}/Debug/gtest)