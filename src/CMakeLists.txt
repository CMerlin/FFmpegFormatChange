#add_executable(printInfo ./main.cc ./print.cc)
add_executable(printInfo ./simplest_ffmpeg_remuxer.cpp ./print.cc
	./simplest_mediadata_aac.cpp ./simplest_mediadata_flv.cpp
	./simplest_mediadata_h264.cpp ./simplest_mediadata_h264.cpp
	./simplest_mediadata_raw.cpp)



#add_executable(mainStarter ./mainStarter/main.cc ./mainStarter/Starter.cc)
#target_link_libraries(mainStarter gdlp_base gdlp_adpters)
#install(TARGETS mainStarter  DESTINATION bin)

#add_executable(protocolParser ./protocols/main.cc ./protocols/protocolParser.cc  ./protocols/protocolParserLua.cc)
#add_executable(protocolParser ./protocols/main.cc ./protocols/protocolParser.cc)
#target_link_libraries(protocolParser gdlp_base gdlp_adpters lua dl boost_program_options quarterLuaCppFunction)
#install(TARGETS protocolPraser  DESTINATION bin)

