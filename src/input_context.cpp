#include "floah-put/input_context.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <ranges>

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "floah-put/input_element.h"

namespace floah
{
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    InputContext::InputContext() = default;

    InputContext::~InputContext() noexcept = default;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    int64_t InputContext::getTime() const noexcept { return time; }

    bool InputContext::getFocus() const noexcept { return focus; }

    math::int2 InputContext::getCursor() const noexcept { return cursor; }

    ////////////////////////////////////////////////////////////////
    // Setters.
    ////////////////////////////////////////////////////////////////

    void InputContext::setTime(const int64_t t) noexcept { time = t; }

    void InputContext::setFocus(const bool f) noexcept { focus = f; }

    void InputContext::setEnter(const bool e) noexcept { enter = e; }

    void InputContext::setCursor(math::int2 c) noexcept { cursor = std::move(c); }

    void InputContext::setMouseButton(const MouseButton    button,
                                      const MouseAction    action,
                                      const MouseModifiers mods) noexcept
    {
        mouseClick = {button, action, mods};
    }

    void InputContext::clearMouseButton() noexcept
    {
        mouseClick  = {};
        mouseScroll = {};
    }

    void InputContext::setScroll(const math::int2 s) noexcept { mouseScroll = {s}; }

    ////////////////////////////////////////////////////////////////
    // Elements.
    ////////////////////////////////////////////////////////////////

    // TODO: InputElements do not (yet?) store a reference to their context.
    // They can be added to any number of contexts and will receive events from all of them.
    // Is this desirable or should it be prevented?

    void InputContext::addElement(InputElement& elem) { inputElements.emplace_back(&elem); }

    bool InputContext::removeElement(InputElement& elem)
    {
        if (const auto it = std::ranges::find(inputElements, &elem); it != inputElements.end())
        {
            inputElements.erase(it);
            return true;
        }

        return false;
    }

    ////////////////////////////////////////////////////////////////
    // Frame.
    ////////////////////////////////////////////////////////////////

    void InputContext::prePoll() { clearMouseButton(); }

    void InputContext::postPoll()
    {
        // Sort by layer descending.
        std::ranges::sort(inputElements,
                          [](const InputElement* lhs, const InputElement* rhs) { return lhs->compare(*rhs); });

        mouseEnterEvents();
        mouseMoveEvents();
        mouseClickEvents();
        mouseScrollEvents();

        previousCursor = cursor;
    }

    void InputContext::mouseEnterEvents()
    {
        // If mouse is not inside window, exit currently entered element.
        if (!enter)
        {
            if (enteredElement)
            {
                enteredElement->onMouseExit();
                enteredElement = nullptr;
            }
            return;
        }

        // Check if currently entered element still contains the cursor position.
        bool stillInside = false;
        if (enteredElement)
        {
            if (enteredElement->intersect(cursor - enteredElement->getInputOffset()))
                stillInside = true;
            else
            {
                enteredElement->onMouseExit();
                enteredElement = nullptr;
            }
        }

        // If an element has claimed input, don't try to enter other elements.
        // But perhaps we need to re-enter the claimed element.
        if (claimedElement)
        {
            if (!enteredElement && claimedElement->intersect(cursor - claimedElement->getInputOffset()))
            {
                enteredElement = claimedElement;
                enteredElement->onMouseEnter();
            }

            return;
        }

        // Try to enter new element.
        for (auto* elem : inputElements)
        {
            if (elem == enteredElement) continue;

            // Elements on the same level or below the currently entered element should not take over the entered state.
            if (stillInside && enteredElement->compare(*elem)) break;

            if (elem->intersect(cursor - elem->getInputOffset()))
            {
                // Exit previous element.
                if (enteredElement) enteredElement->onMouseExit();

                // Enter new element.
                enteredElement = elem;
                enteredElement->onMouseEnter();

                break;
            }
        }
    }

    void InputContext::mouseMoveEvents()
    {
        if (previousCursor == cursor) return;

        auto* elem = claimedElement;
        if (!elem && enteredElement) elem = enteredElement;

        if (elem)
        {
            const auto offset = elem->getInputOffset();
            const auto move   = MouseMove{.previous = previousCursor - offset, .current = cursor - offset};
            static_cast<void>(elem->onMouseMove(move));
        }
    }

    void InputContext::mouseClickEvents()
    {
        if (mouseClick)
        {
            if (claimedElement)
            {
                if (!claimedElement->onMouseClick(*mouseClick).claim) claimedElement = nullptr;
            }
            else if (enteredElement)
            {
                if (enteredElement->onMouseClick(*mouseClick).claim) claimedElement = enteredElement;
            }
        }
    }

    void InputContext::mouseScrollEvents()
    {
        if (mouseScroll)
        {
            if (claimedElement)
                static_cast<void>(claimedElement->onMouseScroll(*mouseScroll));
            else if (enteredElement)
                static_cast<void>(enteredElement->onMouseScroll(*mouseScroll));
        }
    }

}  // namespace floah
