#include "floah-put/input_context.h"

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <ranges>

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
        static_cast<void>(button);
        static_cast<void>(action);
        static_cast<void>(mods);
    }

    ////////////////////////////////////////////////////////////////
    // Elements.
    ////////////////////////////////////////////////////////////////

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

    void InputContext::frame()
    {
        // Sort by layer descending.
        std::ranges::sort(inputElements, [](const InputElement* lhs, const InputElement* rhs) {
            return lhs->getInputLayer() > rhs->getInputLayer();
        });

        mouseMoveEvents();
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
            // Elements on the same level or below the currently entered element should not take over the entered state.
            if (stillInside && elem->getInputLayer() <= enteredElement->getInputLayer()) break;

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

}  // namespace floah