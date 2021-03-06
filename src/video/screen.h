#pragma once

#include <SFML/Graphics.hpp>

#include "framebuffer.h"

class CPU;
class MMU;

class Screen {
public:
    virtual ~Screen() = default;

    virtual void draw(const FrameBuffer& buffer) = 0;
    virtual bool is_open() = 0;
};
