#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {

class ExampleSprite : public Sprite {
    GODOT_CLASS(ExampleSprite, Sprite)

private:
    float time_passed;
    float amplitude;

public:
    static void _register_methods();

    ExampleSprite();
    virtual ~ExampleSprite();

    void _enter_tree();

    void _init(); // our initializer called by Godot

    void _process(float delta);
};

}

#endif
