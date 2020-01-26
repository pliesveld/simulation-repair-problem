
#include <iostream>
#include "exampleSprite.h"

using namespace godot;

void ExampleSprite::_register_methods() {
    register_method("_init", &ExampleSprite::_init);
    register_method("_process", &ExampleSprite::_process);
    register_method("_enter_tree", &ExampleSprite::_enter_tree);
    register_property<ExampleSprite, float>("amplitude", &ExampleSprite::amplitude, 10.0);
}

ExampleSprite::ExampleSprite() {
}

ExampleSprite::~ExampleSprite() {
    // add your cleanup here
}

void ExampleSprite::_init() {
    // initialize any variables here
    time_passed = 0.0;
    amplitude = 10.0;
}

void ExampleSprite::_enter_tree() {
    std::cout << "Enter tree\n";
}

void ExampleSprite::_process(float delta) {

    time_passed += delta;

    Vector2 new_position = Vector2(
	amplitude + (amplitude * sin(time_passed * 2.0)), 
	amplitude + (amplitude * cos(time_passed * 1.5))
    );

    set_position(new_position);
}
