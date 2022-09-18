#include "floah-put/input_element.h"

namespace floah
{
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    InputElement::InputElement() = default;

    InputElement::~InputElement() noexcept = default;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    const InputElement* InputElement::getInputParent() const noexcept { return nullptr; }

    int32_t InputElement::getInputLayer() const noexcept { return 0; }

    math::int2 InputElement::getInputOffset() const noexcept { return {}; }

    ////////////////////////////////////////////////////////////////
    // Input.
    ////////////////////////////////////////////////////////////////

    bool InputElement::compare(const InputElement& other) const noexcept
    {
        // Calculates depth of element by counting number of ancestors.
        constexpr auto calculateDepth = [](const InputElement& elem) -> size_t {
            size_t              depth  = 0;
            const InputElement* parent = elem.getInputParent();
            while (parent)
            {
                parent = parent->getInputParent();
                depth++;
            }
            return depth;
        };

        const auto depthL = calculateDepth(*this);
        const auto depthR = calculateDepth(other);

        // Get parents at same height.
        const auto* elemL = this;
        const auto* elemR = &other;
        for (size_t i = depthR; i < depthL; i++) elemL = elemL->getInputParent();
        for (size_t i = depthL; i < depthR; i++) elemR = elemR->getInputParent();

        // Either of the elements is an ancestor of the other. Descendant should be placed on top.
        if (elemL == elemR) return depthL > depthR;

        while (true)
        {
            // Found elements with common parent (or no parent at all). Compare directly.
            if (elemL->getInputParent() == elemR->getInputParent())
                return elemL->getInputLayer() > elemR->getInputLayer();

            // Go up.
            elemL = elemL->getInputParent();
            elemR = elemR->getInputParent();
        }
    }

    ////////////////////////////////////////////////////////////////
    // Events.
    ////////////////////////////////////////////////////////////////

    void InputElement::onMouseEnter() {}

    void InputElement::onMouseExit() {}

    InputContext::MouseClickResult InputElement::onMouseClick(const InputContext::MouseClick)
    {
        return InputContext::MouseClickResult{};
    }

    InputContext::MouseMoveResult InputElement::onMouseMove(const InputContext::MouseMove)
    {
        return InputContext::MouseMoveResult{};
    }

    InputContext::MouseScrollResult InputElement::onMouseScroll(const InputContext::MouseScroll)
    {
        return InputContext::MouseScrollResult{};
    }

}  // namespace floah
