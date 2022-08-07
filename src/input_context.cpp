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

    void InputContext::clearMouseButton() noexcept { mouseClick = {}; }


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

        mouseMoveEvents();
        mouseClickEvents();
    }

    void InputContext::mouseMoveEvents()
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
            if (enteredElement->intersect(cursor.x, cursor.y))
                stillInside = true;
            else
            {
                enteredElement->onMouseExit();
                enteredElement = nullptr;
            }
        }

        for (auto* elem : inputElements)
        {
            if (elem == enteredElement) continue;

            // Elements on the same level or below the currently entered element should not take over the entered state.
            if (stillInside && enteredElement->compare(*elem)) break;

            if (elem->intersect(cursor.x, cursor.y))
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

    void InputContext::mouseClickEvents()
    {
        if (mouseClick && enteredElement) enteredElement->onMouseClick(*mouseClick);
    }

}  // namespace floah
