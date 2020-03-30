#include "engine.h"


void cEngine::startup()
{
	PLOGI << witchcraft::log_strings::engine_startup;
	_current_engine_state = EEngineState::STARTUP;

	PLOGI << witchcraft::log_strings::resource_manager_start;
	resource = std::make_unique<cResourceManager>();
	//witchcraft::configuration::create_config_files();
	//resource->load_from_xml_file(witchcraft::configuration::birds_asset);
	//resource->load_from_xml_file(witchcraft::configuration::buddha_asset);
	//resource->load_from_xml_file(witchcraft::configuration::person_asset);


}

void cEngine::run()
{
	PLOGI << witchcraft::log_strings::engine_running;
	_current_engine_state = EEngineState::RUNNING;



}

void cEngine::shutdown()
{
	PLOGI << witchcraft::log_strings::engine_shutdown;
	_current_engine_state = EEngineState::SHUTDOWN;


}
