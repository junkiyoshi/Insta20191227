#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);

	for (int i = 0; i < 5; i++) {

		this->noise_seed_list.push_back(glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000)));
	}

	this->draw_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->draw_mesh.clear();

	for (auto& noise_seed : this->noise_seed_list) {

		auto location = glm::vec3(
			ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.003), 0, 1, -300, 300)
		);

		auto next = glm::vec3(
			ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300),
			ofMap(ofNoise(noise_seed.z, (ofGetFrameNum() + 1) * 0.003), 0, 1, -300, 300)
		);
		auto distance = location - next;
		distance *= 2;

		auto future = location + distance * 30;
		auto random_deg_1 = ofRandom(360);
		auto random_deg_2 = ofRandom(360);
		future += glm::vec3(
			30 * cos(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * sin(random_deg_1 * DEG_TO_RAD) * sin(random_deg_2 * DEG_TO_RAD),
			30 * cos(random_deg_2 * DEG_TO_RAD)
		);

		auto future_distance = future - location;

		this->location_list.push_back(location + glm::normalize(future_distance) * 200);
		this->life_list.push_back(255);
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->life_list[i] -= 1.f;
		if (this->life_list[i] < 0) {

			this->location_list.erase(this->location_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		this->draw_mesh.addVertex(this->location_list[i]);
		this->draw_mesh.addColor(ofColor(32, this->life_list[i]));
	}

	for (int i = 0; i < this->draw_mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->draw_mesh.getNumVertices(); k++) {

			if (glm::distance(this->draw_mesh.getVertex(i), this->draw_mesh.getVertex(k)) < 50) {

				this->draw_mesh.addIndex(i);
				this->draw_mesh.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.25);
	ofRotateX(ofGetFrameNum() * 0.5);	

	this->draw_mesh.drawWireframe();

	for (int i = 0; i < this->draw_mesh.getNumVertices(); i++) {

		ofSetColor(ofColor(39, this->life_list[i]));
		ofDrawSphere(this->location_list[i], 5);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}