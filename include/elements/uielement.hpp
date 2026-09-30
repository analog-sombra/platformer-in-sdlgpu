#ifndef __UIELEMENT_HPP__
#define __UIELEMENT_HPP__

namespace Engine::Elements
{
    class UIElement
    {
    public:
        UIElement();
        ~UIElement();
        void render();
        void update();
        void handleEvent();

        void Cleanup();
    };
}

#endif // __UIELEMENT_HPP__