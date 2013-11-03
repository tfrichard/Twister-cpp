project(TwiserCpp)

#set(CMAKE_MODULE_PATH ${TwisterCpp_SOURCE_DIR}/cmake)

subdirs(
	client
	#	message
	#	serialization
	#utility
	#worker
	#tasks
	#types
	#communication
	#config
	)

#build the twistercpp library
add_library(twistercpp STATIC
	client/driver.cpp
	client/driver.h
	client/synchronizer.cpp
	client/synchronizer.h
#	config/driver_config.cpp
#	config/driver_config.h
#	config/job_config.h
#	config/job_config.cpp
#	config/mapper_config.h
#	config/mapper_config.cpp
#	config/reduce_config.h
#	config/reduce_config.cpp
#	config/job_state.h
#	config/job_state.cpp
#	message/ack_mapper_msg.h
#	message/ack_mapper_msg.cpp
#	message/ack_msg.h
#	message/ack_msg.cpp
#	message/gather_start_msg.h
#	message/gather_start_msg.cpp
#	message/map_request_grp_msg.h
#	message/map_request_grp_msg.cpp
#	message/map_request_msg.h
#	message/map_request_msg.cpp
#	message/map_start_grp_msg.h
#	message/map_start_grp_msg.cpp
#	message/map_start_msg.h
#	message/map_start_msg.cpp
#	message/message.h
#	message/message.cpp
#	message/reduce_input_msg.h
#	message/reduce_input_msg.cpp
#	message/reduce_input_request_msg.h
#	message/reduce_input_request_msg.cpp
#	message/reduce_request_grp_msg.h
#	message/reduce_request_grp_msg.cpp
#	message/reduce_request_msg.h
#	message/reduce_request_msg.cpp
#	message/reduce_start_msg.h
#	message/reduce_start_msg.cpp
#	message/task_status_msg.h
#	message/task_status_msg.cpp
#	tasks/map_task.h
#	tasks/map_task.cpp
#	tasks/reduce_task.h
#	tasks/reduce_task.cpp
#	utility/class_factory.h
#	utility/class_factory.cpp
#	utility/map_factory.h
#	utility/map_factory.cpp
#	utility/reduce_selector.h
#	utility/reduce_selector.cpp
#	communication/conn_manager.h
#	communication/conn_manager.cpp
#	serialization/in_archive.h
#	serialization/in_archive.cpp
#	serialization/out_archive.h
#	serialization/out_archive.cpp
#	types/gather_input.h
#	types/gather_input.cpp
#	types/int_value.h
#	types/int_value.cpp
#	types/key.h
#	types/key.cpp
#	types/reduce_input.h
#	types/reduce_input.cpp
#	types/string_key.h
#	types/string_key.cpp
#	types/string_value.h
#	types/string_value.cpp
#	types/value.h
#	types/value.cpp
#	worker/daemon.h
#	worker/daemon.cpp
#	worker/gather.h
#	worker/gather.cpp
#	worker/map_output_collector.h
#	worker/map_output_collector.cpp
#	worker/mapper.h
#	worker/mapper.cpp
#	worker/reduce_output_collector.h
#	worker/reduce_output_collector.cpp
#	worker/reducer.h
#	worker/reducer.cpp
	)


