
#include "ui.h"
#include "text_ui.h"

Ui* Ui::sample = nullptr;

Ui* Ui::get(std::string type){
	
	if(sample == nullptr){
		if(type == "text_ui"){
			sample = new TextUi;
		}
	}
	return sample;
}

