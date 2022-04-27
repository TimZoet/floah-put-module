#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <vector>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "math/include_all.h"

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "floah-put/input_element.h"

namespace floah
{
    class InputContext
    {
    public:
        enum class MouseButton
        {
            Left   = 0,
            Middle = 1,
            Right  = 2
        };

        enum class MouseAction
        {
            Press   = 0,
            Release = 1
        };

        /**
         * \brief Mouse modifier flag bits. (Note: These values match the GLFW values to allow directly casting them.)
         */
        enum class MouseModifiers
        {
            Shift    = 1,
            Control  = 2,
            Alt      = 4,
            Super    = 8,
            CapsLock = 16,
            NumLock  = 32
        };

        ////////////////////////////////////////////////////////////////
        // Constructors.
        ////////////////////////////////////////////////////////////////

        InputContext();

        InputContext(const InputContext&) = delete;

        InputContext(InputContext&&) noexcept = delete;

        ~InputContext() noexcept;

        InputContext& operator=(const InputContext&) = delete;

        InputContext& operator=(InputContext&&) noexcept = delete;

        ////////////////////////////////////////////////////////////////
        // Getters.
        ////////////////////////////////////////////////////////////////

        [[nodiscard]] int64_t getTime() const noexcept;

        [[nodiscard]] bool getFocus() const noexcept;

        [[nodiscard]] math::int2 getCursor() const noexcept;

        ////////////////////////////////////////////////////////////////
        // Setters.
        ////////////////////////////////////////////////////////////////

        void setTime(int64_t t) noexcept;

        void setFocus(bool f) noexcept;

        void setEnter(bool e) noexcept;

        void setCursor(math::int2 c) noexcept;

        void setMouseButton(MouseButton button, MouseAction action, MouseModifiers mods) noexcept;

        ////////////////////////////////////////////////////////////////
        // Elements.
        ////////////////////////////////////////////////////////////////

        void addElement(InputElement& elem);

        bool removeElement(InputElement& elem);

        ////////////////////////////////////////////////////////////////
        // Frame.
        ////////////////////////////////////////////////////////////////

        void frame();

    private:
        void mouseMoveEvents();

        ////////////////////////////////////////////////////////////////
        // Member variables.
        ////////////////////////////////////////////////////////////////

        int64_t time = 0;

        bool focus = false;

        bool enter = false;

        math::int2 cursor;

        std::vector<InputElement*> inputElements;

        /**
         * \brief Input element that currently contains the cursor.
         */
        InputElement* enteredElement = nullptr;
    };
}  // namespace floah
