#include "Asset.h"
#include <iostream>

void Asset::draw() const {

	if (isColor) { // TODO: implement color later
		std::cout << icon;
	}
	else {
		std::cout << icon;
	}
}