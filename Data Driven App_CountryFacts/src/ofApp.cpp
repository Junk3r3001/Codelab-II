#include "ofApp.h"

void ofApp::setup() {
	ofSetWindowShape(900, 650);
	ofSetWindowTitle("Country Facts (REST API Search)");
	ofBackground(24, 26, 32);

	ofRegisterURLNotification(this);

	// Reliable, public, keyless REST endpoint for world country data
	std::string endpoint = "https://cdn.jsdelivr.net/npm/world-countries@5.0.0/countries.json";
	ofLoadURLAsync(endpoint, "countries_req");
}

void ofApp::urlResponse(ofHttpResponse & response) {
	if (response.request.name != "countries_req") return;

	isLoading = false;

	if (response.status != 200) {
		statusMessage = "HTTP " + ofToString(response.status) + ": Connection failed.";
		return;
	}

	try {
		json root = json::parse(response.data.getText());

		if (!root.is_array()) {
			statusMessage = "API returned invalid format.";
			return;
		}

		allCountries.clear();
		for (const auto & item : root) {
			Country c;

			// Common Name
			if (item.contains("name") && item["name"].is_object() && item["name"].contains("common") && item["name"]["common"].is_string()) {
				c.name = item["name"]["common"].get<std::string>();
			} else {
				c.name = "Unknown";
			}

			// Capital
			if (item.contains("capital") && item["capital"].is_array() && !item["capital"].empty()) {
				if (item["capital"][0].is_string()) {
					c.capital = item["capital"][0].get<std::string>();
				}
			} else {
				c.capital = "N/A";
			}

			// Region
			if (item.contains("region") && item["region"].is_string()) {
				c.region = item["region"].get<std::string>();
			} else {
				c.region = "Unknown";
			}

			// Area
			if (item.contains("area") && item["area"].is_number()) {
				c.area = item["area"].get<double>();
			}

			allCountries.push_back(c);
		}

		filterCountries();

	} catch (const std::exception & e) {
		statusMessage = std::string("JSON Error: ") + e.what();
	}
}

void ofApp::filterCountries() {
	filteredCountries.clear();
	currentIndex = 0;

	std::string q = ofToLower(ofTrim(searchQuery));

	if (q.empty()) {
		filteredCountries = allCountries;
		statusMessage = "Showing all " + ofToString(filteredCountries.size()) + " countries.";
		return;
	}

	for (const auto & c : allCountries) {
		std::string countryLower = ofToLower(c.name);
		if (countryLower.find(q) != std::string::npos) {
			filteredCountries.push_back(c);
		}
	}

	if (filteredCountries.empty()) {
		statusMessage = "No countries matched '" + searchQuery + "'";
	} else {
		statusMessage = "Found " + ofToString(filteredCountries.size()) + " match(es).";
	}
}

void ofApp::update() {
}

void ofApp::draw() {
	// Header
	ofSetColor(255);
	ofDrawBitmapString("REST COUNTRIES FINDER", 50, 45);
	ofDrawBitmapString("----------------------------------------------------------------------", 50, 60);

	// Search bar UI
	ofSetColor(50, 55, 65);
	ofDrawRectangle(50, 75, 450, 32);
	ofSetColor(100, 180, 255);
	ofNoFill();
	ofDrawRectangle(50, 75, 450, 32);
	ofFill();

	std::string cursor = (ofGetFrameNum() % 60 < 30) ? "_" : "";
	if (searchQuery.empty()) {
		ofSetColor(140);
		ofDrawBitmapString("[Type country name (e.g. France)]", 62, 96);
	} else {
		ofSetColor(255);
		ofDrawBitmapString(searchQuery + cursor, 62, 96);
	}

	ofSetColor(160);
	ofDrawBitmapString("Controls: Type to Search live | BACKSPACE: Delete | LEFT/RIGHT: Navigate", 50, 130);
	ofDrawBitmapString("----------------------------------------------------------------------", 50, 145);

	// Loading / Status Message
	if (isLoading) {
		ofSetColor(255, 200, 80);
		ofDrawBitmapString("Status: " + statusMessage, 50, 175);
		return;
	}

	if (filteredCountries.empty()) {
		ofSetColor(240, 90, 90);
		ofDrawBitmapString("Status: " + statusMessage, 50, 175);
		return;
	}

	// Display Active Country
	const Country & c = filteredCountries[currentIndex];

	ofSetColor(120, 220, 120);
	ofDrawBitmapString("Status: " + statusMessage, 50, 175);

	ofSetColor(90, 200, 255);
	ofDrawBitmapString("Common Name : " + c.name, 50, 220);

	ofSetColor(230);
	ofDrawBitmapString("Region      : " + c.region, 50, 255);
	ofDrawBitmapString("Capital     : " + c.capital, 50, 290);
	ofDrawBitmapString("Area (sq km): " + ofToString(c.area, 1), 50, 325);

	// Navigation info
	ofSetColor(130);
	ofDrawBitmapString("----------------------------------------------------------------------", 50, 380);
	ofDrawBitmapString("Result: " + ofToString(currentIndex + 1) + " of " + ofToString(filteredCountries.size()), 50, 405);
}

void ofApp::keyPressed(int key) {
	// Backspace: remove last character
	if (key == OF_KEY_BACKSPACE || key == 8) {
		if (!searchQuery.empty()) {
			searchQuery.pop_back();
			filterCountries();
		}
		return;
	}

	// Enter / Return or Esc
	if (key == OF_KEY_RETURN || key == 10 || key == 13) {
		filterCountries();
		return;
	}
	if (key == OF_KEY_ESC || key == 27) {
		searchQuery.clear();
		filterCountries();
		return;
	}

	// Left / Right arrows
	if (key == OF_KEY_RIGHT && !filteredCountries.empty()) {
		currentIndex = (currentIndex + 1) % filteredCountries.size();
		return;
	}
	if (key == OF_KEY_LEFT && !filteredCountries.empty()) {
		currentIndex = (currentIndex - 1 + filteredCountries.size()) % filteredCountries.size();
		return;
	}

	// Printable keys
	if (key >= 32 && key <= 126) {
		searchQuery += static_cast<char>(key);
		filterCountries();
	}
}
