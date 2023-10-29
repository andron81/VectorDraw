#include "configuration.hpp"
using namespace cf_;

Configuration *Configuration::Instance()
{
    if (_instance == nullptr) {
        _instance = new Configuration;
    }

    return _instance;
};



cfg_data_type& Configuration::getSavedValue()
{
    return _savedValue;

};

void Configuration::setValue(cfg_data_type data) {
		_savedValue=data;
};	


Configuration::Configuration(){}