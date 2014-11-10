#include "JCommand.hpp"
	JCommand::JCommand() {}

	std::string JCommand::Action() {
		try {
			std::string s;
			s = this->obj.get<std::string>("action");
			return s;
		}catch(std::exception const& e) {}
		return "err";
	}

	std::string JCommand::Name() {
		try {
			return this->obj.get<std::string>("name");
		}catch(std::exception const& e) {}
		return "err";
	}

	var_float_t JCommand::Value() {
		try {
			return this->obj.get<var_float_t>("value");
		}catch(std::exception const& e) {}
		return (var_float_t)-1.0f;
	}

	bool JCommand::tryParse(std::string s) {
		std::stringstream ss;
		ss << s;
		try {
			boost::property_tree::read_json(ss,this->obj);
			return true;
		}
		catch( std::exception const& e ) {}
		return false;
	}
