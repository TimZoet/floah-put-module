#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <optional>
#include <vector>

////////////////////////////////////////////////////////////////
// Module includes.
////////////////////////////////////////////////////////////////

#include "math/include_all.h"

namespace floah
{
    class InputElement;

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

        /**
         * \brief Properties describing the mouse entering an input element.
         */
        struct MouseEnterEvent
        {
        };

        struct MouseEnterResult
        {
        };

        /**
         * \brief Properties describing the mouse exiting an input element.
         */
        struct MouseExitEvent
        {
        };

        struct MouseExitResult
        {
        };

        /**
         * \brief Properties describing a mouse click on an input element.
         */
        struct MouseClickEvent
        {
            MouseButton    button;
            MouseAction    action;
            MouseModifiers modifiers;
        };

        struct MouseClickResult
        {
            /**
             * \brief If true, the current input element will receive all further input until a result with claim set to false is returned.
             */
            bool claim = false;
        };

        /**
         * \brief Properties describing the mouse moving over an input element.
         */
        struct MouseMoveEvent
        {
            /**
             * \brief Previous cursor position.
             */
            math::int2 previous;

            /**
             * \brief Current cursor position.
             */
            math::int2 current;
        };

        struct MouseMoveResult
        {
        };

        /**
         * \brief Properties describing the mouse scrolling over an input element.
         */
        struct MouseScrollEvent
        {
            /**
             * \brief Horizontal and vertical scroll distance.
             */
            math::int2 scroll;
        };

        struct MouseScrollResult
        {
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

        void clearMouseButton() noexcept;

        void setScroll(math::int2 s) noexcept;

        ////////////////////////////////////////////////////////////////
        // Elements.
        ////////////////////////////////////////////////////////////////

        /**
         * \brief Add an input element to this context.
         * \param elem Element to add.
         */
        void addElement(InputElement& elem);

        /**
         * \brief Remove an input element from this context.
         * \param elem Element to remove.
         * \return True if element was removed, false if it was not in the context.
         */
        bool removeElement(InputElement& elem);

        ////////////////////////////////////////////////////////////////
        // Frame.
        ////////////////////////////////////////////////////////////////

        /**
         * \brief Clears data from previous frame. Should be called before polling for events using your input/windowing library.
         */
        void prePoll();

        /**
         * \brief Processes all input events. Should be called after polling for events (and passing on the events to this context) using your input/windowing library.
         */
        void postPoll();

    private:
        void mouseEnterEvents();

        void mouseMoveEvents();

        void mouseClickEvents();

        void mouseScrollEvents();

        ////////////////////////////////////////////////////////////////
        // Member variables.
        ////////////////////////////////////////////////////////////////

        int64_t time = 0;

        bool focus = false;

        bool enter = false;

        math::int2 previousCursor;

        math::int2 cursor;

        std::vector<InputElement*> inputElements;

        /**
         * \brief Input element that currently contains the cursor.
         */
        InputElement* enteredElement = nullptr;

        /**
         * \brief Input element that has claimed input.
         */
        InputElement* claimedElement = nullptr;

        std::optional<MouseClickEvent> mouseClick;

        std::optional<MouseScrollEvent> mouseScroll;
    };
}  // namespace floah
