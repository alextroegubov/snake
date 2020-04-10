
#include "ui.h"
#include "text_ui.h"

ui* ui::sample = nullptr;

ui* ui::get(std::string type){
	
	if(sample == nullptr){
		if(type == "text_ui"){
			sample = new TextUi;
		}
	}
	return sample;
}

