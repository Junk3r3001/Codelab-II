#pragma once

#include "json.hpp"
#include "ofMain.h"
#include <algorithm>
#include <string>
#include <vector>

using json = nlohmann::json;

struct Country {
	std::string name;
	std::string capital;
	int64_t population = 0;
	double area = 0.0;
	std::string region;
};

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;
	void urlResponse(ofHttpResponse & response);

	void filterCountries();

	std::vector<Country> allCountries;
	std::vector<Country> filteredCountries;
	int currentIndex = 0;
	bool isLoading = true;
	std::string statusMessage = "Fetching country dataset via REST...";
	std::string searchQuery = "";
};
